#include "stdafx.h"
#include <QObject>


class ExchangeDialog;
class SendDataClass;
class TimerClass;
class ExchangeDataClass : public QObject
{
	Q_OBJECT
public:
	ExchangeDataClass(QWidget *parent = 0);
	~ExchangeDataClass();
	static void _log(QString log);
private:
	bool _loadSettings();
	void _saveSettings();
	void _init();
	QString _loadMarks(QString path);
	QString _url(short index);
	
private:
	QWidget* _parent;

	QString _urlKcpsIN;
	QString _urlKcpsOUT;

	QString _urlVpuIN;
	QString _urlVpuOUT;

	QString _url_to_gkcpsIN;
	QString _url_to_gkcpsOUT;

	QString _url_to_shtabIN;
	QString _url_to_shtabOUT;

	QString _idDevice;
	QString _nameOfPc;
	QString _pathToMarks;
	short _toKcps;
	//максимальное количество адресов кцпс
	short _toKcpsMax;
	double _rasstOb;
	int _timePso;
	int _avSpeed;
	short _timeOfStop;
	short _timeToSendData;
	TimerClass* _timeToSend;
	QTimer* m_timer;
	SendDataClass* sendDataToKcps;
	SendDataClass* messQuery;
	bool _calcLoaded;
	bool _ErrSrv;
private slots:
	void _slotCalc(QTime sunrise, QTime sunset, short akb,int timePso, int avspeed,short timeOfStop,double rasstOb);
	void _slotGpsData(QTime time, QDate date, bool status, double lat, double lon, double speed, double angle,short sat, float height);
		//отправка сводных данных о протекающей миссии
	void _slotSendCurrentData(double lat, 
							  double lon, 
							  float height, 
							  QTime time,
							  QDate date, 
							  double rasstOb, 
							  int timePso, 
							  int avSpeed, 
							  short timeOfStop);

	void _slotFSendCurrentData(QString result);
//отправка по умолчанию с КЦПС 0 - КЦПС, 1 - ВПУ , 2 - ? ...
	//треки
	//слот отправки
	void _slotSendTrack(QString path,short toKcps = 0);
	//слот ответа
	void _slotFSendTrack(QString result);
	//отправить все данные скопом
	void _slotSendAllTracks(QStringList listOfTracks,short toKcps = 0);

	//фотографии
	void _slotSendPhoto(QString path, short toKcps = 0);
	void _slotFSendPhoto(QString result);
	void _slotSendAllPhotos(QStringList listOfPhotos,short toKcps = 0);

	//видео
	void _slotSendVideo(QString path, short toKcps = 0);
	void _slotFSendVideo(QString result);
	void _slotSendAllVideos(QStringList listOfVideos,short toKcps = 0);

	//отметки
	void _slotSendMarks(short toKcps = 0);
	void _slotFSendMarks(QString result);

	//донесения
	void _slotSendRaport(QString raport, short toKcps = 0);
	void _slotFSendRaport(QString result);

//прием
	//маршрут продвижения
	void _slotLoadRoute(short fromKcps = 0);
	void _slotFLoadRoute(QString result);

	//палетка поиска
	void _slotLoadGreed(short fromKcps = 0);
	void _slotFLoadGreed(QString result);

	//приметы
	void _slotLoadSigns(short fromKcps = 0);
	void _slotFLoadSigns(QString result);
	void _slotUpdateSettings();
	void _slotDiscret(short discret);

	//ответ на сообщение о прочтении
	void _slotMessageResponse(QString resp,short toKcps);
	void _slotFMessageResponse(QString result);

	void _slotMessageQuery(short toKcps);
	void _slotFMessageQuery(QString result);
	void _slotInitMesQuery();

	void _slotTestConnectionQuery();
	void _slotFTestConnectionQuery(QString result);

	
signals:
	//сигналы с результатами отправки единичных данных 
	void signSendTrackResult(bool sended);
	void signSendPhotoResult(bool sended);
	void signSendMarksResult(bool sended);
	void signSendVideoResult(bool sended);
	void signSendMessageResult(bool sended);

	//сигналы процесса отправки данных скопом
	void signSendTraksPro(short progress,short maximum);
	void signSendPhotoPro(short progress,short maximum);
	void signSendVideosPro(short progress,short maximum);

	//сигнал о получении сообщения
	void signMessage(QString message);
	void signResponseSended(bool sended);

	//сигналы о получении данных
	void signGridLoaded(bool loaded);
	void signRouteLoaded(bool loaded);
	void signSignsLoaded(bool result);

	void signCurrentDataSended(bool sended);
	void singUpdateSettings();


};