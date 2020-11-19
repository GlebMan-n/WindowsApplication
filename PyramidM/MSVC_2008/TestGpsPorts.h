#include "stdafx.h"
#include <QtGui>

class QGpsLib;

class TestGpsPorts : public QObject
{
	Q_OBJECT
public:
	TestGpsPorts(QObject* parent, QStringList ports);
	~TestGpsPorts();
private:
	void _testPorts();
	void  _startGps(QString comPort);
	void  _stopGps();
private:
	QStringList _portsOk;
	QStringList _ports;
	QGpsLib* _gps;
	QTimer* m_timer;
	int size;
	int count;
	int counter;
	bool _ok;
private slots:
	void _slotFail();
	void _slotSuccess();
	void _slotSSS(QString string, bool check);
signals:
	void signListOfGpsPorts(QStringList ports);
};