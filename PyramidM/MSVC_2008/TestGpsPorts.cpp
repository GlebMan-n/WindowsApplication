#include "stdafx.h"
#include <PyramidM\MSVC_2008\TestGpsPorts.h>
#include <PyramidM\MSVC_2008\GpsClass.h>

TestGpsPorts::TestGpsPorts(QObject* parent, QStringList ports)
{
	this->setParent(parent);
	_ports = ports;
	_gps = NULL;
	_ok = false;
	count = 0;
	counter = 0;
	_testPorts();
}

TestGpsPorts::~TestGpsPorts()
{}

void TestGpsPorts::_testPorts()
{
	size = _ports.size();	
	if(size > 0)
		_startGps(_ports.at(count));
	else
		emit signListOfGpsPorts(_portsOk);
}

void  TestGpsPorts::_startGps(QString comPort)
{
	QString _com = comPort;
	QString _brate = "8";
	QString _bsize = "9600";
	QString _par = "0";
	QString _stopb = "0";

	_stopGps();

	_gps = new QGpsLib("\\\\.\\"+_com, _brate, _bsize, _par, _stopb);
	connect(_gps,SIGNAL(signSendAll(QString,bool)),SIGNAL(_slotSSS(QString,bool)));

	if(_gps != NULL)
	{
		_gps->readDataFromGps(true);
	}

}

void TestGpsPorts::_slotSSS(QString string, bool check)
{
	if( string.startsWith("$GPGGA") || string.startsWith("$GPRMC") || string.startsWith("$GNGGA") || string.startsWith("$GNRMC") )
	{
		void _slotSuccess();
		_stopGps();
	}
	else
		count++;
	if(count>500)
	{
		void _slotFail();
		_stopGps();
	}
}

void TestGpsPorts::_slotFail()
{
	counter = 0;
	count +=1;
	if(count == size)
	{
		_stopGps();
		emit signListOfGpsPorts(_portsOk);
	}
	else
		_startGps(_ports.at(count));
}

void TestGpsPorts::_slotSuccess()
{
	counter = 0;
	_portsOk << _ports.at(count);
	count +=1;
	if(count == size)
	{
		_stopGps();
		emit signListOfGpsPorts(_portsOk);
	}
	else
		_startGps(_ports.at(count));
}

void  TestGpsPorts::_stopGps()
{
	if(_gps)
	{
		_gps->readDataFromGps(false);
		_gps->deleteLater();
		_gps->~QGpsLib();
		_gps = NULL;
	}
}