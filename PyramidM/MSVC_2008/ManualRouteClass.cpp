#include "stdafx.h"
#include <PyramidM\MSVC_2008\CalcFunctions.h>
#include <gis\GisBase\cgis\pos.h>
#include <MSVC_2008\TknRoute.h>
#include <PyramidM\MSVC_2008\ManualRouteClass.h>
#include <gis_tool_qt\GisInterfaceQT\MapBaseWidget.h>
#include <PyramidM\MSVC_2008\RouteClass.h>

ManualRoute::ManualRoute(QObject* parent, StoreVectorData<FastVectorData>* layer,MapBaseWidget* map)
{
	this->setParent(parent);
	_mRouteLayer = layer;
	_map = map;
	_mRoute = new TknRoute();
	_mRoute->SetHiddenText(true);
	_mRouteLayer->addObject(_mRoute);
	setTrackParam(1,1);
	std::string str = "name";
	_mRoute->SetNameTkn(str);
	_mRoute->SetNoteTkn(str);
	_mRoute->hideAllSensors();
	if (_loadSettings())
	{
		QFile file(_pathToMRoute);
		if (file.exists())
		{
			if(_loadManualRoute(_pathToMRoute))
				_map->redrawDynamic();
		}
	}
}

ManualRoute::~ManualRoute()
{
}

void ManualRoute::setTrackParam(double width, short color)
{
	if(!_mRoute)
		return;
	_color = color;
	switch(color)
	{
	case 0:
		_mRoute->SetColorLine (GdiGis::Color::Blue);
		break;
	case 1:
		_mRoute->SetColorLine (GdiGis::Color::Red);
		break;
	case 2:
		_mRoute->SetColorLine (GdiGis::Color::Black);
		break;
	case 3:
		_mRoute->SetColorLine (GdiGis::Color::Purple);
		break;
	case 4:
		_mRoute->SetColorLine (GdiGis::Color::Brown);
		break;
	default:
		_mRoute->SetColorLine (GdiGis::Color::Black);
		break;
	}
	_mRoute->SetWline(width);
}

void ManualRoute::clear()
{	
	if(!_mRoute)
		return;

	_mRoute->vectorPoints.clear();
	_map->redrawDynamic();
}

void ManualRoute::_addPoint(double lat,double lon,QTime time)
{
	if(!_mRoute)
		return;

	Pos2D pos2D = {lat, lon};
	QTime _time;
	if(time.hour() == 0 && time.minute() == 0 && time.second() == 0)
		_time = QTime::currentTime();
	else
		_time = time;
	_mRoute->AddPoint(pos2D.e1,pos2D.e2,_time.hour(),_time.minute(),_time.second());
}

void ManualRoute::_slotAddPoint(double lat, double lon, QTime time)
{
	_addPoint(lat,lon,time);
	_map->redrawDynamic();
	_saveManualRoute(_pathToMRoute);
}

void ManualRoute::_slotClearMRoute()
{
	clear();
	_saveManualRoute(_pathToMRoute);
}

bool ManualRoute::_saveManualRoute(QString path)
{
	if(!_mRoute)
		return false;

	return RouteClass::routeToFile(_mRoute,path);
}

bool  ManualRoute::_loadManualRoute(QString path)
{
	if(!_mRoute)
		return false;

	TknRoute* tmp = RouteClass::routeFromFile(path);
	if(tmp)
	{
		clear();
		_mRoute->vectorPoints.insert(_mRoute->vectorPoints.end(), tmp->vectorPoints.begin(), tmp->vectorPoints.end());
		delete tmp;
		return true;
	}

	delete tmp;
	return false;

}

bool ManualRoute::_loadSettings()
{
	QString path_to_ini = QCoreApplication::applicationDirPath () + "/settings_akps.ini";
	if (QFile::exists(path_to_ini) ) 
	{
		QSettings akps_settings (path_to_ini, QSettings::IniFormat);
		_pathToMRoute = akps_settings.value("programm/path_to_mroute").toString();
		_pathToMRoute += "\\manual.route";
		return true;
	}
	return false;
}