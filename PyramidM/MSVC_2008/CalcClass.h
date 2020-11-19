#include "stdafx.h"
#include <QtGui>
#include <gis\GisBase\cgis\pos.h>
class SunsetClass;
class TimerClass;

class CalcClass : public QObject
{
	Q_OBJECT
public:
	CalcClass(QObject* parent);
	~CalcClass();	
private:
	SunsetClass* _sunset;
	TimerClass* _timePso;
	TimerClass* _timeOfStop;
	double _rasstOb;
	double _lat;
	double _lon;
	unsigned long _sumspeed;
	int _counter;
	QTimer* _timer;

	int _psoTime;
	int _stopTime;
	int _avSpeed;

private:
	bool _loadSettings();
	void _saveSettings();
	short _getAkb();
	int _getTimePso();
	int _getAvSpeed();
	void _calcAvSpeed(int speed);
	short _getTimeOfStop();
	double _getRasstOb();
private slots:
	void _slotGpsData(QTime time, QDate date, bool status, double lat, double lon, double speed, double angle,short sat, float height);
	void _slotCalculates();
	void _slotCalcAzimuthTo(double distLat, double distLon);
	//очистим информацию о средней скорости, времени простоя и времени ПСО
	void _slotEraseData();
signals:
	//время восхода, заката
	void signCalculates(QTime sunrise, QTime sunset, short akb,int timePso, int avspeed,short timeOfStop, double rasstOb);
	void signAzimuth(double azimuth);
public:
	static double calcRasstFromLine(PosBL tekPoint,PosBL firstPoint,PosBL secondPoint);
	static double find_angle(double shirota, double shirota2, double dolgota, double dolgota2);
	
};