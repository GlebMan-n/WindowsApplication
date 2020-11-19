#ifndef QGPSTHREAD_H
#define QGPSTHREAD_H

#include <QThread>
#include "qreadwritelock.h"
#include "qfile.h"
#include "qtextstream.h"
#include "QString"
#include "Windows.h"
#include "structures.h"
#include <QReadWriteLock>

class QGpsThread : public QThread
{
	Q_OBJECT
public:
	//Задать параметры ком порта
	QGpsThread(QString qport, QString qbrate, QString qbsize, QString qparity, QString qstopbits);
	//параметры ком порта по умолчанию (3,9600,8,0,0)
	QGpsThread();
	~QGpsThread();
public:
	void stop();
	bool isStopped();
protected:
	void run();
private:
	volatile bool stopped;
	bool readGpsData();
	portset comprt; 
	HANDLE hComPort;
	DCB PortDCB;

signals:
	void signSendCurrentGpsString(QString gpsString);
	void signSendError(QString gpsString);
};

#endif // QGPSTHREAD_H
