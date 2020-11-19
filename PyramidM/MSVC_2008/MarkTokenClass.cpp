#include "stdafx.h"
#include <PyramidM\MSVC_2008\MarkTokenClass.h>
#include <gis\GrUnits\TknMapNonScale.h>
#include <gis_tool_qt\GisInterfaceQT\MapBaseWidget.h>

MarkTokenClass::MarkTokenClass(QObject* parent,MapBaseWidget* map, StoreVectorData<FastVectorData>* tokenLayer)
{
	_marksLayer = tokenLayer;
	_map = map;
	this->setParent(parent);

	_loadSettings();
	_loadMarks();
}

MarkTokenClass::~MarkTokenClass()
{

}

bool MarkTokenClass::_addMark(short typeOfToken, double lat,double lon)
{
	QString fileName = _pathToImages;

	TknNonScale* mark;
	TypeGisToken *_typeGisToken;
	_typeGisToken = new TypeGisToken();
	//выбираем тип картинки
	//отрисовать знаки для картинок
	if( typeOfToken > 0 &&  typeOfToken != 100)
		typeOfToken = typeOfToken - 1;
	switch(typeOfToken)
	{

	case 0:
		fileName += "/LA.tkn";
		break;
	case 1:
		fileName += "/LAcrush.tkn";
		break;
	case 2:
		fileName += "/blackBox.tkn";
		break;
	case 3:
		fileName += "/bodyes.tkn";
		break;
	case 4:
		fileName += "/sled.tkn";
		break;
	case 5:
		fileName += "/badWay.tkn";
		break;
	case 6:
		fileName += "/AS.tkn";
		break;
	case 7:
		fileName += "/des.tkn";
		break;

	default:
		fileName += "/anyth.tkn";
		break;
	}

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
			_typeGisToken->clear();
			isOk = _typeGisToken->loadFromBlob(mem);
			_typeGisToken->scale(1,1);
		}
		else
		{
			cct_error_free(error);
			return isOk;
		}

		cct_file_close( bin_file );
		delete [] buff;
		cct_file_free( bin_file );

		mark = new  TknNonScale(_typeGisToken);
		_marksLayer->addObject(mark);

		Pos2D pos2D = {lat, lon};
		mark->setPosition(pos2D);
		_map->redrawFull();
		return isOk;
	}
	else 
	{
	
		cct_error_free(error);
			return isOk;
	}
	return isOk;
}

bool MarkTokenClass::_loadSettings()
{
	QString path_to_ini = QCoreApplication::applicationDirPath () + "/settings_akps.ini";
	if (QFile::exists(path_to_ini) ) 
	{
		QSettings akps_settings (path_to_ini, QSettings::IniFormat);
		_pathToImages = akps_settings.value("programm/path_to_images").toString();
		_pathToMarks = akps_settings.value("programm/path_to_marks").toString();
		return true;
	}
	return false;
}

bool MarkTokenClass::removeTokens()
{
	_marksLayer->deleteAll();
	QFile fileMarks(_pathToMarks+"//marks.file");
	if (!fileMarks.exists())
		return false;
	else
	{
		bool res =  fileMarks.remove();
		fileMarks.close();
		return res;
	}
}

bool MarkTokenClass::_loadMarks()
{
	QFile fileMarks(_pathToMarks+"//marks.file");
	
	if (!fileMarks.open(QIODevice::ReadOnly | QIODevice::Text))
		return false;
	else
	{
		QTextStream myMarksStream;
		myMarksStream.setDevice(&fileMarks);
		//пока не конец файла брать новую точку из файла и рисовать ее на карте
		while (!myMarksStream.atEnd())
		{
			QString str = myMarksStream.readLine();
			
			short type = str.section(',', 0, 0).toInt();//"001"; // получаем из списка
			double lat = str.section(',', 4, 4).toDouble(); //gps
			double lon = str.section(',', 5, 5).toDouble(); //gps
			_slotMark(type,lat,lon);
		}
		fileMarks.close();
		return true;
	}
}
void MarkTokenClass::_slotMark(short type, double lat, double lon)
{
 _addMark(type,lat,lon);
}

void MarkTokenClass::_slotUpdateMarks()
{
	_marksLayer->deleteAll();
	_loadMarks();
	_map->redrawDynamic();
}