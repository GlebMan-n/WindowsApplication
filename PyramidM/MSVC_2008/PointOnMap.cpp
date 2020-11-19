#include "stdafx.h"
#include <MSVC_2008\PointOnMap.h>

PointOnMapClass::PointOnMapClass(GisView *pt) : GisInputInterface (pt, true)
{
	PosSystem *system = pos_system_new (possys_geodetic);
	m_pLassoLayer = new VectorData(system);
	pos_system_free (system);
	CctError *error = NULL;
	error = m_gisView->addTopGeoData(m_pLassoLayer);
	if( cct_error_fail(error)) cct_error_free(error);
	m_pTkn = new TknSimpleLines(2);
	m_pTkn->setColor (GdiGis::Color::Red);
	m_pTkn->setLineType(TypeLinesObj::LINE_TYPE::DOT_LINE);
	m_pTkn->setWidth(1.0);
	TypeGeoToken tokenLeft;
	CircleUnit *u1 = new CircleUnit;
	u1->m_circle[1].X = 3;
	u1->makeCircle();
	u1->m_width = 0.5;
	u1->m_color = Color::Red;
	tokenLeft.addUnit2BaseFeature(u1);
	m_pTkn->setStartLineCap(tokenLeft);
	m_pTkn->setEndLineCap(tokenLeft);
	m_pLassoLayer->addObject(m_pTkn);
	m_pTkn->hide();

}

PointOnMapClass::~PointOnMapClass()
{
	DELETE_GISDATA(m_gisView, m_pLassoLayer);
}

/** обработчик события нажатия клавиш манипулятора мышь */
RetTypeRedraw PointOnMapClass::mousePressEvent(const Mouse& mouse, ImageDevice *dev)
{
	PosXY pointXY;
	pointXY.x = mouse.position.X;
	pointXY.y = mouse.position.Y;
	trm_transform_buff(m_gisView->dev2map(), &pointXY, 1);
	m_pTkn->show();
	Pos2D mouse1;
	trm_transform(m_gisView->map().map2geo, &pointXY, &mouse1, 1);
	PosBL posBL = {mouse1.e1, mouse1.e2};
	Lat = posBL.b;
	Lon = posBL.l;
	m_pTkn->set(0,mouse1);
	m_pTkn->set(1,mouse1);
	emit signSendCoord(mouse1.e1,mouse1.e2);
	emit clickIs();
	return topRedraw;
}

RetTypeRedraw PointOnMapClass::detach() 
{ 
	DELETE_GISDATA(m_gisView, m_pLassoLayer);
	return topRedraw; 
}

void PointOnMapClass::_slotClearLassoLayer()
{
	//if(m_pLassoLayer)
	//{
	//	m_pLassoLayer->deleteAll();
	//}
}