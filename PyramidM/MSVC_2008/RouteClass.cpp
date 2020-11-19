#include "stdafx.h"
#include <MSVC_2008\RouteClass.h>
#include <MSVC_2008\TknRoute.h>
#include <gis_tool_qt\GisInterfaceQT\MapBaseWidget.h>
#include <QMainWindow>
#include <PyramidM\MSVC_2008\GpsClass.h>
#include <gis\GisBase\cgis\pos.h>
#include <PyramidM\MSVC_2008\CalcFunctions.h>

RouteClass::RouteClass(QWidget *parent, MapBaseWidget* map, StoreVectorData<FastVectorData>* routeLayer)
{
	this->setParent(parent);
	_parent = parent;
	_map = map;
	_routeLayer = routeLayer;
	_tknRoute = NULL;

}

RouteClass::~RouteClass()
{

}


//отрисовать палетку на карте
bool RouteClass::drawRouteOnMap(TknRoute* tknRoute,MapBaseWidget *m_mapWidget, StoreVectorData<FastVectorData>* m_pLayer,bool clearLayer)
{
	if (!tknRoute)
		return false;


	if(!m_pLayer) 
	{
		PosSystem *system = pos_system_new(possys_geodetic);
		m_pLayer = new StoreVectorData<FastVectorData>(system);
		CctError* error = m_mapWidget->getGisView()->addGeoData(m_pLayer, false);
	
		if(! cct_error_success( error ) )
		{
			cct_error_free(error);
			delete m_pLayer;
			m_pLayer = NULL;
			return false;
		}
		pos_system_free(system);
		m_pLayer->clipTkn(TRUE);
		m_pLayer->deleteAll();
	}

	if(!m_pLayer) 
	{
		qDebug() << tr("слоя нет");
		return false;
	}
		
	if(clearLayer)
		m_pLayer->deleteAll();


	m_pLayer->addObject(tknRoute);
	m_mapWidget->redrawFull();
	return true;
}

bool RouteClass::drawRouteOnMap(QString path, MapBaseWidget *m_mapWidget,StoreVectorData<FastVectorData>* m_pLayer,bool clearLayer)
{
	if(_routeFromFile(path))
	{
		 return drawRouteOnMap(_tknRoute,m_mapWidget,m_pLayer,clearLayer);
	}
	return false;
}

bool RouteClass::_renameRoute(QString pathDir)
{
	QString path = pathDir + "\\tmp.routeTmp";
	QString name;
	QFile file(path);
	if(!file.open(QIODevice::ReadOnly))
	{
		qDebug() << tr("файла нет");
		return false;
	}

	int size = file.size ();
	file.close();

	FILE* stream2 = NULL;
	QByteArray ar = path.toLocal8Bit();
	char* path_char=ar.data();

	if( (stream2 = fopen( path_char , "rb" )) != NULL )
	{
		char* m_pBYTE=new char[size];

		long f = fread(m_pBYTE, sizeof(char), size, stream2);

		if(f<=0) return false;

		std::istrstream pFile((const char*)m_pBYTE, size);
		_tknRoute = new TknRoute();


		bool bRet =  _tknRoute->loadFromBlob(pFile);

		if(! bRet)	
		{
			qDebug() << tr("ошибка загрузки");
			delete [] m_pBYTE;
			return false;
		}
		name = tr(_tknRoute->GetNameTkn().c_str());
		delete [] m_pBYTE;
	}
	fclose(stream2);
	

	QFile newFile(path);
	if(!newFile.open(QIODevice::ReadWrite))
	{
		qDebug() << tr("файла нет");
		return false;
	}
	QString neTrackName;
	neTrackName = pathDir + "\\" + name +".route";
	QFile testFile(neTrackName);
	//убиваем файл если уже был с таким именем
	if(testFile.exists ())
	{
		testFile.remove ();
		testFile.close();
	}
	//переименуем временный файл в имя самой палетки
	newFile.rename(neTrackName);
	newFile.close();
	return true;
}

TknRoute* RouteClass::routeFromFile(QString path)
{
	TknRoute* tknRoute;
	tknRoute = NULL;
	QFile file(path);
	if(!file.open(QIODevice::ReadOnly))
	{
		qDebug() << tr("файла нет");
		return NULL;
	}
	int size = file.size ();
	file.close();

	FILE* stream2 = NULL;
	QByteArray ar = path.toLocal8Bit();
	char* path_char=ar.data();

	if( (stream2 = fopen( path_char , "rb" )) != NULL )
	{
		char* m_pBYTE=new char[size];
		long f = fread(m_pBYTE, sizeof(char), size, stream2);
		if(f<=0) return NULL;
		std::istrstream pFile((const char*)m_pBYTE, size);

		tknRoute = new TknRoute();

		bool bRet =  tknRoute->loadFromBlob(pFile);
		if(!bRet)	
		{
			qDebug() << tr("ошибка загрузки");
			delete [] m_pBYTE;
			return NULL;
		}
		delete [] m_pBYTE;
	}
	fclose(stream2);
	return tknRoute;
}

//получить палетку по пути
bool RouteClass::_routeFromFile(QString path)
{
	QFile file(path);
	if(!file.open(QIODevice::ReadOnly))
	{
		qDebug() << tr("файла нет");
		return false;
	}
	int size = file.size ();
	file.close();

	FILE* stream2 = NULL;
	QByteArray ar = path.toLocal8Bit();
	char* path_char=ar.data();

	if( (stream2 = fopen( path_char , "rb" )) != NULL )
	{
		char* m_pBYTE=new char[size];
		long f = fread(m_pBYTE, sizeof(char), size, stream2);
		if(f<=0) return false;
		std::istrstream pFile((const char*)m_pBYTE, size);
		delete _tknRoute;
		_tknRoute = NULL;
		_tknRoute = new TknRoute();

		bool bRet =  _tknRoute->loadFromBlob(pFile);
		if(!bRet)	
		{
			qDebug() << tr("ошибка загрузки");
			delete [] m_pBYTE;
			return false;
		}
		delete [] m_pBYTE;
	}
	fclose(stream2);
	return true;
}

QString RouteClass::renameRoute(QString pathDir)
{
	QString path = pathDir;
	QString name;
	QFile file(path);
	if(!file.open(QIODevice::ReadOnly))
	{
		qDebug() << tr("файла нет");
		return "null";
	}

	int size = file.size ();
	file.close();

	FILE* stream2 = NULL;
	QByteArray ar = path.toLocal8Bit();
	char* path_char=ar.data();

	if( (stream2 = fopen( path_char , "rb" )) != NULL )
	{
		char* m_pBYTE=new char[size];

		long f = fread(m_pBYTE, sizeof(char), size, stream2);

		if(f<=0) return "null";

		std::istrstream pFile((const char*)m_pBYTE, size);
		TknRoute* tknRoute;
		tknRoute = NULL;
		tknRoute = new TknRoute();

		bool bRet =  tknRoute->loadFromBlob(pFile);

		if(! bRet)	
		{
			qDebug() << tr("ошибка загрузки");
			delete [] m_pBYTE;
			return "null";
		}
		name = tr(_tknRoute->GetNameTkn().c_str());
		delete tknRoute;
		tknRoute = NULL;
		delete [] m_pBYTE;
	}
	fclose(stream2);
	

	QFile newFile(path);
	if(!newFile.open(QIODevice::ReadWrite))
	{
		qDebug() << tr("файла нет");
		return "null";
	}
	QString neTrackName;
	neTrackName = _pathToRoute + "\\" + name +".route";
	QFile testFile(neTrackName);
	//убиваем файл если уже был с таким именем
	if(testFile.exists ())
	{
		testFile.remove ();
		testFile.close();
	}
	//переименуем временный файл в имя самой палетки
	newFile.rename(neTrackName);
	newFile.close();
	return neTrackName;

}

void RouteClass::_slotRouteLoaded(bool loaded)
{
	if(loaded)
	{
		if(!_loadSettings())
			return;
		QString pathToRoute(renameRoute(QCoreApplication::applicationDirPath ()+"/tmp/tmp.route"));
		emit signPathToRoute(pathToRoute);
	}
	else
		emit signPathToRoute(tr("null"));

}

bool RouteClass::_loadSettings()
{
	QString path_to_ini = QCoreApplication::applicationDirPath () + "/settings_akps.ini";
	if (QFile::exists(path_to_ini) ) 
	{
		QSettings akps_settings (path_to_ini, QSettings::IniFormat);
		_pathToRoute = akps_settings.value("programm/path_to_route").toString();		
		return true;
	}
	return false;
}

void RouteClass::_slotDrawRoute(QString path)
{
	if(_routeLayer)
		drawRouteOnMap(path,_map,_routeLayer,true);
}

bool RouteClass::drawRoute(QString path)
{
	if(_routeLayer)
		return drawRouteOnMap(path,_map,_routeLayer,true);
	else
		return false;
}

QString RouteClass::nmeaToTkn(QString pathToNmea, int meters)
{
	TknRoute* tknRoute;
	tknRoute = NULL;
	tknRoute = new TknRoute();
	QFileInfo file(pathToNmea);
	std::string nameTkn;
	//str = csv.section('_', 1, 1); 
	QString nameOfRoute = file.baseName().section('_', 1, 4);
	nameTkn = QString::number(meters).toStdString() + "_" + nameOfRoute.toStdString();
	tknRoute->SetNameTkn(nameTkn);
	tknRoute->SetWline(0.3);
	tknRoute->SetColorLine(Color::Blue);
	std::string note;
	note = QString::number(meters).toStdString();
	tknRoute->SetNoteTkn(note);



	double lastLat = 0;
	double lastLon = 0;

	QFile trackFile(pathToNmea);
	if(!trackFile.exists ())
		return NULL;

	if (!trackFile.open(QIODevice::ReadOnly | QIODevice::Text))
		return NULL;

	QTextStream trackStream;
	trackStream.setDevice(&trackFile);

	while (!trackStream.atEnd())
	{
		QString str = trackStream.readLine();
		QTime time(QTime::fromString(str.section(',', 1, 1), "hhmmss"));
		QDate date(QDate::fromString(str.section(',', 0, 0), "ddMMyyyy"));
		double altitude = str.section(',', 6, 6).toDouble();
		double lat = str.section(',', 2, 2).toDouble();
		double lon = str.section(',', 3, 3).toDouble();
		double speed = str.section(',', 7, 7).toDouble();

		//получаем расстояние между точками
		double dist = CalcFunctions::findRasstRadians(lastLat,lat,lastLon,lon)*1000;
		if ( dist > meters)
		{
			//выключаем флаг первый
			//добавляем точку
			tknRoute->AddPoint(lat,lon,time.hour(),time.minute(),time.second(),date.day(),date.month(),date.year(),altitude,speed);
			lastLat = lat;
			lastLon = lon;
		}
			
		
	}


	trackFile.close();

	std::ostrstream pFile;
	bool bSave=tknRoute->saveToBlob(pFile);//сохраняем знак в поток
	if(!bSave)  
		return "";

	long iLen=pFile.pcount(); //длинна знака
	if(iLen<1) 
		return "";
	

	BYTE* m_pChar =(BYTE*)pFile.str(); //записываем в байты
	FILE* stream = NULL;
	QString pathN;
	QFileInfo info(pathToNmea);
	pathN =  QCoreApplication::applicationDirPath () + "\\tmp\\" + info.baseName() + ".tkn";
	std::string path_to_file( pathN.toLocal8Bit().constData(), pathN.toLocal8Bit().length());
	stream = fopen(path_to_file.c_str() , "wb" );
	long f = fwrite(m_pChar, sizeof(char),  iLen, stream); 

	fclose(stream);
	
	delete tknRoute ;


	return pathN;

}

bool RouteClass::routeToFile(TknRoute* tknRoute,QString path)
{
	std::ostrstream pFile;

	bool bSave=tknRoute->saveToBlob(pFile);//сохраняем знак в поток
	
	if(!bSave)  
		return false;

	long iLen=pFile.pcount(); //длинна знака
	if(iLen<1) 
		return false;
	

	BYTE* m_pChar =(BYTE*)pFile.str(); //записываем в байты
	FILE* stream = NULL;
	std::string path_to_file( path.toLocal8Bit().constData(), path.toLocal8Bit().length());
	stream = fopen(path_to_file.c_str() , "wb" );
	long f = fwrite(m_pChar, sizeof(char),  iLen, stream); 
	fclose(stream);
	return true;
}
/*
TknRoute* m_pTknRoute;
	m_pTknRoute = NULL;
	m_pTknRoute = new TknRoute();

	QString fileName = getListOfTracks().at(index).absoluteFilePath();
	nmeaFile.setFileName(fileName);
	
	if (!nmeaFile.open(QIODevice::ReadOnly | QIODevice::Text))
		return "";
	else
	{
		myGpsStream.setDevice(&nmeaFile);
		//пока не конец файла брать новую точку из файла и рисовать ее на карте
		while (!myGpsStream.atEnd())
		{
			QString str = myGpsStream.readLine();
			if((str.startsWith("$GPGGA")) || (str.section(',', 0, 0) == "$GNGGA"))
			{
					GPGGA = func::gPGGADataHandler(str);
					//Pos2D pos2D = {GPGGA.shir*CCT_PI_180, GPGGA.dolg*CCT_PI_180};
					//получаем координаты
					PosBL locate1 = {GPGGA.shir*CCT_PI_180, GPGGA.dolg*CCT_PI_180};
					SYSTEMTIME st1;
					st1 =  func::qstrtosysdatetime(GPGGA.time,"000000");
						double dist = func::_findRasstRadians(locate.b,locate1.b,locate.l,locate1.l)*1000;

						if ( dist > meters || _first )
						{
							//выключаем флаг первый
							_first = false;
							//добавляем точку
							m_pTknRoute->AddPoint(locate1.b,locate1.l,st1.wHour,st1.wMinute,st1.wSecond);
							locate.l = locate1.l;
							locate.b = locate1.b;
							st = st1;
						}
			}
		}
		nmeaFile.close();
	}

	std::string nameTkn;
	SYSTEMTIME st2;
	GetLocalTime (&st2);
	QString date;
	//date = QString::number(st2.wDay) + "_" + QString::number(st2.wMonth) + "_" +  QString::number(st2.wYear) + "_" +  QString::number(st2.wHour)+ "_" +  QString::number(st2.wMinute);
	date = getListOfTracks().at(index).fileName() + tr("_")+ QString::number(meters);
	nameTkn = date.toStdString();
	m_pTknRoute->SetNameTkn(nameTkn);
	m_pTknRoute->SetWline(1.0);
	m_pTknRoute->SetColorLine(Color::Blue);
	m_pTknRoute->SetNoteTkn("note");

	std::ostrstream pFile;
	bool bSave=m_pTknRoute->saveToBlob(pFile);//сохраняем знак в поток
	if(!bSave)  
		return "";

	long iLen=pFile.pcount(); //длинна знака
	if(iLen<1) 
		return "";
	

	BYTE* m_pChar =(BYTE*)pFile.str(); //записываем в байты
	FILE* stream = NULL;
	QString pathN;
	pathN =  tmpPath + "\\tmp.tkn";
	std::string path_to_file( pathN.toLocal8Bit().constData(), pathN.toLocal8Bit().length());
	stream = fopen(path_to_file.c_str() , "wb" );
	long f = fwrite(m_pChar, sizeof(char),  iLen, stream); 

	fclose(stream);
	
	delete m_pTknRoute ;


	return pathN;

*/