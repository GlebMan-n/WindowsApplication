#include "stdafx.h"
#include "pyramidmediator.h"
#include <PyramidWidgets\pyramidwidgets.h>
#include <PyramidM\MSVC_2008\GpsClass.h>
#include <gis_tool_qt/GisInterfaceQT/MapBaseWidget.h>
#include <gis_tool_qt/GisInterfaceQT/GisMapQtInterface.h>
#include "gis/GisDataInterface/ControlSelector.h"
#include <MSVC_2008\SelectRgnClass.h>
#include <PyramidM\MSVC_2008\CalcClass.h>
#include <gis_tool_qt/GisWidgets/GisAccessDlg.h>
#include <MSVC_2008\PointOnMap.h>
#include <gis_tool_qt/GisInterfaceQT/GisFindTopoData.h>
#include <MSVC_2008\ExchangeDataClass.h>
#include <MSVC_2008\PaletkaClass.h>
#include <MSVC_2008\RouteClass.h>
#include <PyramidM\MSVC_2008\ApplyWizardSettings.h>
#include <PyramidM\MSVC_2008\CurrentPosToken.h>
#include <PyramidM\MSVC_2008\TimerClass.h>
#include <PyramidM\MSVC_2008\MapToolsClass.h>
#include <gis_tool_qt\GisInterfaceQT\LayerDlg.h>
#include <PyramidM\MSVC_2008\MarkTokenClass.h>
#include <PyramidM\MSVC_2008\TrackClass.h>
#include <gis\GrUnits\TknMapNonScale.h>
#include <gis\GisBase\gdigis\ImageDevice.h>
#include <PyramidM\MSVC_2008\NaviClass.h>
#include <PyramidM\MSVC_2008\DrawNavi.h>
#include <PyramidM\MSVC_2008\TestGpsPorts.h>
#include <PyramidM\MSVC_2008\CalcFunctions.h>
#include <PyramidM\MSVC_2008\ManualRouteClass.h>
//#include <vld/vld.h>
#include <PyramidM\MSVC_2008\TknPaletka.h>
#include "MSVC_2008\HotTabFunctions.h"
#include "PyramidM\MSVC_2008\KmlParser.h"

//#ifdef _DEBUG
//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>
//#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
//#define new DEBUG_NEW
//#endif

PyramidMediator::PyramidMediator(QMainWindow *parent, MapBaseWidget* mapWidget)
{
	this->setParent(mapWidget);
	_parent = parent;
	_map = mapWidget;
	_selectRgn = NULL;
	_gps = NULL;
	_calculates = NULL;
	_widgets = NULL;
	pointOnMapTool = NULL;
	m_pGisFindTopoData = NULL;
	_exchangeData = NULL;
	_paletka = NULL;
	_trackLayer = NULL;
	_gridLayer = NULL;
	_routeLayer = NULL;
	_tokenLayer = NULL;
	_marksLayer = NULL;
	_naviLayer = NULL;
	_mRouteLayer = NULL;
	_megafonLayer = NULL;
	_mtsLayer = NULL;
	_beelineLayer = NULL;
	_tele2Layer = NULL;
	_moveOrient = false;
	_naviOn = false;
	_coordNaviOn = false;
	_centrPosition = true;
	_manualRouteFlag = false;
	_rewriteLayers = false;
	timerMapUpdate = NULL;
	_mapTools = NULL;
	_m_factory = NULL;
	navigator = NULL;
	_manualRoute = NULL;
	_marksToken = NULL;
	_route = NULL;
	_track = NULL;
	_drawNavi = NULL;
	_posToken = NULL;
	_mapTools = new MapToolsClass(_parent,_map);
	connect(_mapTools,SIGNAL(signUpdateMap()),SLOT(_slotUpdateScale()));
	connect(_map,SIGNAL(signMouseClick(double,double)),SLOT(_slotClickPos(double,double)));
	connect(_map,SIGNAL(signViewPos(double,double)),SLOT(_slotViewPos(double,double)));
	timerMapUpdate = new TimerClass(false);
	timerMapUpdate->start();
	//загружаем настройки программы
	_loadSettings();
	_slotCentrPos();
	_createFolders();
	PosBL posBL = {settings.lastLat, settings.lastLon};
	_map->getGisView()->move2point(posBL);
	_slotSetScale(settings.scale);
	
	_megafonColor = GdiGis::Color(95,73,181,68);
	_beelineColor = GdiGis::Color(90,227,176,21);
	_mtsColor = GdiGis::Color(70,255,0,0);
	_tele2Color = GdiGis::Color(70,0,0,255);
	
	_exchangeData = new ExchangeDataClass(parent);
	connect(this,SIGNAL(signSendTrack(QString,short)),_exchangeData,SLOT(_slotSendTrack(QString,short)));
	connect(this,SIGNAL(signSendPhoto(QString,short)),_exchangeData,SLOT(_slotSendPhoto(QString,short)));
	connect(_exchangeData,SIGNAL(singUpdateSettings()),SLOT(_slotLoadSettings()));
	if(initLayers())
	{
		_stateOfLayers();
		_paletka = new PaletkaClass(parent,_map,&_gridLayer);
		connect(_exchangeData,SIGNAL(signGridLoaded(bool)),_paletka,SLOT(_slotGridLoaded(bool)));

		_route = new RouteClass(parent,_map,_routeLayer);		
		connect(_exchangeData,SIGNAL(signRouteLoaded(bool)),_route,SLOT(_slotRouteLoaded(bool)));
		
		_posToken = new CurrentPosToken(this,_map,_tokenLayer);
		connect(_map,SIGNAL(signViewPos(double,double)),_posToken,SLOT(_slotViewPos(double,double)));
		if (!_posToken->initToken())
		{
			delete _posToken;
			_posToken = NULL;
		}

		_marksToken = new MarkTokenClass(this,_map,_marksLayer);

		_track = new TrackClass(parent,_trackLayer);

		

		_manualRoute = new ManualRoute(this,_mRouteLayer,_map);
		connect(this,SIGNAL(signMRoutePoint(double,double)),_manualRoute,SLOT(_slotAddPoint(double,double)));

		_slotGsmLayers();
	}
	_pathToDrawedTrack = "";
	hotTab = new HotTabClass(mapWidget);
	
}

PyramidMediator::~PyramidMediator()
{
	if(_rewriteLayers)
	{
		_saveLayerToBlob(_megafonLayer,QCoreApplication::applicationDirPath ()+ "/data/gsm/MEGAFON/megafon.layer");
		_saveLayerToBlob(_mtsLayer,QCoreApplication::applicationDirPath ()+ "/data/gsm/MTS/mts.layer");
		_saveLayerToBlob(_beelineLayer,QCoreApplication::applicationDirPath ()+ "/data/gsm/BEELINE/beeline.layer");
		_saveLayerToBlob(_tele2Layer,QCoreApplication::applicationDirPath ()+ "/data/gsm/TELE2/tele2.layer");
	}


	if(_trackLayer)
	{
		delete _trackLayer;
		_trackLayer = NULL;
	}

	if(_gridLayer)
	{
		delete _gridLayer;
		_gridLayer = NULL;
	}

	if(_routeLayer)
	{
		delete _routeLayer;
		_routeLayer = NULL;
	}

	if(_tokenLayer)
	{
		delete _tokenLayer;
		_tokenLayer = NULL;
	}

	if(_marksLayer)
	{
		delete _marksLayer;
		_marksLayer = NULL;
	}

	if(_naviLayer)
	{
		delete _naviLayer;
		_naviLayer = NULL;
	}

	if(_mRouteLayer)
	{
		delete _mRouteLayer;
		_mRouteLayer = NULL;
	}

	if(_megafonLayer)
	{
		delete _megafonLayer;
		_megafonLayer = NULL;
	}

	if(_mtsLayer)
	{
		delete _mtsLayer;
		_mtsLayer = NULL;
	}

	if(_beelineLayer)
	{
		delete _beelineLayer;
		_beelineLayer = NULL;
	}

	if(_tele2Layer)
	{
		delete _tele2Layer;
		_tele2Layer = NULL;
	}

	if(timerMapUpdate) 
		timerMapUpdate->~TimerClass();
	
	if(_calculates)
	{
		delete _calculates;
		_calculates = NULL;
	}
	
	_saveSettings();
	delete _widgets;
	_widgets =NULL;
}

void PyramidMediator::start()
{
	if(_widgets)
	{
		delete _widgets;
		_widgets =NULL;
	}
	_widgets = new PyramidWidgets(_parent,settings.version);
	connect(_widgets,SIGNAL(signZoomIn()),SLOT(_slotZoomIn()));
	connect(_widgets,SIGNAL(signZoomOut()),SLOT(_slotZoomOut()));
	connect(_widgets,SIGNAL(signCenterPos()),SLOT(_slotCentrPos()));
	connect(_widgets,SIGNAL(signSelectRgn()),SLOT(_slotSelectRgn()));
	connect(_widgets,SIGNAL(signDelTools()),SLOT(_slotDelTools()));
	connect(_widgets,SIGNAL(signDelTools()),_mapTools,SLOT(_slotDelTools()));
	connect(_widgets,SIGNAL(signRestartGps()),SLOT(_slotStartGps()));
	connect(_widgets,SIGNAL(signGeoDataSettings()),SLOT(_slotGeoDataSettings()));
	connect(_widgets,SIGNAL(singUpdateSettings()),SLOT(_slotLoadSettings()));
	connect(_widgets,SIGNAL(singUpdateSettings()),_exchangeData,SLOT(_slotUpdateSettings()));

	//заменить механизм точки для ориентирования
	connect(_widgets,SIGNAL(signMapPoint()),SLOT(_slotPointOnMap()));

	connect(_widgets,SIGNAL(signSearch()),SLOT(_slotSearch()));
	connect(_widgets,SIGNAL(signLoadWizSet()),SLOT(_slotLoadWizardSettings()));
	connect(_widgets,SIGNAL(signMoveOrient()),SLOT(_slotMoveOrient()));
	connect(_widgets,SIGNAL(signNothOrient()),SLOT(_slotNothOrient()));
	connect(_widgets,SIGNAL(signSouthOrient()),SLOT(_slotSouthOrient()));

	if(_route)
	{
		connect(_route,SIGNAL(signPathToRoute(QString)),_widgets,SIGNAL(signRoutePath(QString)));
	}

	connect(_widgets,SIGNAL(signLenLine()),_mapTools,SLOT(_slotLenLine()));
	connect(_widgets,SIGNAL(signBreakLenLine()),_mapTools,SLOT(_slotBreakLine()));
	connect(_widgets,SIGNAL(signDayNight()),_mapTools,SLOT(_slotDayNight()));
	connect(_widgets,SIGNAL(signGrid()),_mapTools,SLOT(_slotGrid()));
	connect(_widgets,SIGNAL(signToolLasso()),_mapTools,SLOT(_slotLassoTool()));
	connect(_widgets,SIGNAL(signLayers()),SLOT(_slotLayers()));
	connect(_widgets,SIGNAL(signSetScale(double)),SLOT(_slotSetScale(double)));
	connect(_widgets,SIGNAL(signMapCoord(double,double)),SLOT(_slotMoveToPos(double,double)));
	connect(_widgets,SIGNAL(signDrawGrid(QString)),SLOT(_slotDrawGrid(QString)));
	connect(_widgets,SIGNAL(signDrawRoute(QString)),SLOT(_slotDrawRoute(QString)));
	connect(_widgets,SIGNAL(signMessage(QString,short)),_exchangeData,SLOT(_slotSendRaport(QString,short)));
	if(_marksToken)
	{
		connect(_widgets,SIGNAL(signMark(short, double, double)),_marksToken,SLOT(_slotMark(short, double, double)));
		connect(_widgets,SIGNAL(signUpdateMarks()),_marksToken,SLOT(_slotUpdateMarks()));
	}
	connect(_widgets,SIGNAL(signSendAllMarks(short)),_exchangeData,SLOT(_slotSendMarks(short)));
	connect(_exchangeData,SIGNAL(signSendMarksResult(bool)),_widgets,SIGNAL(signSendMarksResult(bool)));
	connect(_exchangeData,SIGNAL(signSendTrackResult(bool)),_widgets,SIGNAL(signSendTrackResult(bool)));

	connect(_exchangeData,SIGNAL(signSendTraksPro(short,short)),_widgets,SIGNAL(signSendTraksPro(short,short)));
	connect(_exchangeData,SIGNAL(signMessage(QString)),_widgets,SLOT(_slotMessages(QString)));
	connect(this,SIGNAL(signSendPhotoPro(short,short)),_widgets,SIGNAL(signSendPhotosPro(short,short)));
	connect(_exchangeData,SIGNAL(signSendVideosPro(short,short)),_widgets,SIGNAL(signSendVideosPro(short,short)));
	if(_track)
	{
		connect(_track,SIGNAL(signTrackLogged(bool)),_widgets,SIGNAL(signTrackLogged(bool)));
		connect(_track,SIGNAL(signFullRedrawMap()),SLOT(_slotRedrawMap()));
		connect(_widgets,SIGNAL(signBreakTrack(bool)),_track,SLOT(_slotBreakTrack(bool)));
		connect(_widgets,SIGNAL(singUpdateSettings()),_track,SLOT(_slotUpdateSettings()));
		connect(_widgets,SIGNAL(signClearTrack()),_track,SLOT(_slotClearTrack()));
		connect(_widgets,SIGNAL(signDrawTrack(QString,int)),_track,SLOT(_slotDrawTrack(QString,int)));
	}

	if(hotTab)
	{	


		//brightness
		connect(_widgets,SIGNAL(signGetBright()),hotTab,SLOT(_slotStatusBrightness()));
		connect(_widgets,SIGNAL(signBrightUp()),hotTab,SLOT(_slotBrightUp()));
		connect(_widgets,SIGNAL(signBrightDown()),hotTab,SLOT(_slotBrightDown()));
		connect(hotTab,SIGNAL(signBrightStatus(short)),_widgets,SIGNAL(signBrightStatus(short)));
		connect(_widgets,SIGNAL(signBrightMax()),hotTab,SLOT(_slotBrightMax()));
		connect(_widgets,SIGNAL(signBrightMin()),hotTab,SLOT(_slotBrightMin()));
		//device manager
		connect(_widgets,SIGNAL(signGetGpsStatus()),hotTab,SLOT(_slotStatusGps()));
		connect(_widgets,SIGNAL(signGet3gStatus()),hotTab,SLOT(_slotStatus3G()));
		connect(_widgets,SIGNAL(signGetWiFiStatus()),hotTab,SLOT(_slotStatusWiFi()));
		connect(_widgets,SIGNAL(signGetBluetoothStatus()),hotTab,SLOT(_slotStatusBluetooth()));
		connect(_widgets,SIGNAL(signGetIndicatorStatus()),hotTab,SLOT(_slotStatusInd()));
		connect(_widgets,SIGNAL(signGetCamStatus()),hotTab,SLOT(_slotStatusCam()));
		connect(_widgets,SIGNAL(signGetIpWirelessStatus()),hotTab,SLOT(_slotStatusIpWireless()));
		connect(_widgets,SIGNAL(signGetBit3Stat()),hotTab,SLOT(_slotStatusBit3()));

		connect(_widgets,SIGNAL(signSetGps(bool)),hotTab,SLOT(_slotGpsOn(bool)));
		connect(_widgets,SIGNAL(signSet3g(bool)),hotTab,SLOT(_slot3GOn(bool)));
		connect(_widgets,SIGNAL(signSetWiFi(bool)),hotTab,SLOT(_slotWiFiOn(bool)));
		connect(_widgets,SIGNAL(signSetBluetooth(bool)),hotTab,SLOT(_slotBluetoothOn(bool)));
		connect(_widgets,SIGNAL(signSetIndicator(bool)),hotTab,SLOT(_slotIndicatorOn(bool)));
		connect(_widgets,SIGNAL(signSetIpWireless(bool)),hotTab,SLOT(_slotIpWirelessOn(bool)));
		connect(_widgets,SIGNAL(signSetCam(bool)),hotTab,SLOT(_slotCamOn(bool)));
		connect(_widgets,SIGNAL(signSetBit3(bool)),hotTab,SLOT(_slotBit3On(bool)));

 		connect(hotTab,SIGNAL(signGpsStatus(bool)),_widgets,SIGNAL(signGpsStatus(bool)));
		connect(hotTab,SIGNAL(sign3gStatus(bool)),_widgets,SIGNAL(sign3gStatus(bool)));
		connect(hotTab,SIGNAL(signWiFiStatus(bool)),_widgets,SIGNAL(signWiFiStatus(bool)));
		connect(hotTab,SIGNAL(signBluetoothStatus(bool)),_widgets,SIGNAL(signBluetoothStatus(bool)));
		connect(hotTab,SIGNAL(signIndicatorStatus(bool)),_widgets,SIGNAL(signIndicatorStatus(bool)));
		connect(hotTab,SIGNAL(signIpWirelessStatus(bool)),_widgets,SIGNAL(signIpWirelessStatus(bool)));
		connect(hotTab,SIGNAL(signCamStatus(bool)),_widgets,SIGNAL(signCamStatus(bool)));
		connect(hotTab,SIGNAL(signBit3Stat(bool)),_widgets,SIGNAL(signBit3Stat(bool)));

	}
	
	
	

	
	connect(_widgets,SIGNAL(signSendTrack(QString,int,short)),this,SLOT(_slotSendTrack(QString,int,short)));
	
	connect(_widgets,SIGNAL(signSendAllTracks(QStringList,short)),this,SLOT(_slotSendAllTracks(QStringList,short)));
	connect(this,SIGNAL(signSendAllTracks(QStringList,short)),_exchangeData,SLOT(_slotSendAllTracks(QStringList,short)));
	connect(_widgets,SIGNAL(signSendAllPhotos(QStringList,short)),SLOT(_slotSendAllPhotos(QStringList,short)));
	connect(_widgets,SIGNAL(signSendAllVideos(QStringList,short)),_exchangeData,SLOT(_slotSendAllVideos(QStringList,short)));


	connect(_exchangeData,SIGNAL(signCurrentDataSended(bool)),_widgets,SIGNAL(signCurrentDataSended(bool)));
	//команда на загрузку палетки
	connect(_widgets,SIGNAL(signLoadPaletka(short)),_exchangeData,SLOT(_slotLoadGreed(short)));
	//ответ от загрузчика палетки
	connect(_exchangeData,SIGNAL(signGridLoaded(bool)),_widgets,SIGNAL(signGridLoaded(bool)));
	connect(_exchangeData,SIGNAL(signRouteLoaded(bool)),_widgets,SIGNAL(signRouteLoaded(bool)));
	connect(_exchangeData,SIGNAL(signSendMessageResult(bool)),_widgets,SIGNAL(signSendMessageResult(bool)));
	if(_paletka)
	{
		connect(_paletka,SIGNAL(signPathToGrid(QString)),_widgets,SIGNAL(signGridPath(QString)));
	}
	connect(_widgets,SIGNAL(signLoadRoute(short)),_exchangeData,SLOT(_slotLoadRoute(short)));
	connect(this,SIGNAL(signScale(double)),_widgets,SIGNAL(signScale(double)));
	connect(this,SIGNAL(signShowSearch()),_widgets,SIGNAL(signShowSearch()));
	connect(this,SIGNAL(signMapRotete(double)),_widgets,SIGNAL(signMapRotete(double)));
	connect(_exchangeData,SIGNAL(signSendPhotoResult(bool)),_widgets,SIGNAL(signSendPhotoResult(bool)));
	connect(_widgets,SIGNAL(signSendPhoto(QString,short)),this,SLOT(_slotPhoto(QString,short)));
	connect(_widgets,SIGNAL(signSendVideo(QString,short)),_exchangeData,SLOT(_slotSendVideo(QString,short)));
	connect(_exchangeData,SIGNAL(signSendVideoResult(bool)),_widgets,SIGNAL(signSendVideoResult(bool)));
	connect(_widgets,SIGNAL(signCoordNavi(double,double)),SLOT(_slotCoordNavi(double,double)));
	connect(_widgets,SIGNAL(signRouteNavi()),SLOT(_slotMarshNavi()));

	

	connect(this,SIGNAL(signShowNavi()),_widgets,SLOT(_slotNaviOn()));
	connect(this,SIGNAL(signMenuOn(bool)),_widgets,SLOT(_slotMenuOpen(bool)));
	connect(_widgets,SIGNAL(signScanCom()),SLOT(_slotComList()));
	connect(this,SIGNAL(signComList(QStringList,QStringList)),_widgets,SLOT(_slotComList(QStringList,QStringList)));
	connect(_widgets,SIGNAL(signMessageIsRead(QString,short)),_exchangeData,SLOT(_slotMessageResponse(QString,short)));
	connect(_exchangeData,SIGNAL(signResponseSended(bool)),_widgets,SIGNAL(signResponseSended(bool)));
	connect(this,SIGNAL(signClickPos(double,double)),_widgets,SLOT(_slotClickPos(double,double)));
	connect(this,SIGNAL(signAnglePos(short)),_widgets,SIGNAL(signAnglePos(short)));
	connect(_widgets,SIGNAL(signManualRoute()),SLOT(_slotManualRoute()));
	connect(_widgets,SIGNAL(signMRouteCoord(double,double)),SIGNAL(signMRoutePoint(double,double)));


	connect(_widgets,SIGNAL(signGetStatusObjects()),SLOT(_slotStatusObjects()));
	connect(_widgets,SIGNAL(signGetStatusGsmLayers()),SLOT(_slotStatusGsmLayers()));

	connect(_widgets,SIGNAL(signShowRoute(bool)),SLOT(_slotShowRoute(bool)));
	connect(_widgets,SIGNAL(signShowGreed(bool)),SLOT(_slotShowGreed(bool)));
	connect(_widgets,SIGNAL(signShowMarks(bool)),SLOT(_slotShowMarks(bool)));
	connect(_widgets,SIGNAL(signShowTracks(bool)),SLOT(_slotShowTracks(bool)));
	connect(_widgets,SIGNAL(signShowCurrentPos(bool)),SLOT(_slotShowCurrentPos(bool)));
	connect(_widgets,SIGNAL(signShowManualRoute(bool)),SLOT(_slotShowManualRoute(bool)));
	
	connect(_widgets,SIGNAL(signShowMegafon(bool)),SLOT(_slotShowMegafonLayer(bool)));
	connect(_widgets,SIGNAL(signShowMts(bool)),SLOT(_slotShowMtsLayer(bool)));
	connect(_widgets,SIGNAL(signShowBeeline(bool)),SLOT(_slotShowBeelineLayer(bool)));
	connect(_widgets,SIGNAL(signShowTele2(bool)),SLOT(_slotShowTele2Layer(bool)));
	


	connect(this,SIGNAL(signStatusObjects(bool,bool,bool,bool,bool,bool)),_widgets,SIGNAL(signStatusObjects(bool,bool,bool,bool,bool,bool)));
	connect(this,SIGNAL(signStatusGsmLayers(bool,bool,bool,bool)),_widgets,SIGNAL(signStatusGsmLayers(bool,bool,bool,bool)));
	
	connect(this,SIGNAL(signOk()),_widgets,SLOT(_slotDevButOk()));
	connect(this,SIGNAL(signMenu()),_widgets,SLOT(_slotDevButMenu()));
	connect(this,SIGNAL(signF1()),_widgets,SLOT(_slotDevButF1()));
	connect(this,SIGNAL(signF2()),_widgets,SLOT(_slotDevButF2()));
	connect(this,SIGNAL(signF3()),_widgets,SLOT(_slotDevButF3()));
	connect(this,SIGNAL(signKeyUp()),_widgets,SLOT(_slotDevButUp()));
	connect(this,SIGNAL(signKeyDown()),_widgets,SLOT(_slotDevButDown()));
	connect(this,SIGNAL(signKeyLeft()),_widgets,SLOT(_slotDevButLeft()));
	connect(this,SIGNAL(signKeyRight()),_widgets,SLOT(_slotDevButRight()));
	connect(_widgets,SIGNAL(signNaviEnd()),this,SLOT(_slotNaviEndPoint()));

	connect(this,SIGNAL(signNaviCoordinstes(double,double)),_widgets,SLOT(_slotMapCoord(double, double)));
	connect(this,SIGNAL(signNaviCoordinstes(double,double)),SLOT(_slotCoordNavi(double,double)));
	
	if(_manualRoute)
		connect(_widgets,SIGNAL(signClearMRoute()),_manualRoute,SLOT(_slotClearMRoute()));
	_widgets->init();
	_slotUpdateScale();
	_createCalculates();
	//_slotStartGps();	
	_slotNothOrient();
}


void  PyramidMediator::_slotZoomIn()
{
	_slotSetScale(_map->lockControlSelector()->scaleView()*0.8);
	_map->unlockControlSelector();
}

void  PyramidMediator::_slotZoomOut()
{
	_slotSetScale(_map->lockControlSelector()->scaleView()*1.25);
	_map->unlockControlSelector();
}

void  PyramidMediator::_slotCentrPos()
{
	_slotCentrPosFlag(true);
	PosBL posBL = {settings.lastLat, settings.lastLon};
	_map->move2Point(posBL);
}

void PyramidMediator::_slotCentrPosFlag(bool flag)
{
	_centrPosition = flag;
}

void PyramidMediator::_slotSetScale(double scale)
{
	//_map->getGisView()->setScaleRange(1000, 40000000);
	if(_map->lockControlSelector()->_setScaleView(scale))
	{	
		settings.scale = scale;
		_map->unlockControlSelector();
		emit signScale(_map->lockControlSelector()->scaleView());
		_map->unlockControlSelector();
	}
	_map->redrawFull();
	if(!_widgets)
		return;
	if(!_widgets->isGenMenuVisible())
		_widgets->updateMapControls();
}
void PyramidMediator::_slotSelectRgn()
{
	if(!_selectRgn)
	{
		_selectRgn = new SelectRgnClass(_parent,_map);
		_selectRgn->show();
		emit signMenuOn(true);
		return;
	}
	emit signMenuOn(false);
	delete _selectRgn;
	_selectRgn = NULL;

}

void PyramidMediator::_slotUpdateScale()
{
	emit signScale(_map->lockControlSelector()->scaleView());
	_map->unlockControlSelector();
}

//удаляем созданные инструменты
void PyramidMediator::_slotDelTools()
{
	if(_selectRgn)
	{
		delete _selectRgn;
		_selectRgn = NULL;
		emit signMenuOn(false);
	}
	_manualRouteFlag = false;
}

bool PyramidMediator::_loadSettings()
{
	unsigned  long Size = 32 + 1;
	char *Buffer = new  char[Size];
	bool bRet = GetComputerName(Buffer, &Size);
	if (!bRet) 
		settings.name_of_pc = "VPU2PC11";
	else
		settings.name_of_pc = QString::fromUtf8(Buffer);
	delete [] Buffer;
	//для тестирования
	//settings.name_of_pc = "ural-tablet-kz";

	settings.path_to_app = QCoreApplication::applicationDirPath();
	QString path_to_ini = QCoreApplication::applicationDirPath () + "/settings_akps.ini";
	if (QFile::exists(path_to_ini) ) 
	{
		QSettings akps_settings (path_to_ini, QSettings::IniFormat);
		settings.id_device = akps_settings.value("programm/id").toString();
		settings.path_to_track = akps_settings.value("programm/path_to_track").toString();
		settings.path_to_photo = akps_settings.value("programm/path_to_photo").toString();

		settings.url_to_kcpsIN = akps_settings.value("programm/url_to_kcpsIN").toString();
		settings.url_to_kcpsOUT = akps_settings.value("programm/url_to_kcpsOUT").toString();

		settings.url_to_vpuIN = akps_settings.value("programm/url_to_vpuIN").toString();
		settings.url_to_vpuOUT = akps_settings.value("programm/url_to_vpuOUT").toString();

		settings.url_to_gkcpsIN = akps_settings.value("programm/url_to_gkcpsIN").toString();
		settings.url_to_gkcpsOUT = akps_settings.value("programm/url_to_gkcpsOUT").toString();

		settings.url_to_shtabIN = akps_settings.value("programm/url_to_shtabIN").toString();
		settings.url_to_shtabOUT = akps_settings.value("programm/url_to_shtabOUT").toString();

		settings.time_to_send_data = akps_settings.value("programm/time_to_send_data").toInt();
		settings.time_zone = akps_settings.value("programm/time_zone").toInt();
		settings.path_to_video = akps_settings.value("programm/path_to_video").toString();
		settings.path_to_route = akps_settings.value("programm/path_to_route").toString();
		settings.path_to_grid = akps_settings.value("programm/path_to_grid").toString();
		settings.path_to_images = akps_settings.value("programm/path_to_images").toString();
		settings.path_to_signs = akps_settings.value("programm/path_to_signs").toString();
		settings.path_to_marks = akps_settings.value("programm/path_to_marks").toString();
		settings.pathToMRoute = akps_settings.value("programm/path_to_mroute").toString();
		settings.gps_settings.qport = akps_settings.value("gps/qport").toString();
		settings.gps_settings.qbrate = akps_settings.value("gps/qbrate").toString();
		settings.gps_settings.qbsize = akps_settings.value("gps/qbsize").toString();
		settings.gps_settings.qparity = akps_settings.value("gps/qparity").toString();
		settings.gps_settings.qstopbits = akps_settings.value("gps/qstopbits").toString();
		settings.lastLat = akps_settings.value("programm/lastLat").toDouble();
		settings.lastLon = akps_settings.value("programm/lastLon").toDouble();
		settings.scale = akps_settings.value("programm/scale").toDouble();
		settings.defSrv = akps_settings.value("programm/def_serv").toInt();
		settings.trackFormat = akps_settings.value("programm/track_format").toInt();
		settings.nameOfCurrentPso = akps_settings.value("pso/nameOfCurrentPso").toString();
		settings.pathToPso = akps_settings.value("pso/pathToPso").toString();


		settings.showTrackLayer = akps_settings.value("programm/showTrackLayer").toBool();
		settings.showRouteLayer = akps_settings.value("programm/showRouteLayer").toBool();
		settings.showMarksPosLayer = akps_settings.value("programm/showMarksPosLayer").toBool();
		settings.showCurrentPosLayer = akps_settings.value("programm/showCurrentPosLayer").toBool();
		settings.showMRouteLayer = akps_settings.value("programm/showMRouteLayer").toBool();
		settings.showGridLayer = akps_settings.value("programm/showGridLayer").toBool();
		settings.showMegafonLayer = akps_settings.value("programm/showMegafonLayer").toBool();
		settings.showBeelineLayer = akps_settings.value("programm/showBeelineLayer").toBool();
		settings.showMtsLayer = akps_settings.value("programm/showMtsLayer").toBool();
		settings.showTele2Layer = akps_settings.value("programm/showTele2Layer").toBool();
		
		return true;
	}
	else
	{
		_saveSettings();
		return false;
	}
}

void PyramidMediator::_saveSettings()
{
	QSettings akps_settings (QCoreApplication::applicationDirPath () + "/settings_akps.ini", QSettings::IniFormat);
	akps_settings.setValue("programm/pc_name", settings.name_of_pc);
	akps_settings.setValue("programm/id", settings.id_device);
	akps_settings.setValue("programm/path_to_track", settings.path_to_track);
	akps_settings.setValue("programm/path_to_photo", settings.path_to_photo);
	akps_settings.setValue("programm/path_to_video", settings.path_to_video);
	akps_settings.setValue("programm/path_to_route", settings.path_to_route);
	akps_settings.setValue("programm/path_to_grid", settings.path_to_grid);
	akps_settings.setValue("programm/path_to_images", settings.path_to_images);
	akps_settings.setValue("programm/path_to_signs", settings.path_to_signs);
	akps_settings.setValue("programm/path_to_marks", settings.path_to_marks);
	akps_settings.setValue("programm/path_to_mroute", settings.pathToMRoute);

	akps_settings.setValue("programm/url_to_kcpsIN", settings.url_to_kcpsIN);
	akps_settings.setValue("programm/url_to_kcpsOUT", settings.url_to_kcpsOUT);
	akps_settings.setValue("programm/url_to_vpuIN", settings.url_to_vpuIN);
	akps_settings.setValue("programm/url_to_vpuOUT", settings.url_to_vpuOUT);
	akps_settings.setValue("programm/url_to_gkcpsIN", settings.url_to_gkcpsIN);
	akps_settings.setValue("programm/url_to_gkcpsOUT", settings.url_to_gkcpsOUT);
	akps_settings.setValue("programm/url_to_shtabIN", settings.url_to_shtabIN);
	akps_settings.setValue("programm/url_to_shtabOUT", settings.url_to_shtabOUT);

	akps_settings.setValue("programm/time_to_send_data", settings.time_to_send_data);
	akps_settings.setValue("programm/time_zone", settings.time_zone);
	akps_settings.setValue("gps/qport", settings.gps_settings.qport);
	akps_settings.setValue("gps/qbrate", settings.gps_settings.qbrate);
	akps_settings.setValue("gps/qbsize", settings.gps_settings.qbsize);
	akps_settings.setValue("gps/qparity", settings.gps_settings.qparity);
	akps_settings.setValue("gps/qstopbits", settings.gps_settings.qstopbits);
	akps_settings.setValue("programm/lastLat", settings.lastLat);
	akps_settings.setValue("programm/lastLon", settings.lastLon);
	akps_settings.setValue("programm/scale", settings.scale);
	akps_settings.setValue("programm/def_serv", settings.defSrv);
	akps_settings.setValue("programm/track_format", settings.trackFormat);

	akps_settings.setValue("programm/showTrackLayer", settings.showTrackLayer);
	akps_settings.setValue("programm/showRouteLayer", settings.showRouteLayer);
	akps_settings.setValue("programm/showMarksPosLayer", settings.showMarksPosLayer);
	akps_settings.setValue("programm/showCurrentPosLayer", settings.showCurrentPosLayer);
	akps_settings.setValue("programm/showMRouteLayer", settings.showMRouteLayer);
	akps_settings.setValue("programm/showGridLayer", settings.showGridLayer);
	akps_settings.setValue("programm/showMegafonLayer", settings.showMegafonLayer);
	akps_settings.setValue("programm/showBeelineLayer", settings.showBeelineLayer);
	akps_settings.setValue("programm/showMtsLayer", settings.showMtsLayer);
	akps_settings.setValue("programm/showTele2Layer", settings.showTele2Layer);
	akps_settings.sync();
}

void PyramidMediator::_createFolders()
{
		QDir dir;
		bool ok = dir.mkpath(settings.path_to_track);
		if(!ok) qDebug() << "can`t create directory";
		
		ok = dir.mkpath(settings.path_to_video);
		if(!ok) qDebug() << "can`t create directory";

		ok = dir.mkpath(settings.path_to_photo);
		if(!ok) qDebug() << "can`t create directory";

		ok = dir.mkpath(settings.path_to_images);
		if(!ok) qDebug() << "can`t create directory";

		ok = dir.mkpath(settings.path_to_grid);
		if(!ok) qDebug() << "can`t create directory";

		ok = dir.mkpath(settings.path_to_route);
		if(!ok) qDebug() << "can`t create directory";

		ok = dir.mkpath(settings.path_to_signs);
		if(!ok) qDebug() << "can`t create directory";

		ok = dir.mkpath(settings.path_to_marks);
		if(!ok) qDebug() << "can`t create directory";

		ok = dir.mkpath(settings.pathToMRoute);
		if(!ok) qDebug() << "can`t create directory";
		
		ok = dir.mkpath(QCoreApplication::applicationDirPath ()+ "/data/pso");
		if(!ok) qDebug() << "can`t create directory";

		ok = dir.mkpath(QCoreApplication::applicationDirPath ()+ "/data/gsm");
		if(!ok) qDebug() << "can`t create directory";

		ok = dir.mkpath(QCoreApplication::applicationDirPath ()+ "/data/gsm/BEELINE");
		if(!ok) qDebug() << "can`t create directory";

		ok = dir.mkpath(QCoreApplication::applicationDirPath ()+ "/data/gsm/MEGAFON");
		if(!ok) qDebug() << "can`t create directory";

		ok = dir.mkpath(QCoreApplication::applicationDirPath ()+ "/data/gsm/MTS");
		if(!ok) qDebug() << "can`t create directory";

		ok = dir.mkpath(QCoreApplication::applicationDirPath ()+ "/data/gsm/TELE2");
		if(!ok) qDebug() << "can`t create directory";


		ok = dir.mkpath(QCoreApplication::applicationDirPath ()+ "/data/ref");
		if(!ok) qDebug() << "can`t create directory";

		ok = dir.mkpath(settings.pathToPso);
		if(!ok) qDebug() << "can`t create directory";

		ok = dir.mkpath(QCoreApplication::applicationDirPath ()+ "/tmp/");
		if(!ok) qDebug() << "can`t create directory";
		
		ok = dir.mkpath(QCoreApplication::applicationDirPath ()+ "/tmp/psoBackup");
		if(!ok) qDebug() << "can`t create directory";
}

void  PyramidMediator::_slotStartGps()
{
	if(_gps)
	{
		_gps->slotDisconnectGps();
		delete _gps;
		_gps = NULL;
	}

	_gps = new GpsClass(this);
	connect(_gps,SIGNAL(signGps(QTime,QDate,bool,double,double,double,double,short,float)),SLOT(_slotGpsData(QTime,QDate,bool,double,double,double,double,short,float)));
	connect(_gps,SIGNAL(signGps(QTime,QDate,bool,double,double,double,double,short,float)),_widgets, SLOT(_slotGpsData(QTime,QDate,bool,double,double,double,double,short,float)));
	if(_posToken)
	{
		connect(_gps,SIGNAL(signGps(QTime,QDate,bool,double,double,double,double,short,float)),_posToken,SLOT(_slotGpsData(QTime,QDate,bool,double,double,double,double,short,float)));
	}
	connect(_gps,SIGNAL(signSendAll(QString,bool)),_widgets,SLOT(_slotGpsData(QString,bool)));
	connect(_gps,SIGNAL(signErrors(QString)),_widgets,SLOT(_slotGpsErrors(QString)));
	connect(_gps,SIGNAL(signGps(QTime,QDate,bool,double,double,double,double,short,float)),_exchangeData, SLOT(_slotGpsData(QTime,QDate,bool,double,double,double,double,short,float)));
	if(_track)
	{
		connect(_gps,SIGNAL(signGps(QTime,QDate,bool,double,double,double,double,short,float,short)),_track,SLOT(_slotGpsData(QTime,QDate,bool,double,double,double,double,short,float,short)));
		connect(_gps,SIGNAL(signSendAll(QString,bool)),_track,SLOT(_slotGpsData(QString,bool)));
	}
	connect(_gps,SIGNAL(signBadSign()),_widgets,SIGNAL(signBadSignGps()));
	connect(_gps,SIGNAL(signNoGpsFound()),_widgets,SLOT(_slotNoGpsFound()));
	connect(_widgets,SIGNAL(signGetNumbOfGps()),_gps,SLOT(_slotNumbOfGps()));
	connect(_gps,SIGNAL(signNumbOfGps(short)),_widgets,SIGNAL(signNumbOfGps(short)));
	connect(_gps,SIGNAL(signNumbOfFindedGps(short)),_widgets,SLOT(_slotNumbOfGps(short)));

	_gps->slotConnectToGps();

}
void  PyramidMediator::_slotStopGps()
{
	//_gps->slotDisconnectGps();
	delete _gps;
	_gps = NULL;
}

void PyramidMediator::_slotGpsData(QTime time, QDate date, bool status, double lat, double lon, double speed, double angle,short sat, float height)
{
	//записываем координаты в файл настроек
	settings.lastLat = lat;
	settings.lastLon = lon;

	//обновление данных на карте каждые 1 секунд
	if(timerMapUpdate->getTime() == 1 || timerMapUpdate->getTime() == 3 || timerMapUpdate->getTime() == 5)
		return;

	//если открыто главное меню то отменить обработку сигналов от gps
	if(_widgets)
	{
		if(_widgets->isGenMenuVisible())
			return;
	}

	//отображаем слой со знаком текущего положения
	if(_tokenLayer)
		_tokenLayer->show();
	
	//если активен флаг центрировать позицию
	if(_centrPosition)
	{
		if(timerMapUpdate->getTime() > 5 )
		{
			//центрируем позицию
			_slotCentrPos();
		}
		else
			_map->redrawDynamic();
		
		//перерисовка виджетов
		_widgets->updateMapControls();
		return;
	}

	//если включен режим ориентации по движению
	if(_moveOrient)
	{	
		//и временной интервал соответствует заданному значению
		if(timerMapUpdate->getTime() > 5 )
		{
			//поворачиваем карту
			_slotAngleMapRadians(-angle);
			//центрируем на текущую позицию
			_slotCentrPos();
			//поворачиваем знак роза ветров
			emit signMapRotete(angle);
			timerMapUpdate->setTime(0);
		}
		else
			_map->redrawDynamic();
		
		//перерисуем виджет
		_widgets->updateMapControls();
		return;
	}



	//перерисуем верхние слои по умолчанию
	_map->redrawDynamic();
	//перерисовка виджетов
	_widgets->updateMapControls();		
}

void PyramidMediator::_createCalculates()
{
	if(_calculates)
	{
		delete _calculates;
		_calculates = NULL;
	}

	_calculates = new CalcClass(this);
	connect(_calculates,SIGNAL(signCalculates(QTime, QTime,short,int,int,short,double)),_widgets,SLOT(_slotCalc(QTime, QTime,short,int,int,short,double)));
	connect(_calculates,SIGNAL(signCalculates(QTime, QTime,short,int,int,short,double)),_exchangeData,SLOT(_slotCalc(QTime, QTime,short,int,int,short,double)));
	connect(_calculates,SIGNAL(signAzimuth(double)),_widgets,SIGNAL(signAzimuth(double)));	
	connect(_gps,SIGNAL(signGps(QTime,QDate,bool,double,double,double,double,short,float)),_calculates,SLOT(_slotGpsData(QTime,QDate,bool,double,double,double,double,short,float)));
	connect(_widgets,SIGNAL(signNaviCoordinstes(double,double)),_calculates,SLOT(_slotCalcAzimuthTo(double,double)));
	connect(_widgets,SIGNAL(signEraseCurentData()),_calculates,SLOT(_slotEraseData()));
}

void PyramidMediator::_slotGeoDataSettings()
{
	GisAccessDlg wgt(_parent);
	wgt.exec();
	//обновляем карту
	emit signInitMapWidget();		
}

void PyramidMediator::_slotLoadSettings()
{
	_loadSettings();
}

void PyramidMediator::_slotPointOnMap()
{
	_coordNaviOn = true;
	return;

	if(!pointOnMapTool)
	{
		pointOnMapTool = new PointOnMapClass(_map->getGisView());
		_map->addGisTool(pointOnMapTool);
		connect(pointOnMapTool,SIGNAL(signSendCoord(double,double)),_widgets,SLOT(_slotMapCoord(double, double)));
		connect(pointOnMapTool,SIGNAL(signSendCoord(double,double)),SLOT(_slotCoordNavi(double,double)));
		connect(pointOnMapTool,SIGNAL(signSendCoord(double,double)),_calculates,SLOT(_slotCalcAzimuthTo(double,double)));
		
		connect(pointOnMapTool,SIGNAL(clickIs()),SLOT(_slotPointOnMap()));
		//connect(_widgets,SIGNAL(signDelNavi()),pointOnMapTool,SLOT(_slotClearLassoLayer()));
		_map->showFullScreen();

	}
	else
	{
		if(_map)
			_map->delGisTool(pointOnMapTool);
		
		delete pointOnMapTool;
		pointOnMapTool = NULL;
		_map->redrawDynamic();
	}
}

void PyramidMediator::_slotSearch()
{
	if(m_pGisFindTopoData)
	{
		_map->resize(_map->width(),_map->height()+290);
		m_pGisFindTopoData->hide();
		delete m_pGisFindTopoData;
		m_pGisFindTopoData = NULL;
		emit signShowSearch();
		emit signMenuOn(false);
	}
	else
	{
		m_pGisFindTopoData = new GisFindTopoData(_map);
		m_pGisFindTopoData->initGisView(_map->getGisView());
		m_pGisFindTopoData->initUNNData((UNNData*)
		_map->lockControlSelector()->getTopoData(), 
		_map->isCurrentLand ());
		_map->unlockControlSelector();
		QRect qrect = _map->geometry();

		_map->resize(_map->width(),_map->height()-290);

		qrect.setY(qrect.height()-290);
		qrect.setHeight(290);
		
		m_pGisFindTopoData->setGeometry(qrect);
		m_pGisFindTopoData->setWindowFlags(Qt::ToolTip | Qt::WindowStaysOnTopHint | Qt::CustomizeWindowHint);
		m_pGisFindTopoData->show();
		
		connect(m_pGisFindTopoData, SIGNAL(updataDynamic()),
			_map, SLOT(redrawDynamic()));
		connect(m_pGisFindTopoData, SIGNAL(updataStatic()),  
			_map, SLOT(redrawFull()));//*
		connect (m_pGisFindTopoData, SIGNAL(showDialogFind(bool)), 
			this, SLOT(_slotSearch()));
		connect (m_pGisFindTopoData, SIGNAL(close_find_d()), 
			this, SLOT(_slotSearch()));
			emit signShowSearch();
		emit signMenuOn(true);
	}
}

bool PyramidMediator::initLayers()
{
	return (   InitLayerForKalka(&_trackLayer) 
			&& InitLayerForKalka(&_gridLayer) 
			&& InitLayerForKalka(&_routeLayer) 
			&& InitLayerForKalka(&_tokenLayer) 
			&& InitLayerForKalka(&_marksLayer)
			&& InitLayerForKalka(&_naviLayer)
			&& InitLayerForKalka(&_megafonLayer)
			&& InitLayerForKalka(&_mtsLayer)
			&& InitLayerForKalka(&_beelineLayer)
			&& InitLayerForKalka(&_tele2Layer)
			&& InitLayerForKalka(&_mRouteLayer));
}



bool PyramidMediator::InitLayerForKalka(StoreVectorData<FastVectorData>** layer)
{
	PosSystem *system = pos_system_new(possys_geodetic);
	*layer = new StoreVectorData<FastVectorData>(system);
	CctError* error = _map->getGisView()->addGeoData(*layer, false);
	if(! cct_error_success( error ) )
	{
		cct_error_free(error);
		delete *layer;
		*layer = NULL;
		return false;
	}
	pos_system_free(system);
	return true;
}

void PyramidMediator::_slotLoadWizardSettings()
{
	ApplyWizzardSettings loadWizSet(this);
	if(_paletka)
		connect(&loadWizSet,SIGNAL(signDrawGrid(QString)),_paletka,SLOT(_slotDrawGrid(QString)));
	connect(&loadWizSet,SIGNAL(signDrawRoute(QString)),SLOT(_slotDrawRoute(QString)));
	//connect(&loadWizSet,SIGNAL(signNavi(double,double)),_widgets,SIGNAL(signCoordNavi(double,double)));
	connect(&loadWizSet,SIGNAL(signNavi(double,double)),SLOT(_slotCoordNavi(double,double)));
	connect(&loadWizSet,SIGNAL(signDiscret(short)),_exchangeData,SLOT(_slotDiscret(short)));
	connect(&loadWizSet,SIGNAL(signDiscret(short)),this,SLOT(_slotSetTimeToSend(short)));
	loadWizSet.loadSettings();
}

void PyramidMediator::_slotSetTimeToSend(short timeInSec)
{
	settings.time_to_send_data = timeInSec;
}

void PyramidMediator::_slotAngleMapRadians(double angle)
{
	if(_map->getGisView()->scale() == angle)
		return;

	_map->getGisView()->rotate(angle);	
	
	emit signMapRotete(angle);
	
}

void PyramidMediator::_slotMoveOrient()
{
	_moveOrient = !_moveOrient;	
}

void PyramidMediator::_slotNothOrient()
{
	_moveOrient = false;
	_slotAngleMapRadians(0);
	_map->redrawFull();
}

void PyramidMediator::_slotSouthOrient()
{
	_moveOrient = false;
	_slotAngleMapRadians(CCT_PI);	
	_map->redrawFull();
}

void PyramidMediator::_slotMoveToPos(double lat, double lon)
{
	PosBL posBL = {lat, lon};
	_map->move2Point(posBL);
	_map->redrawFull();

	double lat_ = _map->getGisView()->snapBLPoint().b;
	double lon_ = _map->getGisView()->snapBLPoint().l;
	
	_slotCentrPosFlag(false);
	_slotViewPos(lat_,lon_);
	
}

void PyramidMediator::_slotLayers()
{
	if(!_map->getGisView()) return;
	ControlSelector *pControlSelector = NULL;

	if(!_map) return;

	pControlSelector = _map->lockControlSelector();

	if(!pControlSelector)
		return;
	emit signMenuOn(true);
	LayerDlg dlg(_parent);
	dlg.init(pControlSelector);

	if(dlg.exec() != QDialog::Accepted)
	{
		emit signMenuOn(false);
		return;
	}
	
	QString modelName(dlg.getDrawModel());
	if(modelName.isEmpty())
		pControlSelector->setDrawModelDef();
	else
	{

		#ifdef UNICODE
			pControlSelector->setDrawModel(modelName.toLocal8Bit());
		#else
			pControlSelector->setDrawModel(modelName.toLocal8Bit());
		#endif

		dlg.getLayers(&pControlSelector->getTopoData()->layers());
		pControlSelector->updateDrawModel();
	}
	emit signMenuOn(false);
	_map->redrawFull();
	 _map->unlockControlSelector();
}

void PyramidMediator::_slotDrawRoute(QString path)
{
	if(_route)
	{
		delete _route;
		_route = NULL;
	}
	_route = new RouteClass(_parent,_map,_routeLayer);		
	connect(_exchangeData,SIGNAL(signRouteLoaded(bool)),_route,SLOT(_slotRouteLoaded(bool)));
	
	if(_widgets)
		connect(_route,SIGNAL(signPathToRoute(QString)),_widgets,SIGNAL(signRoutePath(QString)));

	//если трек удачно отрисован сохранить путь к нему
	if(_route->drawRoute(path))
		_pathToDrawedTrack = path;

}

void PyramidMediator::_slotDrawGrid(QString path)
{
	if(_paletka)
	{
		delete _paletka;
		_paletka = NULL;
	}
	_paletka = new PaletkaClass(_parent,_map,&_gridLayer);
	connect(_exchangeData,SIGNAL(signGridLoaded(bool)),_paletka,SLOT(_slotGridLoaded(bool)));
	if(_widgets)
		connect(_paletka,SIGNAL(signPathToGrid(QString)),_widgets,SIGNAL(signGridPath(QString)));
	_paletka->drawGrid(path);
}

void PyramidMediator::_slotRedrawMap()
{
	_map->redrawFull();
}

void PyramidMediator::_slotSendTrack(QString path, int discret, short toKcps)
{
	emit signSendTrack(RouteClass::nmeaToTkn(path,discret),toKcps);
}

void PyramidMediator::_slotPhoto(QString path,short toKcps)
{
	emit signSendPhoto(_convPhoto(path),toKcps);
}

QString PyramidMediator::_convPhoto(QString pathToPhoto)
{
	//получаем путь к jpg загружаем его в чар
	QByteArray ar = pathToPhoto.toLocal8Bit();

	char* path=ar.data();
	TknPhoto* m_pTknPhoto;
	m_pTknPhoto = new TknPhoto;

	CctError* error = m_pTknPhoto->init(path,_m_factory);

	if(! cct_error_success( error ) )
	{
		const cct_char* ret=cct_error_message(error);
		delete m_pTknPhoto;
		m_pTknPhoto=NULL;
		cct_error_free(error);
		return "";
	}

	QFileInfo file(pathToPhoto);
	QString name = file.completeBaseName();
	QString day = name.section(',', 1, 1);
	QString month = name.section(',', 2, 2);
	QString year = name.section(',', 3, 3);
	QString hour = name.section(',', 4, 4);
	QString min = name.section(',', 5, 5);
	QString sec = name.section(',', 6, 6);
	QString lat = name.section(',', 7, 7);
	QString lon = name.section(',',8 , 8);


	//получить координаты из файла!!!
	Pos2D bl_pos = { lat.toDouble(), lon.toDouble()};
	m_pTknPhoto->setPosition(bl_pos);
	m_pTknPhoto->recalc();

	std::ostrstream pFile;
	bool bSave=m_pTknPhoto->saveToBlobWithRastr(pFile, _m_factory);
	if(!bSave) return "";
	long iLen=pFile.pcount(); 
	if(iLen<1) return "";
	BYTE* m_pChar=(BYTE*)pFile.str();	
	FILE* stream = NULL;


	QString pathToFoto2 = QCoreApplication::applicationDirPath() + "\\tmp\\"+ name+".photo";
	QByteArray ar2 = pathToFoto2.toLocal8Bit();
	path=ar2.data();
	stream = fopen(path , "wb" );
	long f = fwrite(m_pChar, sizeof(BYTE),  iLen, stream); 
	fclose (stream);
	delete m_pTknPhoto;
	m_pTknPhoto=NULL;
	return pathToFoto2;
}

bool PyramidMediator::setFactory(GisGdiFactory *m_factory)
{
	_m_factory = m_factory;
	if(_m_factory)
		return true;
	return false;
}

void PyramidMediator::_slotSendAllPhotos(QStringList photos, short toKcps)
{
	for (int i = 0; i < photos.size(); ++i)
	{
		QString path(photos.at(i));
		emit signSendPhoto(_convPhoto(path),toKcps);
		emit signSendPhotoPro(i+1,photos.size());
	}	
}


void PyramidMediator::_slotMarshNavi()
{
	_slotDelNavi();
	navigator = new NaviClass(this,_map,_pathToDrawedTrack);
	connect(_gps,SIGNAL(signGps(QTime,QDate,bool,double,double,double,double,short,float)),navigator,SLOT(_slotGpsData(QTime,QDate,bool,double,double,double,double,short,float)));
	connect(navigator,SIGNAL(signNaviData(double,double,double,double,double,double,short,double)),_widgets,SIGNAL(signNaviData(double,double,double,double,double,double,short,double)));
	
	connect(navigator,SIGNAL(signNaviSuccess()),_widgets,SLOT(_slotNaviSuccess()));
	connect(navigator,SIGNAL(signNaviSuccess()),SLOT(_slotNaviEndPoint()));

	//отрисовка навигации
	
	if(_drawNavi)
	{
		_naviLayer->deleteAll();
		delete _drawNavi;
		_drawNavi = NULL;
	}

	_drawNavi = new DrawNaviClass(this,_naviLayer);
	connect(navigator,SIGNAL(signDrawOrientLine(double,double,double,double)),_drawNavi,SLOT(_slotNaviLine(double,double,double,double)));
	


	connect(_widgets,SIGNAL(signUncheckedNavi(bool)),navigator,SLOT(_slotUncheckedNavi(bool)));
	connect(_widgets,SIGNAL(signDistNavi(short)),navigator,SLOT(_slotDistNavi(short)));
	if(navigator->initResult)
	{
		_naviOn = true;
		emit signShowNavi();
	}
	else
		_slotDelNavi();
	
}
void PyramidMediator::_slotCoordNavi(double lat, double lon)
{
	_slotDelNavi();

	if(lat == -99 && lon == -99)
	{
		_slotMarshNavi();
		return;
	}

	navigator = new NaviClass(this,_map,lat,lon);
	connect(_gps,SIGNAL(signGps(QTime,QDate,bool,double,double,double,double,short,float)),navigator,SLOT(_slotGpsData(QTime,QDate,bool,double,double,double,double,short,float)));
	connect(navigator,SIGNAL(signNaviData(double,double,double,double,double,double,short,double)),_widgets,SIGNAL(signNaviData(double,double,double,double,double,double,short,double)));
	connect(navigator,SIGNAL(signNaviSuccess()),_widgets,SLOT(_slotNaviSuccess()));
	connect(navigator,SIGNAL(signNaviSuccess()),SLOT(_slotNaviEndPoint()));
	
	//отрисовка навигации
	if(_drawNavi)
	{
		_naviLayer->deleteAll();
		delete _drawNavi;
		_drawNavi = NULL;
	}

	_drawNavi = new DrawNaviClass(this,_naviLayer);
	connect(navigator,SIGNAL(signDrawOrientLine(double,double,double,double)),_drawNavi,SLOT(_slotNaviLine(double,double,double,double)));
	
	
	connect(_widgets,SIGNAL(signDistNavi(short)),navigator,SLOT(_slotDistNavi(short)));

	if(navigator->initResult)
	{
		_naviOn = true;
		emit signShowNavi();
	}
	else
		_slotDelNavi();
	 
}

void PyramidMediator::_slotDelNavi()
{
	if(navigator)
	{
		_naviOn = false;
		delete navigator;
		navigator = NULL;
		_naviLayer->deleteAll();
	}
}

void PyramidMediator::_slotComList()
{

	//if(_gps)
	//{
	//	_gps->slotDisconnectGps();
	//	delete _gps;
	//	_gps = NULL;
	//}

	//QStringList comList;
	//QStringList comP;
	//QStringList typeComP;
	////ищем рабочие ком порты
	//comList = GpsClass::scanComPorts();
	//for(int i = 0; i < comList.size(); i++)
	//{
	//	short res;
	//	res = GpsClass::isGpsCom(comList.at(i));
	//	//ищем ком порты с GPS
	//	if(res > 0)
	//	{
	//		//сохраняем номер
	//		comP <<	comList.at(i);

	//		//сохраняем тип
	//		if(res == 1)
	//		{
	//			QString tmp(tr("GPS (" ) + comList.at(i) + tr(")"));
	//			typeComP << tmp;
	//		}
	//		if(res == 2)
	//		{
	//			QString tmp(tr("Glonass (" ) + comList.at(i) + tr(")"));
	//			typeComP << tmp;
	//		}
	//	}
	//}
	//emit signComList(comP, typeComP);

}

void PyramidMediator::_slotClickPos(double lat,double lon)
{
	if(_coordNaviOn)
	{
		emit signNaviCoordinstes(lat,lon);
		_coordNaviOn = false;
	}

	CalcFunctions::soundPressedBut(5);
	_slotCentrPosFlag(false);

	emit signClickPos(lat,lon);

	if(_manualRouteFlag)
		emit signMRoutePoint(lat,lon);
	
}
void PyramidMediator::_slotNaviEndPoint()
{
	//_slotDelNavi();
	_slotHideNaviLines();
}

void PyramidMediator::_slotSendAllTracks(QStringList listOfTracks, short toKcps)
{
	QStringList tracksTkn;
	//получаем количество элементов в списке
	for (int i = 0; i < listOfTracks.size(); ++i)
	{
		tracksTkn << RouteClass::nmeaToTkn(listOfTracks.at(i),50);
	}

	emit signSendAllTracks(tracksTkn,toKcps);
}

void PyramidMediator::_slotHideNaviLines()
{
	if(_drawNavi)
	{
		_naviLayer->deleteAll();
		delete _drawNavi;
		_drawNavi = NULL;
	}
	_map->redrawDynamic();
} 

void PyramidMediator::_slotViewPos(double lat,double lon)
{
	short angle = (CalcFunctions::calcTrueAngleRad(lat,settings.lastLat,lon,settings.lastLon) - _map->getGisView()->angle())*CCT_180_PI;
	if(angle < 0)
	{
		angle += 360;
	}
	emit signAnglePos(angle);
}

void PyramidMediator::_slotManualRoute()
{
	_manualRouteFlag = true;
}

//ПРИМЕР РАБОТЫ С XML

	//QFile file("c:\\temp\\track.xml");

	//if (!file.open(QIODevice::Append | QIODevice::Text))
	//	return ;

	//QXmlStreamWriter stream(&file);
	//stream.setAutoFormatting(true);

	//stream.writeStartElement("point");

	//stream.writeTextElement("date", date.toString("dd.MM.yyyy"));
	//stream.writeTextElement("time", time.toString("hh:mm:ss"));
	//stream.writeTextElement("lat", QString::number(lat));
	//stream.writeTextElement("lon", QString::number(lon));
	//stream.writeTextElement("angle", QString::number(speed));
	//stream.writeTextElement("sat", QString::number(sat));
	//stream.writeTextElement("height", QString::number(height));
	//stream.writeTextElement("speed", QString::number(speed));

	//stream.writeEndElement();

		//stream.writeEndDocument();
	

	//QFile file("c:\\temp\\track.xml");
	//if(file.exists())
	//{
	//	//если файл существует добавляем элементы

	//	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	//		return ;

	//	//QByteArray xmlData(file.readAll());

	//	QDomDocument doc;

	//	bool res;
	//	res = doc.setContent(&file);
	//	
	//	QDomElement gps = doc.documentElement();

	//	QDomElement point = doc.createElement("point");
	//	gps.appendChild(point);


	//	QDomElement timeXml = doc.createElement("time");
	//	timeXml.appendChild(doc.createTextNode(time.toString("hh:mm:ss")));
	//	point.appendChild(timeXml);

	//	QDomElement latXml = doc.createElement("lat");
	//	latXml.appendChild(doc.createTextNode(QString::number(lat)));
	//	point.appendChild(latXml);

	//	QDomElement lonXml = doc.createElement("lon");
	//	lonXml.appendChild(doc.createTextNode(QString::number(lon)));
	//	point.appendChild(lonXml);

	//	QDomElement angleXml = doc.createElement("angle");
	//	angleXml.appendChild(doc.createTextNode(QString::number(angle)));
	//	point.appendChild(angleXml);

	//	QDomElement satXml = doc.createElement("sat");
	//	satXml.appendChild(doc.createTextNode(QString::number(sat)));
	//	point.appendChild(satXml);

	//	QDomElement heightXml = doc.createElement("height");
	//	heightXml.appendChild(doc.createTextNode(QString::number(height)));
	//	point.appendChild(heightXml);

	//	QDomElement speedXml = doc.createElement("speed");
	//	speedXml.appendChild(doc.createTextNode(QString::number(speed)));
	//	point.appendChild(speedXml);
	//	file.close();

	//	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
	//		return ;

	//	QTextStream out(&file);
	//	out << doc.toString();
	//	file.close();

	////QDomNodeList points = gps.elementsByTagName("point"); 
	////short sizePoints;
	////sizePoints = points.size();
	//
	//}
	//else
	//{
	//	//если файл не существует создаем его
	//	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
	//		return ;

	//	QDomDocument doc;
	//	QDomElement gps = doc.createElement("GPS");
	//	gps.setAttribute("time", time.toString("hh:mm:ss"));
	//	gps.setAttribute("date", date.toString("dd.MM.yyyy"));

	//	QDomElement point = doc.createElement("point");
	//	gps.appendChild(point);


	//	QDomElement timeXml = doc.createElement("time");
	//	timeXml.appendChild(doc.createTextNode(time.toString("hh:mm:ss")));
	//	point.appendChild(timeXml);
	//	
	//	QDomElement latXml = doc.createElement("lat");
	//	latXml.appendChild(doc.createTextNode(QString::number(lat)));
	//	point.appendChild(latXml);

	//	QDomElement lonXml = doc.createElement("lon");
	//	lonXml.appendChild(doc.createTextNode(QString::number(lon)));
	//	point.appendChild(lonXml);

	//	QDomElement angleXml = doc.createElement("angle");
	//	angleXml.appendChild(doc.createTextNode(QString::number(angle)));
	//	point.appendChild(angleXml);

	//	QDomElement satXml = doc.createElement("sat");
	//	satXml.appendChild(doc.createTextNode(QString::number(sat)));
	//	point.appendChild(satXml);

	//	QDomElement heightXml = doc.createElement("height");
	//	heightXml.appendChild(doc.createTextNode(QString::number(height)));
	//	point.appendChild(heightXml);

	//	QDomElement speedXml = doc.createElement("speed");
	//	speedXml.appendChild(doc.createTextNode(QString::number(speed)));
	//	point.appendChild(speedXml);

	//	doc.appendChild(gps);

	//	//cохраняем файл

	//	QTextStream out(&file);
	//	QString str = doc.toString();
	//	out << doc.toString();
	//	file.close();
	//}

void PyramidMediator::_slotClearGrid()
{
	_gridLayer->deleteAll();
	delete _gridLayer;
	_gridLayer = NULL;
}

void PyramidMediator::_slotAddGrid()
{

	QString path = tr("C:\\Sources\\PyramidM_V_2\\gis_utility\\PyramidM\\MSVC_2008\\Debug\\data\\grid\\new2.pal");
	QString name;
	QFile file(path);
	if(!file.open(QIODevice::ReadOnly))
	{
		qDebug() << tr("файла нет");
		return ;
	}

	int size = file.size ();
	file.close();

	FILE* stream2 = NULL;
	QByteArray ar = path.toLocal8Bit();
	char* path_char=ar.data();

	if( (stream2 = fopen( path_char , "rb" )) != NULL )
	{
		char* m_pBYTE=new char[size];

		long f = fread(m_pBYTE, sizeof(char), size, stream2);

		if(f<=0) return ;

		std::istrstream pFile((const char*)m_pBYTE, size);

		TknPaletka* tknPaletka;
		tknPaletka = NULL;
		tknPaletka = new TknPaletka();


		bool bRet =  tknPaletka->loadFromBlob(pFile);

		if(! bRet)	
		{
			qDebug() << tr("ошибка загрузки");
			delete [] m_pBYTE;
			return;
		}
		_gridLayer->addObject(tknPaletka);

		//delete tknPaletka;
		//tknPaletka = NULL;
		delete [] m_pBYTE;
	}
	fclose(stream2);
}

void PyramidMediator::_slotStatusObjects()
{
	if(!_routeLayer || !_gridLayer || !_marksLayer || !_trackLayer || !_tokenLayer || !_mRouteLayer)
		return;

	emit signStatusObjects(_routeLayer->isVisible(), _gridLayer->isVisible(),_marksLayer->isVisible(), _trackLayer->isVisible(), _tokenLayer->isVisible(),_mRouteLayer->isVisible());
}

void PyramidMediator::_slotStatusGsmLayers()
{
	if(!_megafonLayer || !_mtsLayer || !_beelineLayer || !_tele2Layer)	return;
	emit signStatusGsmLayers(_megafonLayer->isVisible(), _beelineLayer->isVisible(), _mtsLayer->isVisible(), _tele2Layer->isVisible());
}

void PyramidMediator::_slotShowRoute(bool show)
{
	if(!_routeLayer)
		return;
	settings.showRouteLayer = show;
	_routeLayer->show(show);
	_map->redrawDynamic();
}
void PyramidMediator::_slotShowGreed(bool show)
{
	if(!_gridLayer)
		return;
	settings.showGridLayer = show;
	_gridLayer->show(show);
	_map->redrawDynamic();
}
void PyramidMediator::_slotShowMarks(bool show)
{
	if(!_marksLayer)
		return;
	settings.showMarksPosLayer = show;
	_marksLayer->show(show);
	_map->redrawDynamic();
}
void PyramidMediator::_slotShowTracks(bool show)
{
	if(!_trackLayer)
		return;
	settings.showTrackLayer = show;
	_trackLayer->show(show);
	_map->redrawDynamic();
}
void PyramidMediator::_slotShowCurrentPos(bool show)
{

	if(!_tokenLayer)
		return;
	settings.showCurrentPosLayer = show;
	_tokenLayer->show(show);
	_map->redrawDynamic();

}
void PyramidMediator::_slotShowManualRoute(bool show)
{
	if(!_mRouteLayer)
		return;
	settings.showMRouteLayer = show;
	_mRouteLayer->show(show);
	_map->redrawDynamic();
}

void PyramidMediator::_stateOfLayers()
{
	_slotShowRoute(settings.showMRouteLayer);
	_slotShowGreed(settings.showGridLayer);
	_slotShowMarks(settings.showMarksPosLayer);
	_slotShowTracks(settings.showTrackLayer);
	_slotShowCurrentPos(settings.showCurrentPosLayer);
	_slotShowManualRoute(settings.showMRouteLayer);
	_slotShowMegafonLayer(settings.showMegafonLayer);
	_slotShowMtsLayer(settings.showMtsLayer);
	_slotShowBeelineLayer(settings.showBeelineLayer);
	_slotShowTele2Layer(settings.showTele2Layer);
}

void PyramidMediator::_slotDeleteLayers()
{
	if(_trackLayer)
	{
		_trackLayer->deleteAll();
		_trackLayer->hide();
	}

	if(_gridLayer)
	{
		_gridLayer->deleteAll();
		_gridLayer->hide();
	}

	if(_routeLayer)
	{
		_routeLayer->deleteAll();
		_routeLayer->hide();
	}

	if(_mRouteLayer)
	{
		_mRouteLayer->deleteAll();
		_mRouteLayer->hide();
	}
	
}

void PyramidMediator::_slotGsmLayers()
{

	//мегафон
	QString megafonPath(QCoreApplication::applicationDirPath ()+ "/data/gsm/MEGAFON/megafon.layer");
	QFile megafon(megafonPath);	
	if(megafon.exists())
		_loadLayerFromBlob(_megafonLayer,megafonPath);
	else
	{
		QDir dir(QCoreApplication::applicationDirPath ()+ "/tmp/gsm/MEGAFON");
		if(dir.exists())
		{
			loadMegafonKml = new LoadKmlFilesClass(_megafonLayer,_megafonColor,QCoreApplication::applicationDirPath ()+ "/tmp/gsm/MEGAFON",this);
			connect(loadMegafonKml,SIGNAL(signWorkCompleate()),SLOT(_slotGsmKmlLoaded()));
			loadMegafonKml->_slotBegin();
		}
	}


	//билайн
	QString beelinePath(QCoreApplication::applicationDirPath ()+ "/data/gsm/BEELINE/beeline.layer");
	QFile beeline(beelinePath);	
	if(beeline.exists())
		_loadLayerFromBlob(_beelineLayer,beelinePath);
	else
	{
		QDir dir(QCoreApplication::applicationDirPath ()+ "/tmp/gsm/BEELINE");

		if(dir.exists())
		{
			loadBeelineKml = new LoadKmlFilesClass(_beelineLayer,_beelineColor,QCoreApplication::applicationDirPath ()+ "/tmp/gsm/BEELINE",this);
			connect(loadBeelineKml,SIGNAL(signWorkCompleate()),SLOT(_slotGsmKmlLoaded()));
			loadBeelineKml->_slotBegin();
		}
		
	}
	//мтс
	QString mtsPath(QCoreApplication::applicationDirPath ()+ "/data/gsm/MTS/mts.layer");
	QFile mts(mtsPath);	
	if(mts.exists())
		_loadLayerFromBlob(_mtsLayer,mtsPath);
	else
	{
		QDir dir(QCoreApplication::applicationDirPath ()+ "/tmp/gsm/MTS");

		if(dir.exists())
		{
			loadMtsKml = new LoadKmlFilesClass(_mtsLayer,_mtsColor,QCoreApplication::applicationDirPath ()+ "/tmp/gsm/MTS",this);
			connect(loadMtsKml,SIGNAL(signWorkCompleate()),SLOT(_slotGsmKmlLoaded()));	
			loadMtsKml->_slotBegin();
		}
	}
	//теле2

	QString tele2Path(QCoreApplication::applicationDirPath ()+ "/data/gsm/TELE2/tele2.layer");
	QFile tele2(tele2Path);	
	if(tele2.exists())
		_loadLayerFromBlob(_tele2Layer,tele2Path);
	else
	{
		QDir dir(QCoreApplication::applicationDirPath ()+ "/tmp/gsm/TELE2");

		if(dir.exists())
		{
			loadTele2Kml = new LoadKmlFilesClass(_tele2Layer,_tele2Color,QCoreApplication::applicationDirPath ()+ "/tmp/gsm/TELE2",this);
			connect(loadTele2Kml,SIGNAL(signWorkCompleate()),SLOT(_slotGsmKmlLoaded()));
			loadTele2Kml->_slotBegin();
		}
	}
}

bool PyramidMediator::_saveLayerToBlob(StoreVectorData<FastVectorData>* layer,QString path)
{
	std::ostrstream pFile;
	bool bSave=layer->saveToBlob(pFile);

	if(!bSave) return false;
	long iLen=pFile.pcount(); 
	if(iLen<1) return false;

	BYTE* m_pChar=(BYTE*)pFile.str();	
	FILE* stream = NULL;
	char* path2;
	QByteArray ar2 = path.toLocal8Bit();
	path2=ar2.data();
	stream = fopen(path2 , "wb" );
	long f = fwrite(m_pChar, sizeof(BYTE),  iLen, stream); 
	fclose (stream);
	delete[] m_pChar;
}

bool PyramidMediator::_loadLayerFromBlob(StoreVectorData<FastVectorData>* layer,QString path)
{	
	QString name;
	QFile file(path);
	if(!file.exists())
		return false;
	int size = file.size ();
	FILE* stream2 = NULL;
	QByteArray ar = path.toLocal8Bit();
	char* path_char=ar.data();

	if( (stream2 = fopen( path_char , "rb" )) != NULL )
	{
		char* m_pBYTE=new char[size];
		long f = fread(m_pBYTE, sizeof(char), size, stream2);
		if(f<=0) return false;
		std::istrstream pFile((const char*)m_pBYTE, size);
		bool bRet =  layer->loadFromBlob(pFile);

		if(! bRet)	
		{
			qDebug() << tr("ошибка загрузки");
			delete [] m_pBYTE;
			return false;
		}
		delete [] m_pBYTE;
	}
	fclose(stream2);
	return true;
}

void PyramidMediator::_slotShowMegafonLayer(bool show)
{
	if(!_megafonLayer)
		return;
	settings.showMegafonLayer = show;
	_megafonLayer->show(show);
	_map->redrawDynamic();
}

void PyramidMediator::_slotShowMtsLayer(bool show)
{
	if(!_mtsLayer)
		return;
	settings.showMtsLayer = show;
	_mtsLayer->show(show);
	_map->redrawDynamic();
}

void PyramidMediator::_slotShowBeelineLayer(bool show)
{
	if(!_beelineLayer)
		return;
	settings.showBeelineLayer = show;
	_beelineLayer->show(show);
	_map->redrawDynamic();
}

void PyramidMediator::_slotShowTele2Layer(bool show)
{
	if(!_tele2Layer)
		return;
	settings.showTele2Layer = show;
	_tele2Layer->show(show);
	_map->redrawDynamic();
}

void PyramidMediator::_slotGsmKmlLoaded()
{
	_rewriteLayers = true;
}