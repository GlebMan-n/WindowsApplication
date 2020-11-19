#include "pyramidwidgets.h"
#include <StartScreenClass.h>
#include <PyramidWidgets\GeneralMenuClass.h>
#include <PyramidWidgets\QTimeClass.h>
#include <PyramidWidgets\MapControlsClass.h>
#include <PyramidWidgets\MapMenuClass.h>
#include "PyramidWidgets\GeneralGpsSetClass.h"
#include <PyramidWidgets\CurrentGpsData.h>
#include <PyramidWidgets\GenGpsSensorsClass.h>
#include "PyramidWidgets\GeneralSettingsClass.h"
#include <PyramidWidgets\GeneralConnectionSetClass.h>
#include <PyramidWidgets\GeneralSendDataClass.h>
#include <PyramidWidgets\GeneralWizardClass.h>
#include <PyramidWidgets\GeneralSignClass.h>
#include <PyramidWidgets\SelectDialogClass.h>
#include <PyramidWidgets\MapRoteteClass.h>
#include <PyramidWidgets\MapSelectTools.h>
#include <PyramidWidgets\DialInpCoordClass.h>
#include <PyramidWidgets\MapScaleClass.h>
#include <PyramidWidgets\MapExchangeClass.h>
#include <PyramidWidgets\MapLockScreenClass.h>
#include <PyramidWidgets\MapPhotoClass.h>
#include <PyramidWidgets\GenRaportsClass.h>
#include <PyramidWidgets\MapMarkClass.h>
#include <PyramidWidgets\MapSendTrack.h>
#include <PyramidWidgets\MapSendPhoto.h>	
#include <PyramidWidgets\MapVideoClass.h>	
#include <PyramidWidgets\MapNaviClass.h>
#include <PyramidWidgets\MapViewNaviClass.h>	
#include <PyramidWidgets\MapMessageClass.h>
#include <PyramidM\MSVC_2008\CalcFunctions.h>
#include <PyramidWidgets\DialogGpsSelect.h>
#include <PyramidWidgets\MapManualRoute.h>	
#include <PyramidWidgets\MapTrackSettings.h>	
#include <PyramidWidgets\MapMap.h>
#include <PyramidWidgets\MapSettings.h>
#include <PyramidWidgets\DialogAboutClass.h>	
#include <PyramidWidgets\MapBrightness.h>	
#include <PyramidWidgets\MapHotTab.h>	
#include <PyramidWidgets\MapObjects.h>	
#include <PyramidWidgets\MapMarkEdit.h>
#include <PyramidWidgets\DialogPso.h>
#include <PyramidWidgets\FileExplorerClass.h>
#include <PyramidWidgets\PsoExplorerClass.h>
#include <PyramidWidgets\MapReferance.h>	
#include <PyramidWidgets\MapGsmLayers.h>	

PyramidWidgets::PyramidWidgets(QMainWindow* parent,double version)
{
	_parent = parent;
	//this->setParent(parent);
	_generalMenuForm = NULL;
	_startScreen = NULL;
	_timeClass = NULL;
	_mapControls = NULL;
	_mapMenu = NULL;
	_mapRotate = NULL;
	_generalGpsSett = NULL;
	_wizard = NULL;
	_mapSelectTool = NULL;
	_mapScale = NULL;
	_mapExchange = NULL;
	_mapLock = NULL;
	_mapPhoto = NULL;
	_genRaports = NULL;
	_mapMark = NULL;
	_sendTrack = NULL;
	_sendPhoto = NULL;
	_mapVideo = NULL;
	_mapNavi = NULL;
	_mapViewNavi = NULL;
	_mapMes = NULL;
	_mapMRoute = NULL;
	_mapTrack = NULL;
	_mapMap = NULL;
	_mapSettings = NULL;
	_genConSet = NULL;
	_mapHotTab = NULL;
	_mapBrightness = NULL;
	_mapObjects = NULL;
	_mapGsmLayers = NULL;
	_mapMarkEdit = NULL;
	_mapRef = NULL;
	_isFirstRun = true;
	_isNavi = false;
	_wizzardPoint = false;
	_menuOpened = false;
	_isLocked = false;
	_isTool = false;
	_raportsCoord = false;	
	_memoryLeakMaximumSize = 300;
	_timeClass = new QTimeClass(this);
	connect(this,SIGNAL(signGps(QTime,QDate,bool,double,double,double,double,short,float)),_timeClass,SLOT(_slotGpsData(QTime,QDate,bool,double,double,double,double,short,float)));
	vers = version;
	
}

PyramidWidgets::~PyramidWidgets()
{
	_timeClass->stop();
	while (!_timeClass->isFinished())
	{
		pause::msleep(50);
	}

}

void PyramidWidgets::_slotUpdateGpsPorts()
{
	if(!_generalMenuForm)
		_slotCloseMenu();
	SelectDialog dlg1(_parent,tr("Мастер"),
		tr("\n\nНачинаем поиск портов GPS"),true,2);
	dlg1.cancelButton->hide();
	dlg1.okButton->hide();
	dlg1.exec();
	emit signRestartGps();

	dlgInfoGps = new SelectDialog (_parent,tr("Мастер"),
		tr("\n\nИдет поиск портов,\nпожалуйста подождите"),true);
	dlgInfoGps->cancelButton->hide();
	dlgInfoGps->cancelButton->hide();
	dlgInfoGps->okButton->hide();
	dlgInfoGps->exec();
}

void PyramidWidgets::_showStartScreen(double vers)
{
	_startScreen = new StartScreen(_parent,vers);
	connect(_timeClass,SIGNAL(signTime(QString,QString,QString)),_startScreen,SLOT(slotTime(QString,QString,QString)));

	_startScreen->exec();
	_slotMapControls();
	if(_startScreen)
	{
		delete _startScreen;
		_startScreen = NULL;
	}
	_slotGeneralMenu();
	_slotUpdateGpsPorts();
	
	
}

//Главное меню
void PyramidWidgets::_slotGeneralMenu()
{
	_destroyMenus();
	if(_mapMenu)
	{
		delete _mapMenu;
		_mapMenu = NULL;
	}
	_generalMenuForm = new GeneralMenuForm(_parent,vers);
	connect(_timeClass,SIGNAL(signTime(QString,QString,QString)),_generalMenuForm,SLOT(_slotTime(QString,QString,QString)));
	connect(_generalMenuForm,SIGNAL(signBeginMission()),SLOT(_slotShowMap()));
	connect(_generalMenuForm,SIGNAL(signSearch()),SLOT(_slotHideApp()));
	connect(_generalMenuForm,SIGNAL(signSattelites()),SLOT(_slotSensorsClass()));
	connect(_generalMenuForm,SIGNAL(signSync()),SLOT(_slotGeneralSendData()));
	connect(_generalMenuForm,SIGNAL(signGenSettings()),SLOT(_slotGeneralSettings()));
	connect(_generalMenuForm,SIGNAL(signGpsSettings()),SLOT(_slotGeneralGpsSett()));
	connect(_generalMenuForm,SIGNAL(signConnectionSettings()),SLOT(_slotGeneralConnectionSet()));
	connect(_generalMenuForm,SIGNAL(signOmens()),SLOT(_slotSign()));
	connect(_generalMenuForm,SIGNAL(signExit()),SLOT(_slotExit()));
	connect(_generalMenuForm->versionBut,SIGNAL(clicked()),SLOT(_slotAbout()));
	_isTool = false;
	emit signCloseFeature(true);
	_generalMenuForm->showFullScreen();
}

//Контролы на карте
void PyramidWidgets::_slotMapControls()
{
	if(!_mapControls)
	{
		_mapControls = new MapControlsClass(_parent,this);
		connect(_timeClass,SIGNAL(signTime(QString,QString,QString)),_mapControls,SLOT(_slotTime(QString,QString,QString)));
		connect(this,SIGNAL(signGps(QTime,QDate,bool,double,double,double,double,short,float)),_mapControls,SLOT(_slotGpsData(QTime,QDate,bool,double,double,double,double,short,float)));
		connect(this,SIGNAL(signCalc(QTime,QTime,short,int,int,short,double)),_mapControls,SLOT(_slotCalc(QTime,QTime,short,int,int,short,double)));
		connect(this,SIGNAL(signBadSignGps()),_mapControls,SLOT(_slotBadSign()));
		connect(_mapControls,SIGNAL(signBackToMainMenu()),SLOT(_slotGeneralMenu()));
		connect(_mapControls,SIGNAL(signHideApp()),SLOT(_slotHideApp()));
		connect(_mapControls,SIGNAL(signMapMenu()),SLOT(_slotMapMenu()));
		connect(_mapControls,SIGNAL(signZoomIn()),SIGNAL(signZoomIn()));
		connect(_mapControls,SIGNAL(signZoomOut()),SIGNAL(signZoomOut()));
		connect(_mapControls,SIGNAL(signCenterPos()),SIGNAL(signCenterPos()));
		connect(_mapControls,SIGNAL(signMapLock()),SLOT(_slotMapLock()));
		connect(_mapControls,SIGNAL(signPhotoMenu()),SLOT(_slotMapPhoto()));
		connect(_mapControls,SIGNAL(signMessageMenu()),SLOT(_slotGeneralRaports()));

		connect(_mapControls,SIGNAL(signMarkMenu()),SLOT(_slotToolFeature()));
		connect(_mapControls,SIGNAL(signMarkMenu()),SLOT(_slotMapMarks()));
		

		//connect(this,SIGNAL(signHideLeftMenu()),_mapControls,SIGNAL(signHideMapLeftMenu()));
		connect(_mapControls,SIGNAL(signTrackMenu()),SLOT(_slotTrackSettings()));
		connect(_mapControls,SIGNAL(signTrackMenu()),SLOT(_slotToolFeature()));
		connect(_mapControls,SIGNAL(signUpdateGpsPorts()),SLOT(_slotUpdateGpsPorts()));

		connect(this,SIGNAL(signTrackLogged(bool)),_mapControls,SLOT(slotLogTrack(bool)));
		connect(this,SIGNAL(signGpsOk(bool)),_mapControls,SLOT(_slotGpsErrors(bool)));//если нет ошибок ничего не происходит
		connect(this,SIGNAL(signScale(double)),_mapControls,SLOT(slotScale(double)));
		connect(this,SIGNAL(signShowSearch()),_mapControls,SLOT(_slotChangeForSearch()));
		connect(this,SIGNAL(signCurrentDataSended(bool)),_mapControls,SLOT(_slotCorrentDataSended(bool)));
		connect(this,SIGNAL(signMapRotete(double)),_mapControls,SLOT(_slotRoseWndAngle(double)));
		connect(this,SIGNAL(signAnglePos(short)),_mapControls,SLOT(_slotPosAngle(short)));
		connect(this,SIGNAL(signCloseFeature(bool)),_mapControls,SLOT(_slotCloseFeature(bool)));
		connect(this,SIGNAL(signNoGps()),_mapControls,SLOT(_slotNoGPS()));

		connect(_mapControls,SIGNAL(signalMapRotate()),SLOT(_slotToolFeature()));
		connect(_mapControls,SIGNAL(signalMapRotate()),SLOT(_slotMapRoteteMenu()));
		connect(_mapControls,SIGNAL(signConnectionSettings()),SLOT(_slotGeneralConnectionSet()));
		connect(_mapControls,SIGNAL(signDayNight()),SIGNAL(signDayNight()));

		connect(_mapControls,SIGNAL(signGetNumbOfGps() ), this, SIGNAL(signGetNumbOfGps()) );
		connect(_mapControls,SIGNAL(signCloseMenus() ), this, SLOT(_slotCloseMenu()) );
		connect(this,SIGNAL(signNumbOfGps(short)),_mapControls,SLOT(_slotNumbOfGps(short)));
		connect(this,SIGNAL(signGpsAll(QString,bool)),_mapControls, SLOT(_slotSetCurrentData(QString,bool)));
		_parent->centralWidget()->setLayout(_mapControls->gridLayout);
		_mapControls->hide();
		_mapControls->init();
	}
}

void PyramidWidgets::_slotNumbOfGps(short numb)
{
	dlgInfoGps->close();
	SelectDialog dlg1(_parent,tr("Мастер"),
		tr("\n\nКоличество найденных портов GPS: ") + QString::number(numb),true);
	dlg1.okButton->hide();
	dlg1.exec();
}

//главное меню карты
void PyramidWidgets::_slotMapMenu()
{
	//вызов меню карты
	//создаем главное меню

		if(_isTool)
		{
			//удаляем инструменты с карты или меню созданные в медиаторе
			emit signDelTools();
			//удаляем все меню, созданные ранее
			_destroyMenus();
			delete _mapMenu;
			_mapMenu = NULL;	
			_isTool = false;
			emit signCloseFeature(true);
			return;
		}
		_destroyMenus();
		_mapMenu = new MapMenuClass(_parent);
		connect(_mapMenu->mapBut,SIGNAL(clicked()),SLOT(_slotMapMapBut()));
		connect(_mapMenu->mapBut,SIGNAL(clicked()),SLOT(_slotToolFeature()));
		connect(_mapMenu->settings,SIGNAL(clicked()),SLOT(_slotSettings()));
		connect(_mapMenu->rotate_but,SIGNAL(clicked()),SLOT(_slotMapRoteteMenu()));
		connect(_mapMenu->dist_but,SIGNAL(clicked()),SLOT(_slotMapSelectToolMenu()));
		connect(_mapMenu->scale_but,SIGNAL(clicked()),SLOT(_slotScale()));
		connect(_mapMenu->search_but,SIGNAL(clicked()),SIGNAL(signSearch()));
		connect(_mapMenu->search_but,SIGNAL(clicked()),SLOT(_slotCloseMenu()));
		connect(_mapMenu->exchange_but,SIGNAL(clicked()),SLOT(_slotExchange()));
		connect(_mapMenu->naviBut,SIGNAL(clicked()),SLOT(_slotMapNavi()));
		connect(_mapMenu->manualRoute,SIGNAL(clicked()),SLOT(_slotMapMRoute()));
		connect(_mapMenu->objects,SIGNAL(clicked()),SLOT(_slotMapObjects()));
		connect(_mapMenu->referance,SIGNAL(clicked()),SLOT(_slotMapReferance()));
		

		_mapMenu->slotShow();
		_isTool = true;
		emit signCloseFeature(false);
	

}

//Меню настройки GPS (подменю главного меню)
void PyramidWidgets::_slotGeneralGpsSett()
{
	if(!_generalGpsSett)
	{
		_generalGpsSett = new GeneralGpsSetClass(_parent);
		
		connect(_generalGpsSett,SIGNAL(signScanCom()),SIGNAL(signScanCom()));
		connect(_timeClass,SIGNAL(signTime(QString,QString,QString)),_generalGpsSett,SLOT(_slotTime(QString,QString,QString)));
		connect(_generalGpsSett,SIGNAL(signCurrentData()),SLOT(_slotCurrentGpsData()));
		connect(_generalGpsSett,SIGNAL(signGpsRestart()),SIGNAL(signRestartGps()));
		connect(_generalGpsSett,SIGNAL(signBack()),SLOT(_slotGeneralGpsSett()));
		connect(_generalGpsSett,SIGNAL(signUpdateSettings() ), this, SIGNAL(singUpdateSettings()) );
		connect(_generalGpsSett,SIGNAL(signGetNumbOfGps() ), this, SIGNAL(signGetNumbOfGps()) );
		connect(this,SIGNAL(signNumbOfGps(short)),_generalGpsSett,SLOT(_slotNumbOfGps(short)));
		connect(this,SIGNAL(signGpsOk(bool)),_generalGpsSett,SLOT(_slotIsGps(bool)));
		connect(this,SIGNAL(signComList(QStringList,QStringList)),_generalGpsSett,SLOT(_slotComList(QStringList,QStringList)));
		connect(this,SIGNAL(signBadSignGps()),_generalGpsSett,SLOT(_slotBadSign()));
		connect(this,SIGNAL(signGpsAll(QString,bool)),_generalGpsSett, SLOT(_slotSetCurrentData(QString,bool)));
		connect(this,SIGNAL(signGpsErrors(QString)),_generalGpsSett,SLOT(_slotSetCurrentDataErrors(QString)));
		//connect(_generalGpsSett,SIGNAL(accepted() ), this, SIGNAL(signScanCom()) );
		//connect(_generalGpsSett,SIGNAL(signScanCom()),SIGNAL(signScanCom()));
		_generalGpsSett->init();
		return;
	}
	delete _generalGpsSett;
	_generalGpsSett = NULL;
}

void PyramidWidgets::_slotSensorsClass()
{
	GeneralSensorsClass sensorsClass(_parent);
	connect(this,SIGNAL(signGps(QTime,QDate,bool,double,double,double,double,short,float)),&sensorsClass,SLOT(_slotGpsData(QTime,QDate,bool,double,double,double,double,short,float)));
	connect(_timeClass,SIGNAL(signTime(QString,QString,QString)),&sensorsClass,SLOT(_slotTime(QString,QString,QString)));
	connect(this,SIGNAL(signCalc(QTime,QTime,short,int,int,short,double)),&sensorsClass,SLOT(_slotCalc(QTime,QTime,short,int,int,short,double)));
	connect(this,SIGNAL(signGpsAll(QString,bool)),&sensorsClass, SLOT(slotSetCurrentData(QString,bool)));
	sensorsClass.exec();
}

void PyramidWidgets::_slotCurrentGpsData()
{
	//отобразить диалог "текущие данные GPS"
	CurrentGpsData currentGpsDlg(_parent);
	connect(this,SIGNAL(signGpsAll(QString,bool)),&currentGpsDlg, SLOT(slotSetCurrentData(QString,bool)));
	connect(this,SIGNAL(signGpsErrors(QString)),&currentGpsDlg,SLOT(slotSetCurrentDataErrors(QString)));
	currentGpsDlg.exec();
}

//удаление всех меню из памяти
void  PyramidWidgets::_destroyMenus()
{
	emit signHideLeftMenu();
	if(_generalMenuForm)
	{
		delete _generalMenuForm;
		_generalMenuForm = NULL;
	}

	if(_genConSet)
	{
		delete _genConSet;
		_genConSet = NULL;
	}

	if(_mapBrightness)
	{
		delete _mapBrightness;
		_mapBrightness = NULL;
	}
	
	if(_mapObjects)
	{
		delete _mapObjects;
		_mapObjects = NULL;
	}
	if(_mapGsmLayers)
	{
		delete _mapGsmLayers;
		_mapGsmLayers = NULL;
	}


	if(_mapMarkEdit)
	{
		delete _mapMarkEdit;
		_mapMarkEdit = NULL;
	}

	if(_mapHotTab)
	{
		delete _mapHotTab;
		_mapHotTab = NULL;
	}

	if(_wizard)
	{
		delete _wizard;
		_wizard = NULL;
	}

	if(_mapRotate)
	{
		delete _mapRotate;
		_mapRotate = NULL;
	}

	if(_mapSelectTool)
	{
		delete _mapSelectTool;
		_mapSelectTool = NULL;
	}

	if(_mapScale)
	{
		delete _mapScale;
		_mapScale = NULL;
	}
	
	if(_mapExchange)
	{
		delete _mapExchange;
		_mapExchange = NULL;
	}

	if(_mapPhoto)
	{
		delete _mapPhoto;
		_mapPhoto = NULL;
	}

	if(_mapMark)
	{
		delete _mapMark;
		_mapMark = NULL;
	}
	if(_sendTrack)
	{
		delete _sendTrack;
		_sendTrack = NULL;
	}
	if(_sendPhoto)
	{
		delete _sendPhoto;
		_sendPhoto = NULL;
	}
	if(_mapVideo)
	{
		delete _mapVideo;
		_mapVideo = NULL;
	}
	if(_mapNavi)
	{
		delete _mapNavi;
		_mapNavi = NULL;
	}
	if(_mapMes)
	{
		delete _mapMes;
		_mapMes = NULL;
	}

	if(_mapMRoute)
	{
		delete _mapMRoute;
		_mapMRoute = NULL;
	}

	if(_mapViewNavi)
	{
		delete _mapViewNavi;
		_mapViewNavi = NULL;
	}
	if(_mapTrack)
	{
		delete _mapTrack;
		_mapTrack = NULL;
	}

	if(_mapSettings)
	{
		delete _mapSettings;
		_mapSettings = NULL;
	}

	if(_mapMap)
	{
		delete _mapMap;
		_mapMap = NULL;
	}
	
	if(_mapRef)
	{
		delete _mapRef;
		_mapRef = NULL;
	}
}

void PyramidWidgets::_slotShowMap()
{
	_destroyMenus();
	
	if(_isFirstRun)
	{
		_slotGeneralWizard();
		_isFirstRun = false;
		return;
	}
	this->_parent->showFullScreen();
}


void PyramidWidgets::_slotExit()
{
	SelectDialog dlg1(_parent,tr("Мастер"),
		tr("\n\nВыйти из программы"),true);
	dlg1.exec();
	if(dlg1.result == 1)
	{
		_destroyMenus();
		_parent->close();
	}
}


void PyramidWidgets::_slotHideApp()
{
	this->_parent->showMinimized();
}

void PyramidWidgets::_slotCloseMenu()
{
	if(_mapMenu)
	{
		delete _mapMenu;
		_mapMenu = NULL;
	}
	
	_isTool = false;
	emit signCloseFeature(true);
	
	_destroyMenus();
}


void PyramidWidgets::_slotGpsData(QTime time, QDate date, bool status, double lat, double lon, double speed, double angle,short sat, float height)
{
	//возвращаем размер программы в мегабайтах


	emit signGpsOk(true);
	if(status)
	{
		_slotMapViewNavi();
		emit signGps(time,date,status,lat,lon,speed,angle,sat,height);
		
	}
	else
	{
		emit signGps(time,date,status,lat,lon,0,0,0,height);
	}
}
 
void PyramidWidgets::_slotGpsData(QString string,bool check)
{
	emit signGpsAll(string,check);
}

void PyramidWidgets::_slotGpsErrors(QString errors)
{
	emit signGpsErrors(errors);
	emit signGpsOk(false);
}

void PyramidWidgets::_slotCalc(QTime sunrise, QTime sunset, short akb,int timePso, int avspeed,short timeOfStop,double rasstOb)
{

	//short programmSize = CalcFunctions::getProcessSize();
	//
	//if(programmSize > _memoryLeakMaximumSize)
	//{
	//		//скрываем объекты, вызывающие утечку
	//		signStatusObjects(false,false,true,false,true,false);
	//		
	//		SelectDialog dlg(_parent,tr("Мастер"),
	//			tr("Во избежание потери данных\nперезагрузите программу\nвыйти и войти снова\nвыбрать Продолжить ПСО"),false,15);
	//		dlg.okButton->hide();
	//		dlg.exec();
	//		CalcFunctions::logError("LEAK "+QString::number(_memoryLeakMaximumSize));
	//		_memoryLeakMaximumSize += 5;
	//}

	emit signCalc(sunrise,sunset,akb,timePso,avspeed,timeOfStop,rasstOb);
}

void PyramidWidgets::_slotGeneralSettings()
{
	GeneralSettingsClass genSettings(_parent);
	connect(_timeClass,SIGNAL(signTime(QString,QString,QString)),&genSettings,SLOT(_slotTime(QString,QString,QString)));
	connect(&genSettings,SIGNAL(signGeoDataSettings()), this, SIGNAL(signGeoDataSettings()) );
	connect(&genSettings,SIGNAL(singNewTimeZone()), this, SIGNAL(singUpdateSettings()) );
	connect(&genSettings,SIGNAL(singNewTimeZone()), _timeClass, SLOT(_slotUpdateSettings()) );
	genSettings.exec();
}

void PyramidWidgets::_slotGeneralConnectionSet()
{
	if(_genConSet)
	{
		delete _genConSet;
		_genConSet = NULL;
		return;
	}
	_genConSet = new GeneralConnectionSetClass(_parent);
	connect(_timeClass,SIGNAL(signTime(QString,QString,QString)),_genConSet,SLOT(_slotTime(QString,QString,QString)));
	connect(_genConSet,SIGNAL(signUpdateSettings()),this,SIGNAL(singUpdateSettings()));
	connect(_genConSet,SIGNAL(signDeleteMenu()),this,SLOT(_slotGeneralConnectionSet()));
	_genConSet->show();
}

void PyramidWidgets::_slotGeneralSendData()
{
	GeneralSendDataClass sendData(_parent);
	connect(_timeClass,SIGNAL(signTime(QString,QString,QString)),&sendData,SLOT(_slotTime(QString,QString,QString)));
	connect(&sendData,SIGNAL(signSendAllTracks(QStringList,short) ), this, SIGNAL(signSendAllTracks(QStringList,short)) );
	connect(&sendData,SIGNAL(signSendAllPhotos(QStringList,short) ), this, SIGNAL(signSendAllPhotos(QStringList,short)) );
	connect(&sendData,SIGNAL(signSendAllVideos(QStringList,short) ), this, SIGNAL(signSendAllVideos(QStringList,short)) );


	connect(&sendData,SIGNAL(signSendAllPhotos(short) ), this, SIGNAL(singUpdateSettings()) );
	connect(&sendData,SIGNAL(signSendAllMarks(short) ), this, SIGNAL(signSendAllMarks(short)) );
	connect(&sendData,SIGNAL(signSendAllVideos(short) ), this, SIGNAL(singUpdateSettings()) );

	connect(this,SIGNAL(signSendMarksResult(bool) ), &sendData, SLOT(_slotSetSendProgressMarks(bool)) );
	connect(this,SIGNAL(signSendMarksResult(bool) ), &sendData, SLOT(_slotSetSendStatusMarks(bool)) );
	connect(this,SIGNAL(signSendTraksPro(short,short) ), &sendData, SLOT(_slotSetSendProgressTracks(short,short)) );
	connect(this,SIGNAL(signSendPhotosPro(short,short) ), &sendData, SLOT(_slotSetSendProgressPhotos(short,short)) );
	connect(this,SIGNAL(signSendVideosPro(short,short) ), &sendData, SLOT(_slotSetSendProgressVideos(short,short)) );
	connect(this,SIGNAL(signSendPhotoResult(bool)),&sendData,SLOT(_slotSetSendStatusPhotos(bool)));
	connect(this,SIGNAL(signSendTrackResult(bool)),&sendData,SLOT(_slotSetSendStatusTracks(bool)));
	connect(this,SIGNAL(signSendVideoResult(bool)),&sendData,SLOT(_slotSetSendStatusVideos(bool)));
	sendData.exec();
}

void PyramidWidgets::_slotMapCoord(double lat, double lon)
{
	emit signMapCoord(lat, lon);
}


void PyramidWidgets::_slotMapPointWiz()
{
	if(_wizard)
	{
		_wizard->hide();
		_wizzardPoint = true;
	}
}
void PyramidWidgets::_slotGeneralWizard()
{
	bool res = false;

	QString pathToMarks;
	QString pathToMRoute;
	QString pathToVideo;
	QString pathToPhoto;
	QString pathToTracks;
	QString pathToRoute;
	QString pathToGrid;
	QString path_to_ini = QCoreApplication::applicationDirPath () + "/settings_akps.ini";
	if (QFile::exists(path_to_ini) ) 
	{
		QSettings akps_settings (path_to_ini, QSettings::IniFormat);
		pathToMarks = akps_settings.value("programm/path_to_marks").toString();
		pathToMRoute = akps_settings.value("programm/path_to_mroute").toString();
		pathToVideo =  akps_settings.value("programm/path_to_video").toString();
		pathToPhoto = akps_settings.value("programm/path_to_photo").toString();
		pathToTracks = akps_settings.value("programm/path_to_track").toString();
		pathToRoute = akps_settings.value("programm/path_to_route").toString();
		pathToGrid = akps_settings.value("programm/path_to_grid").toString();
	}
	PsoExplorerClass psoExplorer;
	PsoDialog psoDlg(_parent);
	QString tmp =tr("от ") + psoExplorer.getDateOfCurrentPso().toString("dd.MM.yyyy") + " " + psoExplorer.getTimeOfCurrentPso().toString("hh:mm");
	psoDlg.setDateTime(tmp);
	psoDlg.exec();

	if(psoDlg.result == 0)
	{
		//Продолжить ПСО

		//палетка
		QFile pal(QCoreApplication::applicationDirPath ()+ "/data/pso/pso.pal");
		if(pal.exists())
		{
			signDrawGrid(QCoreApplication::applicationDirPath ()+ "/data/pso/pso.pal");
		}
		//маршрут
		QFile route(QCoreApplication::applicationDirPath ()+ "/data/pso/pso.route");
		if(route.exists())
		{
			signDrawRoute(QCoreApplication::applicationDirPath ()+ "/data/pso/pso.route");
		}

		//ручной маршрут
		//отметки

	}
	else
	{
		
		//делаем бэкап
		psoExplorer.backupPso();
		psoExplorer.createPso();
		

		//psoExplorer.
		//новое ПСО
		//палетка
		QFile pal(QCoreApplication::applicationDirPath ()+ "/data/pso/pso.pal");
		if(pal.exists())
		{
			pal.remove();
		}

		//маршрут
		QFile route(QCoreApplication::applicationDirPath ()+ "/data/pso/pso.route");
		if(route.exists())
		{
			route.remove();
		}

		//отметки (удалить с копией)
		QFile marks(pathToMarks + "//marks.file");
		if(marks.exists())
		{
			marks.remove();
		}
		emit signUpdateMarks();

		//ручной маршрут (очистить)


		emit signClearMRoute();
		//QString pathToTracks;
		//QString pathToRoute;
		//QString pathToGrid;
		//трек

		emit signBreakTrack(true);

		//треки
		bool saveStat;
		FileExplorerClass fileExplorerTracks(this,pathToTracks,QStringList() << "*.current" << "*.finished" );
		saveStat = fileExplorerTracks.deleteAllFiles();

		//все фото
		
		FileExplorerClass fileExplorerPhoto(this,pathToPhoto,QStringList() << "*.jpg");
		saveStat = fileExplorerPhoto.deleteAllFiles();

		//все маршруты

		FileExplorerClass fileExplorerRoute(this,pathToRoute,QStringList() << "*.route");
		saveStat = fileExplorerRoute.deleteAllFiles();

		//все палетки

		FileExplorerClass fileExplorerGrid(this,pathToGrid,QStringList() << "*.pal");
		saveStat = fileExplorerGrid.deleteAllFiles();

		//все видео
		FileExplorerClass fileExplorerVideo(this,pathToVideo,QStringList() << "*.avi");
		saveStat = fileExplorerVideo.deleteAllFiles();
		//очищаем временные счетчики
		emit signEraseCurentData();
		//отображаем все слои объектов
		emit signStatusObjects(true,true,true,true,true,true);
	}

	return;

	if(!_wizard)
	{
		_wizard = new GeneralWizardClass(_parent);
		connect(_wizard,SIGNAL(signMapPoint()),SLOT(_slotMapPointWiz()));
		connect(this,SIGNAL(signMapCoord(double,double)),_wizard,SLOT(_slotMapPoint(double,double)));
		connect(_wizard,SIGNAL(signLoadPaletka(short)),this,SIGNAL(signLoadPaletka(short)));
		connect(_wizard,SIGNAL(signLoadRoute(short)),this,SIGNAL(signLoadRoute(short)));
		connect(_wizard,SIGNAL(signLoadWizSet()),this,SIGNAL(signLoadWizSet()));
		connect(this,SIGNAL(signGridLoaded(bool)),_wizard,SLOT(_slotStatPaletka(bool)));
		connect(this,SIGNAL(signGridPath(QString)),_wizard,SLOT(_slotPaletka(QString)));
		connect(this,SIGNAL(signRouteLoaded(bool)),_wizard,SLOT(_slotStatRoute(bool)));
		connect(this,SIGNAL(signRoutePath(QString)),_wizard,SLOT(_slotRoute(QString)));
		connect(_wizard,SIGNAL(signLoadSaveSettings()),SLOT(_slotShowMap()));
		connect(_wizard,SIGNAL(signDeleteThis()),this,SLOT(_slotGeneralWizard()));

		if(!_wizard->isExec)
			emit signLoadWizSet();
	}
	else
	{
		delete _wizard;
		_wizard = NULL;
	}
}

void PyramidWidgets::_slotSign()
{
	SignClass signs(_parent);
	signs.exec();
}

void PyramidWidgets::_slotSearch()
{
	if(!_isFirstRun)
	{
		_slotShowMap();
		emit signSearch();
	}
	else
	{
		SelectDialog dlg(_parent,tr("Мастер"),
			tr("Сначала нужно начать ПСО"),false);
		dlg.okButton->hide();
		dlg.exec();
	}
}

void PyramidWidgets::_slotMapRoteteMenu()
{
	if(_mapMenu)
	{
		delete _mapMenu;
		_mapMenu = NULL;
	}
	_destroyMenus();
	if(_mapRotate)
	{
		delete _mapRotate;
		_mapRotate = NULL;
			
		_isTool = false;
		emit signCloseFeature(true);

		return;
	}
	_mapRotate = new MapRotateClass(_parent);
	connect(_mapRotate,SIGNAL(signNothOrient()),SIGNAL(signNothOrient()));
	connect(_mapRotate,SIGNAL(signSouthOrient()),SIGNAL(signSouthOrient()));
	connect(_mapRotate,SIGNAL(signMoveOrient()),SIGNAL(signMoveOrient()));

	//connect(_mapRotate,SIGNAL(signNothOrient()),SLOT(_slotMapRoteteMenu()));
	//connect(_mapRotate,SIGNAL(signSouthOrient()),SLOT(_slotMapRoteteMenu()));
	//connect(_mapRotate,SIGNAL(signMoveOrient()),SLOT(_slotMapRoteteMenu()));
	_mapRotate->show();
}

void PyramidWidgets::_slotMapSelectToolMenu()
{
	if(_mapMenu)
	{
		delete _mapMenu;
		_mapMenu = NULL;
	}

	if(_mapSelectTool)
	{
		delete _mapSelectTool;
		_mapSelectTool = NULL;
		return;
	}

	_mapSelectTool = new MapSelectToolClass(_parent);
	connect(_mapSelectTool,SIGNAL(signLineTool()),SIGNAL(signLenLine()));
	connect(_mapSelectTool,SIGNAL(signBreakLineTool()),SIGNAL(signBreakLenLine()));
	//connect(_mapSelectTool,SIGNAL(signManualRoute()),SLOT(_slotMapMRoute()));

	connect(_mapSelectTool,SIGNAL(signLineTool()),SLOT(_slotMapSelectToolMenu()));
	connect(_mapSelectTool,SIGNAL(signBreakLineTool()),SLOT(_slotMapSelectToolMenu()));
	//connect(_mapSelectTool,SIGNAL(signManualRoute()),SLOT(_slotMapSelectToolMenu()));
	_mapSelectTool->show();
}

void PyramidWidgets::_slotGoToPoint()
{
	_menuOpened = true;
	_slotCloseMenu();
	double defLat = 0;
	double defLon = 0;
	_menuOpened = true;
	//получаем координаты по умолчанию из файла настроек
	QString path_to_ini = QCoreApplication::applicationDirPath () + "/settings_akps.ini";
	if (QFile::exists(path_to_ini) ) 
	{
		QSettings akps_settings (path_to_ini, QSettings::IniFormat);
		defLat = akps_settings.value("programm/lastLat").toDouble();
		defLon = akps_settings.value("programm/lastLon").toDouble();
	}

	InputCoordinatesClass inputCoordDlg(_parent,defLat,defLon);

	_isTool = false;
	emit signCloseFeature(true);

	inputCoordDlg.exec();
	_menuOpened = false;
	if(inputCoordDlg.result)
	{
		emit signMapCoord(inputCoordDlg.Lat,inputCoordDlg.Lon);
	}
	_menuOpened = false;
}

void PyramidWidgets::_slotLayers()
{
	_menuOpened = true;
	_slotCloseMenu();

	emit signLayers();
}

void PyramidWidgets::_slotScale()
{
	if(_mapMenu)
	{
		delete _mapMenu;
		_mapMenu = NULL;
	}

	if(_mapScale)
	{
		delete _mapScale;
		_mapScale = NULL;
		return;
	}

	_mapScale = new MapScaleClass(_parent);
	connect(_mapScale,SIGNAL(signScale(double)),SIGNAL(signSetScale(double)));
	connect(_mapScale,SIGNAL(signScale(double)),SLOT(_slotCloseMenu()));
	_mapScale->show();
}

void PyramidWidgets::_slotExchange()
{
	if(_mapMenu)
	{
		delete _mapMenu;
		_mapMenu = NULL;
	}

	if(_mapExchange)
	{
		delete _mapExchange;
		_mapExchange = NULL;
		return;
	}

	_mapExchange = new MapExchangeClass(_parent);
	connect(_mapExchange,SIGNAL(signLoadRoute(short)),SIGNAL(signLoadRoute(short)));
	connect(_mapExchange,SIGNAL(signLoadGrid(short)),SIGNAL(signLoadPaletka(short)));
	connect(this,SIGNAL(signRoutePath(QString)),_mapExchange,SLOT(_slotStatusRoute(QString)));
	connect(this,SIGNAL(signGridPath(QString)),_mapExchange,SLOT(_slotStatusGrid(QString)));
	connect(this,SIGNAL(signSendMarksResult(bool) ), _mapExchange, SLOT(_slotSetSendStatusMarks(bool)) );
	connect(_mapExchange,SIGNAL(signDrawRoute(QString)),SIGNAL(signDrawRoute(QString)));
	connect(_mapExchange,SIGNAL(signDrawGrid(QString)),SIGNAL(signDrawGrid(QString)));

	connect(_mapExchange,SIGNAL(signSendTrack()),SLOT(_slotSendTrack()));
	connect(_mapExchange,SIGNAL(signSendPhoto()),SLOT(_slotSendPhoto()));
	connect(_mapExchange,SIGNAL(signSendVideo()),SLOT(_slotSendVideo()));
	connect(_mapExchange,SIGNAL(signSendMarks(short)), this, SIGNAL(signSendAllMarks(short)));

	_mapExchange->show();
}

bool PyramidWidgets::isGenMenuVisible()
{
	return _generalMenuForm;
}

void PyramidWidgets::_slotMapLock()
{
	void signHideLeftMenu();
	
	if(_mapMenu)
	{
		delete _mapMenu;
		_mapMenu = NULL;
	}
	
	if(_mapLock)
	{
		delete _mapLock;
		_mapLock = NULL;
		return;
	}
	_destroyMenus();
	_isTool = false;
	emit signCloseFeature(true);

	_mapLock = new MapLockScreenClass(_parent);
	connect(_mapLock,SIGNAL(signBrightMax()),this,SIGNAL(signBrightMax()));
	connect(_mapLock,SIGNAL(signBrightMin()),this,SIGNAL(signBrightMin()));
	_mapLock->init();
	_isLocked = true;
	_mapLock->exec();
	
	if(_mapLock)
	{
		delete _mapLock;
		_mapLock = NULL;
		_isLocked = false;	
	}
}

void PyramidWidgets::_slotMapPhoto()
{
	if(_mapMenu)
	{
		delete _mapMenu;
		_mapMenu = NULL;
	}

	if(_mapPhoto)
	{
		delete _mapPhoto;
		_mapPhoto = NULL;
		return;
	}
		
	_destroyMenus();

	_mapPhoto = new MapPhotoClass(_parent);
	connect(this,SIGNAL(signGps(QTime,QDate,bool,double,double,double,double,short,float)),_mapPhoto,SLOT(_slotGpsData(QTime,QDate,bool,double,double,double,double,short,float)));
	_isTool = true;
	emit signCloseFeature(false);
	_mapPhoto->exec();

	
	
	if(_mapPhoto)
	{
		delete _mapPhoto;
		_mapPhoto = NULL;
		return;
	}
}

void PyramidWidgets::_slotGeneralRaports()
{
	_slotCloseMenu();
	if(_mapMenu)
	{
		delete _mapMenu;
		_mapMenu = NULL;
	}

	if(_genRaports)
	{
		delete _genRaports;
		_genRaports = NULL;
		return;
	}

	

	_genRaports = new GenRaportsClass(_parent);
	connect(this,SIGNAL(signGps(QTime,QDate,bool,double,double,double,double,short,float)),_genRaports,SLOT(_slotGpsData(QTime,QDate,bool,double,double,double,double,short,float)));
	connect(_genRaports,SIGNAL(signDeleteThis()),this,SLOT(_slotGeneralRaports()));
	//сигнал о необходимости прислать координаты с карты
	connect(_genRaports,SIGNAL(signMapPoint()),this,SLOT(_slotAzimuthAsc()));
	connect(_genRaports,SIGNAL(signDelNavi()),this,SIGNAL(signDelNavi()));
	connect(this,SIGNAL(signAzimuth(double)),_genRaports,SLOT(_slotAzimuth(double)));
	connect(this,SIGNAL(signAzimuth(double)),_genRaports,SLOT(_slotCloseMenu()));

	connect(this,SIGNAL(signSendMessageResult(bool)),_genRaports,SLOT(_slotRaportResult(bool)));

	connect(_genRaports,SIGNAL(signMessage(QString,short)),this,SIGNAL(signMessage(QString,short)));
	_genRaports->show();
}

void PyramidWidgets::_slotAzimuthAsc()
{
	_raportsCoord = true;	
}

void PyramidWidgets::_slotMapMarks()
{
	if(_mapMenu)
	{
		delete _mapMenu;
		_mapMenu = NULL;
	}

	if(_mapMark)
	{
		delete _mapMark;
		_mapMark = NULL;
		return;
	}

	_destroyMenus();

	_mapMark = new MapMarkClass (_parent);
	connect(this,SIGNAL(signGps(QTime,QDate,bool,double,double,double,double,short,float)),_mapMark,SLOT(_slotGpsData(QTime,QDate,bool,double,double,double,double,short,float)));
	connect(_mapMark,SIGNAL(signMark(short, double, double)),SIGNAL(signMark(short, double, double)));
	connect(_mapMark,SIGNAL(signMarkEdit()),SLOT(_slotMapMarkEdit()));

	connect(this,SIGNAL(signMapCoord(double, double)),_mapMark,SLOT(_slotCoordMap(double, double)));
	_mapMark->show();
	_isTool = true;
	emit signCloseFeature(false);
}

void PyramidWidgets::_slotSendTrack()
{
	if(_mapMenu)
	{
		delete _mapMenu;
		_mapMenu = NULL;
	}

	_destroyMenus();
	
	_sendTrack = new  MapSendTrack(_parent);
	connect(_sendTrack,SIGNAL(signDrawTrack(QString,int)),SIGNAL(signDrawTrack(QString,int)));
	connect(_sendTrack,SIGNAL(signClearTrack()),SIGNAL(signClearTrack()));
	connect(_sendTrack,SIGNAL(signSendTrack(QString,int,short)),SIGNAL(signSendTrack(QString,int,short)));
	connect(this,SIGNAL(signSendTrackResult(bool)),_sendTrack,SLOT(_slotSendTrackStatus(bool)));
	
	_sendTrack->show();
}

void PyramidWidgets::_slotSendPhoto()
{
	if(_mapMenu)
	{
		delete _mapMenu;
		_mapMenu = NULL;
	}

	_destroyMenus();
	
	_sendPhoto = new  MapSendPhoto(_parent);
	connect(_sendPhoto,SIGNAL(signSendPhoto(QString,short)),SIGNAL(signSendPhoto(QString,short)));
	connect(this,SIGNAL(signSendPhotoResult(bool)),_sendPhoto,SLOT(_slotSendPhotoStatus(bool)));
	
	_sendPhoto->show();
}

void PyramidWidgets::_slotSendVideo()
{
	if(_mapMenu)
	{
		delete _mapMenu;
		_mapMenu = NULL;
	}

	_destroyMenus();
	
	_mapVideo = new  MapVideoClass(_parent);
	connect(_mapVideo,SIGNAL(signSendVideo(QString,short)),SIGNAL(signSendVideo(QString,short)));
	connect(this,SIGNAL(signSendVideoResult(bool)),_mapVideo,SLOT(_slotSendVideoStatus(bool)));
	
	_mapVideo->show();
}

void PyramidWidgets::_slotMapNavi()
{
	if(_mapMenu)
	{
		delete _mapMenu;
		_mapMenu = NULL;
	}

	_destroyMenus();

	if(_isNavi)
	{
		_slotNaviOff();
		_isTool = false;
		emit signCloseFeature(true);
		return;
	}

	_mapNavi = new  MapNaviClass(_parent);
	connect(_mapNavi,SIGNAL(signRouteNavi()),SIGNAL(signRouteNavi()));
	
	connect(_mapNavi,SIGNAL(signCoordNavi(double,double)),SIGNAL(signCoordNavi(double,double)));
	
	connect(_mapNavi,SIGNAL(signPointNavi()),SIGNAL(signMapPoint()));
	connect(_mapNavi,SIGNAL(signPointNavi()),SLOT(_slotCloseMenu()));

	//connect(_mapNavi,SIGNAL(signRouteNavi()),SLOT(_slotNaviOn()));
	//connect(_mapNavi,SIGNAL(signCoordNavi(double,double)),SLOT(_slotNaviOn()));
	//connect(_mapNavi,SIGNAL(signPointNavi()),SLOT(_slotNaviOn()));

	_mapNavi->show();
}

void PyramidWidgets::_slotNaviOn()
{
	_isNavi = true;
	_destroyMenus();
	_isTool = false;
	emit signCloseFeature(true);
}

void PyramidWidgets::_slotNaviOff()
{
	_isNavi = false;
	emit signNaviEnd();
	if(_mapViewNavi)
	{
		delete _mapViewNavi;
		_mapViewNavi = NULL;
	}
}
void PyramidWidgets::_slotMapViewNavi()
{
	if(!_isNavi)
		return;

	if(_isMenu())
		return;

	if(_mapLock)
		return;

	if(!_mapViewNavi)
	{
		_mapViewNavi = new MapViewNaviClass(_parent);
		connect(this,SIGNAL(signNaviData(double,double,double,double,double,double,short,double)),_mapViewNavi,SLOT(_slotNaviData(double,double,double,double,double,double,short,double)));
		connect(this,SIGNAL(signNaviEnd()),_mapViewNavi,SLOT(_slotEndNavi()));
		connect(_mapViewNavi,SIGNAL(signUncheckedNavi(bool)),SIGNAL(signUncheckedNavi(bool)));
		connect(_mapViewNavi,SIGNAL(signDistNavi(short)),SIGNAL(signDistNavi(short)));
		_mapViewNavi->show();
	}
}
void PyramidWidgets::_slotNaviSuccess()
{
	CalcFunctions::soundPressedBut(2);
	_slotNaviOff();
	SelectDialog dlg(_parent,tr("навигация окончена"),tr("Конечная точка достигнута"));
	dlg.exec();
	
}

bool PyramidWidgets::_isMenu()
{
	bool result = false;
	if(_startScreen)
		result = true;
	if(_generalMenuForm)
		result = true;
	if(_generalGpsSett)
		result = true;
	if(_generalMenuForm)
		result = true;
	if(_mapMenu)
		result = true;
	if(_mapRotate)
		result = true;
	if(_mapSelectTool)
		result = true;
	if(_mapScale)
		result = true;
	if(_mapExchange)
		result = true;
	if(_mapPhoto)
		result = true;
	if(_genRaports)
		result = true;
	if(_mapMark)
		result = true;
	if(_sendTrack)
		result = true;
	if(_sendPhoto)
		result = true;
	if(_mapVideo)
		result = true;
	if(_mapNavi)
		result = true;
	if(_mapMes)
		result = true;
	if(_mapMRoute)
		result = true;
	if(_mapTrack)
		result = true;
	if(_menuOpened)
		result = true;
	if(_mapSettings)
		result = true;
	if(_mapMap)
		result = true;
	if(_genConSet)
		result = true;
	if(_mapMarkEdit)
		result = true;
	if(_mapObjects)
		result = true;
	if(_mapGsmLayers)
		result = true;
	if(_mapBrightness)
		result = true;
	if(_mapHotTab)
		result = true;
	return result;
}

void PyramidWidgets::updateMapControls()
{
	_parent->centralWidget()->hide();
	_parent->centralWidget()->show();
}	

void PyramidWidgets::_slotMenuOpen(bool open)
{
	_menuOpened = open;
}

void PyramidWidgets::_slotMessages(QString messages)
{
	if(_mapMes)
	{
		delete _mapMes;
		_mapMes = NULL;
	}

	if(_isMenu() || 	_isLocked)
		return;

	_mapMes = new MapMessageClass(_parent);
	connect(_mapMes,SIGNAL(signMessageIsRead(QString,short)),this,SIGNAL(signMessageIsRead(QString,short)));
	connect(this,SIGNAL(signResponseSended(bool)),_mapMes,SLOT(_slotResponseSended(bool)));
	connect(this,SIGNAL(signRoutePath(QString)),_mapMes,SLOT(_slotStatusRoute(QString)));
	connect(this,SIGNAL(signGridPath(QString)),_mapMes,SLOT(_slotStatusGrid(QString)));
	connect(_mapMes,SIGNAL(signGetRoute(short)),SIGNAL(signLoadRoute(short)));
	connect(_mapMes,SIGNAL(signGetGrid(short)),SIGNAL(signLoadPaletka(short)));
	connect(_mapMes,SIGNAL(signDrawRoute(QString)),SIGNAL(signDrawRoute(QString)));
	connect(_mapMes,SIGNAL(signDrawGrid(QString)),SIGNAL(signDrawGrid(QString)));
	_mapMes->init(messages);
	_mapMes->show();
}

void PyramidWidgets::_slotClickPos(double lat,double lon)
{
	if(_mapMark)
	{
		emit signMapCoord(lat,lon);
		_mapMark->show();
	}

	if(_isMenu())
		_slotMapMenu();

	if(_mapMenu)
	{
		delete _mapMenu;
		_mapMenu = NULL;
	}

	if(_wizzardPoint)
	{
		emit signMapCoord(lat,lon);
		_wizzardPoint = false;
		if(_wizard)
			_wizard->show();
	}

	if(_raportsCoord)
	{
		emit signNaviCoordinstes(lat,lon);
		_raportsCoord = false;
	}

	_clickDestroyMenus();
}

//удаление всех меню из памяти
void PyramidWidgets::_clickDestroyMenus()
{
	if(_generalMenuForm)
	{
		delete _generalMenuForm;
		_generalMenuForm = NULL;
	}

	if(_mapRotate)
	{
		delete _mapRotate;
		_mapRotate = NULL;
	}

	if(_mapSelectTool)
	{
		delete _mapSelectTool;
		_mapSelectTool = NULL;
	}

	if(_mapScale)
	{
		delete _mapScale;
		_mapScale = NULL;
	}
	
	if(_mapExchange)
	{
		delete _mapExchange;
		_mapExchange = NULL;
	}

	if(_mapPhoto)
	{
		delete _mapPhoto;
		_mapPhoto = NULL;
	}

	if(_sendTrack)
	{
		delete _sendTrack;
		_sendTrack = NULL;
	}
	if(_sendPhoto)
	{
		delete _sendPhoto;
		_sendPhoto = NULL;
	}
	if(_mapVideo)
	{
		delete _mapVideo;
		_mapVideo = NULL;
	}
	if(_mapNavi)
	{
		delete _mapNavi;
		_mapNavi = NULL;
	}
	if(_mapMes)
	{
		delete _mapMes;
		_mapMes = NULL;
	}

	if(_mapMRoute)
	{
		delete _mapMRoute;
		_mapMRoute = NULL;
	}

	if(_mapViewNavi)
	{
		delete _mapViewNavi;
		_mapViewNavi = NULL;
	}

	if(_mapTrack)
	{
		delete _mapTrack;
		_mapTrack = NULL;
	}

	if(_mapSettings)
	{
		delete _mapSettings;
		_mapSettings = NULL;
	}

	if(_mapMap)
	{
		delete _mapMap;
		_mapMap = NULL;
	}

	if(_mapHotTab)
	{
		delete _mapHotTab;
		_mapHotTab = NULL;
	}

	if(_mapBrightness)
	{
		delete _mapBrightness;
		_mapBrightness = NULL;
	}
	
	if(_mapObjects)
	{
		delete _mapObjects;
		_mapObjects = NULL;
	}
	if(_mapGsmLayers)
	{
		delete _mapGsmLayers;
		_mapGsmLayers = NULL;
	}

	if(_mapMarkEdit)
	{
		delete _mapMarkEdit;
		_mapMarkEdit = NULL;
	}
	
	if(_mapRef)
	{
		delete _mapRef;
		_mapRef = NULL;
	}
	emit signHideLeftMenu();
}

void PyramidWidgets::_slotComList(QStringList comList, QStringList comPortTypes)
{
	//if(_generalGpsSett)

	//	emit signComList(comList, comPortTypes);

	//else
	//{
	//	SelectGpsDial selectGps(_parent,comList,comPortTypes);
	//	if(selectGps.result > 0)
	//	{
	//		emit signRestartGps();
	//		singUpdateSettings();
	//	}
	//}
	
}

void PyramidWidgets::init()
{
	_timeClass->start();
	this->_parent->showFullScreen();
	_showStartScreen(vers);	
	
}

void PyramidWidgets::_slotMapMRoute()
{
	if(_mapMenu)
	{
		delete _mapMenu;
		_mapMenu = NULL;
	}

	if(_mapMRoute)
	{
		delete _mapMRoute;
		_mapMRoute = NULL;
		return;
	}

	_mapMRoute = new MapManualRoute(_parent);
	connect(_mapMRoute,SIGNAL(signManualRoute()),SIGNAL(signManualRoute()));	
	connect(_mapMRoute,SIGNAL(signManualRoute()),SLOT(_slotMapMRoute()));	
	connect(_mapMRoute,SIGNAL(signMRouteCoord(double,double)),SIGNAL(signMRouteCoord(double,double)));
	connect(_mapMRoute,SIGNAL(signClearMRoute()),SIGNAL(signClearMRoute()));
	_mapMRoute->show();
}

void PyramidWidgets::_slotToolFeature()
{
	if(_mapMenu)
	{
		delete _mapMenu;
		_mapMenu = NULL;

	}
	_isTool = true;
	emit signCloseFeature(false);
}

void PyramidWidgets::_slotTrackSettings()
{	
	if(_mapTrack)
	{
		_slotCloseMenu();
		return;
	}

	_destroyMenus();

	_mapTrack = new MapTrackSettings(_parent);
	connect(_mapTrack,SIGNAL(signBreakTrack(bool)),SIGNAL(signBreakTrack(bool)));
	connect(_mapTrack,SIGNAL(signUpdateSettings()),SIGNAL(singUpdateSettings()));
	connect(_mapTrack->apply,SIGNAL(clicked()),SLOT(_slotTrackSettings()));
	_mapTrack->show();
}

void PyramidWidgets::_slotMapMapBut()
{
	if(_mapMenu)
	{
		delete _mapMenu;
		_mapMenu = NULL;
	}

	if(_mapMap)
	{
		delete _mapMap;
		_mapMap = NULL;
		return;
	}

	_mapMap = new MapMapClass(_parent);
	connect(_mapMap,SIGNAL(signSelectMap()),SIGNAL(signSelectRgn()));
	connect(_mapMap,SIGNAL(signSelectMap()),SLOT(_slotToolFeature()));
	connect(_mapMap,SIGNAL(signSelectMap()),SLOT(_slotMapMapBut()));

	connect(_mapMap,SIGNAL(signLasso()),SIGNAL(signToolLasso()));
	connect(_mapMap,SIGNAL(signLasso()),SLOT(_slotToolFeature()));
	connect(_mapMap,SIGNAL(signLasso()),SLOT(_slotMapMapBut()));

	connect(_mapMap,SIGNAL(signDayNight()),SIGNAL(signDayNight()));
	//connect(_mapMap,SIGNAL(signDayNight()),SLOT(_slotCloseMenu()));

	connect(_mapMap,SIGNAL(signLayers()),SLOT(_slotLayers()));

	connect(_mapMap,SIGNAL(signGoToPoint()),SLOT(_slotGoToPoint()));
	connect(_mapMap,SIGNAL(signGoToPoint()),SLOT(_slotToolFeature()));
	connect(_mapMap,SIGNAL(signGoToPoint()),SLOT(_slotMapMapBut()));
	connect(_mapMap->gridBut,SIGNAL(clicked()),SIGNAL(signGrid()));

	_mapMap->show();
}

void PyramidWidgets::_slotSettings()
{
	if(_mapMenu)
	{
		delete _mapMenu;
		_mapMenu = NULL;
	}

	if(_mapSettings)
	{
		delete _mapSettings;
		_mapSettings = NULL;
		return;
	}

	_mapSettings = new MapSettingsClass(_parent);
	connect(_mapSettings,SIGNAL(signTrackSettings()),SLOT(_slotTrackSettings()));
	connect(_mapSettings,SIGNAL(signTrackSettings()),SLOT(_slotToolFeature()));
	connect(_mapSettings,SIGNAL(signSearchGps()),SLOT(_slotUpdateGpsPorts()));
	connect(_mapSettings,SIGNAL(signSpeedFormat()),_mapControls,SLOT(_slotKmBut()));
	connect(_mapSettings,SIGNAL(signBrightness()),SLOT(_slotMapBrightness()));
	connect(_mapSettings,SIGNAL(signDeviceManager()),SLOT(_slotMapHotTab()));
	_mapSettings->show();
}

void PyramidWidgets::_slotAbout()
{
	DialogAboutClass dlg(_parent,vers);
	dlg.exec();
}

void PyramidWidgets::_slotNoGpsFound()
{
		SelectDialog dlg(_parent,tr("Мастер"),
		tr("требуется подключение GPS\nповторите поиск\nпосле подключения"),false);
		dlg.okButton->hide();
		dlg.exec();
		emit signNoGps();
}

void PyramidWidgets::_slotMapBrightness()
{
	_destroyMenus();
	if(_mapMenu)
	{
		delete _mapMenu;
		_mapMenu = NULL;
	}

	if(_mapBrightness)
	{
		delete _mapBrightness;
		_mapBrightness = NULL;
		return;
	}

	_mapBrightness = new MapBrightness(_parent);
	connect(_mapBrightness,SIGNAL(signGetBright()),SIGNAL(signGetBright()));
	connect(_mapBrightness,SIGNAL(signBrightUp()),SIGNAL(signBrightUp()));
	connect(_mapBrightness,SIGNAL(signBrightDown()),SIGNAL(signBrightDown()));
	connect(this,SIGNAL(signBrightStatus(short)),_mapBrightness,SLOT(_slotGetBright(short)));
	_mapBrightness->show();
	_mapBrightness->init();
}

void PyramidWidgets::_slotMapHotTab()
{
	_destroyMenus();
	if(_mapMenu)
	{
		delete _mapMenu;
		_mapMenu = NULL;
	}

	if(_mapHotTab)
	{
		delete _mapHotTab;
		_mapHotTab = NULL;
		return;
	}

	_mapHotTab = new MapHotTab(_parent);
	connect(_mapHotTab,SIGNAL(signSetGps(bool)),SIGNAL(signSetGps(bool)));
	connect(_mapHotTab,SIGNAL(signSet3g(bool)),SIGNAL(signSet3g(bool)));
	connect(_mapHotTab,SIGNAL(signSetWiFi(bool)),SIGNAL(signSetWiFi(bool)));
	connect(_mapHotTab,SIGNAL(signSetBluetooth(bool)),SIGNAL(signSetBluetooth(bool)));
	connect(_mapHotTab,SIGNAL(signSetIndicator(bool)),SIGNAL(signSetIndicator(bool)));
	connect(_mapHotTab,SIGNAL(signSetIpWireless(bool)),SIGNAL(signSetIpWireless(bool)));
	
	connect(_mapHotTab,SIGNAL(signGetGpsStatus()),SIGNAL(signGetGpsStatus()));
	connect(_mapHotTab,SIGNAL(signGet3gStatus()),SIGNAL(signGet3gStatus()));
	connect(_mapHotTab,SIGNAL(signGetWiFiStatus()),SIGNAL(signGetWiFiStatus()));
	connect(_mapHotTab,SIGNAL(signGetBluetoothStatus()),SIGNAL(signGetBluetoothStatus()));
	connect(_mapHotTab,SIGNAL(signGetIndicatorStatus()),SIGNAL(signGetIndicatorStatus()));
	connect(_mapHotTab,SIGNAL(signGetIpWirelessStatus()),SIGNAL(signGetIpWirelessStatus()));

	connect(this,SIGNAL(signGpsStatus(bool)),_mapHotTab,SLOT(_slotStatusGps(bool)));
	connect(this,SIGNAL(sign3gStatus(bool)),_mapHotTab,SLOT(_slotStatus3G(bool)));
	connect(this,SIGNAL(signWiFiStatus(bool)),_mapHotTab,SLOT(_slotStatusWiFi(bool)));
	connect(this,SIGNAL(signBluetoothStatus(bool)),_mapHotTab,SLOT(_slotStatusBluetooth(bool)));
	connect(this,SIGNAL(signIndicatorStatus(bool)),_mapHotTab,SLOT(_slotStatusInd(bool)));
	connect(this,SIGNAL(signIpWirelessStatus(bool)),_mapHotTab,SLOT(_slotStatusIpWireless(bool)));
	connect(this,SIGNAL(signCamStatus(bool)),_mapHotTab,SLOT(_slotStatusCam(bool)));
	connect(this,SIGNAL(signBit3Stat(bool)),_mapHotTab,SLOT(_slotStatusBit3(bool)));

	_mapHotTab->show();
	_mapHotTab->init();
}

void PyramidWidgets::_slotMapObjects()
{
	_destroyMenus();
	if(_mapMenu)
	{
		delete _mapMenu;
		_mapMenu = NULL;
	}

	if(_mapObjects)
	{
		delete _mapObjects;
		_mapObjects = NULL;
		return;
	}

	_mapObjects = new MapObjects(_parent);

	connect(_mapObjects,SIGNAL(signGetStatusObjects()),SIGNAL(signGetStatusObjects()));
	connect(_mapObjects,SIGNAL(signShowRoute(bool)),SIGNAL(signShowRoute(bool)));
	connect(_mapObjects,SIGNAL(signShowGreed(bool)),SIGNAL(signShowGreed(bool)));
	connect(_mapObjects,SIGNAL(signShowMarks(bool)),SIGNAL(signShowMarks(bool)));
	connect(_mapObjects,SIGNAL(signShowTracks(bool)),SIGNAL(signShowTracks(bool)));
	connect(_mapObjects,SIGNAL(signShowCurrentPos(bool)),SLOT(_slotGsmLayers()));
	connect(_mapObjects,SIGNAL(signShowManualRoute(bool)),SIGNAL(signShowManualRoute(bool)));
	connect(this,SIGNAL(signStatusObjects(bool,bool,bool,bool,bool,bool)),_mapObjects,SLOT(_slotStatusObjects(bool,bool,bool,bool,bool,bool)));

	_mapObjects->show();
	_mapObjects->init();
}

/////////////
void PyramidWidgets::_slotGsmLayers()
{
	_destroyMenus();
	if(_mapMenu)
	{
		delete _mapMenu;
		_mapMenu = NULL;
	}

	if(_mapGsmLayers)
	{
		delete _mapGsmLayers;
		_mapGsmLayers = NULL;
		return;
	}

	_mapGsmLayers = new MapGsmLayers(_parent);

	connect(_mapGsmLayers,SIGNAL(signGetStatusGsmLayers()),SIGNAL(signGetStatusGsmLayers()));
	connect(this,SIGNAL(signStatusGsmLayers(bool,bool,bool,bool)),_mapGsmLayers,SLOT(_slotStatusGsmLayers(bool,bool,bool,bool)));
	
	connect(_mapGsmLayers,SIGNAL(signShowMegafon(bool)),SIGNAL(signShowMegafon(bool)));
	connect(_mapGsmLayers,SIGNAL(signShowBeeline(bool)),SIGNAL(signShowBeeline(bool)));
	connect(_mapGsmLayers,SIGNAL(signShowMts(bool)),SIGNAL(signShowMts(bool)));
	connect(_mapGsmLayers,SIGNAL(signShowTele2(bool)),SIGNAL(signShowTele2(bool)));
	
	_mapGsmLayers->show();
	_mapGsmLayers->init();
}

void PyramidWidgets::_slotMapMarkEdit()
{
	_destroyMenus();

	if(_mapMenu)
	{
		delete _mapMenu;
		_mapMenu = NULL;
	}

	if(_mapMarkEdit)
	{
		delete _mapMarkEdit;
		_mapMarkEdit = NULL;
		return;
	}

	_mapMarkEdit = new MapMarkEdit(_parent);

	connect(_mapMarkEdit,SIGNAL(signUpdateMarks()),SIGNAL(signUpdateMarks()));

	_mapMarkEdit->show();

}

bool PyramidWidgets::isGeneralMenuShow()
{
	if(_generalMenuForm)
	{
		return _generalMenuForm->isVisible();
	}
	else
		return false;
}


void  PyramidWidgets::_slotDevButOk()
{
	if(!_isLocked)
	{
		if(_mapExchange)
		{
			_slotCloseMenu();
			delete _mapExchange;
			_mapExchange = NULL;
			return;
		}
		_destroyMenus();
		_slotToolFeature();
		_slotExchange();
	}
}

void  PyramidWidgets::_slotDevButMenu()
{
	if(!_isLocked)
	{
		_destroyMenus();
		_slotGeneralMenu();
	}
}

void  PyramidWidgets::_slotDevButF1()
{
	if(!_isLocked)
	{
		if(_mapScale)
		{
			_slotCloseMenu();
			delete _mapScale;
			_mapScale = NULL;
			return;
		}
		_destroyMenus();
		_slotToolFeature();
		_slotScale();
	}
}

void  PyramidWidgets::_slotDevButF2()
{
	if(!_isLocked)
	{
		if(_mapPhoto)
		{
			_slotCloseMenu();
			delete _mapPhoto;
			_mapPhoto = NULL;
			return;
		}
		_destroyMenus();
		_slotToolFeature();
		_slotMapPhoto();
	}
}

void  PyramidWidgets::_slotDevButF3()
{
	if(!_isLocked)
	{
		if(_mapRotate)
		{
			_slotCloseMenu();
			delete _mapRotate;
			_mapRotate = NULL;
			return;
		}
		_destroyMenus();
		_slotToolFeature();
		_slotMapRoteteMenu();
	}
}

void  PyramidWidgets::_slotDevButUp()
{
	if(!_isLocked)
	{
		emit signZoomIn();
	}
}

void  PyramidWidgets::_slotDevButDown()
{
	if(!_isLocked)
	{
		emit signZoomOut();
	}
}

void PyramidWidgets::_slotDevButLeft()
{
	if(!_isLocked)
	{
		emit signBrightDown();
	}
}

void  PyramidWidgets::_slotDevButRight()
{
	
	if(!_isLocked)
	{
		emit signBrightUp();
	}
}

void PyramidWidgets::_slotMapReferance()
{
	_destroyMenus();

	QString pathToRef = QCoreApplication::applicationDirPath () + "/data/ref/";

	QFileInfoList fileInfo;
	QDir dir(pathToRef);
	fileInfo = dir.entryInfoList(QStringList() << "*.file" , QDir::Files);
	if(fileInfo.size() < 1)
		return;
	if(_mapMenu)
	{
		delete _mapMenu;
		_mapMenu = NULL;
	}

	if(_mapRef)
	{
		delete _mapRef;
		_mapRef = NULL;
		return;
	}

	QString ref;
	_mapRef = new MapReferanceClass(_parent);

	
	_mapRef->setPathList(fileInfo);
	_mapRef->init();
}