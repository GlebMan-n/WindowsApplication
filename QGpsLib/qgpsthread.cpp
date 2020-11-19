#include "qgpsthread.h"
#include <fstream>
#include <iostream>
#include <conio.h>
#include <tchar.h>
#include "Windows.h"
#include <WinBase.h>

QGpsThread::QGpsThread(QString qport, QString qbrate, QString qbsize, QString qparity, QString qstopbits)
{
	stopped = true;
	comprt.qport = qport;
	comprt.qbsize = qbsize;
	comprt.qbrate = qbrate;
	comprt.qparity = qparity;
	comprt.qstopbits = qstopbits;
}
QGpsThread::QGpsThread()
{
	stopped = true;
}

QGpsThread::~QGpsThread()
{
	this->deleteLater();
}



bool QGpsThread::isStopped()
{	
	bool ret = stopped;
	return ret;
}


void QGpsThread::run()
{		
	stopped = false;
	while(!stopped)	
	{	
		//здесь читаем данные
		
		readGpsData();
		this->msleep(250);
	}
}

void QGpsThread::stop()
{

	stopped = true;

}

bool QGpsThread::readGpsData()
{
	QString error;
	error = "NO ERROR\n";
	hComPort = NULL;

	hComPort = CreateFileA(comprt.qport.toStdString().c_str(), GENERIC_READ , 0, NULL, OPEN_EXISTING, 0, NULL);


	if(!(hComPort == INVALID_HANDLE_VALUE))
	{
		PortDCB.DCBlength = sizeof(DCB);
		GetCommState(hComPort, &PortDCB);
		PortDCB.BaudRate = comprt.qbrate.toInt();
		PortDCB.ByteSize = comprt.qbsize.toInt();
		PortDCB.Parity =  comprt.qparity.toInt();
		PortDCB.StopBits = comprt.qstopbits.toInt();
	}

	else 
	{
		error = "INVALID_HANDLE_VALUE\n";
		emit signSendError(error);
		bool b_res;
		b_res  = CloseHandle(hComPort);
		//stopped = true;
		emit signSendError(error);
		return false;
	}

	if (!(SetCommState(hComPort, &PortDCB))) 
	{
		error = "INVALID_PORT_SETTINGS\n";
		emit signSendError(error);
		bool b_res;
		b_res  = CloseHandle(hComPort);
		//stopped = true;
		emit signSendError(error);
		return false;
	}

	COMMTIMEOUTS ComTimeouts;
    ComTimeouts.ReadIntervalTimeout = 100;
    ComTimeouts.ReadTotalTimeoutConstant = 100;
    ComTimeouts.ReadTotalTimeoutMultiplier = 100;

	
    if(!SetCommTimeouts(hComPort, &ComTimeouts))
    {
        error = "CANT SET TIMEOUTS\n";
		emit signSendError(error);
		bool b_res;
		b_res  = CloseHandle(hComPort);
		//stopped = true;
		emit signSendError(error);
        return false;
    }
	
	char buf[100];
	DWORD BytesRead;
	QString strTmp;
	strTmp = "";
	short counter = 150;
	while(counter > 0)
	//for(int y = 0; y< 40; y++)
	{
		counter--;
		if(stopped)
		{
			bool b_res;
			b_res  = CloseHandle(hComPort);
			return false;
		}
		
		if (ReadFile(hComPort, &buf, sizeof(buf), &BytesRead, NULL))
		{
			if (BytesRead == 0) 
			{
				bool b_res;
				b_res  = CloseHandle(hComPort);
				return false;
			}

			for (int i = 0; i < BytesRead; i++)
			{	
				strTmp += QString(buf[i]);

				if(buf[i] == 0x0A) 
				{
					if(strTmp.startsWith("$"))
					{
						emit signSendCurrentGpsString(strTmp);
						counter = 150;
					}
					strTmp.clear();
				}
				
			}
		
		}
		else
		{
				QString error;
				error = "CANT READ DATA FROM COM PORT\n";
				emit signSendError(error);
				ZeroMemory(buf,sizeof(buf));
				bool b_res;
				b_res  = CloseHandle(hComPort);
				emit signSendError(error);
				return false;
		}
	
	}
	error = "NEW CONNECT TO COM PORT\n";
	emit signSendError(error);
	ZeroMemory(buf,sizeof(buf));
	bool b_res;
	b_res  = CloseHandle(hComPort);
	return false;

}
