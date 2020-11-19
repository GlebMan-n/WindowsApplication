#include "stdafx.h"
#include <MSVC_2008\CalcClass.h>
#include <MSVC_2008\SunsetClass.h>
#include <MSVC_2008\TimerClass.h>
#include <PyramidM\MSVC_2008\CalcFunctions.h>


CalcClass::CalcClass(QObject* parent)
{
	_sunset = NULL;
	_timePso = NULL;
	_timeOfStop = NULL;
	_timer = NULL;
	_sumspeed = 0;
	_counter = 0;
	_rasstOb = 0;
	_lat = 0;
	_lon = 0;

	_psoTime = 0;
	_stopTime = 0;
	_avSpeed = 0;

	_sunset = new SunsetClass(this);
	_timePso = new TimerClass(this);
	_timePso->start();
	_timeOfStop = new TimerClass();
	_timeOfStop->start();

	_loadSettings();


	_timer = new QTimer(this);
	connect(_timer, SIGNAL( timeout ()), this, SLOT (_slotCalculates ()));
	_timer->start (3000);

}

CalcClass::~CalcClass()
{
	_psoTime = _getTimePso();
	_stopTime = _getTimeOfStop();
	_avSpeed = _getAvSpeed();
	_saveSettings();

	if(_timePso) 
	{
		_timePso->stop();
		delete _timePso;
		_timePso = NULL;
	}
	if(_timeOfStop) 
	{
		_timeOfStop->stop();
		delete _timeOfStop;
		_timeOfStop = NULL;

	}
	
	if(_sunset)
	{
		delete _sunset;
		_sunset = NULL;
	}
	if(_timer)
	{
		_timer->stop();
		delete _timer;
		_timer = NULL;
	}
}

void CalcClass::_slotGpsData(QTime time, QDate date, bool status, double lat, double lon, double speed, double angle,short sat, float height)
{	
	//расчеты

	//восход закат
	_sunset->setGpsData(time,lat,lon);
	_calcAvSpeed((int)speed);
	if(speed > 1)
		_timeOfStop->start();
	else
		_timeOfStop->stop();

	if(_lat == 0 && _lon == 0)
	{
		//еще не было данных
		_lat = lat;
		_lon = lon;
	}
	else
	{
		double rasst = CalcFunctions::findRasstRadians(_lat, lat,_lon,lon);
		//пока расстояние между точкам не станет больше 100 метров данные о передвижении
		//не сохраняются
		if(rasst > 0.1)
		{
 			_lat = lat;
			_lon = lon;
			_rasstOb += rasst;
		}
	}

}	

short CalcClass::_getAkb()
{
	SYSTEM_POWER_STATUS sps;
	GetSystemPowerStatus(&sps);
	short power;
	power = (short)sps.BatteryLifePercent;
	if (power > 100) power = 100;
	if (power < 0) power = 0;
	return power;

}

int CalcClass::_getTimePso()
{
	return _psoTime + _timePso->getTime();
}

void CalcClass::_calcAvSpeed(int speed)
{
	if (speed > -1)
	{
		_counter++;
		_sumspeed += speed;
	}

}

int CalcClass::_getAvSpeed()
{
	if(_counter>0)
		return  (_avSpeed + _sumspeed/_counter)/2;
	else 
		return _avSpeed;
}

short CalcClass::_getTimeOfStop()
{
	return _stopTime + _timeOfStop->getTime();
}

double CalcClass::_getRasstOb()
{
	return _rasstOb;
}


void CalcClass::_slotCalculates()
{
	try
	{
		QTime time(QTime::currentTime());
		_sunset->setGpsData(time,_lat,_lon);
		emit signCalculates(_sunset->getSunriseTime(),_sunset->getSunsetTime(),_getAkb(),_getTimePso(),_getAvSpeed(),_getTimeOfStop(),_getRasstOb());
	}
	catch(...)
	{

		CalcFunctions::logError("CalcClass::_slotCalculates");
	}
}

void CalcClass::_slotCalcAzimuthTo(double distLat, double distLon)
{
	double azimuth = 0;
	azimuth = CalcFunctions::calcTrueAngleRad(_lat,distLat,_lon,distLon);
	emit signAzimuth(azimuth*CCT_180_PI);
}

bool CalcClass::_loadSettings()
{
	QString path_to_ini = QCoreApplication::applicationDirPath () + "/settings_akps.ini";
	if (QFile::exists(path_to_ini) ) 
	{
		QSettings akps_settings (path_to_ini, QSettings::IniFormat);
		_lat = akps_settings.value("programm/lastLat").toDouble();
		_lon = akps_settings.value("programm/lastLon").toDouble();
		_psoTime = akps_settings.value("programm/timeOfPso").toDouble();
		_stopTime = akps_settings.value("programm/timeOfStop").toDouble();
		_avSpeed = akps_settings.value("programm/AVS").toDouble();
		return true;
	}
	return false;
}

void CalcClass::_saveSettings()
{
	QSettings akps_settings (QCoreApplication::applicationDirPath () + "/settings_akps.ini", QSettings::IniFormat);

	akps_settings.setValue("programm/timeOfPso", _psoTime);
	akps_settings.setValue("programm/timeOfStop", _stopTime);
	akps_settings.setValue("programm/AVS", _avSpeed);

	akps_settings.sync();
}

void CalcClass::_slotEraseData()
{
	_psoTime = 0;
	_stopTime = 0;
	_avSpeed = 0;
	_saveSettings();
}

double CalcClass::calcRasstFromLine(PosBL tekPoint,PosBL firstPoint,PosBL secondPoint)
	{
			/*
			1) Формула Геррона.

				Собственно, эта формула нам говорит, что при наличии длин трёх сторон (они у нас есть - это OA, OB и AB) мы будем знать площадь треугольника.

			double dist_p_s(double ox, double oy, double oz, double ax, double ay, double az, double bx, double by, double bz){
				double p, s, a, b, c;
			    
				a=dist_p_p(ox,oy,oz,ax,ay,az);
				b=dist_p_p(ox,oy,oz,bx,by,bz);
				c=dist_p_p(ax,ay,az,bx,by,bz);
			    
				if(a>=b+c) return sqrt(b);
				if(b>=a+c) return sqrt(a);
			    
				a=sqrt(a); b=sqrt(b); c=sqrt(c);
				p=(a+b+c)/2;
				s=sqrt((p-a)*(p-b)*(p-c)*p);
			    
				return s*2/c;
			}
			*/
		double result;
		result = 0;
		double rasstTo0;
		double rasstTo1;
		double rasstFrom0To1;
		//находим квадраты расстояний между точками

		//Расстояние от текущей точки до первой точки отрезка
		//a
		rasstTo0 = CalcFunctions::findRasstRadians(tekPoint.b,firstPoint.b,tekPoint.l,firstPoint.l);
		rasstTo0 *= rasstTo0;
		//Расстояние от текущей точки до второй точки отрезка
		//b
		rasstTo1 =  CalcFunctions::findRasstRadians(tekPoint.b,secondPoint.b,tekPoint.l,secondPoint.l);
		rasstTo1 *= rasstTo1;
		//Расстояние от первой до второй точки отрезка
		//c
		rasstFrom0To1 =  CalcFunctions::findRasstRadians(firstPoint.b,secondPoint.b,firstPoint.l,secondPoint.l);
		rasstFrom0To1 *= rasstFrom0To1;

		if(rasstTo0 >= rasstTo1 + rasstFrom0To1) 
			return sqrt(rasstTo1);

		if(rasstTo1>=rasstTo0+rasstFrom0To1)
			return sqrt(rasstTo0);

		rasstTo0 = sqrt(rasstTo0); 
		rasstTo1 = sqrt(rasstTo1); 
		rasstFrom0To1 = sqrt(rasstFrom0To1);
		double p=(rasstTo0+rasstTo1+rasstFrom0To1)/2;
		double s=sqrt((p-rasstTo0)*(p-rasstTo1)*(p-rasstFrom0To1)*p);
		result = s * 2/rasstFrom0To1;
		return result;

	}

	double CalcClass::find_angle(double shirota, double shirota2, double dolgota, double dolgota2)
	{
		double rad = 6372795;
		//получаем координаты точек в радианах
		double lat1 = shirota*CCT_PI/180;
		double lat2 = shirota2*CCT_PI/180;
		double long1 = dolgota*CCT_PI/180;
		double long2 = dolgota2*CCT_PI/180;
		//косинусы и синусы широт и разниц долгот
		double cl1 = cos(lat1);
		double cl2 = cos(lat2);
		double sl1 = sin(lat1);
		double sl2 = sin(lat2);
		double delta = long2 - long1;
		double cdelta = cos(delta);
		double sdelta = sin(delta);
		double x = (cl1*sl2) - (sl1*cl2*cdelta);
		double y = sdelta*cl2;
		double z = (atan(-y/x))*CCT_180_PI;//градусы = Радианы * CCT_180_PI; //радианы = градусы * CCT_PI_180;
		
		if (x < 0)  //влияет направление юг
		{
			z += 180;
		}
		z = (( fmod ((z + 180),360) - 180) * CCT_PI_180);
		double anglerad2 = z - ((2*CCT_PI)* ( floor(z/(2*CCT_PI)) ));
		//double angledeg = (anglerad2*180)/CCT_PI;
		
		return anglerad2;
		//return angledeg;
	}