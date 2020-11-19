#include "stdafx.h"
#include <PyramidM\MSVC_2008\CurrentPosToken.h>
#include <gis\GrUnits\TknMapNonScale.h>
#include <gis_tool_qt\GisInterfaceQT\MapBaseWidget.h>

CurrentPosToken::CurrentPosToken(QObject* parent,MapBaseWidget* map, StoreVectorData<FastVectorData>* tokenLayer)
{
	_myPosToken = NULL;
	_tokenLayer = tokenLayer;
	_map = map;
	this->setParent(parent);
}

CurrentPosToken::~CurrentPosToken()
{
	//if(_myPosToken)
	//{
	//	delete _myPosToken;
	//	_myPosToken = NULL;
	//}
}

bool CurrentPosToken::_init(QString pathToImages,double lat,double lon)
{
	QString fileName = pathToImages + "/arrow.tkn";
	QFile img(fileName);
	if(!img.exists()) 
		return false;
	
	CctFile* bin_file = CCT_FILE(cct_buff_file_new());
	CctError *error = cct_file_open( bin_file, 
	fileName.toLocal8Bit().constData(), open_read_only, 0);

	bool isOk = false;
	if( cct_error_success (error) )
	{
		cct_uint ret_size = 0;
		cct_ulong countRead = cct_file_getFileSize(bin_file);
		char *buff = new char[countRead];
		error = cct_file_read(bin_file, buff, countRead, &ret_size);
		
		if( cct_error_success (error) )
		{
			std::istrstream mem((const char*)buff, countRead);
			_typeGisToken.clear();
			isOk = _typeGisToken.loadFromBlob(mem);
			_typeGisToken.scale(1,1);
		}
		else
		{
			cct_error_free(error);
			return isOk;
		}

		cct_file_close( bin_file );
		delete [] buff;
		cct_file_free( bin_file );
		if(_myPosToken)
		{
			delete _myPosToken;
			_myPosToken = NULL;
		}
		_myPosToken = new  TknNonScale(&_typeGisToken);
		_tokenLayer->addObject(_myPosToken);
		Pos2D pos2D = {lat, lon};
		_myPosToken->setPosition(pos2D);
		return isOk;
	}
	else 
	{
	
		cct_error_free(error);
			return isOk;
	}
	return isOk;
}

bool CurrentPosToken::initToken()
{
	QString path_to_ini = QCoreApplication::applicationDirPath () + "/settings_akps.ini";
	if (QFile::exists(path_to_ini) ) 
	{
		QSettings akps_settings (path_to_ini, QSettings::IniFormat);
		QString pathToImages = akps_settings.value("programm/path_to_images").toString();
		double lat = akps_settings.value("programm/lastLat").toDouble();
		double lon = akps_settings.value("programm/lastLon").toDouble();
		return _init(pathToImages,lat,lon);
	}
	return false;
}

void CurrentPosToken::_slotGpsData(QTime time, QDate date, bool status, double lat, double lon, double speed, double angle,short sat, float height)
{
	if(_myPosToken)
	{
		Pos2D pos2D = {lat, lon};
		_myPosToken->setPosition(pos2D);
		
		//угол для ориентированной на север карты
		//нужно получать текущий угол карты и исходя из него поворачивать знак
		if (angle*CCT_180_PI >-361 && angle*CCT_180_PI < 361)
		{
			double ang = angle + _map->getGisView()->angle();
			_myPosToken->setAlpha(ang);
		}

		_myPosToken->show();		
	}
}

void CurrentPosToken::removeToken()
{
	_tokenLayer->deleteAll();
	_myPosToken = NULL;
}

void CurrentPosToken::_slotViewPos(double lat, double lon)
{
	
}