#include "stdafx.h"
#include <MSVC_2008\BreakLenLine.h>


BreakLenLine::BreakLenLine(GisView *pt) : GisInputInterface (pt, true)
{
	PosSystem *system = pos_system_new (possys_geodetic);
	m_pLassoLayer = new VectorData(system);
	pos_system_free (system);
	CctError *error = NULL;
	error = m_gisView->addTopGeoData(m_pLassoLayer);
	if( cct_error_fail(error)) cct_error_free(error);
	m_pTkn = new TknSimpleLines(1);
	m_pTkn1 = new TknSimpleLines(2);
	m_pTkn2 = new TknSimpleLines(2);
	m_pTkn->setColor (GdiGis::Color::Red);
	m_pTkn->setLineType(TypeLinesObj::LINE_TYPE::DOT_LINE);
	m_pTkn->setWidth(1.0);
	m_pTkn1->setColor (GdiGis::Color::Red);
	m_pTkn1->setLineType(TypeLinesObj::LINE_TYPE::DOT_LINE);
	m_pTkn1->setWidth(1.0);
	TypeGeoToken tokenLeft;
	CircleUnit *u1 = new CircleUnit;
	u1->m_circle[1].X = 3;
	u1->makeCircle();
	u1->m_width = 0.5;
	u1->m_color = Color::Red;
	tokenLeft.addUnit2BaseFeature(u1);
	m_pTkn->setStartLineCap(tokenLeft);
	m_pTkn->setEndLineCap(tokenLeft);
	m_pTkn1->setStartLineCap(tokenLeft);

	//m_pTkn->setFillColor(GdiGis::Color::Red);
	//m_pTkn->setFillType(TypePolygonObj::TO_CENTER_FILL);

	m_pLassoLayer->addObject(m_pTkn);
	m_pLassoLayer->addObject(m_pTkn1);
	isPressOnce = false;
	m_pTkn->hide();
	m_pTkn1->hide();
}

BreakLenLine::~BreakLenLine()
{
	DELETE_GISDATA(m_gisView, m_pLassoLayer);
}

/** обработчик события нажатия клавиш манипулятора мышь */
RetTypeRedraw BreakLenLine::mousePressEvent(const Mouse& mouse, ImageDevice *dev)
{
	PosXY pointXY;
	pointXY.x = mouse.position.X;
	pointXY.y = mouse.position.Y;

	trm_transform_buff(m_gisView->dev2map(), &pointXY, 1);
	if (!isPressOnce)
	{
		m_pTkn->show();
		trm_transform(m_gisView->map().map2geo, &pointXY, &mouse1, 1);
		PosBL posBL = {mouse1.e1, mouse1.e2};
		COORDINATES.lat1 = posBL.b;
		COORDINATES.lon1 = posBL.l;
		isPressOnce = true;
		m_pTkn->set(0,mouse1);
		//m_pTkn->set(1,mouse1);
		m_pTkn->add(mouse1);
		return topRedraw;
	}
	else 
	{
		m_pTkn->show();
		trm_transform(m_gisView->map().map2geo, &pointXY, &mouse1, 1);
		PosBL posBL = {mouse1.e1, mouse1.e2};
		COORDINATES.lat1 = posBL.b;
		COORDINATES.lon1 = posBL.l;
		//isPressOnce = true;
		/*m_pTkn->set(0,mouse1);*/
		//m_pTkn->set(1,mouse1);
		m_pTkn->add(mouse1);
		points = m_pTkn->getPoints();
		emit lineIs();
		return topRedraw;
		

		//isPressOnce = false;
		//m_pTkn->show();
		//trm_transform(m_gisView->map().map2geo, &pointXY, &mouse2, 1);
		//PosBL posBL = {mouse2.e1, mouse2.e2};
		//COORDINATES.lat2 = posBL.b;
		//COORDINATES.lon2 = posBL.l;
		////m_pTkn->set(1,mouse2);
		//m_pTkn->add(mouse2);

		
		
		
	}
	
	//m_pTkn->set(0,mouse1);
	//m_pTkn->set(1,mouse2);

	//m_pTkn->add (0,mouse1);
	//m_pTkn->add (mouse2);
	return topRedraw;
}

RetTypeRedraw BreakLenLine::mouseMoveEvent(const Mouse& mouse, ImageDevice *dev)
{
	/*if (isPressOnce)
	{
		PosXY pointXY;
		pointXY.x = mouse.position.X;
		pointXY.y = mouse.position.Y;

		trm_transform_buff(m_gisView->dev2map(), &pointXY, 1);
		m_pTkn->show();
		trm_transform(m_gisView->map().map2geo, &pointXY, &mouse2, 1);
		PosBL posBL = {mouse2.e1, mouse2.e2};
		COORDINATES.lat2 = posBL.b;
		COORDINATES.lon2 = posBL.l;
		m_pTkn->set(1,mouse2);
		emit lineIs();
		return topRedraw;
	}*/
	return none;
}


RetTypeRedraw BreakLenLine::detach() 
{ 
	DELETE_GISDATA(m_gisView, m_pLassoLayer);
	return topRedraw; 
}