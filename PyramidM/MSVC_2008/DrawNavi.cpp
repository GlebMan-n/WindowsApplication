#include "stdafx.h"
#include "gis\GrUnits\TknMapScale.h"
#include <PyramidM\MSVC_2008\DrawNavi.h>

DrawNaviClass::DrawNaviClass(QObject* parent,StoreVectorData<FastVectorData>* layer)
{
	this->setParent(parent);
	_workLayer = layer;
	_showNavi = true;
	if(layer)
	//инициируем линии
		_init();
	//layer->addObject(_endRoutePoint);
	layer->addObject(_naviLine);
	//_endRoutePoint->hide();
	_naviLine->hide();	
}

DrawNaviClass::~DrawNaviClass()
{
}

void DrawNaviClass::_slotEndPoint(double lat, double lon)
{
	//if(_showNavi)
	//{
	//	Pos2D coord;
	//	coord.e1 = lat;
	//	coord.e2 = lon;
	//	_endRoutePoint->set(0,coord);
	//	_endRoutePoint->set(1,coord);
	//	_endRoutePoint->show();

	//}
}

void DrawNaviClass::_slotNaviLine(double posLat, double posLon, double pointLat, double pointLon)
{
	Pos2D coord1;
	coord1.e1 = posLat;
	coord1.e2 = posLon;

	Pos2D coord2;
	coord2.e1 = pointLat;
	coord2.e2 = pointLon;

	_naviLine->set(0,coord1);
	_naviLine->set(1,coord2);
	_naviLine->show();
}

void DrawNaviClass::_init()
{
	//_endRoutePoint = new TknSimpleLines(2);
	_naviLine = new TknSimpleLines(2);
	//_endRoutePoint->setColor (GdiGis::Color::Red);
	_naviLine->setColor (GdiGis::Color::Red);

	//_endRoutePoint->setLineType(TypeLinesObj::LINE_TYPE::DOT_LINE);
	_naviLine->setLineType(TypeLinesObj::LINE_TYPE::DOT_LINE);

	//_endRoutePoint->setWidth(1.0);
	_naviLine->setWidth(1.0);

	TypeGeoToken tokenLeft;
	CircleUnit *u1 = new CircleUnit;
	u1->m_circle[1].X = 3;
	u1->makeCircle();
	u1->m_width = 0.5;
	u1->m_color = Color::Red;
	tokenLeft.addUnit2BaseFeature(u1);

	//_endRoutePoint->setStartLineCap(tokenLeft);
	//_endRoutePoint->setEndLineCap(tokenLeft);

	_naviLine->setStartLineCap(tokenLeft);
	_naviLine->setEndLineCap(tokenLeft);
}
void DrawNaviClass::hide()
{
	Pos2D coord1;
	coord1.e1 = 0;
	coord1.e2 = 0;
	_naviLine->set(0,coord1);
	_naviLine->set(1,coord1);

	//_endRoutePoint->hide();
}

void DrawNaviClass::_slotClearNavi()
{
	//_showNavi = false;
}