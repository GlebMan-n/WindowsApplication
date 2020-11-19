	
#include <PyramidWidgets\GenGpsSensorsClass.h>
#include <PyramidM\MSVC_2008\CalcFunctions.h>

GeneralSensorsClass::GeneralSensorsClass(QWidget *parent)
{
	this->setParent(parent);
	setupUi(this);
	this->setGeometry(parent->geometry());
	connect(this->back_but,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	_initTime();
}
GeneralSensorsClass::~GeneralSensorsClass()
{

}

void GeneralSensorsClass::_slotTime(QString day, QString sdate, QString stime)
{
	this->day_lbl_sensor->setText(day);
	this->date_lbl_sensor->setText(sdate);
	this->time_lbl_sensor->setText(stime);
}

void GeneralSensorsClass::_slotGpsData(QTime time, QDate date, bool status, double lat, double lon, double speed, double angle,short sat, float height)
{
	_setSpeed(speed);
	_setCource(angle*CCT_180_PI);
	_setLat(lat);
	_setLon(lon);
	_setSat(sat);
	_setTimeUtc(time);
	_setHeight(height);
}

void GeneralSensorsClass::_slotSetCurrentData(QString string,bool check)
{
	if(string.startsWith("$GPGGA"))
	{
		float sat = string.section(',', 7, 7).toInt();
		if(sat < 4)
			_setSat((short)sat);
	}
}

//сводная информация GPS
void GeneralSensorsClass::_setSpeed(double speed)
{
	speed_lbl->setText(QString::number((int)speed) + tr(" км/ч"));
}
void GeneralSensorsClass::_setCource(double cource)
{
	true_course_lbl->setText(QString::number((int)cource) + tr(" °"));
}
void GeneralSensorsClass::_setLat(double lat)
{
	if(lat>0)
	{
		QString tmp;
		tmp = CalcFunctions::dDToDDMMSS(lat);
		tmp += tr("N");
		lat_lbl->setText(tmp);
	}
	else
	{
		QString tmp;
		tmp = CalcFunctions::dDToDDMMSS(lat);
		tmp += tr("S");
		lat_lbl->setText(tmp);
	}
}
void GeneralSensorsClass::_setLon(double lon)
{
	if(lon>0)
	{
		QString tmp;
		tmp =CalcFunctions::dDToDDMMSS(lon);
		tmp += tr("E");
		lon_lbl->setText(tmp);
	}
	else
	{
		QString tmp;
		tmp = CalcFunctions::dDToDDMMSS(lon);
		tmp += tr("W");
		lon_lbl->setText(tmp);
	}

}
void GeneralSensorsClass::_setSat(short sat)
{
	sat_lbl->setText(QString::number(sat));
}
void GeneralSensorsClass::_setTimeUtc(QTime time)
{
	time_UT_lbl->setText(time.toString("HH:mm:ss"));
}
void GeneralSensorsClass::_setHeight(double height)
{
	height_lbl->setText(QString::number(height) + tr(" м"));
}

//рассчетная информация
void GeneralSensorsClass::_setAvSpeed(int avSpeed)
{
	av_speed_lbl->setText(QString::number(avSpeed) + tr(" км/ч"));
}
void GeneralSensorsClass::_setTimePso(int timePso)
{
	time_pso_lbl->setText(QString::number(timePso) + QString(tr(" мин")));
}
void GeneralSensorsClass::_setTimeSunset(QTime timeSunset)
{
	sunset_lbl->setText(timeSunset.toString("HH:mm"));
}
void GeneralSensorsClass::_setTimeSunrise(QTime timeSunrise)
{
	sunrise_lbl->setText(timeSunrise.toString("HH:mm"));
}
void GeneralSensorsClass::_setBatState(short batState)
{
	bat_lbl->setText(QString::number(batState) + QString(tr("%")));
}

void GeneralSensorsClass::_slotCalc(QTime sunrise,QTime sunset,short akb,int timePso, int avspeed,short timeOfStop,double rasstOb)
{
	_setTimeSunset(sunset);
	_setTimeSunrise(sunrise);
	_setBatState(akb);
	_setAvSpeed(avspeed);
	_setTimePso(timePso);
}

void GeneralSensorsClass::_initTime()
{
	QString day, sdate, stime;
	SYSTEMTIME st; 
	GetLocalTime(&st);
	QTime time(st.wHour,st.wMinute);
	QDate date(st.wYear, st.wMonth, st.wDay);
	stime = time.toString("HH:mm");
	sdate = date.toString("dd.MM.yyyy");
	day = date.toString("dddd");
	day[0] = day[0].toUpper(); 
	_slotTime(day, sdate, stime);
}

void GeneralSensorsClass::_slotButtonClickSound()
{
	CalcFunctions::soundPressedBut(5);
}