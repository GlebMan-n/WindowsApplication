#ifndef QGPSLIB_H
#define QGPSLIB_H
#include "qgpslib_global.h"
#include "..\QGpsLib\qgpsthread.h"
#include <QDebug>
class GPSThread;
//класс GPS
class QGPSLIB_EXPORT QGpsLib : public QObject
{
	Q_OBJECT
public:
	QGpsLib(QString qport, QString qbrate, QString qbsize, QString qparity, QString qstopbits);
	QGpsLib();
	~QGpsLib();
	void readDataFromGps(bool read);
	bool isStopped();
	bool checkSum(QString *string);
private:
	double dDMM2D(QString str);
	GPRMCstruct gPRMCDataHandler(QString str);
	GPGGAstruct gPGGADataHandler(QString str);
	struct MyThread : public QThread { using QThread::msleep;};
	short _com;
protected:
	QGpsThread *gpsTr;
private slots:
	void slotGPS(QString string);
	void slotGPSerror(QString string);
signals:
	void signSendGpgga(GPGGAstruct gpgga, bool check,short com);
	void signSendGprmc(GPRMCstruct gprmc, bool check,short com);
	void signSendAll(QString allString,bool check);
	void signSendError(QString errorString);
};


#endif // QGPSLIB_H
