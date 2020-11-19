#ifndef PYRAMIDWIDGETS_H
#define PYRAMIDWIDGETS_H


#include <QtGui>
#include <QGridLayout>
#include "pyramidwidgets_global.h"

class PyramidMediator;
class MapControlsClass;
class StartScreen;
class GeneralMenuForm;
class QTimeClass;
class MapMenuClass;
class GeneralGpsSetClass;
class CurrentGpsData;
class GeneralSensorsClass;
class GeneralWizardClass;
class MapRotateClass;
class MapSelectToolClass;
class MapScaleClass;
class MapExchangeClass;
class MapLockScreenClass;
class MapPhotoClass;
class GenRaportsClass;
class MapMarkClass;
class MapSendTrack;
class MapSendPhoto;
class MapVideoClass;
class MapNaviClass;
class MapViewNaviClass;
class MapMessageClass;
class SelectDialog;
class MapManualRoute;
class MapTrackSettings;
class MapSettingsClass;
class MapMapClass; 
class GeneralConnectionSetClass;
class MapHotTab;
class MapBrightness;
class MapObjects;
class MapMarkEdit;
class SelectDialog;
class MapReferanceClass;
class MapGsmLayers;
//класс пользовательских интерфейсов
class PYRAMIDWIDGETS_EXPORT PyramidWidgets : public QWidget
{
	Q_OBJECT
public:
	PyramidWidgets(QMainWindow* parent,double version);
	~PyramidWidgets();
	bool isGenMenuVisible();
	void updateMapControls();
	void init();
	bool isGeneralMenuShow();
	
private:
	void _showStartScreen(double vers = 1.0);
	void _destroyMenus();
	void _clickDestroyMenus();
	bool _isMenu();
	bool _raportsCoord;
private:
	QMainWindow* _parent;
	QWidget* _parentWgt;
	QWidget* _parent2;
	QTimeClass* _timeClass;
	StartScreen* _startScreen;
	GeneralMenuForm* _generalMenuForm;
	GeneralGpsSetClass* _generalGpsSett;
	MapControlsClass* _mapControls;
	MapMenuClass* _mapMenu;	
	MapRotateClass* _mapRotate;
	MapSelectToolClass* _mapSelectTool;
	MapScaleClass* _mapScale;
	MapExchangeClass* _mapExchange;
	MapLockScreenClass* _mapLock;
	GeneralWizardClass* _wizard;
	MapPhotoClass* _mapPhoto;
	GenRaportsClass* _genRaports;
	MapMarkClass* _mapMark;
	MapSendTrack* _sendTrack;
	MapSendPhoto* _sendPhoto;
	MapVideoClass* _mapVideo;
	MapManualRoute* _mapMRoute;
	//выбор типа навигации
	MapNaviClass* _mapNavi;
	//окно навигации
	MapViewNaviClass* _mapViewNavi;
	MapMessageClass* _mapMes;
	SelectDialog* _dlgGpsQwestion;
	MapTrackSettings* _mapTrack;
	MapSettingsClass* _mapSettings;
	MapMapClass* _mapMap;
	MapHotTab* _mapHotTab;
	MapBrightness* _mapBrightness;
	MapObjects* _mapObjects;
	MapGsmLayers* _mapGsmLayers;
	MapMarkEdit* _mapMarkEdit;
	MapReferanceClass* _mapRef;
	GeneralConnectionSetClass* _genConSet;
	double vers;
	struct pause : public QThread { using QThread::msleep;};
	bool _isFirstRun;
	bool _isNavi;
	bool _menuOpened;
	bool _wizzardPoint;
	bool _isTool;
	bool _isLocked;


	short _memoryLeakMaximumSize;

	SelectDialog* dlgInfoGps;

private slots:
	void _slotShowMap();
	void _slotGeneralMenu();
	void _slotGeneralGpsSett();
	void _slotHideApp();
	void _slotExit();
	void _slotCloseMenu();
	//страницы главного меню
	void _slotCurrentGpsData();
	void _slotSensorsClass();
	void _slotGeneralSettings();
	void _slotGeneralConnectionSet();
	void _slotGeneralSendData();
	void _slotGeneralWizard();
	void _slotGeneralRaports();
	//меню карты
	void _slotMapRoteteMenu();
	void _slotMapSelectToolMenu();
	void _slotMapPhoto();
	void _slotMapControls();
	void _slotMapMenu();
	void _slotMapMarks();
	void _slotSendTrack();
	void _slotSendPhoto();
	void _slotSendVideo();
	void _slotMapNavi();
	void _slotMapViewNavi();

	void _slotSign();
	void _slotSearch();

	void _slotCalc(QTime sunrise, QTime sunset, short akb,int timePso, int avspeed,short timeOfStop,double rasstOb);
	void _slotGpsData(QTime time, QDate date, bool status, double lat, double lon, double speed, double angle,short sat, float height);
	void _slotGpsData(QString string,bool check);
	void _slotGpsErrors(QString errors);
	void _slotMapCoord(double lat, double lon);
	void _slotMapPointWiz();
	void _slotGoToPoint();
	void _slotLayers();
	void _slotScale();
	void _slotExchange();
	void _slotMapLock();
	void _slotNaviOn();
	void _slotNaviOff();
	//контроль открытых меню
	void _slotMenuOpen(bool open);
	void _slotMessages(QString messages);
	void _slotNaviSuccess();
	void _slotClickPos(double lat,double lon);
	void _slotComList(QStringList comList, QStringList comPortTypes);
	void _slotMapMRoute();
	void _slotToolFeature();
	void _slotTrackSettings();
	void _slotUpdateGpsPorts();
	void _slotMapMapBut();
	void _slotSettings();
	void _slotAbout();
	void _slotNoGpsFound();
	void _slotMapBrightness();
	void _slotMapHotTab();
	void _slotMapObjects();
	void _slotGsmLayers();
	void _slotMapMarkEdit();
	void _slotDevButOk();
	void _slotDevButMenu();
	void _slotDevButF1();
	void _slotDevButF2();
	void _slotDevButF3();
	void _slotDevButUp();
	void _slotDevButDown();
	void _slotDevButLeft();
	void _slotDevButRight();
	void _slotNumbOfGps(short numb);
	void _slotAzimuthAsc();
	void _slotMapReferance();
signals:
	//команды работы с картой
	void signZoomIn();
	void signZoomOut();
	void signCenterPos();
	void signBrakeTrack();
	void signCloseTool();	
	void signSelectRgn();
	void signScale(double scale);
	void signSetScale(double scale);
	void signDelTools();
	void signSearch();
	void signShowSearch();
	
	//ориентация
	void signNothOrient();
	void signSouthOrient();
	void signMoveOrient();
	
	//инструменты
	void signLenLine();
	void signBreakLenLine();
	void signToolLasso();
	void signDayNight();
	void signGrid();
	void signLayers();
	void signDrawGrid(QString path);
	void signDrawRoute(QString path);

		//Ручной ввод маршрута
	void signManualRoute();
	void signMRouteCoord(double lat, double lon);
	void signClearMRoute();

	//GPS и рассчеты на его основе
	void signGps(QTime time, QDate date, bool status, double lat, double lon, double speed, double angle,short sat, float height);
	void signCalc(QTime sunrise, QTime sunset, short akb,int timePso, int avspeed,short timeOfStop,double rasstOb);
	void signGpsOk(bool ok);
	void signGpsAll(QString string,bool check);
	void signGpsErrors(QString errors);
	void signRestartGps();
	void signBadSignGps();
	void signGeoDataSettings();
	void singUpdateSettings();
	//запрос на инициализацию инструмента получения координаты с карты
	void signMapPoint();
	void signMapCoord(double lat, double lon);
	//палетки
	void signLoadPaletka(short source);
	void signGridLoaded(bool loaded);
	void signGridPath(QString path);
	void signRouteLoaded(bool loaded);
	void signRoutePath(QString path);
	void signLoadRoute(short source);
	//загрузка настроек мастера
	void signLoadWizSet();
	
	//текущие данные отправлены
	void signCurrentDataSended(bool sended);
	//поворот карты на градусов
	void signMapRotete(double angle);
	//направление
	void signAzimuth(double azimuth);
	//отправка сообщения
	void signMessage(QString message, short toKcps);

	//результат отправки сообщения
	void signSendMessageResult(bool sended);
	//
	void signMark(short type, double lat, double lon);
	//
	void signSendAllMarks(short toKcps);
	//
	void signSendMarksResult(bool sended);

	void signTrackLogged(bool logged);
	void signBreakTrack(bool clear);

	void signDrawTrack(QString path,int discret);
	void signSendTrack(QString path, int discret, short toKcps);
	void signSendTrackResult(bool result);
	void signClearTrack();

	void signSendAllTracks(QStringList listOfTracks,short source);
	void signSendTraksPro(short progress,short maximum);

	void signSendPhoto(QString path, short toKcps);
	void signSendPhotoResult(bool result);

	void signSendAllPhotos(QStringList listOPhotos,short source);
	void signSendPhotosPro(short progress,short maximum);

	void signSendAllVideos(QStringList listOfVideos,short source);
	void signSendVideosPro(short progress,short maximum);
	void signSendVideoResult(bool sended);

	void signSendVideo(QString path, short toKcps);

	void signRouteNavi();
	void signCoordNavi(double lat,double lon);
	void signPointNavi();
	void signNaviData(double routeDist,double pointDist,double calcDist,
			   double routeAngle,double pointAngle,double currentAngle,
			   short pointNumber,double calcTime);

	void signNaviEnd();
	void signUncheckedNavi(bool unchNavi);
	void signDistNavi(short dist);
	void signScanCom();
	void signComList(QStringList comList, QStringList comPortTypes);
	void signMessageIsRead(QString response,short toKcps);
	void signResponseSended(bool sended);
	void signHideLeftMenu();
	void signAnglePos(short Angle);
	void signCloseFeature(bool ok);
	void signNoGps();
	
	//яркость
	void signGetBright();
	void signBrightUp();
	void signBrightDown();
	void signBrightStatus(short status);
	void signBrightMax();
	void signBrightMin();
	//настройки утсройства
	//установить
	void signSetGps(bool status);
	void signSet3g(bool status);
	void signSetWiFi(bool status);
	void signSetBluetooth(bool status);
	void signSetIndicator(bool status);
	void signSetIpWireless(bool status);
	void signSetCam(bool status);
	void signSetBit3(bool status);
	//получить статус
	void signGetGpsStatus();
	void signGet3gStatus();
	void signGetWiFiStatus();
	void signGetBluetoothStatus();
	void signGetIndicatorStatus();
	void signGetIpWirelessStatus();
	void signGetCamStatus();
	void signGetBit3Stat();

	//статус кнопок
	void signGpsStatus(bool status);
	void sign3gStatus(bool status);
	void signWiFiStatus(bool status);
	void signBluetoothStatus(bool status);
	void signIndicatorStatus(bool status);
	void signIpWirelessStatus(bool status);
	void signCamStatus(bool status);
	void signBit3Stat(bool status);

	void signGetStatusObjects();
	void signGetStatusGsmLayers();

	void signShowRoute(bool show);
	void signShowGreed(bool show);
	void signShowMarks(bool show);
	void signShowTracks(bool show);
	void signShowCurrentPos(bool show);
	void signShowManualRoute(bool show);

	void signShowMegafon(bool show);
	void signShowBeeline(bool show);
	void signShowMts(bool show);
	void signShowTele2(bool show);

	void signStatusGsmLayers(bool megafon_, bool beeline_,bool mts_, bool tele2_);
	void signStatusObjects(bool megafon, bool greed_,bool marks_, bool tracks_, bool currentPos_,bool manualRoute_);
	void signUpdateMarks();
	void signNumbOfGps(short numb);
	void signGetNumbOfGps();
	void signGetNumbOfGps2();
	void signDelNavi();
	void signNaviCoordinstes(double lat,double lon);
	//очистка данных накопленных в классе калькуляций
	void signEraseCurentData();
};

#endif // PYRAMIDWIDGETS_H
