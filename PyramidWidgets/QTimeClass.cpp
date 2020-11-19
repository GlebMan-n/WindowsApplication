
#include <QTimeClass.h>

QTimeClass::QTimeClass(QObject* parent)
{
	this->setParent(parent);
	stopped = true;
	_timeZone = 0;
	_updateTime = false;
	_timeToUpdate = 0;
	if(!_loadSettings())
		_saveSettings();
}

QTimeClass::~QTimeClass()
{
}

void QTimeClass::stop()
{
	stopped = true;
}

void QTimeClass::run()
{
	stopped = false;
	while(!stopped)	
	{	
		SYSTEMTIME st; 
		GetLocalTime(&st);
		QTime time(st.wHour,st.wMinute);
		QDate date(st.wYear, st.wMonth, st.wDay);
		QString stime = time.toString("HH:mm");
		QString sdate = date.toString("dd.MM.yyyy");
		QString day = date.toString("dddd");
		day[0] = day[0].toUpper(); 
		emit signTime(day,sdate,stime);
		this->msleep(50);
		_timeToUpdate--;
	}
}

bool QTimeClass::isStopped()
{
	bool ret = stopped;
	return ret;
}

void QTimeClass::_slotGpsData(QTime time, QDate date, bool status, double lat, double lon, double speed, double angle,short sat, float height)
{
	if(_timeToUpdate > 0)
	return;
	_timeToUpdate = 1200;
	QTime current(QTime::currentTime());

	if(current == time)
		return;

	_setTime(time);
}

void QTimeClass::_setTime(QTime timeUtc)
{
	SYSTEMTIME st;
	GetLocalTime(&st);

	//получаем время
	st.wHour = timeUtc.hour () + _timeZone;
	st.wMinute = timeUtc.minute ();
	st.wSecond = timeUtc.second ();

	SetLocalTime(&st);

}

bool QTimeClass::_loadSettings()
{
	QString path_to_ini = QCoreApplication::applicationDirPath () + "/settings_akps.ini";

	if (QFile::exists(path_to_ini) ) 
	{
		QSettings akps_settings (path_to_ini, QSettings::IniFormat);
		//получаем часовой пояс из файла ini
		short timeZone = akps_settings.value("programm/time_zone").toInt();
		
		
		if(_timeZone == timeZone)
			return true;
		else
		{
			//если новое значение отличается от старого
			_timeZone = timeZone;
			//даем разрешение на обновление общего времени системы
			_updateTime = true;

			return true;
		}
	}
	return false;
}

void QTimeClass::_saveSettings()
{
	QSettings akps_settings (QCoreApplication::applicationDirPath () + "/settings_akps.ini", QSettings::IniFormat);
	akps_settings.setValue("programm/time_zone", _timeZone);
	akps_settings.sync();
}

void QTimeClass::_slotUpdateSettings()
{
	SYSTEMTIME st;
	GetLocalTime(&st);
	//откатываем время до utc
	st.wHour = st.wHour - _timeZone;
	SetLocalTime(&st);
	_loadSettings();
	QTime time(QTime::currentTime());
	_setTime(time);
}