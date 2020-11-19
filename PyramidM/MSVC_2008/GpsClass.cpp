#include "stdafx.h"
#include <PyramidM\MSVC_2008\GpsClass.h>
#include <gis\GisBase\cgis\transform.h>
#include <gis\GisBase\cgis\ellipsoid.h>
#include "Windows.h"
#include <QVector>
#include <PyramidM\MSVC_2008\TimerClass.h>
#include <QStringList>
#include "MSVC_2008\HotTabFunctions.h"
#include "PyramidM\MSVC_2008\SearchGpsClass.h"

//#include <vld/vld.h>
GpsClass::GpsClass(QObject* parent)
{
	this->setParent(parent);
	_sat = 0;
	_height = 0;
	gps = NULL;
	_searchGps = NULL;
	_test = false;
	_numberPorts = 0;
	_badSign = 5;
	_currentPort = 0;
	_countOfCom = 0;
	_testLat = 0.890552327236075;
	_testLon = 1.16126910868531;

	m_timer = new QTimer(this);
	connect(m_timer, SIGNAL( timeout ()), 
		this, SLOT (_slotTest()));

	m_timer->start(5000);
}


void GpsClass::slotConnectToGps()
{
	qpss.clear();
	//получаем список GPS
	QStringList listOfPorts;
	
	HotTabClass hotTab;
	if(!hotTab._getStatusGps())
		hotTab._gpsOn(true);

	//listOfPorts << scanComPorts();
	
	if(_searchGps)
	{
		delete _searchGps;
		_searchGps = NULL;
	}

	_searchGps = new SearchGpsClass();
	connect(_searchGps, SIGNAL(signListOfGpsPorts(QStringList)),SLOT(_slotGetListGps(QStringList)));
	_searchGps->start();
	
}

void GpsClass::_slotGetListGps(QStringList listOfPorts)
{
	//подключаемся к каждому из них и устанавливаем связь с слотом
	for(int i = 0; i < listOfPorts.size(); i++)
	{
		QGpsLib *gps;
		gps = new QGpsLib(tr("\\\\.\\")+listOfPorts.at(i),"9600","8","0","0");
		qpss.push_back(gps);
		connect(qpss.back(),SIGNAL(signSendError(QString)),SIGNAL(signErrors(QString)));
		connect(qpss.back(),SIGNAL(signSendAll(QString,bool)),SLOT(_slotGetAll(QString,bool)));
		connect(qpss.back(),SIGNAL(signSendGpgga(GPGGAstruct, bool,short)),SLOT(_slotGpgga(GPGGAstruct, bool,short)));
		connect(qpss.back(),SIGNAL(signSendGprmc(GPRMCstruct, bool,short)),SLOT(_slotGprmc(GPRMCstruct, bool,short)));
	}
	_countOfCom = qpss.size();
	if(qpss.size() > 0)
	{
		for (unsigned int i = 0; i < qpss.size(); i++)
		{
			qpss.at(i)->readDataFromGps(true);
		}	
	}
	emit signNumbOfGps(qpss.size());
	emit signNumbOfFindedGps(qpss.size());
}

void GpsClass::slotDisconnectGps()
{
	if(qpss.size() > 0)
	{
		for (unsigned int i = 0; i < qpss.size(); i++)
		{
			qpss.at(i)->readDataFromGps(false);
			qpss.at(i)->deleteLater();
			qpss.at(i)->~QGpsLib();
			qpss.at(i) = NULL;
		}
	}
	qpss.clear();
}

GpsClass::~GpsClass()
{
}

bool GpsClass::_loadSettings()
{
	QString path_to_ini = QCoreApplication::applicationDirPath () + "/settings_akps.ini";

	if (QFile::exists(path_to_ini) ) 
	{
		QSettings akps_settings (path_to_ini, QSettings::IniFormat);
		_com = akps_settings.value("gps/qport").toString();
		_brate = akps_settings.value("gps/qbrate").toString();
		_bsize  = akps_settings.value("gps/qbsize").toString();
		_par = akps_settings.value("gps/qparity").toString();
		_stopb = akps_settings.value("gps/qstopbits").toString();
		return true;
	}
	return false;
}

void GpsClass::_slotGpgga(GPGGAstruct gpgga, bool checkSum, short com)
{
	_test = true;
	if(checkSum || gpgga.observ != 0 )
	{
		_sat = (short)gpgga.satellite;
		if(_sat == 0)
			_sat = 1;
		_height = gpgga.visota;

		_currentPort = _getbestGps(com,_sat);
	}
}

void GpsClass::_slotGprmc(GPRMCstruct gprmc, bool checkSum, short com)
{
	if(_currentPort != com)
		return;

	//получаем дату и время
	SYSTEMTIME st;
	st = qstrtosysdatetime(gprmc.time,gprmc.data);
	QTime time(st.wHour,st.wMinute,st.wSecond);
	QDate date(st.wYear,st.wMonth,st.wDay);

	bool status = !gprmc.status;	
	double lat = gprmc.shir*CCT_PI_180;
	double lon = gprmc.dolg*CCT_PI_180;
	double speed = gprmc.speed;
	double angle = gprmc.napr*CCT_PI_180;
	short sat = _sat;
	
	float height = _height;
	PosBL bl = convSk42(lat,lon);
	lat = bl.b;
	lon = bl.l;

	//нужно определять лучший порт
	if(sat > 3)
		emit signGps(time, date, status, lat, lon,  speed, angle,sat, height,com);
	else
		emit signBadSign();

}


SYSTEMTIME GpsClass::qstrtosysdatetime(QString time, QString date )
{
	QString d;
	QString mo;
	QString y;
	d[0] = date[0];
	d[1] = date[1];
	mo[0] = date[2];
	mo[1] = date[3];
	y[0] = '2';
	y[1] = '0';
	y[2] = date[4];
	y[3] = date[5];

	QString h;
	QString m;
	QString s;
	h[0] = time[0];
	h[1] = time[1];
	m[0] = time[2];
	m[1] = time[3];
	s[0] = time[4];
	s[1] = time[5];
	
	short hour = h.toDouble(); //сделать поправку на часовой пояс
	short minute = m.toDouble();
	short sec = s.toDouble();
	short day = d.toDouble(); //сделать поправку на часовой пояс
	short year = y.toDouble();
	short month = mo.toDouble();

	SYSTEMTIME st; 
	GetLocalTime(&st);
	st.wHour = hour;
	st.wMinute = minute;
	st.wSecond = sec; 
	st.wDay = day;
	st.wMonth = month;
	st.wYear = year; 
	return st;
}

PosBL GpsClass::convSk42(double lat, double lon)
{
	PosBL posBL = {lat, lon};
	//преобразование в ск42
	// b - широта l - долгота до изменения: 55.39, 37.1, после изменения результаты: 55.39 37.1019
	TrmGeo2Geo* m_pTrmWGS84toKrass=trm_geo2geo_new(ellps_WGS84, ellps_krass);       
	PosBL bl_old, bl_new;
	bl_old.b=posBL.b;
	bl_old.l=posBL.l;
    trm_transform(m_pTrmWGS84toKrass, &bl_old, &bl_new, 1);
    posBL.b=bl_new.b;
    posBL.l=bl_new.l;
	if(m_pTrmWGS84toKrass) trm_transform_free(m_pTrmWGS84toKrass);
	return  posBL;
}


QStringList GpsClass::scanComPorts()
{
	QStringList result;
	QString comPort;
    HANDLE hCom = NULL;  
 
    for (int i = 1; i <= 255; ++i)  
    {  
		comPort = "COM" + QString::number(i);
		QString comport;
		if(i>9)
			comport = tr("\\\\.\\") + comPort;
		else
			comport = comPort;

		hCom = CreateFileA(comport.toStdString().c_str(),  
        GENERIC_READ|GENERIC_WRITE, // desired access should be read&write  
        0,                          // COM port must be opened in non-sharing mode  
        NULL,                       // don't care about the security  
        OPEN_EXISTING,              // IMPORTANT: must use OPEN_EXISTING for a COM port  
        0,                          // usually overlapped but non-overlapped for existance test  
        NULL);                      // always NULL for a general purpose COM port  
		
        if (INVALID_HANDLE_VALUE != hCom)  
		{
			CloseHandle(hCom);

			short res = 0;
		    res = isGpsCom(comport);
			if(res > 0)
				result << comPort;

              
		}
		else
		  CloseHandle(hCom);
    }  

	if( result.size() < 1 )
		emit signNoGpsFound();
	return result;
}


short GpsClass::isGpsCom(QString comPort, int brate) 
{
	HANDLE hComPort;
	DCB PortDCB;
	QString error;
	error = "NO ERROR\n";
	hComPort = NULL;

	QString comport;
	comport = tr("\\\\.\\") + comPort;
	hComPort = CreateFileA(comport.toStdString().c_str(), GENERIC_READ , 0, NULL, OPEN_EXISTING, 0, NULL);


	if(!(hComPort == INVALID_HANDLE_VALUE))
	{
		PortDCB.DCBlength = sizeof(DCB);
		GetCommState(hComPort, &PortDCB);
		
		QString qbsize = "8";
		QString qparity = "0";
		QString qstopbits = "0";

		PortDCB.BaudRate = brate;
		PortDCB.ByteSize = qbsize.toInt();
		PortDCB.Parity =  qparity.toInt();
		PortDCB.StopBits = qstopbits.toInt();
	}

	else 
	{
		error = "INVALID_HANDLE_VALUE\n";
		bool b_res;
		b_res  = CloseHandle(hComPort);
		return 0;
	}

	if (!(SetCommState(hComPort, &PortDCB))) 
	{
		bool b_res;
		b_res  = CloseHandle(hComPort);
		return 0;
	}

	COMMTIMEOUTS ComTimeouts;
    ComTimeouts.ReadIntervalTimeout = 100;
    ComTimeouts.ReadTotalTimeoutConstant = 100;
    ComTimeouts.ReadTotalTimeoutMultiplier = 100;

	
    if(!SetCommTimeouts(hComPort, &ComTimeouts))
    {
		bool b_res;
		b_res  = CloseHandle(hComPort);
        return 0;
    }
	
	char buf[100];
	DWORD BytesRead;
	QString strTmp;
	strTmp = "";
	short counter = 500;
	while(counter > 0)
	{
		counter--;
		
		if (ReadFile(hComPort, &buf, sizeof(buf), &BytesRead, NULL))
		{
			if (BytesRead == 0) 
			{
				bool b_res;
				b_res  = CloseHandle(hComPort);
				return 0;
			}

			for (unsigned int i = 0; i < BytesRead; i++)
			{	
				strTmp += QString(buf[i]);

				if(buf[i] == 0x0A) 
				{
					if(strTmp.startsWith("$GN"))
					{
						bool b_res;
						b_res  = CloseHandle(hComPort);
						return 2;
					}

					if(strTmp.startsWith("$GP"))
					{
						bool b_res;
						b_res  = CloseHandle(hComPort);
						return 1;
					}
					strTmp.clear();
				}
				
			}
		
		}
		else
		{
				QString error;
				error = "CANT READ DATA FROM COM PORT\n";
				ZeroMemory(buf,sizeof(buf));
				bool b_res;
				b_res  = CloseHandle(hComPort);
				return 0;
		}
	
	}
	error = "NEW CONNECT TO COM PORT\n";
	ZeroMemory(buf,sizeof(buf));
	bool b_res;
	b_res  = CloseHandle(hComPort);
	return 0;
}

void GpsClass::_slotGetAll(QString string,bool isCheck)
{
	if(isCheck)
	{
		emit signSendAll(string,isCheck);
	}
}

short GpsClass::_getbestGps(short com, short sat)
{
	short result = 0;
	bool comInPorts = false;
	unsigned int i = 0;
	for(; i < ports.size(); i++)
	{
		
		if(ports.at(i).com == com)
			comInPorts = true;
	}

	//добавляем порт в вектор если его не было раньше
	if(!comInPorts)
	{
		gpsPorts port;
		port.com = com;
		port.sat = sat;
		ports.push_back(port);
		return 0;
	}

	//добавляем данные о порте в вектор
	for(i = 0; i < ports.size(); i++)
	{
		short com_ = ports.at(i).com;
		if(com_ == com)
		{
			if(ports.at(i).sat == 0)
			{
				
				ports.at(i).sat = sat;
				return 0;
			}
		}
	}
	
	//выбираем лучший порт
	short bestSat = 0;
	for(i = 0; i < ports.size(); i++)
	{
		
		if(ports.at(i).sat > bestSat)
		{
			bestSat = ports.at(i).sat;
			result = ports.at(i).com;			
		}

		ports.at(i).sat = 0;
	}


	//выбираем строку с данными
	return result;
}

void GpsClass::_slotTest()
{
	if(_test)
		_test = false;
	else
		emit signErrors("NO DATA FROM COM PORT\n");
		
}

void GpsClass::_slotNumbOfGps()
{
	emit signNumbOfGps(qpss.size());
}