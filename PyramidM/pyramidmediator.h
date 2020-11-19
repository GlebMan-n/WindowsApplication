#ifndef PYRAMIDMEDIATOR_H
#define PYRAMIDMEDIATOR_H

#include "stdafx.h"
#include <QtGui>
#include "struct.h"
#include <gis\GrUnits\StoreVectorData.h>

class MapBaseWidget;
class PyramidWidgets;
class SelectRgnClass;
class GpsClass;
class CalcClass;
class PointOnMapClass;
class GisFindTopoData;
class ExchangeDataClass;
class PaletkaClass;
class RouteClass;
class CurrentPosToken;
class TimerClass;
class MapToolsClass;
class MarkTokenClass;
class TrackClass;
class NaviClass;
class DrawNaviClass;
class TestGpsPorts;
class ManualRoute;
class HotTabClass;
class LoadKmlFilesClass;


class PyramidMediator : public QObject
{
	Q_OBJECT
public:
	PyramidMediator(QMainWindow *parent, MapBaseWidget* mapWidget);
	~PyramidMediator();
	//начало, вызывается из главного класса
	void start();
	//установить фабрику для конвертации фотографий
	bool setFactory(GisGdiFactory *m_factory);
	
private:
	QMainWindow* _parent;
	QWidget* _pyram;
	PyramidWidgets* _widgets;
	MapBaseWidget* _map;
	SelectRgnClass* _selectRgn;
	GpsClass* _gps;
	TestGpsPorts* testGps;
	HotTabClass* hotTab;
	CalcClass* _calculates;
	//Класс получения координат нажатой точки
	PointOnMapClass* pointOnMapTool;
	//класс поиска
	GisFindTopoData* m_pGisFindTopoData;
	//класс приема отправки данных
	ExchangeDataClass* _exchangeData;
	//отрисовка
	//палетки
	PaletkaClass* _paletka;
	//маршрута
	RouteClass* _route;
	//трека
	TrackClass* _track;
	//класс отображения текущей позиции
	CurrentPosToken* _posToken;
	//класс отрисовки отметок
	MarkTokenClass* _marksToken;
	//класс отрисовки навигационных отметок (точки, линии)
	DrawNaviClass* _drawNavi;
	//слои для карт
	//треки
	StoreVectorData<FastVectorData> *_trackLayer;
	//палетки
	StoreVectorData<FastVectorData> *_gridLayer;
	//маршруты
	StoreVectorData<FastVectorData>*_routeLayer;
	//текущее положение
	StoreVectorData<FastVectorData>*_tokenLayer;
	//отметки
	StoreVectorData<FastVectorData>*_marksLayer;
	//навигационные отметки
	StoreVectorData<FastVectorData>*_naviLayer;
	//Слой мануального маршрута
	StoreVectorData<FastVectorData>* _mRouteLayer;
	//Слой для зоны покрытия мегафон
	StoreVectorData<FastVectorData>* _megafonLayer;
	//Слой для зоны покрытия мтс
	StoreVectorData<FastVectorData>* _mtsLayer;
	//Слой для зоны покрытия билайн
	StoreVectorData<FastVectorData>* _beelineLayer;
	//Слой для зоны покрытия теле2
	StoreVectorData<FastVectorData>* _tele2Layer;

	LoadKmlFilesClass* loadMegafonKml;
	LoadKmlFilesClass* loadMtsKml;
	LoadKmlFilesClass* loadBeelineKml;
	LoadKmlFilesClass* loadTele2Kml;
	//таймер обновления карты
	TimerClass* timerMapUpdate;
	//нужен для конвертирования фотографий
	GisGdiFactory *_m_factory;

	bool _naviOn;
	bool _coordNaviOn;
	//флаг центрирования позиции
	bool _centrPosition;
	//флаг ориентации по движению
	bool _moveOrient;
	bool _manualRouteFlag;
	bool _rewriteLayers;
	//класс инструментов
	MapToolsClass* _mapTools;
	//класс навигационных расчетов
	NaviClass* navigator;
	//путь к отрисованному треку (хранится для навигации)
	QString _pathToDrawedTrack;
	ManualRoute* _manualRoute;
	GdiGis::Color _megafonColor;
	GdiGis::Color _beelineColor;
	GdiGis::Color _mtsColor;
	GdiGis::Color _tele2Color;
private:
	//инициализация слоя
	bool InitLayerForKalka(StoreVectorData<FastVectorData>** layer);
	//инициализация всех слоев
	bool initLayers();
	//применяем настройки видимости слоев
	void _stateOfLayers();
	//конвертация фотографий
	QString _convPhoto(QString pathToPhoto);
	//загрузка настроек
	bool _loadSettings();
	//сохранение настроек
	void _saveSettings();
	//создание папок
	void _createFolders();
	//создаем расчетный класс
	void _createCalculates();
	
	
//структуры
private:
	//структура настроек
	SETTINGS settings;
private slots:
	void _slotZoomIn();
	void _slotZoomOut();
	void _slotCentrPos();
	void _slotCentrPosFlag(bool flag);
	void _slotSelectRgn();
	void _slotUpdateScale();
	void _slotDelTools();
	//Отображение окна поиска
	void _slotSearch();

	//GPS
	void _slotStartGps();
	void _slotStopGps();
	void _slotGpsData(QTime time, QDate date, bool status, double lat, double lon, double speed, double angle,short sat, float height);

	//Загрузка настроек
	void _slotLoadSettings();
	//Диалог выбора новой папки для картографического материала
	void _slotGeoDataSettings();
	void _slotPointOnMap();
	//загрузка настроек мастера
	void _slotLoadWizardSettings();
	//задать угол поворота карты в радианах
	void _slotAngleMapRadians(double angle/*угол в радианах*/);
	//установить режим ориентации по движению
	void _slotMoveOrient();
	void _slotNothOrient();
	void _slotSouthOrient();
	void _slotMoveToPos(double lat, double lon);
	void _slotLayers();
	void _slotSetScale(double scale);
	void _slotDrawRoute(QString path);
	void _slotDrawGrid(QString path);
	void _slotRedrawMap();
	void _slotSendTrack(QString path, int discret, short toKcps);
	void _slotPhoto(QString path,short toKcps);
	void _slotSendAllPhotos(QStringList photos, short toKcps);
	
	void _slotMarshNavi();
	void _slotCoordNavi(double lat, double lon);
	void _slotDelNavi();
	void _slotComList();
	void _slotClickPos(double lat,double lon);
	
	void _slotNaviEndPoint();
	void _slotSendAllTracks(QStringList listOfTracks,short toKcps = 0);
	void _slotHideNaviLines();
	void _slotViewPos(double lat,double lon);
	void _slotManualRoute();
	void _slotSetTimeToSend(short timeInSec);

	void _slotClearGrid();
	void _slotAddGrid();

	void _slotStatusObjects();
	void _slotStatusGsmLayers();
	void _slotShowRoute(bool show);
	void _slotShowGreed(bool show);
	void _slotShowMarks(bool show);
	void _slotShowTracks(bool show);
	void _slotShowCurrentPos(bool show);
	void _slotShowManualRoute(bool show);
	
	void _slotShowMegafonLayer(bool show);
	void _slotShowMtsLayer(bool show);
	void _slotShowBeelineLayer(bool show);
	void _slotShowTele2Layer(bool show);

	//отобразить зоны покрытия GSM сетей
	void _slotGsmLayers();
	bool _saveLayerToBlob(StoreVectorData<FastVectorData>* layer,QString path);
	bool _loadLayerFromBlob(StoreVectorData<FastVectorData>* layer,QString path);
	//удалить слои
	void _slotDeleteLayers();
	void _slotGsmKmlLoaded();

signals:
	void signScale(double scale);
	void signMapRotete(double angle);
	void signInitMapWidget();
	void signShowSearch();
	void signSendTrack(QString path,short toKcps);
	void signSendPhoto(QString path,short toKcps);
	void signSendPhotoPro(short progress,short maximum);
	void signSendAllTracks(QStringList listOfTracks,short toKcps = 0);
	void signShowNavi();
	void signMenuOn(bool menu);
	void signComList(QStringList comList, QStringList comTypes);
	void signClickPos(double lat,double lon);

	void signNaviCoordinstes(double lat, double lon);

	void signAnglePos(short angle);
	void signMRoutePoint(double lat, double lon);
	void signStatusObjects(bool route_, bool greed_,bool marks_, bool tracks_, bool currentPos_,bool manualRoute_);
	void signStatusGsmLayers(bool megafon_, bool beeline_,bool mts_, bool tele2_);
	
	void signOk();
	void signMenu();
	void signF1();
	void signF2();
	void signF3();
	void signKeyUp();
	void signKeyDown();
	void signKeyLeft();
	void signKeyRight();
};

#endif // PYRAMIDMEDIATOR_H
