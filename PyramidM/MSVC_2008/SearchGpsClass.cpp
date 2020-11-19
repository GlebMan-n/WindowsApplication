#include "stdafx.h"
#include "PyramidM\MSVC_2008\SearchGpsClass.h"
#include <fstream>
#include <iostream>
#include <conio.h>
#include <tchar.h>
#include "Windows.h"
#include <WinBase.h>

SearchGpsClass::SearchGpsClass()
{
	
}

SearchGpsClass::~SearchGpsClass()
{
	this->deleteLater();
}



void SearchGpsClass::run()
{		
	while(true)
	{
		QStringList result;
		result.clear();
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



		emit signListOfGpsPorts(result);
		break;
	}
}


short SearchGpsClass::isGpsCom(QString comPort, int brate) 
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