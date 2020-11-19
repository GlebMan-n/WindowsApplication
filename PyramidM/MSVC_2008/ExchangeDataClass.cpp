#include "stdafx.h"
#include <QMainWindow>
#include <MSVC_2008\ExchangeDataClass.h>
#include <MSVC_2008\ExchangeDialogClass.h>
#include "MSVC_2008\senddataclass.h"
#include <MSVC_2008\PaletkaClass.h>
#include <MSVC_2008\TimerClass.h>

ExchangeDataClass::ExchangeDataClass(QWidget *parent)
{
	this->setParent(parent);
	_parent = parent;
	_calcLoaded = false;
	_ErrSrv = false;
	_toKcpsMax = 2;
	_idDevice = "null";
	_nameOfPc = "null";
	_rasstOb = 0;
	_timePso = 0;
	_avSpeed = 0;
	_timeOfStop = 0;
	_timeToSendData = 0;
	_timeToSend = NULL;
	sendDataToKcps = NULL;
	messQuery = NULL;
	_loadSettings();
	_timeToSend = new TimerClass(false);
	_timeToSend->start();

	m_timer = new QTimer(this);
	connect(m_timer, SIGNAL( timeout ()), 
		this, SLOT (_slotInitMesQuery()));
	m_timer->start(5000);
	sendDataToKcps = new SendDataClass(this);
	messQuery = new SendDataClass(this);
	connect(sendDataToKcps,SIGNAL(signReturn(QString)),this,SLOT(_slotFSendCurrentData(QString)));
	connect(messQuery,SIGNAL(signReturn(QString)),this,SLOT(_slotFMessageQuery(QString)));
}

ExchangeDataClass::~ExchangeDataClass()
{
	_saveSettings();
	if(_timeToSend) 
	{
		_timeToSend->~TimerClass();
	}
}

bool ExchangeDataClass::_loadSettings()
{
	QString path_to_ini = QCoreApplication::applicationDirPath () + "/settings_akps.ini";
	if (QFile::exists(path_to_ini) ) 
	{
		QSettings akps_settings (path_to_ini, QSettings::IniFormat);

		_idDevice = akps_settings.value("programm/id").toString();
		_nameOfPc = akps_settings.value("programm/pc_name").toString();
			
		//_idDevice = "tablet1";
		//_nameOfPc = tr("VPU2PC11");

		_urlKcpsIN = akps_settings.value("programm/url_to_kcpsIN").toString();
		_urlKcpsOUT = akps_settings.value("programm/url_to_kcpsOUT").toString();

		_urlVpuIN = akps_settings.value("programm/url_to_vpuIN").toString();
		_urlVpuOUT = akps_settings.value("programm/url_to_vpuOUT").toString();

		_url_to_gkcpsIN = akps_settings.value("programm/url_to_gkcpsIN").toString();
		_url_to_gkcpsOUT = akps_settings.value("programm/url_to_gkcpsOUT").toString();

		_url_to_shtabIN = akps_settings.value("programm/url_to_shtabIN").toString();
		_url_to_shtabOUT = akps_settings.value("programm/url_to_shtabOUT").toString();

		_toKcps = akps_settings.value("programm/def_serv").toInt();
		_timeToSendData = akps_settings.value("programm/time_to_send_data").toInt();
		_pathToMarks = akps_settings.value("programm/path_to_marks").toString();

		return true;
	}
	return false;
}

void ExchangeDataClass::_slotSendTrack(QString path,short toKcps )
{
	//отправляем трек под текущим индексом index

	SendDataClass sendTrack(this);
	ExchangeDialog dlgLoad(_parent,tr("Идет обмен данными,\nпожалуйста подождите..."));
	connect(&sendTrack,SIGNAL(signProgress(qint64,qint64,double)),&dlgLoad,SLOT(slotSetProgress(qint64,qint64,double)));
	connect(&sendTrack,SIGNAL(finished(int)),&dlgLoad,SLOT(done(int)));
	connect(&sendTrack,SIGNAL(signReturn(QString)),this,SLOT(_slotFSendTrack(QString)));
	
	QString url;
	url = _url(_toKcps);
	
	sendTrack.setParam(url,							//Адрес сервиса http
	tr("text/xml;charset=UTF-8"),				//Тип содержимого
	tr("no-compression"),		
	_idDevice, //Поддержка сжатия //Логин
	_nameOfPc,//_nameOfPc,					//Пароль
	tr("http://tempuri.org/ITabletService"),//Имя сервиса
	tr("SaveDeviceRealTrack"),					//Имя метода
	tr("SaveDeviceRealTrackResult"));			//Имя метода возврата результата
	
	QFile file(path);
	if (!file.open(QIODevice::ReadWrite))
		return;
	QString body;
	body = file.readAll().toBase64();//data.toBase64();

	sendTrack.sendDataTrack(body);
	dlgLoad.exec();
}

//слот ответа
void ExchangeDataClass::_slotFSendTrack(QString result)
{
	if(result == "true")
	{
		emit signSendTrackResult(true);
		return;
	}
	emit signSendTrackResult(false);
}

//отправить все данные скопом
void ExchangeDataClass::_slotSendAllTracks(QStringList listOfTracks, short toKcps)
{
	//получаем количество элементов в списке
	    for (int i = 0; i < listOfTracks.size(); ++i)
		{
			_slotSendTrack(listOfTracks.at(i),_toKcps);
			//отправляем по очереди каждый элемент списка
			signSendTraksPro(i+1,listOfTracks.size());
		}	
}

//фотографии
void ExchangeDataClass::_slotSendPhoto(QString path, short toKcps)
{
	
	SendDataClass _sendPhoto(this);
	QString url;
	url = _url(_toKcps);

	_sendPhoto.setParam(url,						//Адрес сервиса htpp
		tr("text/xml;charset=UTF-8"),			//Тип содержимого
		tr("no-compression"),					//Поддержка сжатия
		_idDevice,				//Логин
		_nameOfPc,			//Пароль
		tr("http://tempuri.org/ITabletService"),//Имя сервиса
		tr("SaveDevicePicture"),				//Имя метода
		tr("SaveDevicePictureResult"));			//Имя метода возврата результата
	
	ExchangeDialog dlgLoad(_parent,tr("Идет обмен данными,\nпожалуйста подождите..."));
	connect(&_sendPhoto,SIGNAL(signProgress(qint64,qint64,double)),&dlgLoad,SLOT(slotSetProgress(qint64,qint64,double)));
	connect(&_sendPhoto,SIGNAL(finished(int)),&dlgLoad,SLOT(done(int)));
	connect(&_sendPhoto,SIGNAL(signReturn(QString)),this,SLOT(_slotFSendPhoto(QString)));


	QString fileName;
	fileName = "";
	fileName = path;
	
	if(fileName == "") 
	{
		return;
	}

	//преобразуем файл
	QString name = fileName;
	QString day = name.section(',', 1, 1);
	QString month = name.section(',', 2, 2);
	QString year = name.section(',', 3, 3);
	QString hour = name.section(',', 4, 4);
	QString min = name.section(',', 5, 5);
	QString sec = name.section(',', 6, 6);
	QString lat = name.section(',', 7, 7);
	QString lon = name.section(',',8 , 8);


	QFile file(fileName);
	if (!file.open(QIODevice::ReadWrite))
		return;
	QString body;
	body = file.readAll().toBase64();//data.toBase64();
	QString id = tr("1000");
	QString any_text = "&lt;cur&gt;&lt;lon val=\"" + lon.toUtf8() + "\"/&gt;&lt;lat val=\"" + lat.toUtf8() + "\"/&gt;&lt;height val=\"0\"/&gt;&lt;date day=\"" + day.toUtf8() + "\" month=\"" + month.toUtf8() + "\" year=\""+ year.toUtf8()+"\"/&gt;&lt;time sec=\"" + sec.toUtf8() + "\" min=\"" + min.toUtf8() + "\" hour=\"" + hour.toUtf8() + "\" /&gt;&lt;id val=\"" + id.toUtf8() + "\"/&gt;&lt;/cur&gt;";
	_sendPhoto.sendData(any_text,body);
	dlgLoad.exec();
}

void ExchangeDataClass::_slotFSendPhoto(QString result)
{
	if(result == "true")
	{
		emit signSendPhotoResult(true);
		return;
	}
	emit signSendPhotoResult(false);
}

void ExchangeDataClass::_slotSendAllPhotos(QStringList listOfPhotos,short toKcps)
{
	for (int i = 0; i < listOfPhotos.size(); ++i)
	{
		QString path(listOfPhotos.at(i));
		_slotSendPhoto(path,_toKcps);
		//отправляем по очереди каждый элемент списка
		signSendPhotoPro(i+1,listOfPhotos.size());
	}	
}


//видео
void ExchangeDataClass::_slotSendVideo(QString path, short toKcps)
{
	SendDataClass sendVideo(this);
	QString url;
	url = _url(_toKcps);

	sendVideo.setParam(url,						//Адрес сервиса htpp
		tr("text/xml;charset=UTF-8"),			//Тип содержимого
		tr("no-compression"),					//Поддержка сжатия
		_idDevice,				//Логин
		_nameOfPc,			//Пароль
		tr("http://tempuri.org/ITabletService"),//Имя сервиса
		tr("SaveDeviceVideo"),				//Имя метода
		tr("SaveDeviceVideoResult"));			//Имя метода возврата результата
	
	ExchangeDialog dlgLoad(_parent,tr("Идет обмен данными,\nпожалуйста подождите..."));
	connect(&sendVideo,SIGNAL(signProgress(qint64,qint64,double)),&dlgLoad,SLOT(slotSetProgress(qint64,qint64,double)));
	connect(&sendVideo,SIGNAL(finished(int)),&dlgLoad,SLOT(done(int)));
	connect(&sendVideo,SIGNAL(signReturn(QString)),this,SLOT(_slotFSendVideo(QString)));
	
	QString name(path);
	
	if(name == "") 
	{
		return;
	}

	QString day = name.section(',', 1, 1);
	QString month = name.section(',', 2, 2);
	QString year = name.section(',', 3, 3);
	QString hour = name.section(',', 4, 4);
	QString min = name.section(',', 5, 5);
	QString sec = name.section(',', 6, 6);
	QString lat = name.section(',', 7, 7);
	QString lon = name.section(',', 8, 8);
	QFile file(name);
	if (!file.open(QIODevice::ReadWrite))
		return;

	QString body;
	body = file.readAll().toBase64();//data.toBase64();
	QString id = tr("1000");
	QString any_text = "&lt;cur&gt;&lt;lon val=\"" + lon.toUtf8() + "\"/&gt;&lt;lat val=\"" + lat.toUtf8() + "\"/&gt;&lt;height val=\"0\"/&gt;&lt;date day=\"" + day.toUtf8() + "\" month=\"" + month.toUtf8() + "\" year=\""+ year.toUtf8()+"\"/&gt;&lt;time sec=\"" + sec.toUtf8() + "\" min=\"" + min.toUtf8() + "\" hour=\"" + hour.toUtf8() + "\" /&gt;&lt;id val=\"" + id.toUtf8() + "\"/&gt;&lt;/cur&gt;";
	sendVideo.sendVideoData(any_text,body);
	dlgLoad.exec();
}

void ExchangeDataClass::_slotFSendVideo(QString result)
{
	if(result == "true")
	{
		emit signSendVideoResult(true);
		return;
	}
	emit signSendVideoResult(false);
}

void ExchangeDataClass::_slotSendAllVideos(QStringList listOfVideos,short toKcps)
{
	for (int i = 0; i < listOfVideos.size(); ++i)
	{
		QString path(listOfVideos.at(i));
		_slotSendVideo(path,_toKcps);
		//отправляем по очереди каждый элемент списка
		signSendVideosPro(i+1,listOfVideos.size());
	}	
}

//отметки

//загружаем из файла
QString ExchangeDataClass::_loadMarks(QString path)
{
	QString result(tr("&lt;mark&gt;&lt;id val=\"1000\"/&gt;"));
	QFile fileMarks(path);
	
	if (!fileMarks.open(QIODevice::ReadOnly | QIODevice::Text))return result;
	else
	{
		QTextStream myMarksStream;
		myMarksStream.setDevice(&fileMarks);
		//result.clear();
		//пока не конец файла брать новую точку из файла и рисовать ее на карте
		while (!myMarksStream.atEnd())
		{
			QString str = myMarksStream.readLine();
			
			QString type = str.section(',', 0, 0);//"001"; // получаем из списка
			QString date = str.section(',', 1, 1); //получаем текущую
			QString time = str.section(',', 2, 2); //получаем текущее
			QString info = str.section(',', 3, 3);
			QString lat = str.section(',', 4, 4); //gps
			QString lon = str.section(',', 5, 5); //gps
			QString height = str.section(',', 6, 6); //gps
			QTime qtime(QTime::fromString(time,"hh:mm:ss"));
			QDate qdate(QDate::fromString(date,"dd.MM.yyyy"));

			if(str.section(',', 7, 7) == "marks")
				result += "&lt;point type=\"" + type.toUtf8() + "\" info=\"" +  info + "\" day=\"" +  QString::number( qdate.day() ).toUtf8() + "\" month=\"" +  QString::number( qdate.month() ).toUtf8()  + "\" year=\"" +  QString::number( qdate.year() ).toUtf8()  + "\" sec=\"" +  QString::number( qtime.second() ).toUtf8()  +"\" min=\""+  QString::number( qtime.minute() ).toUtf8() +"\" hour=\"" +  QString::number( qtime.hour() ).toUtf8() +"\" lon=\"" + lon.toUtf8() + "\" lat=\"" + lat.toUtf8() + "\" height=\"" + height.toUtf8() + "\"/&gt;"; //собираем тело запроса

		}

		fileMarks.close();
		result += "&lt;/mark&gt;";
		return result;
	}
}

void ExchangeDataClass::_slotSendMarks(short toKcps)
{
	QString url;
	
	url = _url(_toKcps);

	SendDataClass sendMarks(this);
	ExchangeDialog dlgLoad(_parent,tr("Идет обмен данными,\nпожалуйста подождите..."));
	connect(&sendMarks,SIGNAL(signProgress(qint64,qint64,double)),&dlgLoad,SLOT(slotSetProgress(qint64,qint64,double)));
	connect(&sendMarks,SIGNAL(finished(int)),&dlgLoad,SLOT(done(int)));
	connect(&sendMarks,SIGNAL(signReturn(QString)),this,SLOT(_slotFSendRaport(QString)));

	sendMarks.setParam(url,			//Адрес сервиса http
							tr("text/xml;charset=UTF-8"),					//Тип содержимого
							tr("no-compression"),								//Поддержка сжатия
							_idDevice,									//Логин
							_nameOfPc,								//Пароль
							tr("http://tempuri.org/ITabletService"),	//Имя сервиса
							tr("SaveDevicePoint"),								//Имя метода
							tr("SaveDevicePointResult"));					//Имя метода возврата результата

	connect(&sendMarks,SIGNAL(signReturn(QString)),this,SLOT(_slotFSendMarks(QString)));
	
	QString markPath = _pathToMarks +"\\marks.file";
	QString temp(_loadMarks(markPath));
	sendMarks.sendData(temp);
	dlgLoad.exec();
}


void ExchangeDataClass::_slotFSendMarks(QString result)
{
	if(result == "true")
	{
		emit signSendMarksResult(true);

		return;
	}
	emit signSendMarksResult(false);
}



//донесения
void ExchangeDataClass::_slotSendRaport(QString raport, short toKcps)
{
	QString url;
	
	url = _url(_toKcps);

	SendDataClass sendRaport(this);
	ExchangeDialog dlgLoad(_parent,tr("Идет обмен данными,\nпожалуйста подождите..."));
	connect(&sendRaport,SIGNAL(signProgress(qint64,qint64,double)),&dlgLoad,SLOT(slotSetProgress(qint64,qint64,double)));
	connect(&sendRaport,SIGNAL(finished(int)),&dlgLoad,SLOT(done(int)));
	connect(&sendRaport,SIGNAL(signReturn(QString)),this,SLOT(_slotFSendRaport(QString)));

	sendRaport.setParam(url,			//Адрес сервиса http
							tr("text/xml;charset=UTF-8"),					//Тип содержимого
							tr("no-compression"),								//Поддержка сжатия
							_idDevice,									//Логин
							_nameOfPc,								//Пароль
							tr("http://tempuri.org/ITabletService"),	//Имя сервиса
							tr("SaveDeviceMessage"),								//Имя метода
							tr("SaveDeviceMessageResult"));					//Имя метода возврата результата

	//connect(&sendRaport,SIGNAL(signReturn(QString)),this,SLOT(_slotFSendCurrentData(QString)));

	sendRaport.sendData(raport);
	dlgLoad.exec();
}
void ExchangeDataClass::_slotFSendRaport(QString result)
{
	if(result == "true")
	{
		emit signSendMessageResult(true);
		return;
	}
	emit signSendMessageResult(false);
}

//прием
//маршрут продвижения

void ExchangeDataClass::_slotLoadRoute(short fromKcps)
{
	if(!_loadSettings())
		return;

	SendDataClass getTrack(this);
	ExchangeDialog dlgLoad(_parent,tr("Идет обмен данными,\nпожалуйста подождите..."));
	connect(&getTrack,SIGNAL(signProgress(qint64,qint64,double)),&dlgLoad,SLOT(slotSetProgress(qint64,qint64,double)));
	connect(&getTrack,SIGNAL(finished(int)),&dlgLoad,SLOT(done(int)));
	connect(&getTrack,SIGNAL(signReturn(QString)),this,SLOT(_slotFLoadRoute(QString)));
	QString url;
	url = _url(fromKcps);

		getTrack.setParam(url,						//Адрес сервиса http
			tr("text/xml;charset=UTF-8"),			//Тип содержимого
			tr("no-compression"),					//Поддержка сжатия
			_idDevice,						//Логин
			_nameOfPc,					//Пароль
			tr("http://tempuri.org/ITabletService"),//Имя сервиса
			tr("GetDevicePlanTrack"),				//Имя метода
			tr("GetDevicePlanTrackResult"));		//Имя метода возврата результата

	getTrack.sendData();
	dlgLoad.exec();
}

void ExchangeDataClass::_slotFLoadRoute(QString result)
{
	if(result == "false" ||  result.size() == 0)
	{

		emit  signRouteLoaded(false);
		return;
	}
	if(result.startsWith("error"))
	{
		emit signRouteLoaded(false);
		return;
	}

	QByteArray qba_tmp;
	qba_tmp = QByteArray::fromBase64(result.toAscii());
	QBuffer buf(&qba_tmp);
	
	QString path;
	QString path_html;
	path = QCoreApplication::applicationDirPath () + "/tmp/tmp.route";

	QFile file(path);
	if(!file.open(QIODevice::WriteOnly))
	{
		emit signRouteLoaded(false);
		return;
	}
	file.write(qba_tmp);
	file.copy(QCoreApplication::applicationDirPath ()+ "/data/pso/pso.route");
	file.close();
	emit signRouteLoaded(true);
}

//палетка поиска
void ExchangeDataClass::_slotLoadGreed(short fromKcps)
{
	if(!_loadSettings())
		return;

	SendDataClass getPaletka(this);
	ExchangeDialog dlgLoad(_parent,tr("Идет обмен данными,\nпожалуйста подождите..."));
	QString url;
	url = _url(fromKcps);

	getPaletka.setParam(url,					//Адрес сервиса http
							tr("text/xml;charset=UTF-8"),			//Тип содержимого
							tr("no-compression"),					//Поддержка сжатия
							_idDevice,						//Логин
							_nameOfPc,					//Пароль
							tr("http://tempuri.org/ITabletService"),//Имя сервиса
							tr("GetPaletka2"),						//Имя метода
							tr("GetPaletka2Result"));			//Имя метода возврата результата

	connect(&getPaletka,SIGNAL(signProgress(qint64,qint64,double)),&dlgLoad,SLOT(slotSetProgress(qint64,qint64,double)));
	connect(&getPaletka,SIGNAL(finished(int)),&dlgLoad,SLOT(done(int)));
	connect(&getPaletka, SIGNAL(signReturn(QString)),this, SLOT(_slotFLoadGreed(QString)));
	getPaletka.sendData();
	dlgLoad.exec();
}

void ExchangeDataClass::_slotFLoadGreed(QString result)
{
	if(result == "false" ||  result.size() == 0)
	{
		emit signGridLoaded(false);
		return;
	}

	if(result.startsWith("error"))
	{
		emit signGridLoaded(false);
		return;
	}

	QByteArray qba_tmp;
	qba_tmp = QByteArray::fromBase64(result.toAscii());
	QBuffer buf(&qba_tmp);
	
	QString path;
	path = QCoreApplication::applicationDirPath () + "/tmp/tmp.pal";

	QFile file(path);
	if(!file.open(QIODevice::WriteOnly))
	{
		emit signGridLoaded(false);
		return;
	}

	file.write(qba_tmp);
	file.copy(QCoreApplication::applicationDirPath ()+ "/data/pso/pso.pal");
	file.close();

	emit signGridLoaded(true);

}

//приметы
void ExchangeDataClass::_slotLoadSigns(short fromKcps)
{

}

void ExchangeDataClass::_slotFLoadSigns(QString result)
{

}

void ExchangeDataClass::_slotCalc(QTime sunrise, 
								  QTime sunset, 
								  short akb,
								  int timePso, 
								  int avspeed,
								  short timeOfStop,
								  double rasstOb)
{
	//рассчеты
	_avSpeed = avspeed;
	_timePso = timePso;
	_timeOfStop = timeOfStop;
	_rasstOb = rasstOb;
	_calcLoaded = true;

}

void ExchangeDataClass::_slotGpsData(QTime time, 
									 QDate date, 
									 bool status, 
									 double lat, 
									 double lon, 
									 double speed, 
									 double angle,
									 short sat, 
									 float height)
{
	
	//данные GPS
	if(_timeToSend->getTime() > _timeToSendData && _calcLoaded)
	{
		_slotSendCurrentData(lat,lon,height,time,date,_rasstOb,_timePso,_avSpeed,_timeOfStop);
		_timeToSend->setTime(0);
	}
}

void ExchangeDataClass::_slotSendCurrentData(double lat, 
											  double lon, 
											  float height, 
											  QTime time,
											  QDate date, 
											  double rasstOb, 
											  int timePso, 
											  int avSpeed, 
											  short timeOfStop)
{
	
	//отправка текущих данных в КЦПС
	QString login = _idDevice;//
	QString password = _nameOfPc;//tr("12345");//Имя компьютера
	QString lat_ =  QString::number(lat);//tr("03732.733");
	QString lon_	= QString::number(lon);//tr("5553.646");
	QString hei = QString::number(height);//tr("198.4");
	QString hour =  QString::number(time.hour()); //tr("17");
	QString min = QString::number(time.minute()); //tr("20");
	QString sec = QString::number(time.second()); //tr("48");
	QString day = QString::number(date.day());//tr("9");
	QString mon = QString::number(date.month());//tr("4");
	QString year = QString::number(date.year());//tr("2012");
	QString dist = QString::number(_rasstOb);  //tr("55.6");
	QString tom = QString::number(_timePso);//tr("55");
	QString avs = QString::number(_avSpeed);//tr("44");
	QString tos = QString::number(_timeOfStop);
	QString id = tr("1000");
	QString temD;
	temD = "&lt;cur&gt;&lt;lon val=\"" + lon_ + "\"/&gt;&lt;lat val=\"" + lat_ + "\"/&gt;&lt;height val=\"" + hei + "\"/&gt;&lt;date day=\"" + day + "\" month=\"" + mon + "\" year=\"" + year + "\"/&gt;&lt;time sec=\"" + sec + "\" min=\"" + min + "\" hour=\"" + hour + "\" /&gt;&lt;dist val=\"" + dist.toUtf8() + "\"/&gt;&lt;timeOfMission val=\"" + tom + "\"/&gt;&lt;avspeed val=\"" + avs.toUtf8() + "\"/&gt;&lt;timeOfStop val=\"" + tos + "\"/&gt;&lt;id val=\"" + id.toUtf8() + "\"/&gt;&lt;/cur&gt;";
	QString url;

	url = _url(_toKcps);
	sendDataToKcps->setParam(url,			//Адрес сервиса http
							tr("text/xml;charset=UTF-8"),					//Тип содержимого
							tr("no-compression"),								//Поддержка сжатия
							_idDevice,									//Логин
							_nameOfPc,								//Пароль
							tr("http://tempuri.org/ITabletService"),	//Имя сервиса
							tr("SaveDeviceState"),								//Имя метода
							tr("SaveDeviceStateResult"));					//Имя метода возврата результата
	_log("\n\n" + QTime::currentTime().toString("hh:mm:ss") +" send current GPS data:" + "\nURL: "+ url+ ": " + "\nLOGIN: " +login + "\nPASSWORD: " + password + "\nLAT: " + lat_ + "\nLON: "+ lon_+ "\nHEIGHT: " + hei+ "\nTOM: " + tom+ "\nAVS: " + avs+ "\nTOS: " + tos+ "\nEND.");
	sendDataToKcps->sendData(temD);
}

void ExchangeDataClass::_slotFSendCurrentData(QString result)
{
	//_log("\n" + QTime::currentTime().toString("hh:mm:ss") +" send current data response: " + result);
	if(result == "true")
	{

		return;
	}

	if(result == "false")
	{

	
		return;
	}

	if(result.startsWith("error"))
	{
		qDebug() << result;

		return;
	}

	if(result.isEmpty())
	{

		return;
	}

}

void ExchangeDataClass::_slotUpdateSettings()
{
	_loadSettings();
}

void ExchangeDataClass::_slotDiscret(short discret)
{
	_timeToSendData = discret;
}

void ExchangeDataClass::_slotMessageResponse(QString resp,short toKcps)
{
	QString url;
	
	url = _url(_toKcps);

	SendDataClass sendMessResp(this);
	ExchangeDialog dlgLoad(_parent,tr("Идет обмен данными,\nпожалуйста подождите..."));
	connect(&sendMessResp,SIGNAL(signProgress(qint64,qint64,double)),&dlgLoad,SLOT(slotSetProgress(qint64,qint64,double)));
	connect(&sendMessResp,SIGNAL(finished(int)),&dlgLoad,SLOT(done(int)));
	connect(&sendMessResp,SIGNAL(signReturn(QString)),this,SLOT(_slotFSendRaport(QString)));

	sendMessResp.setParam(url,			//Адрес сервиса http
							tr("text/xml;charset=UTF-8"),					//Тип содержимого
							tr("no-compression"),								//Поддержка сжатия
							_idDevice,									//Логин
							_nameOfPc,								//Пароль
							tr("http://tempuri.org/ITabletService"),	//Имя сервиса
							tr("SaveDeviceStateResult"),								//Имя метода
							tr("SaveDeviceStateResultResult"));					//Имя метода возврата результата

	connect(&sendMessResp,SIGNAL(signReturn(QString)),this,SLOT(_slotFMessageResponse(QString)));
	sendMessResp.sendData(resp);
	dlgLoad.exec();

}

void ExchangeDataClass::_slotFMessageResponse(QString result)
{
	if(result == "true")
	{
		emit signResponseSended(true);
		return;
	}
	emit signResponseSended(false);
}

QString ExchangeDataClass::_url(short index)
{
	QString url;
	switch(index)
	{
	case 0 :
		url = _urlKcpsOUT;
		break;
	case 1:
		url = _urlKcpsIN;
		break;
	case 2 :
		url = _url_to_shtabOUT;
		break;
	case 3:
		url = _url_to_shtabIN;
		break;
	case 4 :
		url = _urlVpuOUT;
		break;
	case 5:
		url = _urlVpuIN;
		break;
	case 6 :
		url = _url_to_gkcpsOUT;
		break;
	case 7:
		url = _url_to_gkcpsIN;
		break;
	}

	return ("http://" + url +  "/svc/TabletDataService.svc");
}

void ExchangeDataClass::_slotInitMesQuery()
{
	_slotMessageQuery(_toKcps);	
}

void ExchangeDataClass::_slotMessageQuery(short toKcps)
{
	QString url;
	url = _url(_toKcps);
	messQuery->setParam(url,			//Адрес сервиса http
							tr("text/xml;charset=UTF-8"),					//Тип содержимого
							tr("no-compression"),								//Поддержка сжатия
							_idDevice,									//Логин
							_nameOfPc,								//Пароль
							tr("http://tempuri.org/ITabletService"),	//Имя сервиса
							tr("GetNewMessages"),								//Имя метода
							tr("GetNewMessagesResult"));					//Имя метода возврата результата

	
	_log("\n\n" + QTime::currentTime().toString("hh:mm:ss") +" send message query: "+ "\nURL: "+ url + "\nLOGIN: " + _idDevice + "\nPASS: " + _nameOfPc + "\nEND.");
	messQuery->sendData();
}

void ExchangeDataClass::_slotFMessageQuery(QString result)
{
	if(result.startsWith("error") )
	{

		emit signCurrentDataSended(false);
		_log(_url(_toKcps) + " NOT AVAILABLE");
		bool err = _toKcps;
		_toKcps = !err;
		return;
	}

	_saveSettings();
	_log("\n" + QTime::currentTime().toString("hh:mm:ss") +" message data: " + result);
	//неверен пароль
	if(result.startsWith("false"))
	{	
		//сообщить, что пароль не тот
		_log("INVALID PASSWORD");
		_ErrSrv = false;
		emit signCurrentDataSended(false);
		return;
	}
	
	//не правильный сервер
	
	
	emit signCurrentDataSended(true);
	if(result.startsWith("true"))
		_ErrSrv = false;
	if(!( result.section("$",1,1) == "+"))
		return;
	_ErrSrv = false;
	int countOfBlocks = result.count("$+$+");

	for (int i = 0; i < countOfBlocks; i++)
	{

		QString mes = result;
		int number = mes.section("$+$+",i,i).toInt();
		i++;
		int type = mes.section("$+$+",i,i).toInt();
		i++;
		QString mess = mes.section("$+$+",i,i);
		i++;
		int id = mes.section("$+$+",i,i).toInt();
		//i++;
		//QString name_sender = mes.section("$+$+",i,i);

		QString path;
		path.clear();
		
		switch(type)
		{

		case 102:
			path = QString(QCoreApplication::applicationDirPath () + "/data/ref/ref102.file");				
			break;
		case 103:
			 path = QCoreApplication::applicationDirPath () + "/data/ref/ref103.file";
			break;
		case 104:
			path = QCoreApplication::applicationDirPath () + "/data/ref/ref104.file";
			break;

		}

		if(!(path.isEmpty() || path.isNull()))
		{
		
			QFile file(path);
			if(file.exists())
				file.remove ();
			if (file.open(QIODevice::WriteOnly | QIODevice::Text))
			{
				QTextStream out(&file);
				out << mess;
				file.close();
			}
		}
	}
	
	emit signMessage(result);
}

void ExchangeDataClass::_log(QString log)
{
	QFile file(QCoreApplication::applicationDirPath ()+ "/tmp/logfile.log");
	bool b_res = file.open(QIODevice::Append | QIODevice::Text);
	if(b_res)
	{
		QTextStream out(&file);
		out << log;
		file.close();
	}
}


void ExchangeDataClass::_saveSettings()
{
	QSettings akps_settings (QCoreApplication::applicationDirPath () + "/settings_akps.ini", QSettings::IniFormat);
	akps_settings.setValue("programm/def_serv", _toKcps);
	akps_settings.sync();
	singUpdateSettings();
}

void ExchangeDataClass::_slotTestConnectionQuery()
{

}

void ExchangeDataClass::_slotFTestConnectionQuery(QString result)
{

}