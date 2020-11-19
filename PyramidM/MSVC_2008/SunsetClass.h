#include "stdafx.h"
#include <QtGui>

class SunsetClass : public QObject
{
	Q_OBJECT
public:
	SunsetClass(QObject* parent);
	~SunsetClass();
	void setGpsData(QTime time, double lat, double lon);
	QTime getSunriseTime();
	QTime getSunsetTime();
private:
	QTime _calcSunsetTime(double lat, double lon, QTime time);
	QTime _calcSunriseTime(double lat, double lon, QTime time);
	QTime _sunset;
	QTime _sunrise;
signals:
	void signSunrise(QTime sunrise, QTime sunset);
};