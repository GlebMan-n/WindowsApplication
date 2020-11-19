#include "stdafx.h"
#include <PyramidM\MSVC_2008\ApplyWizardSettings.h>


ApplyWizzardSettings::ApplyWizzardSettings(QObject* parent)
{
	this->setParent(parent);
}

ApplyWizzardSettings::~ApplyWizzardSettings()
{
}

bool ApplyWizzardSettings::loadSettings()
{
	QString path_to_ini = QCoreApplication::applicationDirPath () + "/wizard_akps.ini";
	if (QFile::exists(path_to_ini) ) 
	{
		
		//если установлен признак установки для чтения то читать остальное
		QSettings wizard_settings (path_to_ini, QSettings::IniFormat);
		QString pathToGrid = wizard_settings.value("wizard/palFullPath").toString();
		QString pathToRoute = wizard_settings.value("wizard/routeFullPath").toString();
		double lat = wizard_settings.value("wizard/naviLat").toDouble();
		double lon = wizard_settings.value("wizard/naviLon").toDouble();
		short discret = wizard_settings.value("wizard/discret").toInt();
		if(pathToGrid != "null")
			emit signDrawGrid(pathToGrid);
		if(pathToRoute != "null")
			emit signDrawRoute(pathToRoute);

		emit signNavi(lat, lon);
		emit signDiscret(discret);
		return true;
	}
	return false;
}