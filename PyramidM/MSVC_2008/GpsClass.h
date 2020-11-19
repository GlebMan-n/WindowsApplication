#include "stdafx.h"
#include <QtGui>
#include <QGpsLib\qgpslib.h>
#include <gis\GisBase\cgis\pos.h>

class QGpsLib;
class TimerClass;
class SearchGpsClass;

struct gpsPorts
{
	gpsPorts()
	{
		com = 0;
		sat = 0;

	}
	//расстояние
	short com;
	short sat;
};

class GpsClass : public QObject
{
	Q_OBJECT
public:
	GpsClass(QObject* parent);
	~GpsClass();
	static PosBL convSk42(double lat, double lon);
	QStringList scanComPorts();
	static short isGpsCom(QString comPort, int brate = 9600 );

private:
	QGpsLib* gps;
	short _sat;
	float _height;
	QString _com, _brate, _bsize, _par, _stopb;
	bool _test;
	short _badSign;
	short _numberPorts;
	short _currentPort;
	short _countOfCom;

	std::vector<QGpsLib*> qpss;
	std::vector<gpsPorts> ports;
	QTimer* m_timer;
	double _testLat;
	double _testLon;
	bool erasedPorts;
	SearchGpsClass* _searchGps;
private:
	bool _loadSettings();
	SYSTEMTIME qstrtosysdatetime(QString time, QString date );
	short _getbestGps(short com, short sat);
public slots:
	void slotDisconnectGps();
	void slotConnectToGps();
	
private slots:
	void _slotGetAll(QString string,bool isCheck);
	void _slotGpgga(GPGGAstruct gpgga, bool checkSum, short com);
	void _slotGprmc(GPRMCstruct gprmc, bool checkSum, short com);	
	void _slotTest();
	void _slotNumbOfGps();

	void _slotGetListGps(QStringList listOfPorts);
signals:
	void signNoGpsFound();
	void signBadSettingsFile();
	void signBadSign();
	void signSendAll(QString string,bool check);
	void signGps(QTime time, QDate date, bool status, double lat, double lon, double speed, double angle,short sat, float height, short com = -1);
	void signErrors(QString errors);
	void signNumbOfGps(short numb);
	void signNumbOfFindedGps(short numb);
	
};