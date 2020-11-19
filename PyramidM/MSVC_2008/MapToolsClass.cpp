#include "stdafx.h"
#include <PyramidM\MSVC_2008\MapToolsClass.h>
#include <PyramidM\MSVC_2008\LineGetInterface.h>
#include <PyramidM\MSVC_2008\BreakLenLine.h>
#include <gis_tool_qt/GisInterfaceQT/MapBaseWidget.h>
#include <PyramidM\MSVC_2008\LineDialogClass.h>
#include <PyramidM\MSVC_2008\CalcFunctions.h>
#include <gis_tool_qt\GisInterfaceQT\QToolBarMap.h>
#include <gis\GisBase\cppgis\WndData.h>

MapToolsClass::MapToolsClass(QWidget* parent,MapBaseWidget* map)
{
	this->setParent(parent);
	_parent = parent;
	_map = map;
	_breakLineTool = NULL;
	_lenLineTool = NULL;
	lineDialog = NULL;
	toolLasso = NULL;
	gridLayer = NULL;

}

MapToolsClass::~MapToolsClass()
{

}

void MapToolsClass::_slotBreakLine()
{
	if(_breakLineTool)
	{
		if(_map)
			_map->delGisTool(_breakLineTool);
		delete _breakLineTool;
		_breakLineTool = NULL;
		return;
	}

	_breakLineTool = new BreakLenLine(_map->getGisView());
	connect(_breakLineTool,SIGNAL(lineIs()),SLOT(_slotLineDialog()));
	_map->addGisTool(_breakLineTool);
}

void MapToolsClass::_slotLenLine()
{
	if(_lenLineTool)
	{
		if(_map)
		_map->delGisTool(_lenLineTool);
		delete _lenLineTool;
		_lenLineTool = NULL;
		return;
	}

	_lenLineTool = new LineGetInterface(_map->getGisView());
	connect(_lenLineTool,SIGNAL(lineIs()),SLOT(_slotLineDialog()));
	_map->addGisTool(_lenLineTool);
}

void MapToolsClass::_slotLineDialog()
{
	if (!lineDialog)
	{
		lineDialog = new LineDialogClass(_parent,150,100);
		lineDialog->show();
	}

	if (_lenLineTool)
	{
		int  meters = CalcFunctions::findRasst(_lenLineTool->COORDINATES.lat1*CCT_180_PI,_lenLineTool->COORDINATES.lat2*CCT_180_PI,_lenLineTool->COORDINATES.lon1*CCT_180_PI,_lenLineTool->COORDINATES.lon2*CCT_180_PI)*1000;
		short napr = CalcFunctions::calcTrueAngleRad(_lenLineTool->COORDINATES.lat1,_lenLineTool->COORDINATES.lat2,_lenLineTool->COORDINATES.lon1,_lenLineTool->COORDINATES.lon2)*CCT_180_PI;
		lineDialog->setLenLineData(meters,napr);
	}

	if(_breakLineTool)
	{
			int  meters = 0;
			int size = _breakLineTool->points->size()-1;
			for(unsigned int i = 0; i < _breakLineTool->points->size(); i++)
			{
				if(i != size)
					meters += CalcFunctions::findRasst(_breakLineTool->points->at(i).e1*CCT_180_PI,_breakLineTool->points->at(i+1).e1*CCT_180_PI,_breakLineTool->points->at(i).e2*CCT_180_PI,_breakLineTool->points->at(i+1).e2*CCT_180_PI)*1000;

			}
		//int  meters = findRasst(_lenLineTool->COORDINATES.lat1*CCT_180_PI,_lenLineTool->COORDINATES.lat2*CCT_180_PI,_lenLineTool->COORDINATES.lon1*CCT_180_PI,_lenLineTool->COORDINATES.lon2*CCT_180_PI)*1000;
		short napr;
		if(size >1)
			napr = CalcFunctions::calcTrueAngleRad(_breakLineTool->points->at(size-1).e1, _breakLineTool->points->at(size).e1,_breakLineTool->points->at(size-1).e2, _breakLineTool->points->at(size).e2)*CCT_180_PI;
		//lineDialog->setLenLineData(meters,napr);
		lineDialog->setLenLineData(meters,napr);
	}

	
}

void MapToolsClass::_slotDelTools()
{
	if (lineDialog)
	{
		delete lineDialog;
		lineDialog = NULL;
	}

	if(_breakLineTool)
		_slotBreakLine();

	if(_lenLineTool)
		_slotLenLine();

	if(toolLasso)
	{
		if(_map)
			_map->delGisTool(toolLasso);

		delete toolLasso;
		toolLasso = NULL;
	}

}

void MapToolsClass::_slotLassoTool()
{
	if(toolLasso)
	{
		if(_map)
			_map->delGisTool(toolLasso);
		delete toolLasso;
		toolLasso = NULL;
	}
	else
	{
		toolLasso = new LassoTool(_map, NULL);
		connect(toolLasso, SIGNAL(signUpdateMap()), SIGNAL(signUpdateMap()));
		_map->addGisTool(toolLasso);
		
	}
}

void MapToolsClass::_slotDayNight()
{
	bool bRe = _map->getMemBuffer()->isPaletteGrayBg();
	_map->getMemBuffer()->setPaletteBgGray(!bRe);
	_map->redrawFull();
}


void MapToolsClass::_slotGrid()
{
 	if(!(gridLayer))
	{
		gridLayer = new GridBLData();
		CctError * error;

		error = _map->getGisView()->addWndData(gridLayer);
		if(cct_error_fail(error))
		{
			cct_error_free(error);
			delete gridLayer;
			gridLayer = NULL;
			return;
		}
		gridLayer->show (true);
		_map->redrawWnd();
	}
	else
	{
		_map->getGisView()->delWndData(gridLayer);
		_map->redrawWnd();
		delete gridLayer;
		gridLayer = NULL;
	}
}