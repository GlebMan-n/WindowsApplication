#include "stdafx.h"
#include <MSVC_2008\SelectRgnClass.h>
#include <gis_tool_qt/GisInterfaceQT/MapBaseWidget.h>
#include <gis_tool_qt/GisInterfaceQT/GisMapQtInterface.h>
#include "gis/GisDataInterface/ControlSelector.h"
#include <gis_tool_qt\GisInterfaceQT\QToolBarRegion.h>
#include <PyramidM\MSVC_2008\CalcFunctions.h>

SelectRgnClass::SelectRgnClass(QMainWindow *parent, MapBaseWidget* mapWidget)
{
	this->setParent(parent);
	setupUi(this);
	_createRegion(mapWidget);
	this->setGeometry(_calcWindowPos(parent));
}

SelectRgnClass::~SelectRgnClass()
{
	delete _toolBarRegion;
	_toolBarRegion = NULL;
}

bool SelectRgnClass::_createRegion(MapBaseWidget* m_mapWidget)
{
	
	_toolBarRegion = new ToolBarRegion(this, QSize(370,50),24);
	_toolBarRegion->setGeometry( CalcFunctions::calcWindowPos(this->geometry(),_toolBarRegion->geometry()) );
	//_toolBarRegion = new ToolBarRegion(this,QSize(370,50));
	_toolBarRegion->init(m_mapWidget); 
	//_toolBarRegion->set
	QFont font = _toolBarRegion->font(); 
	font.setPointSize(24);
	_toolBarRegion->setFont(font);
	_toolBarRegion->show();
	return true;
}

QRect SelectRgnClass::_calcWindowPos(QWidget* widget)
{
	QRect pRect = widget->geometry();
	QRect mRect = this->geometry();
	QRect result;
	result.setX(pRect.width()/2-mRect.width()/2);
	result.setY(pRect.height()/2-mRect.height()/2);
	result.setWidth(mRect.width());
	result.setHeight(mRect.height());
	return result;
}