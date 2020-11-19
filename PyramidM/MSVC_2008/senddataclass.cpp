#include "StdAfx.h"
#include "senddataclass.h"
#include <MSVC_2008\ExchangeDialogClass.h>
#include <MSVC_2008\ExchangeDataClass.h>

SendDataClass::SendDataClass(QObject *parent): QObject(parent)
{
	data1 = "";
	data2 = "";
	data3 = "";
	data4 = "";
	data5 = "";
	data6 = "";

	//lock1 = false;
	//lock2 = false;
	//lock3 = false;
	//lock4 = false;
	//lock5 = false;
	//lock6 = false;

	qNetworkReply1 = NULL;
	qNetworkReply2 = NULL;
	qNetworkReply3 = NULL;
	qNetworkReply4 = NULL;
	qNetworkReply5 = NULL;
	qNetworkReply6 = NULL;

	qNetworkAccessManager1 = new QNetworkAccessManager(this);
	connect(qNetworkAccessManager1, SIGNAL(finished(QNetworkReply*)), this, SLOT(finished1(QNetworkReply*)));
	qNetworkAccessManager2 = new QNetworkAccessManager(this);
	connect(qNetworkAccessManager2, SIGNAL(finished(QNetworkReply*)), this, SLOT(finished2(QNetworkReply*)));
	qNetworkAccessManager3 = new QNetworkAccessManager(this);
	connect(qNetworkAccessManager3, SIGNAL(finished(QNetworkReply*)), this, SLOT(finished3(QNetworkReply*)));
	qNetworkAccessManager4 = new QNetworkAccessManager(this);
	connect(qNetworkAccessManager4, SIGNAL(finished(QNetworkReply*)), this, SLOT(finished4(QNetworkReply*)));
	qNetworkAccessManager5 = new QNetworkAccessManager(this);
	connect(qNetworkAccessManager5, SIGNAL(finished(QNetworkReply*)), this, SLOT(finished5(QNetworkReply*)));

	qNetworkAccessManager6 = new QNetworkAccessManager(this);
	connect(qNetworkAccessManager6, SIGNAL(finished(QNetworkReply*)), this, SLOT(finished6(QNetworkReply*)));
	
}

SendDataClass::~SendDataClass()
{

}
//палетка, текущие данные, запрос сообщений
void SendDataClass::sendData(QString temD)
{
	//if(qNetworkReply1)
	//{
	//	delete qNetworkReply1;
	//	qNetworkReply1 = NULL;
	//}

	//if(lock1)
	//	return;
	//lock1 = true;
	QNetworkRequest request;
	par.temD = temD.toUtf8();
	par.sOAPAction = par.nameOfService + tr("/").toUtf8() + par.nameOfMethod;
	request.setUrl(QUrl(par.urlParam));
	request.setRawHeader("Content-Type", par.contentType);
	request.setRawHeader ("SOAPAction", par.sOAPAction);
	request.setRawHeader ("Accept-Encoding", par.acceptEncoding);
	
	
	body1 = "<soapenv:Envelope xmlns:soapenv=\"http://schemas.xmlsoap.org/soap/envelope/\" xmlns:tem=\"http://tempuri.org/\"><soapenv:Header/><soapenv:Body><tem:"+ par.nameOfMethod +"><!--Optional:--><tem:idd>"+ par.senderLogin +"</tem:idd><!--Optional:--><tem:hp>"+ par.senderPassword +"</tem:hp><!--Optional:--><tem:d>"+ par.temD +"</tem:d></tem:"+ par.nameOfMethod+"></soapenv:Body></soapenv:Envelope>";
	
	
	
	//qNetworkReply1 = 
	ExchangeDataClass::_log("\nqNetworkAccessManager1 post");
		qNetworkAccessManager1->post(request, body1 );	

	//connect(qNetworkReply1, SIGNAL(finished()),this, 
	//	SLOT(finishedSendData1()));

	//connect(qNetworkReply1, SIGNAL(downloadProgress(qint64,qint64)),
 //           SLOT(downloadProgress(qint64,qint64)));

	//connect(qNetworkReply1, SIGNAL(uploadProgress(qint64,qint64)),
 //           SLOT(downloadProgress(qint64,qint64)));

	//connect(qNetworkReply1, SIGNAL(readyRead()),
    //        SLOT(downloadReadyRead1()));

	

	downloadTime.start();
}

void SendDataClass::sendData(QString temD, QString temPic)
{
	//if(qNetworkReply2)
	//{
	//	delete qNetworkReply2;
	//	qNetworkReply2 = NULL;
	//}

	//if(lock2)
	//	return;
	//lock2 = true;
	QNetworkRequest request;
	par.temD = temD.toUtf8();
	par.sOAPAction = par.nameOfService + tr("/").toUtf8() + par.nameOfMethod;
	request.setUrl(QUrl(par.urlParam));
	request.setRawHeader("Content-Type", par.contentType);
	request.setRawHeader ("SOAPAction", par.sOAPAction);
	request.setRawHeader ("Accept-Encoding", par.acceptEncoding);
	
	
	body2 = "<soapenv:Envelope xmlns:soapenv=\"http://schemas.xmlsoap.org/soap/envelope/\" xmlns:tem=\"http://tempuri.org/\"><soapenv:Header/><soapenv:Body><tem:"+ par.nameOfMethod +"><!--Optional:--><tem:idd>"+ par.senderLogin +"</tem:idd><!--Optional:--><tem:hp>"+ par.senderPassword +"</tem:hp><!--Optional:--><tem:d>"+ par.temD +"</tem:d><tem:pic>" + temPic.toAscii() + "</tem:pic></tem:"+ par.nameOfMethod+"></soapenv:Body></soapenv:Envelope>";
	
		
	
	//qNetworkReply2 = 
	ExchangeDataClass::_log("\nqNetworkAccessManager2 post");
	qNetworkAccessManager2->post(request, body2);	
	

	/*connect(qNetworkReply2, SIGNAL(finished()),this, 
		SLOT(finishedSendData2()));

	connect(qNetworkReply2, SIGNAL(downloadProgress(qint64,qint64)),
            SLOT(downloadProgress(qint64,qint64)));

	connect(qNetworkReply2, SIGNAL(uploadProgress(qint64,qint64)),
            SLOT(downloadProgress(qint64,qint64)));*/

	//connect(qNetworkReply2, SIGNAL(readyRead()),
 //           SLOT(downloadReadyRead2()));

	downloadTime.start();
}


void SendDataClass::sendVideoData(QString temD, QString temPic)
{
	////if(qNetworkReply3)
	////{
	////	delete qNetworkReply3;
	////	qNetworkReply3 = NULL;
	////}

	//if(lock3)
	//	return;
	//lock3 = true;
	QNetworkRequest request;
	par.temD = temD.toUtf8();
	par.sOAPAction = par.nameOfService + tr("/").toUtf8() + par.nameOfMethod;
	request.setUrl(QUrl(par.urlParam));
	request.setRawHeader("Content-Type", par.contentType);
	request.setRawHeader ("SOAPAction", par.sOAPAction);
	request.setRawHeader ("Accept-Encoding", par.acceptEncoding);
	
	
	body3 = "<soapenv:Envelope xmlns:soapenv=\"http://schemas.xmlsoap.org/soap/envelope/\" xmlns:tem=\"http://tempuri.org/\"><soapenv:Header/><soapenv:Body><tem:"+ par.nameOfMethod +"><!--Optional:--><tem:idd>"+ par.senderLogin +"</tem:idd><!--Optional:--><tem:hp>"+ par.senderPassword +"</tem:hp><!--Optional:--><tem:d>"+ par.temD +"</tem:d><tem:vid>" + temPic.toAscii() + "</tem:vid></tem:"+ par.nameOfMethod+"></soapenv:Body></soapenv:Envelope>";
	
	
	//qNetworkReply3 = 
	ExchangeDataClass::_log("\nqNetworkAccessManager3 post");
		qNetworkAccessManager3->post(request, body3 );	
	

	/*connect(qNetworkReply3, SIGNAL(finished()),this, 
		SLOT(finishedSendData3()));

	connect(qNetworkReply3, SIGNAL(downloadProgress(qint64,qint64)),
            SLOT(downloadProgress(qint64,qint64)));

	connect(qNetworkReply3, SIGNAL(uploadProgress(qint64,qint64)),
            SLOT(downloadProgress(qint64,qint64)));*/


	/*connect(qNetworkReply3, SIGNAL(readyRead()),
            SLOT(downloadReadyRead3()));*/

	downloadTime.start();
}

void SendDataClass::sendDataTrack(QString temTrack)
{
	//if(qNetworkReply4)
	//{
	//	delete qNetworkReply4;
	//	qNetworkReply4 = NULL;
	//}
	//if(lock4)
	//	return;
	//lock4 = true;
	QNetworkRequest request;
	par.sOAPAction = par.nameOfService + tr("/").toUtf8() + par.nameOfMethod;
	request.setUrl(QUrl(par.urlParam));
	request.setRawHeader("Content-Type", par.contentType);
	request.setRawHeader ("SOAPAction", par.sOAPAction);
	request.setRawHeader ("Accept-Encoding", par.acceptEncoding);
	
	
	body4 = "<soapenv:Envelope xmlns:soapenv=\"http://schemas.xmlsoap.org/soap/envelope/\" xmlns:tem=\"http://tempuri.org/\"><soapenv:Header/><soapenv:Body><tem:"+ par.nameOfMethod +"><!--Optional:--><tem:idd>"+ par.senderLogin +"</tem:idd><!--Optional:--><tem:hp>"+ par.senderPassword +"</tem:hp><!--Optional:--><tem:d>1</tem:d><tem:track>" + temTrack.toAscii() + "</tem:track></tem:"+ par.nameOfMethod+"></soapenv:Body></soapenv:Envelope>";
	
	
	//qNetworkReply4 = 
	ExchangeDataClass::_log("\nqNetworkAccessManager4 post");
		qNetworkAccessManager4->post(request, body4 );	
	

	/*connect(qNetworkReply4, SIGNAL(finished()),this, 
		SLOT(finishedSendData4()));

	connect(qNetworkReply4, SIGNAL(downloadProgress(qint64,qint64)),
            SLOT(downloadProgress(qint64,qint64)));

	connect(qNetworkReply4, SIGNAL(uploadProgress(qint64,qint64)),
            SLOT(downloadProgress(qint64,qint64)));*/

	//connect(qNetworkReply4, SIGNAL(readyRead()),
 //           SLOT(downloadReadyRead4()));

	downloadTime.start();
}
//в том числе получение маршрут и сообщения
void SendDataClass::sendData()
{
	//if(qNetworkReply5)
	//{
	//	delete qNetworkReply5;
	//	qNetworkReply5 = NULL;
	//}
	//if(lock5)
	//	return;
	//lock5 = true;
	QNetworkRequest request;
	par.sOAPAction = par.nameOfService + tr("/").toUtf8() + par.nameOfMethod;
	request.setUrl(QUrl(par.urlParam));
	request.setRawHeader("Content-Type", par.contentType);
	request.setRawHeader ("SOAPAction", par.sOAPAction);
	request.setRawHeader ("Accept-Encoding", par.acceptEncoding);
		
	body5 = "<soapenv:Envelope xmlns:soapenv=\"http://schemas.xmlsoap.org/soap/envelope/\" xmlns:tem=\"http://tempuri.org/\"><soapenv:Header/><soapenv:Body><tem:"+ par.nameOfMethod +"><!--Optional:--><tem:idd>"+ par.senderLogin +"</tem:idd><!--Optional:--><tem:hp>"+ par.senderPassword +"</tem:hp></tem:"+ par.nameOfMethod+"></soapenv:Body></soapenv:Envelope>";
	
	
	//qNetworkReply5 = 
	ExchangeDataClass::_log("\nqNetworkAccessManager5 post");
		qNetworkAccessManager5->post(request, body5 );	
	

	/*connect(qNetworkReply5, SIGNAL(finished()),this, 
		SLOT(finishedSendData5()));

	connect(qNetworkReply5, SIGNAL(downloadProgress(qint64,qint64)),
            SLOT(downloadProgress(qint64,qint64)));

	connect(qNetworkReply5, SIGNAL(uploadProgress(qint64,qint64)),
            SLOT(downloadProgress(qint64,qint64)));*/

	//connect(qNetworkReply5, SIGNAL(readyRead()),
 //           SLOT(downloadReadyRead5()));

	downloadTime.start();
}

void SendDataClass::queryPaletka(QString temD)
{
	//if(qNetworkReply6)
	//{
	//	delete qNetworkReply6;
	//	qNetworkReply6 = NULL;
	//}

	//if(lock6)
	//	return;
	//lock6 = true;
	
	QNetworkRequest request;
	par.temD = temD.toUtf8();
	par.sOAPAction = par.nameOfService + tr("/").toUtf8() + par.nameOfMethod;
	request.setUrl(QUrl(par.urlParam));
	request.setRawHeader("Content-Type", par.contentType);
	request.setRawHeader ("SOAPAction", par.sOAPAction);
	request.setRawHeader ("Accept-Encoding", par.acceptEncoding);
	
	
	body6 = "<soapenv:Envelope xmlns:soapenv=\"http://schemas.xmlsoap.org/soap/envelope/\" xmlns:tem=\"http://tempuri.org/\"><soapenv:Header/><soapenv:Body><tem:"+ par.nameOfMethod +"><!--Optional:--><tem:idd>"+ par.senderLogin +"</tem:idd><!--Optional:--><tem:hp>"+ par.senderPassword +"</tem:hp><!--Optional:--><tem:d>"+ par.temD +"</tem:d></tem:"+ par.nameOfMethod+"></soapenv:Body></soapenv:Envelope>";
	
	
	
	//qNetworkReply6 = 
	ExchangeDataClass::_log("\nqNetworkAccessManager6 post");
		qNetworkAccessManager6->post(request, body6 );

	/*connect(qNetworkReply6, SIGNAL(finished()),this, 
		SLOT(finishedSendData6()));

	connect(qNetworkReply6, SIGNAL(downloadProgress(qint64,qint64)),
            SLOT(downloadProgress(qint64,qint64)));

	connect(qNetworkReply6, SIGNAL(uploadProgress(qint64,qint64)),
            SLOT(downloadProgress(qint64,qint64)));*/

	/*connect(qNetworkReply6, SIGNAL(readyRead()),
            SLOT(downloadReadyRead6()));*/

	

	downloadTime.start();
}


void SendDataClass::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
	 double speed = bytesReceived * 1000.0 / downloadTime.elapsed();
	 emit signProgress(bytesReceived, bytesTotal,speed);
}

void SendDataClass::finishedSendData1()
{
	if(qNetworkReply1)
	{
		delete qNetworkReply1;
		qNetworkReply1 = NULL;
	}
	return;

	if (qNetworkReply1->error())
	{
		emit signReturn (tr("error fail"));
		emit finished(0);
	}
	else 
	{
		QXmlStreamReader *xml = new QXmlStreamReader(data1);
		while (!xml->atEnd()) 
		{
			xml->readNext();
			if (xml->name()==par.nameOfMethodResult)
				data1 = xml->readElementText();
		}
	//	xml->~QXmlStreamReader();
		delete xml;
		xml = NULL;
		emit finished(1);
		emit signReturn (data1);		
		data1 = "";
		body1 = "";
	}
	
	
//	lock1 = false;
}

void SendDataClass::finishedSendData2()
{
	if(qNetworkReply2)
	{
		delete qNetworkReply2;
		qNetworkReply2 = NULL;
	}
	return;

	if (qNetworkReply2->error())
	{
		emit signReturn (tr("error fail"));
		emit finished(0);
	}
	else 
	{
		QXmlStreamReader *xml = new QXmlStreamReader(data2);
		while (!xml->atEnd()) 
		{
			xml->readNext();
			if (xml->name()==par.nameOfMethodResult)
				data2 = xml->readElementText();
		}
		xml->~QXmlStreamReader();
		emit finished(1);
		emit signReturn (data2);		
		data2 = "";
		body2 = "";
	}


    //lock2 = false;
}

void SendDataClass::finishedSendData3()
{
	if(qNetworkReply3)
	{
		delete qNetworkReply3;
		qNetworkReply3 = NULL;
	}
	return;

	if(qNetworkReply3)
	{
		delete qNetworkReply3;
		qNetworkReply3 = NULL;
	}
	return;

	if (qNetworkReply3->error())
	{
		emit signReturn (tr("error fail"));
		emit finished(0);
	}
	else 
	{
		QXmlStreamReader *xml = new QXmlStreamReader(data3);
		while (!xml->atEnd()) 
		{
			xml->readNext();
			if (xml->name()==par.nameOfMethodResult)
				data3 = xml->readElementText();
		}
		xml->~QXmlStreamReader();
		emit finished(1);
		emit signReturn (data3);		
		data3 = "";
		body3 = "";
	}

    //lock3 = false;
}

void SendDataClass::finishedSendData4()
{
	if(qNetworkReply4)
	{
		delete qNetworkReply4;
		qNetworkReply4 = NULL;
	}
	return;

	if (qNetworkReply4->error())
	{
		emit signReturn (tr("error fail"));
		emit finished(0);
	}
	else 
	{
		QXmlStreamReader *xml = new QXmlStreamReader(data4);
		while (!xml->atEnd()) 
		{
			xml->readNext();
			if (xml->name()==par.nameOfMethodResult)
				data4 = xml->readElementText();
		}
		xml->~QXmlStreamReader();
		emit finished(1);
		emit signReturn (data4);		
		data4 = "";
		body4 = "";
	}

    //lock4 = false;
}

void SendDataClass::finishedSendData5()
{
	if(qNetworkReply5)
	{
		delete qNetworkReply5;
		qNetworkReply5 = NULL;
	}
	return;

	if (qNetworkReply5->error())
	{
		emit signReturn (tr("error fail"));
		emit finished(0);
	}
	else 
	{
		QXmlStreamReader *xml = new QXmlStreamReader(data5);
		while (!xml->atEnd()) 
		{
			xml->readNext();
			if (xml->name()==par.nameOfMethodResult)
				data5 = xml->readElementText();
		}
		xml->~QXmlStreamReader();
		emit finished(1);
		emit signReturn (data5);		
		data5 = "";
		body5 = "";
	}
	//qNetworkReply5->abort();
	qNetworkReply5->deleteLater();
   // lock5 = false;
}

void SendDataClass::finishedSendData6()
{
	if(qNetworkReply6)
	{
		delete qNetworkReply6;
		qNetworkReply6 = NULL;
	}
	return;

	if (qNetworkReply6->error())
	{
		emit signReturn (tr("error fail"));
		emit finished(0);
	}
	else 
	{
		QXmlStreamReader *xml = new QXmlStreamReader(data6);
		while (!xml->atEnd()) 
		{
			xml->readNext();
			if (xml->name()==par.nameOfMethodResult)
				data6 = xml->readElementText();
		}
		xml->~QXmlStreamReader();
		emit finished(1);
		emit signReturn (data6);		
		data6 = "";
		body6 = "";
	}

	disconnect(qNetworkReply6, SIGNAL(finished()),this, 
		SLOT(finishedSendData6()));

	connect(qNetworkReply6, SIGNAL(downloadProgress(qint64,qint64)),
            SLOT(downloadProgress(qint64,qint64)));

	connect(qNetworkReply6, SIGNAL(uploadProgress(qint64,qint64)),
            SLOT(downloadProgress(qint64,qint64)));

	connect(qNetworkReply6, SIGNAL(readyRead()),
            SLOT(downloadReadyRead6()));

	qNetworkReply6->abort();
	qNetworkReply6->deleteLater();
 //   lock6 = false;
}

void SendDataClass::downloadReadyRead1()
{
	//тут ок
	QByteArray bytes = qNetworkReply1->readAll();
	QString string(QString::fromUtf8(bytes));
	data1 += string;

}

void SendDataClass::downloadReadyRead2()
{
	//тут ок
	QByteArray bytes = qNetworkReply2->readAll();
	QString string(QString::fromUtf8(bytes));
	data2 += string;

}

void SendDataClass::downloadReadyRead3()
{
	//тут ок
	QByteArray bytes = qNetworkReply3->readAll();
	QString string(QString::fromUtf8(bytes));
	data3 += string;

}

void SendDataClass::downloadReadyRead4()
{
	//тут ок
	QByteArray bytes = qNetworkReply4->readAll();
	QString string(QString::fromUtf8(bytes));
	data4 += string;

}

void SendDataClass::downloadReadyRead5()
{
	//тут ок
	QByteArray bytes = qNetworkReply5->readAll();
	QString string(QString::fromUtf8(bytes));
	data5 += string;

}

void SendDataClass::downloadReadyRead6()
{
	//тут ок
	QByteArray bytes = qNetworkReply6->readAll();
	QString string(QString::fromUtf8(bytes));
	data6 += string;

}

void SendDataClass::setParam(QString url, 
							 QString contentType, 
							 QString acceptEncoding, 
							 QString senderLogin, 
							 QString senderPassword, 
							 QString nameOfService, 
							 QString nameOfMethod, 
							 QString nameOfMethodResult)
{
	//тут ок
	par.urlParam = url.toUtf8();
	par.contentType = contentType.toUtf8();
	par.acceptEncoding = acceptEncoding.toUtf8();
	par.senderLogin = senderLogin.toUtf8();
	par.senderPassword = senderPassword.toUtf8();
	par.nameOfService = nameOfService.toUtf8();
	par.nameOfMethod = nameOfMethod.toUtf8();
	par.nameOfMethodResult = nameOfMethodResult.toUtf8();
}


void SendDataClass::finished1(QNetworkReply* reply)
{
	
	if (reply->error())
	{
		emit signReturn (tr("error fail"));
		emit finished(0);
		ExchangeDataClass::_log("\nfinished1 result: " + reply->errorString());
	}
	else 
	{
		QByteArray bytes = reply->readAll();
		QString string(QString::fromUtf8(bytes));
		QXmlStreamReader *xml = new QXmlStreamReader(string);
		while (!xml->atEnd()) 
		{
			xml->readNext();
			if (xml->name()==par.nameOfMethodResult)
				string = xml->readElementText();
		}
		//xml->~QXmlStreamReader();
		delete xml;
		xml = NULL;
		emit finished(1);
		emit signReturn (string);	
		ExchangeDataClass::_log("\nfinished1 result: " + string + "\nOK.");
	}
	reply->deleteLater();
	//qNetworkReply1->deleteLater();
}

void SendDataClass::finished2(QNetworkReply* reply)
{
	
	if (reply->error())
	{
		emit signReturn (tr("error fail"));
		emit finished(0);
		ExchangeDataClass::_log("\nfinished2 result: " + reply->errorString());
	}
	else 
	{
		QByteArray bytes = reply->readAll();
		QString string(QString::fromUtf8(bytes));
		QXmlStreamReader *xml = new QXmlStreamReader(string);
		while (!xml->atEnd()) 
		{
			xml->readNext();
			if (xml->name()==par.nameOfMethodResult)
				string = xml->readElementText();
		}
	//	xml->~QXmlStreamReader();
		delete xml;
		xml = NULL;
		emit finished(1);
		emit signReturn (string);
		ExchangeDataClass::_log("\nfinished2 result: " + string + "\nOK.");
	}
	reply->deleteLater();
	//qNetworkReply2->deleteLater();
}

void SendDataClass::finished3(QNetworkReply* reply)
{
	
	if (reply->error())
	{
		emit signReturn (tr("error fail"));
		emit finished(0);
		ExchangeDataClass::_log("\nfinished3 result: " + reply->errorString());
	}
	else 
	{
		QByteArray bytes = reply->readAll();
		QString string(QString::fromUtf8(bytes));
		QXmlStreamReader *xml = new QXmlStreamReader(string);
		while (!xml->atEnd()) 
		{
			xml->readNext();
			if (xml->name()==par.nameOfMethodResult)
				string = xml->readElementText();
		}
		//xml->~QXmlStreamReader();
		delete xml;
		xml = NULL;
		emit finished(1);
		emit signReturn (string);	
		ExchangeDataClass::_log("\nfinished3 result: " + string + "\nOK.");
	}
	reply->deleteLater();
	//qNetworkReply3->deleteLater();
}

void SendDataClass::finished4(QNetworkReply* reply)
{
	
	if (reply->error())
	{
		emit signReturn (tr("error fail"));
		emit finished(0);
		ExchangeDataClass::_log("\nfinished4 result: " + reply->errorString());
	}
	else 
	{
		QByteArray bytes = reply->readAll();
		QString string(QString::fromUtf8(bytes));
		QXmlStreamReader *xml = new QXmlStreamReader(string);
		while (!xml->atEnd()) 
		{
			xml->readNext();
			if (xml->name()==par.nameOfMethodResult)
				string = xml->readElementText();
		}
		//xml->~QXmlStreamReader();
		delete xml;
		xml = NULL;
		emit finished(1);
		emit signReturn (string);	
		ExchangeDataClass::_log("\nfinished4 result: " + string + "\nOK.");
	}
	reply->deleteLater();
	//qNetworkReply4->deleteLater();
}

void SendDataClass::finished5(QNetworkReply* reply)
{
	if (reply->error())
	{
		emit signReturn (tr("error fail"));
		emit finished(0);
		ExchangeDataClass::_log("\nfinished5 result: " + reply->errorString());
	}
	else 
	{
		QByteArray bytes = reply->readAll();
		QString string(QString::fromUtf8(bytes));
		QXmlStreamReader *xml = new QXmlStreamReader(string);
		while (!xml->atEnd()) 
		{
			xml->readNext();
			if (xml->name()==par.nameOfMethodResult)
				string = xml->readElementText();
		}
		//xml->~QXmlStreamReader();
		delete xml;
		xml = NULL;
		emit finished(1);
		emit signReturn (string);	
		ExchangeDataClass::_log("\nfinished5 result: " + string + "\nOK.");
	}
	reply->deleteLater();
	//qNetworkReply5->deleteLater();
}

void SendDataClass::finished6(QNetworkReply* reply)
{
	
	if (reply->error())
	{
		ExchangeDataClass::_log("\nfinished6 result: " + reply->errorString());
		emit signReturn (tr("error fail"));
		emit finished(0);
	}
	else 
	{
		QByteArray bytes = reply->readAll();
		QString string(QString::fromUtf8(bytes));
		QXmlStreamReader *xml = new QXmlStreamReader(string);
		while (!xml->atEnd()) 
		{
			xml->readNext();
			if (xml->name()==par.nameOfMethodResult)
				string = xml->readElementText();
		}
		xml->~QXmlStreamReader();
		emit finished(1);
		emit signReturn (string);	
		ExchangeDataClass::_log("\nfinished6 result: " + string + "\nOK.");
	}
	reply->deleteLater();
	//qNetworkReply6->deleteLater();
}