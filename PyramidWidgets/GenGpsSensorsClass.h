
#include "ui_GenSensorsGps.h"
#include <QtGui>

class GeneralSensorsClass : public QDialog, public Ui::SensorGpsMenu
{
	Q_OBJECT
public:
	GeneralSensorsClass(QWidget *parent = 0);
	~GeneralSensorsClass();
private:
	//сводная информация GPS
	void _setSpeed(double speed);
	void _setCource(double cource);
	void _setLat(double lat);
	void _setLon(double lon);
	void _setSat(short sat);
	void _setTimeUtc(QTime time);
	void _setHeight(double height);
	
	//рассчетная информация
	void _setAvSpeed(int avSpeed);
	void _setTimePso(int timePso);
	void _setTimeSunset(QTime timeSunset);
	void _setTimeSunrise(QTime timeSunrise);
	void _setBatState(short batState);
	void _initTime();

private slots:
	void _slotTime(QString day, QString sdate, QString stime);
	void _slotGpsData(QTime time, QDate date, bool status, double lat, double lon, double speed, double angle,short sat, float height);
	void _slotSetCurrentData(QString string,bool check);
	void _slotCalc(QTime sunrise,QTime sunset,short akb,int timePso, int avspeed,short timeOfStop,double rasstOb);
	void _slotButtonClickSound();
signals:
	void signBack();
};