#include <PyramidWidgets\GeneralWizardClass.h>
#include <PyramidWidgets\FileExplorerClass.h>
#include <PyramidWidgets\SelectDialogClass.h>
#include <PyramidWidgets\DialInpCoordClass.h>
#include <PyramidM\MSVC_2008\CalcFunctions.h>

GeneralWizardClass::GeneralWizardClass(QWidget *parent)
{
	this->setParent(parent);
	setupUi(this);
	this->setGeometry(parent->geometry());
	_parent = parent;
	isExec = true;
	Page2->hide();
	Page3->hide();
	Page1->show();
	saveLbl->hide();

	////////////**************//////////////
	connect(nextButPage1,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(cancelButPage1,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(loadNewPalBut,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(pervPaletka,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(drawSelectedPal,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(nextPaletka,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(useLastPaletka,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(dontUsePaletka,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));

	connect(nextButPage2,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(cancelButPage2,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(backButPage2,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(nextRoute,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(loadNewRouteBut,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(pervRoute,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(drawSelectedRoute,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(useLastRoute,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(dontUseRoute,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));

	connect(onMarsh,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(onPoint,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(onCoord,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(sec30,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(min1,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(min3,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(backButPage3,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(saveBut,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(cancelButPage3,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(FinishBut,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(paletSource,SIGNAL(highlighted(int)),SLOT(_slotButtonClickSound()));
	connect(routeSource,SIGNAL(highlighted(int)),SLOT(_slotButtonClickSound()));
	//////////////*****************/////////

	connect(nextButPage1,SIGNAL(clicked()),SLOT(_slotNextPage1()));
	connect(cancelButPage1,SIGNAL(clicked()),SLOT(_slotCancel()));
	connect(loadNewPalBut,SIGNAL(clicked()),SLOT(_slotLoadPaletka()));
	connect(pervPaletka,SIGNAL(clicked()),SLOT(_slotPervPaletka()));
	connect(drawSelectedPal,SIGNAL(clicked()),SLOT(_slotDrawSelectedPaletka()));
	connect(nextPaletka,SIGNAL(clicked()),SLOT(_slotNextPaletka()));
	connect(useLastPaletka,SIGNAL(clicked()),SLOT(_slotDrawLastPaletka()));
	connect(dontUsePaletka,SIGNAL(clicked()),SLOT(_slotDontUsePaletka()));

	connect(nextButPage2,SIGNAL(clicked()),SLOT(_slotNextPage2()));
	connect(cancelButPage2,SIGNAL(clicked()),SLOT(_slotCancel()));
	connect(backButPage2,SIGNAL(clicked()),SLOT(_slotBackPage2()));
	connect(nextRoute,SIGNAL(clicked()),SLOT(_slotNextRoute()));
	connect(loadNewRouteBut,SIGNAL(clicked()),SLOT(_slotLoadRoute()));
	connect(pervRoute,SIGNAL(clicked()),SLOT(_slotPervRoute()));
	connect(drawSelectedRoute,SIGNAL(clicked()),SLOT(_slotDrawSelectedRoute()));
	connect(useLastRoute,SIGNAL(clicked()),SLOT(_slotDrawLastRoute()));
	connect(dontUseRoute,SIGNAL(clicked()),SLOT(_slotDontUseRoute()));

	connect(onMarsh,SIGNAL(clicked()),SLOT(_slotNavMarsh()));
	connect(onPoint,SIGNAL(clicked()),SLOT(_slotNavCoord()));
	connect(onCoord,SIGNAL(clicked()),SLOT(_slotNavPoint()));
	connect(sec30,SIGNAL(clicked()),SLOT(_slotDiscr30()));
	connect(min1,SIGNAL(clicked()),SLOT(_slotDiscr60()));
	connect(min3,SIGNAL(clicked()),SLOT(_slotDiscr180()));
	connect(backButPage3,SIGNAL(clicked()),SLOT(_slotBackPage3()));
	connect(saveBut,SIGNAL(clicked()),SLOT(_slotSave()));
	connect(cancelButPage3,SIGNAL(clicked()),SLOT(_slotCancel()));
	connect(FinishBut,SIGNAL(clicked()),SLOT(_slotFinish()));
	connect(paletSource,SIGNAL(currentIndexChanged (int)),SLOT(_slotPalServChange(int)));
	connect(routeSource,SIGNAL(currentIndexChanged (int)),SLOT(_slotRouteServChange(int)));

	

	if(!_loadSettings())
		return;
		
	_init();
}

GeneralWizardClass::~GeneralWizardClass()
{
}

void GeneralWizardClass::_init()
{
	if(_loadPathSettings())
	{
		fileExplorerPal = new FileExplorerClass(this,_pathToPal,QStringList()<< "*.pal");
		fileExplorerRoute = new FileExplorerClass(this,_pathToRoute,QStringList()<< "*.route");

		_udpatePal();
		_updateRoute();
		paletSource->setCurrentIndex(_toKcps);
		routeSource->setCurrentIndex(_toKcps);
		this->show();
	}
}

bool GeneralWizardClass::_loadPathSettings()
{
	QString path_to_ini = QCoreApplication::applicationDirPath () + "/settings_akps.ini";
	if (QFile::exists(path_to_ini) ) 
	{
		QSettings akps_settings (path_to_ini, QSettings::IniFormat);
		_pathToRoute = akps_settings.value("programm/path_to_route").toString();
		_pathToPal = akps_settings.value("programm/path_to_grid").toString();
		_urlToVpu = akps_settings.value("programm/url_to_vpu").toString();
		_urlToKcps = akps_settings.value("programm/url_to_kcps").toString();
		defLat = akps_settings.value("programm/lastLat").toDouble();
		defLon = akps_settings.value("programm/lastLon").toDouble();
		_toKcps = akps_settings.value("programm/def_serv").toInt();
		return true;
	}
	return false;
}

bool GeneralWizardClass::_loadSettings()
{
	QString path_to_ini = QCoreApplication::applicationDirPath () + "/wizard_akps.ini";
	if (QFile::exists(path_to_ini) ) 
	{
		//если установлен признак установки для чтения то читать остальное
		QSettings wizard_settings (path_to_ini, QSettings::IniFormat);
		_settings.saveIni = wizard_settings.value("wizard/saveIni").toBool();
	
		if(_settings.saveIni)
		{
			SelectDialog dlg(_parent,tr("Мастер"),
					tr("Имеются сохраненные настройки.\nИспользовать их?"),false);
			
			dlg.exec();	
			if(dlg.result == 1)
			{
				isExec = false;
				
				return false;
			}	
		}
		_deleteSettingsFile();
		return true;		
		
	}
	return true;
}

void GeneralWizardClass::_saveSettings()
{
	QSettings wizardSettings (QCoreApplication::applicationDirPath () + "/wizard_akps.ini", QSettings::IniFormat);
	wizardSettings.setValue("wizard/palFullPath", _settings.palFullPath);
	wizardSettings.setValue("wizard/routeFullPath", _settings.routeFullPath);
	
	if(_toKcps == 0)
		wizardSettings.setValue("wizard/palUrl", _urlToKcps);
	else
		wizardSettings.setValue("wizard/palUrl", _urlToVpu);

	if(_toKcps == 0)
		wizardSettings.setValue("wizard/routeUrl", _urlToKcps);
	else
		wizardSettings.setValue("wizard/routeUrl", _urlToVpu);

	
	wizardSettings.setValue("wizard/naviLat", _settings.naviLat);
	wizardSettings.setValue("wizard/naviLon", _settings.naviLon);
	wizardSettings.setValue("wizard/discret", _settings.discret);
	wizardSettings.setValue("wizard/saveIni", _settings.saveIni);
	wizardSettings.sync();
}

//страница 1
void GeneralWizardClass::_slotLoadPaletka()
{
	paletStatus->setText(tr("ожидание ответа"));
	emit signLoadPaletka(_toKcps);
}
void GeneralWizardClass::_slotNextPaletka()
{
	if(fileExplorerPal->getCountOfFiles()>0)
		nameOfPaletka->setText(fileExplorerPal->getNextFile(false));
	else
		nameOfPaletka->setText(tr("нет файлов"));
}
void GeneralWizardClass::_slotPervPaletka()
{
	if(fileExplorerPal->getCountOfFiles()>0)
		nameOfPaletka->setText(fileExplorerPal->getPervFile(false));
	else
		nameOfPaletka->setText(tr("нет файлов"));
}
void GeneralWizardClass::_slotDrawSelectedPaletka()
{
	if(fileExplorerPal->getCountOfFiles()>0)
	{
		_settings.palFullPath = fileExplorerPal->getCurrentFile(true);
		_lockPaletkaButtons();
	}
	else
		nameOfPaletka->setText(tr("нет файлов для отображения"));
}
void GeneralWizardClass::_slotDrawLastPaletka()
{
	if(fileExplorerPal->getCountOfFiles()>0)
	{
		_settings.palFullPath = fileExplorerPal->getLastFile(true);
		_lockPaletkaButtons();
	}
	else
		nameOfPaletka->setText(tr("нет файлов для отображения"));


}
void GeneralWizardClass::_slotDontUsePaletka()
{
	_lockPaletkaButtons();
}

void GeneralWizardClass::_slotNextPage1()
{
	Page1->hide();
	Page2->show();
}

//страница 2
void GeneralWizardClass::_slotLoadRoute()
{
	routeStatus->setText(tr("ожидание ответа"));
	emit signLoadRoute(_toKcps);


}

void GeneralWizardClass::_slotNextRoute()
{
	if(fileExplorerRoute->getCountOfFiles()>0)
		nameOfRoute->setText(fileExplorerRoute->getNextFile(false));
	else
		nameOfRoute->setText(tr("нет файлов"));

}

void GeneralWizardClass::_slotPervRoute()
{
	if(fileExplorerRoute->getCountOfFiles()>0)
		nameOfRoute->setText(fileExplorerRoute->getPervFile(false));
	else
		nameOfRoute->setText(tr("нет файлов"));
}

void GeneralWizardClass::_slotDrawSelectedRoute()
{
	if(fileExplorerRoute->getCountOfFiles()>0)
	{
		_settings.routeFullPath = fileExplorerRoute->getCurrentFile(true);
		_lockRouteButtons();
	}
	else
		nameOfRoute->setText(tr("нет файлов для отображения"));
}

void GeneralWizardClass::_slotDrawLastRoute()
{
	if(fileExplorerRoute->getCountOfFiles()>0)
	{
		_settings.routeFullPath = fileExplorerRoute->getLastFile(true);
		_lockRouteButtons();
	}
	else
		nameOfRoute->setText(tr("нет файлов для отображения"));
}

void GeneralWizardClass::_slotDontUseRoute()
{
	_lockRouteButtons();
}

void GeneralWizardClass::_slotBackPage2()
{
	Page2->hide();
	Page1->show();
}

void GeneralWizardClass::_slotNextPage2()
{
	Page2->hide();
	Page3->show();
}

//страница 3
void GeneralWizardClass::_slotNavMarsh()
{
	_settings.naviLat = -99;
	_settings.naviLon = -99;
	_lockNaviButtons();
}

void GeneralWizardClass::_slotNavCoord()
{
	InputCoordinatesClass inputCoordDlg(_parent,defLat,defLon);
	inputCoordDlg.exec();

	if(inputCoordDlg.result)
	{
		_settings.naviLat = inputCoordDlg.Lat;
		_settings.naviLon = inputCoordDlg.Lon;
		_lockNaviButtons();
	}
	
}

void GeneralWizardClass::_slotNavPoint()
{
	SelectDialog dlg(_parent,tr("Мастер"),
		tr("Щелкнете по карте,\nчто бы задать координаты\nГотовы?"),false);
	dlg.exec();	
	if(dlg.result == 1)
	{
		emit signMapPoint();
	}	
}

void  GeneralWizardClass::_slotMapPoint(double lat, double lon)
{
	//this->show();
	_settings.naviLat = lat;
	_settings.naviLon = lon;
	_lockNaviButtons();
}

void GeneralWizardClass::_slotDiscr30()
{
	_settings.discret = 30;
	_lockDisctButtons();
}

void GeneralWizardClass::_slotDiscr60()
{
	_settings.discret = 60;
	_lockDisctButtons();
}

void GeneralWizardClass::_slotDiscr180()
{
	_settings.discret = 180;
	_lockDisctButtons();
}

void GeneralWizardClass::_slotBackPage3()
{
	Page3->hide();
	Page2->show();
}

void GeneralWizardClass::_slotSave()
{
	_settings.saveIni = true;
	_saveSettings();
	saveLbl->show();
}

void GeneralWizardClass::_slotCancel()
{
	emit signDeleteThis();	
}

void GeneralWizardClass::_slotFinish()
{
	_saveSettings();
	emit signLoadWizSet();
	emit signDeleteThis();
}

void GeneralWizardClass::_lockRouteButtons()
{
	loadNewRouteBut->setEnabled(false);
	pervRoute->setEnabled(false);
	drawSelectedRoute->setEnabled(false);
	nextRoute->setEnabled(false);
	useLastRoute->setEnabled(false);
	dontUseRoute->setEnabled(false);
}

void GeneralWizardClass::_lockPaletkaButtons()
{
	loadNewPalBut->setEnabled(false);
	pervPaletka->setEnabled(false);
	drawSelectedPal->setEnabled(false);
	nextPaletka->setEnabled(false);
	useLastPaletka->setEnabled(false);
	dontUsePaletka->setEnabled(false);
}

void GeneralWizardClass::_lockNaviButtons()
{
	onMarsh->setEnabled(false);
	onPoint->setEnabled(false);
	onCoord->setEnabled(false);
}

void GeneralWizardClass::_lockDisctButtons()
{
	sec30->setEnabled(false);
	min1->setEnabled(false);
	min3->setEnabled(false);
}

bool GeneralWizardClass::_deleteSettingsFile()
{
	_settings = wizardSettings();
	QFile file(QCoreApplication::applicationDirPath () + "/wizard_akps.ini");
	if(file.open(QIODevice::ReadWrite))
	{
		bool result;
		result = file.remove();
		file.close();
		return result;
	}
	return false;
}

void GeneralWizardClass::_slotPaletka(QString fullPath)
{
	if(fullPath ==  "null")
	{
		_slotStatPaletka(false);
		return;
	}
	_settings.palFullPath = fullPath;
	QFileInfo grid(fullPath);
	nameOfPaletka->setText(grid.fileName());
	_lockPaletkaButtons();

}

void GeneralWizardClass::_slotStatPaletka(bool status)
{
	if(status)
	{
		paletStatus->setText(tr("загружена"));
		_lockPaletkaButtons();
	}
	else
		paletStatus->setText(tr("не загружена"));
}

void GeneralWizardClass::_slotRoute(QString fullPath)
{
	if(fullPath ==  "null")
	{
		_slotStatRoute(false);
		return;
	}
	_settings.routeFullPath = fullPath;
	QFileInfo route(fullPath);
	nameOfRoute->setText(route.fileName());
	_lockRouteButtons();
}

void GeneralWizardClass::_slotStatRoute(bool status)
{
	if(status)
	{
		routeStatus->setText(tr("загружен"));
		_lockPaletkaButtons();
	}
	else
		routeStatus->setText(tr("не загружен"));
}

void GeneralWizardClass::_udpatePal()
{
	if(fileExplorerPal->getCountOfFiles()>0)
		nameOfPaletka->setText(fileExplorerPal->getLastFile(false));
	else
		nameOfPaletka->setText(tr("нет файлов"));
}

void GeneralWizardClass::_updateRoute()
{
	if(fileExplorerRoute->getCountOfFiles()>0)
		nameOfRoute->setText(fileExplorerRoute->getLastFile(false));
	else
		nameOfRoute->setText(tr("нет файлов"));
}


void GeneralWizardClass::_slotPalServChange(int toKcps)
{
	_toKcps = toKcps;
}

void GeneralWizardClass::_slotRouteServChange(int toKcps)
{
	_toKcps = toKcps;
}

void GeneralWizardClass::_slotButtonClickSound()
{
	CalcFunctions::soundPressedBut(5);
}