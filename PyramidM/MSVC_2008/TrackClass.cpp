#include "stdafx.h"
#include <PyramidM\MSVC_2008\TrackClass.h>
#include <gis\GrUnits\TknMapScale.h>
#include <PyramidM\MSVC_2008\CalcFunctions.h>
#include <PyramidM\MSVC_2008\GpsClass.h>
#include <gis\GisBase\cgis\pos.h>
#include <MSVC_2008\TknRoute.h>

TrackClass::TrackClass(QObject* parent, StoreVectorData<FastVectorData>* layer)
{
	this->setParent(parent);
	_badSign = 5;
	_width = 0.7;
	_pointsSaved = 0;
	_isDataCurrent = 10;
	_loadSettings();
	_trackLayer = layer;

	_track = new TknSimpleLines();
	_trackTmp = new TknSimpleLines();

	
	_tknTrack = new TknRoute();
	_tknTrackTmp = new TknRoute();

	_tknTrackTmp->SetColorLine(GdiGis::Color::Green);

	_tknTrack->SetWline(_width);
	_tknTrackTmp->SetWline(_width);

	setTrackParam(_width,_color);
	_trackLayer->addObject(_track);
	_trackLayer->addObject(_trackTmp);
	_trackLayer->addObject(_tknTrack);
	_trackLayer->addObject(_tknTrackTmp);
	_trackTmp->setColor (GdiGis::Color::Green);
	_trackTmp->setWidth(1);

	_lat = 0;
	_lon = 0;

	if(_isCurrentTrackCreated())
		_drawTrack(_nameOfCurrentTrack(),100);
		
	_showTrack();
	
}

TrackClass::~TrackClass()
{
	_saveSettings();
}

void TrackClass::setTrackParam(double width, short color)
{
	if(!_track || !_tknTrack)
		return;
	switch(color)
	{
	case 0:
		_track->setColor (GdiGis::Color::Blue);
		_tknTrack->SetColorLine (GdiGis::Color::Blue);
		break;
	case 1:
		_track->setColor (GdiGis::Color::Red);
		_tknTrack->SetColorLine (GdiGis::Color::Red);
		break;
	case 2:
		_track->setColor (GdiGis::Color::Black);
		_tknTrack->SetColorLine (GdiGis::Color::Black);
		break;
	case 3:
		_track->setColor (GdiGis::Color::Purple);
		_tknTrack->SetColorLine (GdiGis::Color::Purple);
		break;
	case 4:
		_track->setColor (GdiGis::Color::Brown);
		_tknTrack->SetColorLine (GdiGis::Color::Brown);
		break;
	case 5:
		_track->setColor (GdiGis::Color::Yellow);
		_tknTrack->SetColorLine (GdiGis::Color::Yellow);
		break;
	case 6:
		_track->setColor (GdiGis::Color::Green);
		_tknTrack->SetColorLine (GdiGis::Color::Green);
		break;

	default:
		_track->setColor (GdiGis::Color::Black);
		_tknTrack->SetColorLine (GdiGis::Color::Black);
		break;
	}
	
	_track->setWidth(width);	
	_tknTrack->SetWline(width);
}

void TrackClass::_slotGpsData(QString string, bool isChecked)
{
	emit signTrackLogged(false);
	return;


	//////////////////////////////////
	emit signTrackLogged(false);
	//сохраняем полученную строку 
	//$GNGGA,094008.112,5645.2941,N,06048.3069,E,1,06,2.2,264.6,M,-9.4,M,,0000*5F
	if ( ( string.startsWith("$GPGGA") || string.startsWith("$GNGGA") ) && isChecked )
	{
		short observ =  string.section(',', 6, 6).toInt();
		if(observ == 0) 
		{
			//_badSign = 5;
			return;
		}

		short sat =  string.section(',', 7, 7).toInt();
		if(sat < 4)
		{
			//_badSign = 5;
			return;
		}



		if(_isCurrentTrackCreated())
			_logTrack(string, _nameOfCurrentTrack());
		else
			_logTrack(string, _createCurrentTrack());
	}
}

bool TrackClass::_logTrack(QString string, QString path)
{
	bool b_res;
	QFile file(path);
	b_res = file.open(QIODevice::Append | QIODevice::Text);
	if(b_res)
	{
		QTextStream out(&file);
		out << string;
		file.close();
		emit signTrackLogged(true);
	}
	return b_res;
}

bool TrackClass::_logTrack(QTime time, QDate date, bool status, double lat, double lon, double speed, double angle,short sat, float height, QString path,short com)
{
	

	bool b_res;
	QFile file(path);
	b_res = file.open(QIODevice::Append | QIODevice::Text);


	if(b_res)
	{
		QTextStream out(&file);
		QString string;
		//формируем строку
		string = date.toString("ddMMyyyy") + "," 
			   + time.toString("hhmmss") + "," 
			   + QString::number(lat) + ","
			   + QString::number(lon) + ","
			   + QString::number(angle) + ","
			   + QString::number(sat) + ","
			   + QString::number(height) + ","
			   + QString::number(speed) + ","
			   + "COM" + QString::number(com) + "\n";
		out << string;
		file.close();
		emit signTrackLogged(true);		
	}
	return b_res;
}

bool TrackClass::_loadSettings()
{
	QString path_to_ini = QCoreApplication::applicationDirPath () + "/settings_akps.ini";
	if (QFile::exists(path_to_ini) ) 
	{
		QSettings akps_settings (path_to_ini, QSettings::IniFormat);
		_pathToTrack = akps_settings.value("programm/path_to_track").toString();
		_nameOfPc =  akps_settings.value("programm/pc_name").toString();
		//параметры трека
		_format = akps_settings.value("track/format").toInt();
		_color = akps_settings.value("track/color").toInt();
		_width = akps_settings.value("track/width").toDouble();
		_discretType = akps_settings.value("track/discretType").toInt();
		_discret = akps_settings.value("track/discret").toInt();
		_autoBreak = akps_settings.value("track/autobreak").toInt();
		_pointsSaved = akps_settings.value("track/pointsSaved").toInt();
		if(_width < 0.3 || _width > 3)
			_width = 0.7;

		return true;
	}
	return false;
}

void TrackClass::_saveSettings()
{
	QSettings akps_settings (QCoreApplication::applicationDirPath () + "/settings_akps.ini", QSettings::IniFormat);
	akps_settings.setValue("track/pointsSaved", _pointsSaved);
	akps_settings.sync();
}

void TrackClass::_slotUpdateSettings()
{
	if(_loadSettings())
	{
		if(_width < 0.3 || _width > 3)
			_width = 0.3;
		setTrackParam(_width,_color);
		_showTrack();
	}
	
}

void TrackClass::_showTrack()
{
	if(!_tknTrack || !_tknTrackTmp || ! _track || !_trackTmp)
		return;

	switch(_format)
	{
		case 0:
			_tknTrack->show();
			_tknTrackTmp->show();
			_track->hide();
			_trackTmp->hide();
		break;
		
		case 1:
			_track->show();
			_trackTmp->show();
			_tknTrack->hide();
			_tknTrackTmp->hide();
		break;
		
		default:
			_tknTrack->show();
			_tknTrackTmp->show();
			_track->hide();
			_trackTmp->hide();
		break;
	}

	emit signFullRedrawMap();
}

void TrackClass::_slotBreakTrack(bool clear)
{
	if(!_tknTrack || !_tknTrackTmp || ! _track || !_trackTmp)
		return;

	if(_finishCurrentTrack())
	{
		if(clear)
			_clearTrack();
		else
		{
			_trackTmp->clear();
			_track->clone(_trackTmp);
			_tknTrackTmp->CopyFromTkn(_tknTrack);
			_clearTrack();
			_showTrack();
		}
		
		setTrackParam(_track->getWidth(),_color);
		emit signFullRedrawMap();
		_pointsSaved = 0;
		_saveSettings();
	}
}

bool TrackClass::_isCurrentTrackCreated()
{
	//находим трек с расширением *.current
	QFileInfoList trackList;
	QDir dir(_pathToTrack);
    trackList = dir.entryInfoList(QStringList()
        << "*.current",
	QDir::Files );

	//если находим возвращаем истину
	if(trackList.count() > 0) 
		return true;
	//если не находим возвращаем ложь
	else
		return false;
}

QString TrackClass::_nameOfCurrentTrack()
{
	//находим трек с расширением *.current
	//записываем его имя в переменную
	QFileInfoList trackList;
	QDir dir(_pathToTrack);
    trackList = dir.entryInfoList(QStringList()
        << "*.current",
	QDir::Files );

	QString name(trackList.at(0).absoluteFilePath());
	return name;
}

QString TrackClass::_createCurrentTrack()
{
	//создаем файл с расширением *.current
	//записываем и возвращаем его имя

	QString dateTime;
	QTime timeQt(QTime::currentTime());
	QDate dateQs(QDate::currentDate());

	dateTime = timeQt.toString("hhmmss") + "_" + dateQs.toString("ddMMyyyy");
	QString name(_pathToTrack + _nameOfPc + "_" + dateTime + ".current");

	QFile file(name);
	bool b_res = file.open(QIODevice::ReadWrite | QIODevice::Text);
	file.close();

	if (b_res)
		return name;
	else 
		return tr("");

}

bool TrackClass::_finishCurrentTrack()
{
	if(!_isCurrentTrackCreated())
		return false;
	//получаем имя текущего трека
	QString nameOfCurrentTrack(_nameOfCurrentTrack());
	//убираем его расширение
	nameOfCurrentTrack.chop(8);

	QString dateTime;
	QTime timeQt(QTime::currentTime());
	QDate dateQs(QDate::currentDate());

	dateTime = timeQt.toString("hhmmss") + "_" + dateQs.toString("ddMMyyyy");

	QString nameOfFinished(nameOfCurrentTrack + "_"+ dateTime + ".finished");

	QFile track(_nameOfCurrentTrack());

	if(track.open(QIODevice::ReadWrite))
	{
		track.rename(nameOfFinished);
		track.close();
		return true;
	}
	return false;
}

void TrackClass::setLayer(StoreVectorData<FastVectorData>* layer)
{
	_trackLayer = layer;
}

void TrackClass::_clearTrack()
{	
	if(!_tknTrack ||  ! _track )
		return;

	_track->clear();
	_tknTrack->vectorPoints.clear();
	_lat = 0;
	_lon = 0;
	emit signFullRedrawMap();
}


void TrackClass::_slotGpsData(QTime time, QDate date, bool status, double lat, double lon, double speed, double angle,short sat, float height,short com)
{
	if(!_tknTrack || !_tknTrackTmp || ! _track || !_trackTmp)
		return;

	if(_autoBreak == 4 || _autoBreak == 5)
	{
		//разбиение по времени.
		if(_autoBreak == 4)
		{
			if(_timeToBreak(720))
			{
				_slotBreakTrack(false);
			}
		}
		
		if(_autoBreak == 5)
		{
			if(_timeToBreak(1440))
			{
				_slotBreakTrack(false);
			}
		}
	}

	if(_autoBreak == 1 || _autoBreak == 2 || _autoBreak == 3)
	{

		//разбиение по точкам
		if(_autoBreak == 1)
			if(_pointsSaved > 10000)
				_slotBreakTrack(false);

		if(_autoBreak == 2)
			if(_pointsSaved > 15000)
				_slotBreakTrack(false);

		if(_autoBreak == 3)
			if(_pointsSaved > 20000)
				_slotBreakTrack(false);

	}

	emit signTrackLogged(false);

	if(sat > 3)
	{
		//если расстояние между точками больше 5 метров отображаем в знаке на карте
		double rasst = CalcFunctions::findRasstRadians(_lat,lat,_lon,lon) * 1000 ;
		
		if(rasst > 1000  )
		{
			if(_isDataCurrent > 1)
			{
				_isDataCurrent--;
				return;
			}
			
			
		}

		_isDataCurrent = 15;

		if ( 100 < rasst )
		{

			_addTrackPoint(lat,lon,_track,_tknTrack,time,date,height,speed);
			_lat = lat;
			_lon = lon;
		}


		if(_isCurrentTrackCreated())
		{
			_logTrack(time, date, status, lat, lon, speed, angle,sat, height, _nameOfCurrentTrack(),com);
			_pointsSaved++;
		}
		else
		{
			_logTrack(time, date, status, lat, lon, speed, angle,sat, height, _createCurrentTrack(),com);
			_pointsSaved++;
		}
	}
}

bool TrackClass::_addTrackPoint(double lat, double lon)
{
	if(!_tknTrack || !_tknTrackTmp || ! _track || !_trackTmp)
		return false;

	Pos2D pos2D = {lat, lon};
	if(lat != 0 && lon != 0)
	{
		_track->add(pos2D);
		SYSTEMTIME st;
		GetLocalTime(&st);
		_tknTrack->AddPoint(pos2D.e1,pos2D.e2,st.wHour,st.wMinute,st.wSecond);
		return true;
	}
	return false;
}

bool TrackClass::_addTrackPoint(double lat, double lon,TknSimpleLines* track, TknRoute* tknTrack,QTime time, QDate date, double altitude, double speed)
{
	Pos2D pos2D = {lat, lon};
	if(lat != 0 && lon != 0)
	{
		track->add(pos2D);
		tknTrack->AddPoint(pos2D.e1,pos2D.e2,time.hour(),time.minute(),time.second(), date.day(), date.month(), date.year(), altitude, speed);
		return true;
	}
	return false;
}

void TrackClass::_drawTrack(QString path,double discret, bool drawTime)
{
	if(!_tknTrack || !_tknTrackTmp || ! _track || !_trackTmp)
		return;

	QFile trackFile(path);
	if(!trackFile.exists ())
		return;

	if (!trackFile.open(QIODevice::ReadOnly | QIODevice::Text))
		return ;

	QTextStream trackStream;
	trackStream.setDevice(&trackFile);

	double latOld = 0;
	double lonOld = 0;

	while (!trackStream.atEnd())
	{
		QString str = trackStream.readLine();
		QTime time(QTime::fromString(str.section(',', 1, 1), "hhmmss"));
		double lat = str.section(',', 2, 2).toDouble();
		double lon = str.section(',', 3, 3).toDouble();
		double rasst = CalcFunctions::findRasstRadians(latOld,lat,lonOld,lon) * 1000 ;
		QDate date(QDate::fromString(str.section(',', 0, 0), "ddMMyyyy"));
		double altitude = str.section(',', 6, 6).toDouble();
		double speed = str.section(',', 7, 7).toDouble();
		if ( discret < rasst  )
		{
			latOld = lat;
			lonOld = lon;
			_addTrackPoint(lat,lon,_track,_tknTrack,time,date,altitude,speed);
		}					
	}
	trackFile.close();
	emit signFullRedrawMap();
	
}

void TrackClass::_drawTrack(QString path,TknSimpleLines* track,double discret)
{
	QFile trackFile(path);
	if(!trackFile.exists ())
		return;

	if (!trackFile.open(QIODevice::ReadOnly | QIODevice::Text))
		return ;

	QTextStream trackStream;
	trackStream.setDevice(&trackFile);
	double latOld = 0;
	double lonOld = 0;
	while (!trackStream.atEnd())
	{
		QString str = trackStream.readLine();
		QTime time(QTime::fromString(str.section(',', 1, 1), "hhmmss"));
		double lat = str.section(',', 2, 2).toDouble();
		double lon = str.section(',', 3, 3).toDouble();
		QDate date(QDate::fromString(str.section(',', 0, 0), "ddMMyyyy"));
		double altitude = str.section(',', 6, 6).toDouble();
		double speed = str.section(',', 7, 7).toDouble();
		double rasst = CalcFunctions::findRasstRadians(latOld,lat,lonOld,lon) * 1000 ;
		if ( discret < rasst  )
		{
			latOld = lat;
			lonOld = lon;
			_addTrackPoint(lat,lon,track,_tknTrackTmp,time,date,altitude,speed);
		}		
	}


	trackFile.close();
	emit signFullRedrawMap();
	
}

void TrackClass::_slotDrawTrack(QString trackPath,int discret)
{
	_trackTmp->clear();
	_drawTrack(trackPath,_trackTmp,discret);
}

void TrackClass::_slotClearTrack()
{
	_trackTmp->clear();
	_tknTrackTmp->vectorPoints.clear();

	emit signFullRedrawMap();
}

bool TrackClass::_timeToBreak(int minsToBreak)
{
	if(!_isCurrentTrackCreated())
		return false;
	QString path(_nameOfCurrentTrack());
	QDate date(_dateOfCreateCurrentTrack(path));
	QTime time(_timeOfCreateCurrentTrack(path));
	//если дата трека более дня
	int mins = 0;
	int days = date.daysTo(QDate::currentDate());
	if(days > 0)
	{		
		//дни в минутах
		mins = days * 1440;
	}
	int secs = time.secsTo(QTime::currentTime()) ;
	mins = mins + secs / 60;
	return minsToBreak < mins;
}

QTime TrackClass::_timeOfCreateCurrentTrack(QString fullPath)
{
	QTime result;
	QString tmp;
	QFileInfo info(fullPath);
	tmp = info.baseName();
	QString stime;
	stime = tmp.section('_', 1, 1); 
	result = QTime::fromString(stime, "hhmmss");
	return result;
}

QDate TrackClass::_dateOfCreateCurrentTrack(QString fullPath)
{
	QDate result;
	QString tmp;
	QFileInfo info(fullPath);
	tmp = info.baseName();
	QString sdate;
	sdate = tmp.section('_', 2, 2); 
	result = QDate::fromString(sdate,"ddMMyyyy");
	return result;
}