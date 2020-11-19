#include "stdafx.h"
#include <MSVC_2008\SunsetClass.h>

SunsetClass::SunsetClass(QObject* parent)
{
	this->setParent(parent);
	_sunset = QTime(0,0);
	_sunrise = QTime(0,0);
}

SunsetClass::~SunsetClass()
{
}

QTime SunsetClass::_calcSunsetTime(double lat, double lon, QTime time)
{
	SYSTEMTIME st;
	GetLocalTime(&st);
	double N1;
	double N2;
	double N3;
	double N;
	double lngHour;
	double t;
	double longitude = lon*CCT_180_PI;
	double latitude = lat*CCT_180_PI;
	double M;
	double L;
	double RA;
	double Lquadrant;
	double RAquadrant;
	double sinDec;
	double cosDec;
	double cosH;
	double zenith;
	double H;
	double T;
	zenith = -0.01454;
	double UT;
	double localOffset = 4;
	double localT;
	double x;

	//1 получаем  день года
	N1 = floor((double)(275 * st.wMonth / 9));
	N2 = floor((double)((st.wMonth + 9) / 12));
	x = floor( (double) (st.wYear / 4) );
	N3 = 1 + floor( (double) ((st.wYear - 4 * x + 2 )  / 3))	;
	N = N1 - (N2 * N3) + st.wDay - 30;

	//переводим долготу в часовое значение и вычисляем приблизительное время
	lngHour = longitude / 15;

	//желаем время заката
	t = N + ((18 - lngHour) / 24);

	 //calculate the Sun's mean anomaly
	M = (0.9856 * t) - 3.289;

	//calculate the Sun's true longitude
	
	L = M + (1.916 * sin(M*CCT_PI_180)) + (0.020 * sin(2 * M*CCT_PI_180)) + 282.634;
	L = L - 360;

	//calculate the Sun's right ascension

	RA = (atan(0.91764 * tan(L*CCT_PI_180)))* CCT_180_PI;
	

	//right ascension value needs to be in the same quadrant as L
	Lquadrant  = (floor( L/90)) * 90;
	RAquadrant = (floor(RA/90)) * 90;
	RA = RA + (Lquadrant - RAquadrant);

	//right ascension value needs to be converted into hours
	RA = RA / 15;

	//calculate the Sun's declination
	sinDec = 0.39782 * sin(L*CCT_PI_180);
	cosDec = cos(asin(sinDec));


	//calculate the Sun's local hour angle

	cosH = (zenith - (sinDec * sin(latitude*CCT_PI_180))) / (cosDec * cos(latitude*CCT_PI_180));
	
		H = acos(cosH)*CCT_180_PI;
	
	H = H / 15;
	//calculate local mean time of rising/setting
	T = H + RA - (0.06571 * t) - 6.622;
	// adjust back to UTC
	UT = T - lngHour;
	localT = UT ;//+ localOffset;
	
	if(localT<0) localT += 24;
	if(localT>24) localT -=24;
	int m_zak = (localT - (int)localT)*60;
	int h_zak = (int)localT;

	int m_cur = time.minute();
	int h_cur = time.hour();
	
	int m_do_zak = m_zak - m_cur;
	int h_do_zak = h_zak - h_cur;

	if (m_do_zak < 0)
	{
		m_do_zak +=60;
		h_do_zak -= 1;
	}

	if (h_do_zak < 0)
		h_do_zak +=24;

	QTime result(h_do_zak,m_do_zak);
 	return result;
}

QTime SunsetClass::_calcSunriseTime(double lat, double lon, QTime time)
{
	SYSTEMTIME st;
	GetLocalTime(&st);
	double N1;
	double N2;
	double N3;
	double N;
	double lngHour;
	double t;
	double longitude = lon*CCT_180_PI;
	double latitude = lat*CCT_180_PI;
	double M;
	double L;
	double RA;
	double Lquadrant;
	double RAquadrant;
	double sinDec;
	double cosDec;
	double cosH;
	double zenith;
	double H;
	double T;
	zenith = -0.01454;
	double UT;
	double localOffset = 4;
	double localT;
	double x;

	//1 получаем  день года
	N1 = floor((double)(275 * st.wMonth / 9));
	N2 = floor((double)((st.wMonth + 9) / 12));
	x = floor( (double) (st.wYear / 4) );
	N3 = 1 + floor( (double) ((st.wYear - 4 * x + 2 )  / 3))	;
	N = N1 - (N2 * N3) + st.wDay - 30;

	//переводим долготу в часовое значение и вычисляем приблизительное время
	lngHour = longitude / 15;

	//желаем время восхода
	t = N + ((6 - lngHour) / 24);

	 //calculate the Sun's mean anomaly
	M = (0.9856 * t) - 3.289;

	//calculate the Sun's true longitude
	
	L = M + (1.916 * sin(M*CCT_PI_180)) + (0.020 * sin(2 * M*CCT_PI_180)) + 282.634;
	L = L - 360;

	//calculate the Sun's right ascension

	RA = (atan(0.91764 * tan(L*CCT_PI_180)))* CCT_180_PI;
	

	//right ascension value needs to be in the same quadrant as L
	Lquadrant  = (floor( L/90)) * 90;
	RAquadrant = (floor(RA/90)) * 90;
	RA = RA + (Lquadrant - RAquadrant);

	//right ascension value needs to be converted into hours
	RA = RA / 15;

	//calculate the Sun's declination
	sinDec = 0.39782 * sin(L*CCT_PI_180);
	cosDec = cos(asin(sinDec));


	//calculate the Sun's local hour angle

	cosH = (zenith - (sinDec * sin(latitude*CCT_PI_180))) / (cosDec * cos(latitude*CCT_PI_180));
	

	H = acos(cosH)*CCT_180_PI;
	H = 360 - H;

	H = H / 15;
	//calculate local mean time of rising/setting
	T = H + RA - (0.06571 * t) - 6.622;
	// adjust back to UTC
	UT = T - lngHour;

	localT = UT ;//+ localOffset;
	if(localT<0) localT += 24;
	if(localT>24) localT -=24;

	int m_vosh = (localT - (int)localT)*60;
	int h_vosh = (int)localT;

	int m_cur = time.minute();
	int h_cur = time.hour();
	
	int m_do_vosh = m_vosh - m_cur;
	int h_do_vosh = h_vosh - h_cur;

	if (m_do_vosh < 0)
	{
		m_do_vosh +=60;
		h_do_vosh -= 1;
	}

	if (h_do_vosh < 0)
		h_do_vosh +=24;

	QTime result(h_do_vosh,m_do_vosh);
 	return result;
}

void SunsetClass::setGpsData(QTime time,double lat, double lon)
{
	_sunset = _calcSunsetTime(lat,lon,time);
	_sunrise = _calcSunriseTime(lat,lon,time);
	emit signSunrise(_calcSunriseTime(lat,lon,time),_calcSunsetTime(lat,lon,time));
}

QTime SunsetClass::getSunriseTime()
{
	return _sunrise;
}
QTime SunsetClass::getSunsetTime()
{
	return _sunset;
}