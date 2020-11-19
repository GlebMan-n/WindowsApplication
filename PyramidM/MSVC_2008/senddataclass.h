#ifndef SENDDATACLASS_H
#define SENDDATACLASS_H

#include <QObject>
#include <QNetworkReply>
 #include <QNetworkAccessManager>
#include <QXmlStreamReader>
#include <QByteArray>
#include <QString>


class SendDataClass : public QObject
{
	Q_OBJECT

public:
	SendDataClass(QObject *parent = 0);
	~SendDataClass();

	void sendData(QString temD);							//стандарт
	void sendData(QString temD, QString temPic);	//фото
	void sendData();												//трек (получение)
	void sendDataTrack(QString temTrack);						//трек (отправка)
	void sendVideoData(QString temD, QString temPic);	//отправка видео
	void queryPaletka(QString temD);
	void setParam(QString url, QString contentType, QString acceptEncoding, QString senderLogin, QString senderPassword, QString nameOfService, QString nameOfMethod, QString nameOfMethodResult);
	
private:
	QNetworkAccessManager* qNetworkAccessManager1;
	QNetworkAccessManager* qNetworkAccessManager2;
	QNetworkAccessManager* qNetworkAccessManager3;
	QNetworkAccessManager* qNetworkAccessManager4;
	QNetworkAccessManager* qNetworkAccessManager5;
	QNetworkAccessManager* qNetworkAccessManager6;
	QNetworkReply* qNetworkReply1;
	QNetworkReply* qNetworkReply2;
	QNetworkReply* qNetworkReply3;
	QNetworkReply* qNetworkReply4;
	QNetworkReply* qNetworkReply5;
	QNetworkReply* qNetworkReply6;
	QTime downloadTime;
	QString data1;
	QString data2;
	QString data3;
	QString data4;
	QString data5;
	QString data6;
	QByteArray body1;
	QByteArray body2;
	QByteArray body3;
	QByteArray body4;
	QByteArray body5;
	QByteArray body6;
	QWidget *parent_;
	
	//bool lock1;
	//bool lock2;
	//bool lock3;
	//bool lock4;
	//bool lock5;
	//bool lock6;
private slots:
	void finishedSendData1();
	void finishedSendData2();
	void finishedSendData3();
	void finishedSendData4();
	void finishedSendData5();
	void finishedSendData6();

	void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
	void downloadReadyRead1();
	void downloadReadyRead2();
	void downloadReadyRead3();
	void downloadReadyRead4();
	void downloadReadyRead5();
	void downloadReadyRead6();

	void finished1(QNetworkReply* reply);
	void finished2(QNetworkReply* reply);
	void finished3(QNetworkReply* reply);
	void finished4(QNetworkReply* reply);
	void finished5(QNetworkReply* reply);
	void finished6(QNetworkReply* reply);
signals:
	void signReturn(QString);
	void finished(int x);
	void signProgress(qint64 bytesReceived, qint64 bytesTotal,double speed);

public:
	
	struct paramsForNetworkReply
	{
		paramsForNetworkReply()
		{
			urlParam = tr("http://demo.spkb-lemz.ru/svc/TabletDataService.svc");
			nameOfMethodResult = tr("SaveDeviceStateResultResult");

			contentType = tr("text/xml;charset=UTF-8").toUtf8();
			acceptEncoding = tr("no-compression").toUtf8();
			senderLogin = tr("tablet1").toUtf8();
			senderPassword = tr("VPU2PC11").toUtf8();
			nameOfService = tr("http://tempuri.org/ITabletService").toUtf8();
			nameOfMethod = tr("SaveDeviceStateResult").toUtf8();
			temD = tr("1;2;3;").toUtf8();
		}
		QString urlParam;
		QString nameOfMethodResult;
		QByteArray contentType;
		QByteArray sOAPAction;
		QByteArray acceptEncoding;
		QByteArray senderLogin;
		QByteArray senderPassword;
		QByteArray temD;
		QByteArray nameOfService;
		QByteArray nameOfMethod;
		

	};

	paramsForNetworkReply par;
};

#endif // SENDDATACLASS_H
