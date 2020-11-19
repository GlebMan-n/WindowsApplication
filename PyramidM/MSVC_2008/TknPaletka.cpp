#include "stdafx.h"
#include <MSVC_2008\TknPaletka.h>
#include <gis\GrUnits\TokenTypes.h>
//#include <vld/vld.h>
#include <sstream>

//////////////////////////////////////////////////////////////////////
// TknPaletka
TknPaletka::TknPaletka()//CPyramidFrame* par_frame) 
: TknMapNonScaleBase()
{
	m_className		= "TknPaletka";
	
//	m_pParentFrame	= par_frame;

	iVersion		= 4;

	fKurs			= 1.5707963267949;//0;

	iCountNumber	= 6;
	iCountLetter	= 3;

	iIndNumberB31	= 0;
	iIndLetterB31	= 0;

	strNameTkn[0]	= '\0';
//	iSizeLeft		= -3;
//	iSizeRight		= 3;
//	fSizeTop		= 1.5;
//	fSizeBottom		= -1.5;

	m_pGeoToKsa		= NULL;
	m_pKsaToGeo		= NULL;

//	fB0				= 0;
//	fL0				= 0;

	fSizeKvadrWnd	= 10;

	m_pSquare		= NULL;
	iCountSquare	= 0;

	bIsDraw			= false;
	iStateDrawRegionSearch= 0;

	fB0				= 0;
	fL0				= 0;
	fB1				= 0;
	fL1				= 0;
	fError_0		= 0;
	fDeviate		= 0;

//	vectorLetterFromLeft.push_back( STR_IDENT_LETTER("К", 98) );
	vectorLetterFromLeft.push_back( STR_IDENT_LETTER("А", 99) );
	vectorLetterFromLeft.push_back( STR_IDENT_LETTER("Б", 100) );
	vectorLetterFromLeft.push_back( STR_IDENT_LETTER("В", 101) );
//	vectorLetterFromLeft.push_back( STR_IDENT_LETTER("Л", 102) );

	m_pTypeLineLvl0	=  new TypeLinesObj();
	m_pTypeLineLvl0->setLineType(TypeLinesObj::SOLID_LINE, GdiGis::Color(255, 0, 0, 0), 0.35);

	fWline			= 0.5;
	pColorLine		= GdiGis::Color(255, 0, 0, 0);

	pFontLvl0.m_font.fontHeight=20.0;
	pFontLvl1.m_font.fontHeight=10.0;
	pFontLvl2.m_font.fontHeight=5.0;
	pFontMain.m_font.fontHeight=20.0;

	pFontLvl0.m_font.fontColor=GdiGis::Color(150, 0, 0, 0).GetValue();
	pFontLvl1.m_font.fontColor=GdiGis::Color(255, 0, 0, 0).GetValue();
	pFontLvl2.m_font.fontColor=GdiGis::Color(255, 0, 0, 0).GetValue();
	pFontMain.m_font.fontColor=GdiGis::Color(255, 0, 0, 0).GetValue();

	pFontLvl0.m_font.fontSet=gis_font_arial;
	pFontLvl1.m_font.fontSet=gis_font_arial;
	pFontLvl2.m_font.fontSet=gis_font_arial;
	pFontMain.m_font.fontSet=gis_font_arial;

	bDrawMarking0	= true;
	bDrawMarking1	= false;
	bDrawMarking2	= false;
	bDrawMarkingMain= true;

	bIsDrawSquare1	= false;
	bIsDrawSquare2	= false;
	/////////////////////////////
	fBsvyaz			= 0;
	fLsvyaz			= 0;

	fBnot_svyaz		= 0;
	fLnot_svyaz		= 0;

	iDeviateSvyaz	= 25000;
	iErrorSvyaz		= 10000;
	/////////////////////////////
	fBvs			= 0;
	fLvs			= 0;

	iErrorVs		= 10000;
	iHvs			= 10000;
	fVvertVs		= 10;
	fVvs			= 850;
	fKursVs			= 0;
	iDeviateVs		= 1000;

	pos_ellps_set_params_ext(ellps_krass, &pEllpsExt);
}

TknPaletka::~TknPaletka() 
{
	if(m_pGeoToKsa) trm_transform_free(m_pGeoToKsa);
	m_pGeoToKsa=NULL;

	if(m_pKsaToGeo) trm_transform_free(m_pKsaToGeo);
	m_pKsaToGeo=NULL;

	if(m_pSquare) delete[] m_pSquare;
	m_pSquare=NULL;

	if(m_pTypeLineLvl0) delete m_pTypeLineLvl0;
	m_pTypeLineLvl0=NULL;
}

/** Отображение геопространственного объекта системой визуализации */
CctError* TknPaletka::draw(GisLayer* gisLayer, GdiGis::ImageDevice* dev)
{
	double PI=3.1415926535897932;

	if(bIsDraw || iStateDrawRegionSearch==2) DrawRegionSearch(gisLayer, dev);

//	GdiGis::Color colorBlack(m_Klight, 0, 0, 0);
	GdiGis::Pen pen(dev->factory(), pColorLine, float(fWline*dev->m_mm2pt));
	GdiGis::Pen pen_arrow(dev->factory(), pColorLine, float(0.35*dev->m_mm2pt));
	GdiGis::Pen pen3(dev->factory(), pColorLine, float(0.05*dev->m_mm2pt));

	GdiGis::SolidBrush brush_black(dev->factory(), pColorLine);

	double size_kvadrat=20000;
	double x0=0, y0=0;
	////////////////////////////////////////////////////////////
	double r=(iCountNumber/2.0+0.2)*size_kvadrat;//sqrt(x_left*x_left+y_left*y_left);
	double angle=fKurs;
	double x_left=r*cos(angle);
	double y_left=r*sin(angle);

//	double x_top=x0+(fSizeTop+0.1)*size_kvadrat;
//	double y_top=y0+(fSizeTop+0.1)*size_kvadrat;
	r=(iCountLetter/2.0+0.2)*size_kvadrat;//sqrt(x_top*x_top+y_top*y_top);
	angle=fKurs;
	double x_top=r*cos(angle);
	double y_top=r*sin(angle);

	double fBleft=0, fLleft=0;
	double fBtop=0, fLtop=0;

	KsaToGeo(x_left, y_left, fBleft, fLleft);
	KsaToGeo(x_top, y_top, fBtop, fLtop);

	PosDev pWnd0, pWndleft, pWndtop;
	double b=0, l=0;
	getPosition(b, l);
	double x=0, y=0;
	BLtoWnd(gisLayer, b, l, x, y);
	pWnd0.x=float( x );
	pWnd0.y=float( y );

	BLtoWnd(gisLayer, fBleft, fLleft, x, y);
	pWndleft.x=float( x );
	pWndleft.y=float( y );

	BLtoWnd(gisLayer, fBtop, fLtop, x, y);
	pWndtop.x=float( x );
	pWndtop.y=float( y );

	dev->DrawLine(&pen_arrow, pWnd0.x, pWnd0.y, pWndleft.x, pWndleft.y);
	dev->DrawLine(&pen_arrow, pWnd0.x, pWnd0.y, pWndtop.x, pWndtop.y);

	GdiGis::PointF point1, point2;
	point1.X=pWnd0.x;
	point1.Y=pWnd0.y;

	point2.X=pWndleft.x;
	point2.Y=pWndleft.y;

	Ray rayForStr=Ray(point1, point2);
//	if(point2.X<point1.X) rayForStr=Ray(point2, point1);
	double ray_angle=rayForStr.polarAngle();
	////////////////////////////////////////////////////////////
	GdiGis::SolidBrush squareBrush(dev->factory(), GdiGis::Color(70, pColorLine.GetRed(), pColorLine.GetGreen(), pColorLine.GetBlue()));
	for(long j=0;j<iCountSquare;j++)
	{
		m_pSquare[j].DrawSquare(gisLayer, dev, &pen, &pen3, &pen3, &squareBrush, m_pKsaToGeo, fKurs, ray_angle,
								pFontLvl0, pFontLvl1, pFontLvl2, pFontMain);
	}
/*	for(long goriz=iSizeLeft;goriz<iSizeRight;goriz++)
	{
		for(double vert=fSizeBottom;vert<fSizeTop-0.000001;vert+=1)
		{
			double x_bottom=x0+vert*size_kvadrat;
			double y_left=y0+goriz*size_kvadrat;
			double x_top=x_bottom+size_kvadrat;
			double y_right=y_left+size_kvadrat;

			double x_center=(x_bottom+x_top)/2;
			double y_center=(y_left+y_right)/2;

			DrawKvadrat(gisLayer, dev, &pen3, x_bottom, y_left, x_center, y_center);
			DrawKvadrat(gisLayer, dev, &pen3, x_center, y_left, x_top, y_center);
			DrawKvadrat(gisLayer, dev, &pen3, x_center, y_center, x_top, y_right);
			DrawKvadrat(gisLayer, dev, &pen3, x_bottom, y_center, x_center, y_right);

			DrawKvadrat(gisLayer, dev, &pen, x_bottom, y_left, x_top, y_right);

			DrawText(gisLayer, dev, ray_angle, fSizeKvadrWnd/2, (x_center+x_top)/2, (y_center+y_left)/2, "г");
			DrawText(gisLayer, dev, ray_angle, fSizeKvadrWnd/2, (x_center+x_top)/2, (y_center+y_right)/2, "д");
			DrawText(gisLayer, dev, ray_angle, fSizeKvadrWnd/2, (x_center+x_bottom)/2, (y_center+y_right)/2, "е");
			DrawText(gisLayer, dev, ray_angle, fSizeKvadrWnd/2, (x_center+x_bottom)/2, (y_center+y_left)/2, "ж");

			////////////////////////////////////////////////////////////////////////
			if(goriz==iSizeLeft)
			{
				double x_center=x0+(vert+0.5)*size_kvadrat;
				double y_center=y0+(iSizeLeft-0.5)*size_kvadrat;

				CString strTemp;
				if(fabs(vert+1.5)<0.001) strTemp="В";
				else if(fabs(vert+0.5)<0.001) strTemp="Б";
				else if(fabs(vert-0.5)<0.001) strTemp="А";

				DrawText(gisLayer, dev, ray_angle, fSizeKvadrWnd, x_center, y_center, strTemp);
			}
		}
		////////////////////////////////////////////////////
		double x_center=x0+(fSizeTop+0.5)*size_kvadrat;
		double y_center=y0+(goriz+0.5)*size_kvadrat;

		CString strTemp;
		if(goriz<0) strTemp.Format("%ld", 10-goriz);
		else strTemp.Format("%ld", 31+goriz);

		DrawText(gisLayer, dev, ray_angle, fSizeKvadrWnd, x_center, y_center, strTemp);
	}*/

	////////////////////////////////////////////////////////////
//	double x_left=x0+(iSizeRight+0.1)*size_kvadrat;
//	double y_left=y0+(iSizeRight+0.1)*size_kvadrat;
/*	double r=(iSizeRight+0.2)*size_kvadrat;//sqrt(x_left*x_left+y_left*y_left);
	double angle=fKurs;
	double x_left=r*cos(angle);
	double y_left=r*sin(angle);

//	double x_top=x0+(fSizeTop+0.1)*size_kvadrat;
//	double y_top=y0+(fSizeTop+0.1)*size_kvadrat;
	r=(fSizeTop+0.2)*size_kvadrat;//sqrt(x_top*x_top+y_top*y_top);
	angle=fKurs-PI/2;
	double x_top=r*cos(angle);
	double y_top=r*sin(angle);

	double fBleft=0, fLleft=0;
	double fBtop=0, fLtop=0;

	KsaToGeo(x_left, y_left, fBleft, fLleft);
	KsaToGeo(x_top, y_top, fBtop, fLtop);

	PosDev pWnd0, pWndleft, pWndtop;
	double b=0, l=0;
	getPosition(b, l);
	double x=0, y=0;
	BLtoWnd(gisLayer, b, l, x, y);
	pWnd0.x=x;
	pWnd0.y=y;

	BLtoWnd(gisLayer, fBleft, fLleft, x, y);
	pWndleft.x=x;
	pWndleft.y=y;

	BLtoWnd(gisLayer, fBtop, fLtop, x, y);
	pWndtop.x=x;
	pWndtop.y=y;

	dev->DrawLine(&pen_arrow, pWnd0.x, pWnd0.y, pWndleft.x, pWndleft.y);
	dev->DrawLine(&pen_arrow, pWnd0.x, pWnd0.y, pWndtop.x, pWndtop.y);*/
	////////////////////////////////////////////////////////////

	return NULL;
} 

void TknPaletka::DrawRegionSearch(GisLayer* gisLayer, GdiGis::ImageDevice* dev)
{
	if(fDeviate<fError_0) fDeviate=fError_0;

	GdiGis::Color colorBlack(m_Klight, 0, 0, 0);
	GdiGis::Pen pen(dev->factory(), colorBlack, float(0.75*dev->m_mm2pt));

	double PI=3.1415926535897932;

	double kurs=GetKursOrtodrom(fB0, fL0, fB1, fL1);
	double len=GetLenToPoint(fB0, fL0, fB1, fL1)+fError_0;

	double b0_draw=0, l0_draw=0;
	double b1_draw=0, l1_draw=0;
	GetBLforPointKursLenOrtodrom(fB0, fL0, kurs-PI, fError_0+len*0.1, b0_draw, l0_draw);
	GetBLforPointKursLenOrtodrom(fB1, fL1, kurs, len*0.1, b1_draw, l1_draw);
	////////////////////////////////////////////////////
	double x0=0, y0=0;
	double x1=0, y1=0;

	BLtoWnd(gisLayer, b0_draw, l0_draw, x0, y0);
	BLtoWnd(gisLayer, b1_draw, l1_draw, x1, y1);

	dev->DrawLine(&pen, (float)x0, (float)y0, (float)x1, (float)y1);
	////////////////////////////////////////////////////
	double b01_draw=0, l01_draw=0;
	double b11_draw=0, l11_draw=0;
	double x01=0, y01=0;
	double x11=0, y11=0;

	GetBLforPointKursLenOrtodrom(fB1, fL1, kurs+PI/2, fDeviate, b01_draw, l01_draw);
	GetBLforPointKursLenOrtodrom(fB1, fL1, kurs-PI/2, fDeviate, b11_draw, l11_draw);

	BLtoWnd(gisLayer, b01_draw, l01_draw, x01, y01);
	BLtoWnd(gisLayer, b11_draw, l11_draw, x11, y11);

	dev->DrawLine(&pen, (float)x01, (float)y01, (float)x11, (float)y11);
	////////////////////////////////////////////////////
	GetBLforPointKursLenOrtodrom(fB0, fL0, kurs+PI/2, fError_0, b0_draw, l0_draw);
	GetBLforPointKursLenOrtodrom(fB0, fL0, kurs-PI/2, fError_0, b1_draw, l1_draw);

	BLtoWnd(gisLayer, b0_draw, l0_draw, x0, y0);
	BLtoWnd(gisLayer, b1_draw, l1_draw, x1, y1);

	dev->DrawLine(&pen, (float)x0, (float)y0, (float)x1, (float)y1);
	////////////////////////////////////////////////////
	dev->DrawLine(&pen, (float)x0, (float)y0, (float)x01, (float)y01);
	dev->DrawLine(&pen, (float)x1, (float)y1, (float)x11, (float)y11);
	////////////////////////////////////////////////////
	GdiGis::PointF polyTemp[180];
	polyTemp[0].X=float(x0);
	polyTemp[0].Y=float(y0);

	polyTemp[179].X=float(x1);
	polyTemp[179].Y=float(y1);

	double beg_angle=kurs+PI/2;
	double delta_angle=PI/180.0;

	for(long angle=1;angle<179;angle++)
	{
		double per_angle=beg_angle+angle*delta_angle;

		double b=0, l=0;
		double fAngle=angle/180.*3.14159265;
		GetBLforPointKursLenOrtodrom(fB0, fL0, per_angle, fError_0, b1_draw, l1_draw);

		BLtoWnd(gisLayer, b1_draw, l1_draw, x1, y1);

		polyTemp[angle].X=(REAL)x1;
		polyTemp[angle].Y=(REAL)y1;
	}

	dev->DrawPolygon(&pen, polyTemp, 180);
}

void TknPaletka::SetNameTkn(std::string str_name)
{
	int len=str_name.size();
	if(len>150) str_name.resize(150);

	sprintf(strNameTkn, "%s", str_name.c_str());
}

double TknPaletka::GetKursOrtodrom(double b0, double l0, double b1, double l1)
{
	double PI=3.1415926535898;

	double a=6378245;
	double b=6356863.0188;
	double e2=(a*a-b*b)/a/a;
	double es2=e2/(1-e2);

	double n0=a;
	double n2=e2*n0/2;
	double n4=3*e2*n2/4;

	double NT4_0=(n0-n4/8)+(n2+n4)*sin(b0);
	double NT4_1=(n0-n4/8)+(n2+n4)*sin(b1);
	
	double X0=NT4_0*cos(b0)*cos(l0);
	double Y0=NT4_0*cos(b0)*sin(l0);
	double Z0=NT4_0*(1-e2)*sin(b0);

	double X1=NT4_1*cos(b1)*cos(l1);
	double Y1=NT4_1*cos(b1)*sin(l1);
	double Z1=NT4_1*(1-e2)*sin(b1);

	double d1=(X1-X0)*(X1-X0)+(Y1-Y0)*(Y1-Y0)+(Z1-Z0)*(Z1-Z0);
	d1=sqrt(d1);

	double cos_B0=cos(b0);
	double sin_B0=sin(b0);
	double cos_L0=cos(l0);
	double sin_L0=sin(l0);

	double v_1=Y1*cos_L0-X1*sin_L0;
	double eps1_1=Z1+e2*NT4_0*sin_B0;
	double eps2_1=X1*cos_L0+Y1*sin_L0;
	double eps_1=eps1_1*cos_B0-eps2_1*sin_B0;

	double az_1=atan2(v_1, eps_1);
	if(az_1<0) az_1+=2*PI;

	return az_1;
}

double TknPaletka::GetLenToPoint(double b0, double l0, double b1, double l1)
{
	double ret = geo_calcLengthArcNormalSection(&pEllpsExt, b0, l0, b1, l1);
	return ret;
}

void TknPaletka::GetBLforPointKursLenOrtodrom(double b0, double l0, double kurs, double per_len, double& b, double& l)
{
	double PI=3.1415926535897932;
	if(per_len>=0)
	{
		PosBL bl;
		bl.b=b0;
		bl.l=l0;

		TrmRA2BL* m_pTemp=trm_ra2bl_new(bl);

		PosBL bl1;
		PosRA ra;
		ra.a=kurs;
		ra.r=per_len;

		trm_transform(m_pTemp, &ra, &bl1, 1);

		b=bl1.b;
		l=bl1.l;
//		kurs=str_kta1->fKurs;

		trm_transform_free(m_pTemp);/**/
	}
	else
	{
		PosBL bl;
		bl.b=b0;
		bl.l=l0;

		TrmRA2BL* m_pTemp=trm_ra2bl_new(bl);

		PosBL bl1;
		PosRA ra;
		ra.a=kurs-PI;
		ra.r=-per_len;

		trm_transform(m_pTemp, &ra, &bl1, 1);

		b=bl1.b;
		l=bl1.l;
//		kurs=str_kta1->fKurs;

		trm_transform_free(m_pTemp);
	}
}

void TknPaletka::BLtoWnd(GisLayer* gisLayer, double b, double l, double& x, double& y)
{
	PosBL posBL;

	posBL.b=b;
	posBL.l=l;

	PosXY posXY;
	gisLayer->m_gisview->geo2map(&posBL, &posXY, 1);

	PosDev pWnd;
	trm_transform_dev(gisLayer->m_gisview->map2dev(), POS_2D(&posXY), &pWnd, 1);

	x=pWnd.x;
	y=pWnd.y;
}

void TknPaletka::DrawKvadrat(GisLayer* gisLayer, GdiGis::ImageDevice* dev, GdiGis::Pen* pen_draw, double x_bottom, double y_left, double x_top, double y_right)
{
	double PI=3.1415926535897932;

//	Color colorBlack(m_Klight, 0, 0, 0);
//	Pen pen(dev->factory(), colorBlack, 0.3*dev->m_mm2pt);

	PosXY posXYleft_top, posXYleft_bottom, posXYright_top, posXYright_bottom;
	
	double r=sqrt(x_bottom*x_bottom+y_left*y_left);
	double angle=atan2( y_left, x_bottom )+fKurs-PI/2;

	posXYleft_bottom.x=r*cos(angle);
	posXYleft_bottom.y=r*sin(angle);
	/////////////
	r=sqrt(x_bottom*x_bottom+y_right*y_right);
	angle=atan2( y_right, x_bottom )+fKurs-PI/2;

	posXYright_bottom.x=r*cos(angle);
	posXYright_bottom.y=r*sin(angle);
	/////////////
	r=sqrt(x_top*x_top+y_left*y_left);
	angle=atan2( y_left, x_top )+fKurs-PI/2;

	posXYleft_top.x=r*cos(angle);
	posXYleft_top.y=r*sin(angle);
	/////////////
	r=sqrt(x_top*x_top+y_right*y_right);
	angle=atan2( y_right, x_top )+fKurs-PI/2;

	posXYright_top.x=r*cos(angle);
	posXYright_top.y=r*sin(angle);
	////////////////////////////////////////////////////////////////////////
	double fBleft_top=0, fLleft_top=0;
	double fBleft_bottom=0, fLleft_bottom=0;
	double fBright_top=0, fLright_top=0;
	double fBright_bottom=0, fLright_bottom=0;

	KsaToGeo(posXYleft_top.x,		posXYleft_top.y, 	fBleft_top,		fLleft_top);
	KsaToGeo(posXYleft_bottom.x,	posXYleft_bottom.y,	fBleft_bottom,	fLleft_bottom);
	KsaToGeo(posXYright_top.x,		posXYright_top.y,	fBright_top,	fLright_top);
	KsaToGeo(posXYright_bottom.x,	posXYright_bottom.y,fBright_bottom, fLright_bottom);
	////////////////////////////////////////////////////////////////////////
	PosBL posBLleft_top, posBLleft_bottom, posBLright_top, posBLright_bottom;
	posBLleft_top.b=fBleft_top;
	posBLleft_top.l=fLleft_top;

	posBLleft_bottom.b=fBleft_bottom;
	posBLleft_bottom.l=fLleft_bottom;

	posBLright_top.b=fBright_top;
	posBLright_top.l=fLright_top;

	posBLright_bottom.b=fBright_bottom;
	posBLright_bottom.l=fLright_bottom;
	////////////////////////////////////////////////////////////////////////
//	PosXY posXYleft_top, posXYleft_bottom, posXYright_top, posXYright_bottom;
	gisLayer->m_gisview->geo2map(&posBLleft_top, &posXYleft_top, 1);
	gisLayer->m_gisview->geo2map(&posBLleft_bottom, &posXYleft_bottom, 1);
	gisLayer->m_gisview->geo2map(&posBLright_top, &posXYright_top, 1);
	gisLayer->m_gisview->geo2map(&posBLright_bottom, &posXYright_bottom, 1);

	PosDev pWndleft_top, pWndleft_bottom, pWndright_top, pWndright_bottom;
	trm_transform_dev(gisLayer->m_gisview->map2dev(), POS_2D(&posXYleft_top), &pWndleft_top, 1);
	trm_transform_dev(gisLayer->m_gisview->map2dev(), POS_2D(&posXYleft_bottom), &pWndleft_bottom, 1);
	trm_transform_dev(gisLayer->m_gisview->map2dev(), POS_2D(&posXYright_top), &pWndright_top, 1);
	trm_transform_dev(gisLayer->m_gisview->map2dev(), POS_2D(&posXYright_bottom), &pWndright_bottom, 1);

	GdiGis::PointF polyTemp[4];
	polyTemp[0].X=pWndleft_top.x;
	polyTemp[0].Y=pWndleft_top.y;

	polyTemp[1].X=pWndright_top.x;
	polyTemp[1].Y=pWndright_top.y;

	polyTemp[2].X=pWndright_bottom.x;
	polyTemp[2].Y=pWndright_bottom.y;

	polyTemp[3].X=pWndleft_bottom.x;
	polyTemp[3].Y=pWndleft_bottom.y;

	dev->DrawPolygon(pen_draw, polyTemp, 4);

	fSizeKvadrWnd=(pWndleft_top.x-pWndright_top.x)*(pWndleft_top.x-pWndright_top.x)+
				(pWndright_top.y-pWndleft_top.y)*(pWndright_top.y-pWndleft_top.y);
	fSizeKvadrWnd=sqrt(fSizeKvadrWnd);
	fSizeKvadrWnd=dev->pt2mm((REAL)fSizeKvadrWnd);
}

void TknPaletka::DrawText(GisLayer* gisLayer, GdiGis::ImageDevice *dev, double ray_polar, double size_mm, double x_center, double y_center, std::string str_text, BYTE transparent)
{
	double PI=3.1415926535897932;

/*  Ray rayForStr=Ray(polyTemp[3], polyTemp[0]);

	double cosA = cos(rayForStr.polarAngle());
	double sinA = sin(rayForStr.polarAngle());

	dev->SetTransform(&oldMatrix);
*/
	GdiGis::Color colorBlack(transparent, 0, 0, 0);
	GdiGis::SolidBrush brush_black(dev->factory(), colorBlack);

	double r=sqrt(x_center*x_center+y_center*y_center);
	double angle=atan2( y_center, x_center )+fKurs-PI/2;

	x_center=r*cos(angle);
	y_center=r*sin(angle);

	double fBcenter=0, fLcenter=0;
	KsaToGeo(x_center, y_center, fBcenter, fLcenter);

	PosBL posBLcenter;
	posBLcenter.b=fBcenter;
	posBLcenter.l=fLcenter;

	PosXY posXYcenter;
	gisLayer->m_gisview->geo2map(&posBLcenter, &posXYcenter, 1);

	PosDev pWndcenter;
	trm_transform_dev(gisLayer->m_gisview->map2dev(), POS_2D(&posXYcenter), &pWndcenter, 1);

	GdiGis::StringFormat stringFormat(dev->factory());
	stringFormat.SetAlignment(GdiGis::StringAlignmentCenter);
	stringFormat.SetLineAlignment(GdiGis::StringAlignmentCenter);

	double scale=1;

	GdiGis::Font* font = dev->buildFont(gis_font_arial, float(size_mm/2 * scale), GdiGis::FontStyleBold);

	GdiGis::PointF pointText;
	pointText.X=pWndcenter.x;
	pointText.Y=pWndcenter.y;
	////////////////////////////////////////////////////
	GdiGis::Matrix matrix(dev->factory()), oldMatrix(dev->factory());
	dev->GetTransform(&oldMatrix);

	double cosA = cos(ray_polar);
	double sinA = sin(ray_polar);

	matrix.SetElements( (REAL)cosA, (REAL)sinA, -(REAL)sinA, (REAL)cosA,
						(REAL)(pointText.X*(1-cosA) + pointText.Y*sinA),
						(REAL)(pointText.Y*(1-cosA) - pointText.X*sinA));

	dev->SetTransform(&matrix);
	////////////////////////////////////////////////////
	if(str_text.size()>0)
	{
		WCHAR* bstr = NULL;
		cct_char_to_wchar(str_text.c_str(), &bstr);

		int len = wcslen((wchar_t*)bstr);
		dev->DrawString(bstr, len, font, pointText, &stringFormat, &brush_black);

		if(bstr) cct_free_string(bstr);
	}
	if(font) delete font;

	dev->SetTransform(&oldMatrix);
}

void TknPaletka::GeoToKsa(double b, double l, double& x, double& y)
{
	if(!m_pGeoToKsa) return;

	PosBL bl;
	PosXY xy;

	bl.b=b;
	bl.l=l;
	trm_transform(m_pGeoToKsa, &bl, &xy, 1);
	x=xy.x;
	y=xy.y;
}

void TknPaletka::KsaToGeo(double x, double y, double& b, double& l)
{
	if(!m_pKsaToGeo) return;

	PosBL bl;
	PosXY xy;

	xy.x=x;
	xy.y=y;
	trm_transform(m_pKsaToGeo, &xy, &bl, 1);
	b=bl.b;
	l=bl.l;
}

void TknPaletka::SetCountLineAndColumn(short count_line, short count_column)
{
	iCountNumber	= count_column;
	iCountLetter	= count_line;
//	iSizeLeft		= -count_column/2;
//	iSizeRight		= count_column/2;
//	fSizeTop		= double(count_line)/2.0;
//	fSizeBottom		= -double(count_line)/2.0;

//	UpdateTrm();
}

void TknPaletka::InitTknPaletka()
{
//	if(m_pSquare) delete[] m_pSquare;

	iCountSquare=iCountLetter*iCountNumber;
	m_pSquare=new STR_SQUARE[iCountSquare];

	double size_kvadrat=20000;
	double x0=-(iIndNumberB31)*size_kvadrat;
	double y0=-(iIndLetterB31+0.5)*size_kvadrat;

	for(long let=0;let<iCountLetter;let++)
	{
		long ident_letter=let-iIndLetterB31+100;
		int index_vector=GetIndVectorForIdentLetter(ident_letter);
		std::string str_letter_marking;
		if(index_vector>=0 && index_vector<(long)vectorLetterFromLeft.size())
			str_letter_marking=vectorLetterFromLeft[index_vector].strMarkingLetter;
		else
		{
			char str_temp[20];
			sprintf(str_temp, "%ld", ident_letter);
			str_letter_marking=str_temp;
		}

		for(long num=0;num<iCountNumber;num++)
		{	
			long ind=let*iCountNumber+num;

			m_pSquare[ind].fXksaBottom	= x0+num*size_kvadrat;
			m_pSquare[ind].fXksaTop		= m_pSquare[ind].fXksaBottom+size_kvadrat;
			m_pSquare[ind].fYksaLeft	= y0+let*size_kvadrat;
			m_pSquare[ind].fYksaRight	= m_pSquare[ind].fYksaLeft+size_kvadrat;

			m_pSquare[ind].strMarking_1=str_letter_marking;
			m_pSquare[ind].iIdent_1=ident_letter;

			m_pSquare[ind].SetDrawMarkingLvl0(true, (m_pSquare[ind].fXksaBottom+m_pSquare[ind].fXksaTop)/2, (m_pSquare[ind].fYksaLeft+m_pSquare[ind].fYksaRight)/2);

/*			if(let==iIndLetterB31-2)
			{
				m_pSquare[ind].strMarking_1="К";
				m_pSquare[ind].iIdent_1=98;
			}
			else if(let==iIndLetterB31-1)
			{
				m_pSquare[ind].strMarking_1="А";
				m_pSquare[ind].iIdent_1=99;
			}
			else if(let==iIndLetterB31)
			{
				m_pSquare[ind].strMarking_1="Б";
				m_pSquare[ind].iIdent_1=100;
			}
			else if(let==iIndLetterB31+1)
			{
				m_pSquare[ind].strMarking_1="В";
				m_pSquare[ind].iIdent_1=101;
			}
			else if(let==iIndLetterB31+2)
			{
				m_pSquare[ind].strMarking_1="Л";
				m_pSquare[ind].iIdent_1=102;
			}*/

			char strTemp[100];
			if(num<iIndNumberB31)
			{
				m_pSquare[ind].iIdent_2=10+(iIndNumberB31-num);
			}
			else
			{
				m_pSquare[ind].iIdent_2=31+(num-iIndNumberB31);
			}
			sprintf(strTemp, "%ld", m_pSquare[ind].iIdent_2);
			m_pSquare[ind].strMarking_2=strTemp;

			if(num==0) 
			{
				double x_center=m_pSquare[ind].fXksaBottom-size_kvadrat/2;//x0+(vert+0.5)*size_kvadrat;
				double y_center=(m_pSquare[ind].fYksaLeft+m_pSquare[ind].fYksaRight)/2;//y0+(-iCountColumn/2-0.3)*size_kvadrat;

				m_pSquare[ind].SetDrawMarking_1(true, x_center, y_center);
			}
			if(let==iCountLetter-1)
			{
				double x_center=(m_pSquare[ind].fXksaTop+m_pSquare[ind].fXksaBottom)/2;//x0+(iCountLine/2.0+0.3)*size_kvadrat;
				double y_center=m_pSquare[ind].fYksaRight+size_kvadrat/2;//y0+(goriz+0.5)*size_kvadrat;

				m_pSquare[ind].SetDrawMarking_2(true, x_center, y_center);
			}

			m_pSquare[ind].bIsDrawSquare=true;

			m_pSquare[ind].DivideSquareLvl1();
		
//			ind_square++;
		}
	}
}

void TknPaletka::UpdateTrm()
{
	if(m_pGeoToKsa) trm_transform_free(m_pGeoToKsa);
	if(m_pKsaToGeo) trm_transform_free(m_pKsaToGeo);

	double b=0, l=0;
	getPosition(b, l);
	PosBL posKsa;
	posKsa.b=b;
	posKsa.l=l;

	m_pGeoToKsa = trm_geo2ksa_new(posKsa);
	m_pKsaToGeo = trm_ksa2geo_new(posKsa);
	///////////////////////////////////////////////////////////////////

/*	STR_SQUARE* m_pTempStr=m_pSquare;
	long iTempCount=iCountSquare;

	iCountSquare=iCountLine*iCountColumn;
	m_pSquare=new STR_SQUARE[iCountSquare];
	///////////////////////////////////////////////////////////
	iIndColB31	= iCountColumn/2;
	iIndLineB31	= iCountLine/2;

	double x0=0, y0=0;
	double size_kvadrat=20000;

	long ind_square=0;
	for(long goriz=-iCountColumn/2;goriz<iCountColumn/2;goriz++)
	{
		for(double vert=-iCountLine/2.0;vert<iCountLine/2.0-0.000001;vert+=1)
		{
			if(ind_square>=iCountSquare) break;

			m_pSquare[ind_square].fXksaBottom=x0+vert*size_kvadrat;
			m_pSquare[ind_square].fXksaTop=m_pSquare[ind_square].fXksaBottom+size_kvadrat;
			m_pSquare[ind_square].fYksaLeft=y0+goriz*size_kvadrat;
			m_pSquare[ind_square].fYksaRight=m_pSquare[ind_square].fYksaLeft+size_kvadrat;

			if(fabs(vert+2.5)<0.001) 
			{
				m_pSquare[ind_square].strMarking_1="Л";
				m_pSquare[ind_square].iIdent_1=102;
			}
			if(fabs(vert+1.5)<0.001) 
			{
				m_pSquare[ind_square].strMarking_1="В";
				m_pSquare[ind_square].iIdent_1=101;
			}
			else if(fabs(vert+0.5)<0.001) 
			{
				m_pSquare[ind_square].strMarking_1="Б";
				m_pSquare[ind_square].iIdent_1=100;
			}
			else if(fabs(vert-0.5)<0.001) 
			{
				m_pSquare[ind_square].strMarking_1="А";
				m_pSquare[ind_square].iIdent_1=99;
			}
			else if(fabs(vert-1.5)<0.001) 
			{
				m_pSquare[ind_square].strMarking_1="К";
				m_pSquare[ind_square].iIdent_1=98;
			}

			char strTemp[100];
			if(goriz<0) 
			{
				sprintf(strTemp, "%ld", 10-goriz);
				m_pSquare[ind_square].iIdent_2=10-goriz;
			}
			else 
			{
				sprintf(strTemp, "%ld", 31+goriz);
				m_pSquare[ind_square].iIdent_2=31+goriz;
			}
			m_pSquare[ind_square].strMarking_2=strTemp;

			if(goriz==-iCountColumn/2) 
			{
				double x_center=x0+(vert+0.5)*size_kvadrat;
				double y_center=y0+(-iCountColumn/2-0.3)*size_kvadrat;

				m_pSquare[ind_square].SetDrawMarking_1(true, x_center, y_center);
			}
			if(vert+1>iCountLine/2.0-0.000001)
			{
				double x_center=x0+(iCountLine/2.0+0.3)*size_kvadrat;
				double y_center=y0+(goriz+0.5)*size_kvadrat;

				m_pSquare[ind_square].SetDrawMarking_2(true, x_center, y_center);
			}

			m_pSquare[ind_square].bIsDrawSquare=true;

			m_pSquare[ind_square].DivideSquareLvl1();
		
			ind_square++;
		}
	}

	CopyParamDrawFill(m_pTempStr, iTempCount);

	if(m_pTempStr) delete[] m_pTempStr;
	m_pTempStr=NULL;*/
}

void TknPaletka::SetParamPaletka(double b, double l, double kurs)
{
	fKurs=kurs;
	SetPositionPaletka(b, l);
}

void TknPaletka::SetPositionPaletka(double b, double l)
{
	Pos2D bl_pos;
	bl_pos.e1=b;
	bl_pos.e2=l;
	setPosition(bl_pos);

	UpdateTrm();
}

void TknPaletka::SetStatusFill(short level, short ident_line, short ident_column, short ident_10_10, short ident_5_5, bool show)
{
	if(level==0)
	{
		for(long j0=0;j0<iCountSquare;j0++)
		{
			if(m_pSquare[j0].iIdent_1==ident_line && m_pSquare[j0].iIdent_2==ident_column)
				m_pSquare[j0].bIsDrawFill=show;
		}
	}
	else if(level==1)
	{
		for(long j0=0;j0<iCountSquare;j0++)
		{
			if(m_pSquare[j0].iIdent_1==ident_line && m_pSquare[j0].iIdent_2==ident_column)
			{
				for(long j1=0;j1<m_pSquare[j0].iCountChild;j1++)
				{
					if(m_pSquare[j0].m_pArrayChild[j1].iIdent_1==ident_10_10)
						m_pSquare[j0].m_pArrayChild[j1].bIsDrawFill=show;
				}
			}
		}
	}
	else if(level==2)
	{
		for(long j0=0;j0<iCountSquare;j0++)
		{
			if(m_pSquare[j0].iIdent_1==ident_line && m_pSquare[j0].iIdent_2==ident_column)
			{
				for(long j1=0;j1<m_pSquare[j0].iCountChild;j1++)
				{
					if(m_pSquare[j0].m_pArrayChild[j1].iIdent_1==ident_10_10)
					{
						for(long j2=0;j2<m_pSquare[j0].m_pArrayChild[j1].iCountChild;j2++)
						{
							if(m_pSquare[j0].m_pArrayChild[j1].m_pArrayChild[j2].iIdent_1==ident_5_5)
								m_pSquare[j0].m_pArrayChild[j1].m_pArrayChild[j2].bIsDrawFill=show;
						}
					}
				}
			}
		}
	}
}

void TknPaletka::ClearAllParamCell()
{
	vectorLetterFromLeft.clear();
	for(long j0=0;j0<iCountSquare;j0++)
	{
		m_pSquare[j0].bIsDrawFill=false;
		for(long j1=0;j1<m_pSquare[j0].iCountChild;j1++)
		{
			m_pSquare[j0].m_pArrayChild[j1].bIsDrawFill=false;
			for(long j2=0;j2<m_pSquare[j0].m_pArrayChild[j1].iCountChild;j2++)
			{
				m_pSquare[j0].m_pArrayChild[j1].m_pArrayChild[j2].bIsDrawFill=false;
			}
		}
	}
}

void TknPaletka::GetVectorSelectedSquare(std::vector<STR_SQUARE*>& vector_square, double b, double l)
{
	double x=0, y=0;
	GeoToKsa(b, l, x, y);

	PosXY posXY;
	posXY.x=x;
	posXY.y=y;

	for(long j0=0;j0<iCountSquare;j0++)
	{
		long iRet=pointInPolygon(m_pSquare[j0].polyKsaLastDraw, 4, &posXY);
		if(iRet==INSIDE || iRet==BOUNDARY)
		{
			vector_square.push_back(&(m_pSquare[j0]));
			if(!m_pSquare[j0].bIsDrawSquare) break;

			for(long j1=0;j1<m_pSquare[j0].iCountChild;j1++)
			{
				iRet=pointInPolygon(m_pSquare[j0].m_pArrayChild[j1].polyKsaLastDraw, 4, &posXY);
				if(iRet==INSIDE || iRet==BOUNDARY)
				{
					vector_square.push_back(&(m_pSquare[j0].m_pArrayChild[j1]));

					for(long j2=0;j2<m_pSquare[j0].m_pArrayChild[j1].iCountChild;j2++)
					{
						iRet=pointInPolygon(m_pSquare[j0].m_pArrayChild[j1].m_pArrayChild[j2].polyKsaLastDraw, 4, &posXY);
						if(iRet==INSIDE || iRet==BOUNDARY)
						{
							vector_square.push_back(&(m_pSquare[j0].m_pArrayChild[j1].m_pArrayChild[j2]));
						}
					}
				}
			}
		}
	}

//	long iRet=pointInPolygon(m_pTempXY, arrayPolyRuvdInKsa[j].arrayPoints.GetSize(), &posXY);
//	if(iRet==INSIDE || iRet==BOUNDARY) is_in_poly=1;
}

void TknPaletka::SetDrawCell(short level, bool show)
{
	if(level==1)
	{
		for(long j0=0;j0<iCountSquare;j0++)
		{
			for(long j1=0;j1<m_pSquare[j0].iCountChild;j1++)
			{
				m_pSquare[j0].m_pArrayChild[j1].bIsDrawSquare=show;
			}
		}
	}
	else if(level==2)
	{
		for(long j0=0;j0<iCountSquare;j0++)
		{
			for(long j1=0;j1<m_pSquare[j0].iCountChild;j1++)
			{
				for(long j2=0;j2<m_pSquare[j0].m_pArrayChild[j1].iCountChild;j2++)
				{
					m_pSquare[j0].m_pArrayChild[j1].m_pArrayChild[j2].bIsDrawSquare=show;
				}
			}
		}
	}
}

void TknPaletka::SetDrawTextCell(short level, bool show)
{
	if(level==-1)
	{
		for(long j0=0;j0<iCountSquare;j0++)
		{
			m_pSquare[j0].bDrawMarkingMain=show;
		}
	}
	else if(level==0)
	{
		for(long j0=0;j0<iCountSquare;j0++)
		{
			m_pSquare[j0].bDrawMarkingLvl0=show;
		}
	}
	else if(level==1)
	{
		for(long j0=0;j0<iCountSquare;j0++)
		{
			for(long j1=0;j1<m_pSquare[j0].iCountChild;j1++)
			{
				m_pSquare[j0].m_pArrayChild[j1].bDrawMarking_1=show;
			}
		}
	}
	else if(level==2)
	{
		for(long j0=0;j0<iCountSquare;j0++)
		{
			for(long j1=0;j1<m_pSquare[j0].iCountChild;j1++)
			{
				for(long j2=0;j2<m_pSquare[j0].m_pArrayChild[j1].iCountChild;j2++)
				{
					m_pSquare[j0].m_pArrayChild[j1].m_pArrayChild[j2].bDrawMarking_1=show;
				}
			}
		}
	}
}

void TknPaletka::CopyParamDrawFill(STR_SQUARE* square, long count)
{
	for(long j0=0;j0<count;j0++)
	{
		SetStatusFill(0, short(square[j0].iIdent_1), short(square[j0].iIdent_2), 0, 0, square[j0].bIsDrawFill);
		SetDrawCell(short(square[j0].iIdent_1), short(square[j0].iIdent_2), 0, 0, square[j0].bIsDrawSquare);

		for(long j1=0;j1<square[j0].iCountChild;j1++)
		{
			SetStatusFill(1, short(square[j0].iIdent_1), short(square[j0].iIdent_2), short(square[j0].m_pArrayChild[j1].iIdent_1), 0, square[j0].m_pArrayChild[j1].bIsDrawFill);
			SetDrawCell(short(square[j0].iIdent_1), short(square[j0].iIdent_2), short(square[j0].m_pArrayChild[j1].iIdent_1), 0, 
				square[j0].m_pArrayChild[j1].bIsDrawSquare);

			for(long j2=0;j2<square[j0].m_pArrayChild[j1].iCountChild;j2++)
			{
				SetStatusFill(2, short(square[j0].iIdent_1), short(square[j0].iIdent_2), short(square[j0].m_pArrayChild[j1].iIdent_1), 
					short(square[j0].m_pArrayChild[j1].m_pArrayChild[j2].iIdent_1), square[j0].m_pArrayChild[j1].m_pArrayChild[j2].bIsDrawFill);

				SetDrawCell(short(square[j0].iIdent_1), short(square[j0].iIdent_2), short(square[j0].m_pArrayChild[j1].iIdent_1), 
					short(square[j0].m_pArrayChild[j1].m_pArrayChild[j2].iIdent_1), square[j0].m_pArrayChild[j1].m_pArrayChild[j2].bIsDrawSquare);
			}
		}
	}
}

bool TknPaletka::saveToBlob(std::ostream &mem) const
{
//	TknMapNonScaleBase::saveToBlob(mem);

	mem.write((char*)&iVersion, sizeof(iVersion));

	mem.write((char*)&fKurs, sizeof(fKurs));

	double b=0, l=0;
	getPosition(b, l);

	mem.write((char*)&b, sizeof(b));
	mem.write((char*)&l, sizeof(l));
	//////////////////////////////////////////////////////////////////////
	mem.write((char*)&fWline, sizeof(fWline));

	BYTE alfa_color=pColorLine.GetAlpha();
	BYTE red_color=pColorLine.GetRed();
	BYTE grean_color=pColorLine.GetGreen();
	BYTE blue_color=pColorLine.GetBlue();

	mem.write((char*)&alfa_color, sizeof(alfa_color));
	mem.write((char*)&red_color, sizeof(red_color));
	mem.write((char*)&grean_color, sizeof(grean_color));
	mem.write((char*)&blue_color, sizeof(blue_color));
	//////////////////////////////////////////////////////////////////////
	mem.write((char*)&iCountNumber, sizeof(iCountNumber));
	mem.write((char*)&iCountLetter, sizeof(iCountLetter));

	mem.write((char*)&strNameTkn, sizeof(strNameTkn));

	mem.write((char*)&iIndNumberB31, sizeof(iIndNumberB31));
	mem.write((char*)&iIndLetterB31, sizeof(iIndLetterB31));

	long size=vectorLetterFromLeft.size();
	mem.write((char*)&size, sizeof(size));

	for(long j=0;j<size;j++)
	{
//		mem.write((char*)&(vectorLetterFromLeft[j]), sizeof(STR_IDENT_LETTER));
		char str_temp[5];
		strcpy(str_temp, vectorLetterFromLeft[j].strMarkingLetter.c_str());
		mem.write((char*)&(str_temp), sizeof(char)*5);
		mem.write((char*)&(vectorLetterFromLeft[j].iIdentLetter), sizeof(long));
	}
	/////////////////////////////////////////////////////////////////////////////////
	mem.write((char*)&bDrawMarking0, sizeof(bDrawMarking0));
	mem.write((char*)&bDrawMarking1, sizeof(bDrawMarking1));
	mem.write((char*)&bDrawMarking2, sizeof(bDrawMarking2));
	mem.write((char*)&bDrawMarkingMain, sizeof(bDrawMarkingMain));

	mem.write((char*)&bIsDrawSquare1, sizeof(bIsDrawSquare1));
	mem.write((char*)&bIsDrawSquare2, sizeof(bIsDrawSquare2));
	/////////////////////////////////////////////////////////////////////////////////
	long count_cell=0;
	for(long lvl0=0;lvl0<iCountSquare;lvl0++)
	{
		count_cell++;
		for(long lvl1=0;lvl1<m_pSquare[lvl0].iCountChild;lvl1++)
		{
			count_cell++;
			for(long lvl2=0;lvl2<m_pSquare[lvl0].m_pArrayChild[lvl1].iCountChild;lvl2++)
			{
				count_cell++;
			}
		}
	}

	mem.write((char*)&count_cell, sizeof(count_cell));
	/////////////////////////////////////////////////
	for(long lvl0=0;lvl0<iCountSquare;lvl0++)
	{
		saveParamCellToBlob(mem, short(m_pSquare[lvl0].iIdent_1), short(m_pSquare[lvl0].iIdent_2), 
			0, 0,
			m_pSquare[lvl0].bIsDrawFill, 
			m_pSquare[lvl0].bIsDrawSquare, 
			m_pSquare[lvl0].bDrawMarking_1,
			m_pSquare[lvl0].bDrawMarkingLvl0);

		for(long lvl1=0;lvl1<m_pSquare[lvl0].iCountChild;lvl1++)
		{
			saveParamCellToBlob(mem, short(m_pSquare[lvl0].iIdent_1), short(m_pSquare[lvl0].iIdent_2), 
				short(m_pSquare[lvl0].m_pArrayChild[lvl1].iIdent_1), 0,
				m_pSquare[lvl0].m_pArrayChild[lvl1].bIsDrawFill, 
				m_pSquare[lvl0].m_pArrayChild[lvl1].bIsDrawSquare, 
				m_pSquare[lvl0].m_pArrayChild[lvl1].bDrawMarking_1,
				false);

			for(long lvl2=0;lvl2<m_pSquare[lvl0].m_pArrayChild[lvl1].iCountChild;lvl2++)
			{
				saveParamCellToBlob(mem, short(m_pSquare[lvl0].iIdent_1), short(m_pSquare[lvl0].iIdent_2), 
					short(m_pSquare[lvl0].m_pArrayChild[lvl1].iIdent_1), short(m_pSquare[lvl0].m_pArrayChild[lvl1].m_pArrayChild[lvl2].iIdent_1),
					m_pSquare[lvl0].m_pArrayChild[lvl1].m_pArrayChild[lvl2].bIsDrawFill, 
					m_pSquare[lvl0].m_pArrayChild[lvl1].m_pArrayChild[lvl2].bIsDrawSquare, 
					m_pSquare[lvl0].m_pArrayChild[lvl1].m_pArrayChild[lvl2].bDrawMarking_1,
					false);
			}
		}
	}
	///////////////////////////////////////////////////////
	mem.write((char*)&fBsvyaz, sizeof(fBsvyaz));
	mem.write((char*)&fLsvyaz, sizeof(fLsvyaz));

	mem.write((char*)&fBnot_svyaz, sizeof(fBnot_svyaz));
	mem.write((char*)&fLnot_svyaz, sizeof(fLnot_svyaz));

	mem.write((char*)&iDeviateSvyaz, sizeof(iDeviateSvyaz));
	mem.write((char*)&iErrorSvyaz, sizeof(iErrorSvyaz));
	/////////////////////////////
	mem.write((char*)&fBvs, sizeof(fBvs));
	mem.write((char*)&fLvs, sizeof(fLvs));

	mem.write((char*)&iErrorVs, sizeof(iErrorVs));
	mem.write((char*)&iHvs, sizeof(iHvs));
	mem.write((char*)&fVvertVs, sizeof(fVvertVs));
	mem.write((char*)&fVvs, sizeof(fVvs));
	mem.write((char*)&fKursVs, sizeof(fKursVs));
	mem.write((char*)&iDeviateVs, sizeof(iDeviateVs));
	/////////////////////////////
	mem.write((char*)&fB0, sizeof(fB0));
	mem.write((char*)&fL0, sizeof(fL0));
	mem.write((char*)&fB1, sizeof(fB1));
	mem.write((char*)&fL1, sizeof(fL1));
	mem.write((char*)&fError_0, sizeof(fError_0));
	mem.write((char*)&fDeviate, sizeof(fDeviate));

	mem.write((char*)&iStateDrawRegionSearch, sizeof(iStateDrawRegionSearch));
	///////////////////////////////////////////////////////
	pFontLvl0.saveToBlob(mem);
	pFontLvl1.saveToBlob(mem);
	pFontLvl2.saveToBlob(mem);
	pFontMain.saveToBlob(mem);
	///////////////////////////////////////////////////////

	return true;
}

void TknPaletka::saveParamCellToBlob(std::ostream &mem, short ident_line, short ident_column, short ident_10_10, short ident_5_5,
									 bool draw_was_search, bool draw_square, bool draw_marking, bool draw_marking_main) const
{
	mem.write((char*)&ident_line, sizeof(ident_line));
	mem.write((char*)&ident_column, sizeof(ident_column));
	mem.write((char*)&ident_10_10, sizeof(ident_10_10));
	mem.write((char*)&ident_5_5, sizeof(ident_5_5));
	mem.write((char*)&draw_was_search, sizeof(draw_was_search));
	mem.write((char*)&draw_square, sizeof(draw_square));
//	mem.write((char*)&draw_marking, sizeof(draw_marking));
//	mem.write((char*)&draw_marking_main, sizeof(draw_marking_main));
}

bool TknPaletka::loadFromBlob(std::istream &mem)
{
//	TknMapNonScaleBase::loadFromBlob(mem);

	unsigned char version;
	mem.read((char*)&version, sizeof(version));

	bool flag = true;

	switch(version)
	{
	case 1:
		flag = loadFromBlob_1(mem);
		break;
	case 2:
		flag = loadFromBlob_2(mem);
		break;
	case 3:
		flag = loadFromBlob_3(mem);
		break;
	case 4:
		flag = loadFromBlob_4(mem);
		break;
	default: flag = false;
	}
	return flag;
}

bool TknPaletka::loadFromBlob_1(std::istream &mem)
{
	ClearAllParamCell();

	mem.read((char*)&fKurs,  sizeof(fKurs));

	double b=0, l=0;
	mem.read((char*)&b, sizeof(b));
	mem.read((char*)&l, sizeof(l));
	Pos2D bl_pos;
	bl_pos.e1=b;
	bl_pos.e2=l;
	setPosition(bl_pos);

	mem.read((char*)&iCountNumber,  sizeof(iCountNumber));
	mem.read((char*)&iCountLetter,  sizeof(iCountLetter));

	if(iCountLetter==3)
	{
		vectorLetterFromLeft.push_back( STR_IDENT_LETTER("А", 99) );
		vectorLetterFromLeft.push_back( STR_IDENT_LETTER("Б", 100) );
		vectorLetterFromLeft.push_back( STR_IDENT_LETTER("В", 101) );
	}
	else if(iCountLetter==5)
	{
		vectorLetterFromLeft.push_back( STR_IDENT_LETTER("К", 98) );
		vectorLetterFromLeft.push_back( STR_IDENT_LETTER("А", 99) );
		vectorLetterFromLeft.push_back( STR_IDENT_LETTER("Б", 100) );
		vectorLetterFromLeft.push_back( STR_IDENT_LETTER("В", 101) );
		vectorLetterFromLeft.push_back( STR_IDENT_LETTER("Л", 102) );
	}

	mem.read((char*)&strNameTkn, sizeof(strNameTkn));
	strNameTkn[150]='\0';

	UpdateTrm();
	iIndNumberB31=iCountNumber/2;
	iIndLetterB31=iCountLetter/2;

	if(m_pSquare) delete[] m_pSquare;
	m_pSquare=NULL;

	InitTknPaletka();


	long count_cell=0;
	mem.read((char*)&count_cell, sizeof(count_cell));
	
	for(long lvl=0;lvl<count_cell;lvl++)
	{
		short ident_line=0;
		short ident_column=0;
		short ident_10_10=0;
		short ident_5_5=0;
		bool draw_was_search=false; 
		bool draw_square=false;
		bool draw_marking=false;

		mem.read((char*)&ident_line, sizeof(ident_line));
		mem.read((char*)&ident_column, sizeof(ident_column));
		mem.read((char*)&ident_10_10, sizeof(ident_10_10));
		mem.read((char*)&ident_5_5, sizeof(ident_5_5));
		mem.read((char*)&draw_was_search, sizeof(draw_was_search));
		mem.read((char*)&draw_square, sizeof(draw_square));
		mem.read((char*)&draw_marking, sizeof(draw_marking));

		SetStatusFill(ident_line, ident_column, ident_10_10, ident_5_5, draw_was_search);
		SetDrawCell(ident_line, ident_column, ident_10_10, ident_5_5, draw_square);
		SetDrawTextCell(ident_line, ident_column, ident_10_10, ident_5_5, draw_marking, false);
	}

	return true;
}

bool TknPaletka::loadFromBlob_2(std::istream &mem)
{
	ClearAllParamCell();

	mem.read((char*)&fKurs,  sizeof(fKurs));

	double b=0, l=0;
	mem.read((char*)&b, sizeof(b));
	mem.read((char*)&l, sizeof(l));
	Pos2D bl_pos;
	bl_pos.e1=b;
	bl_pos.e2=l;
	setPosition(bl_pos);
	///////////////////////////////////////////////////////////////////////
	mem.read((char*)&fWline,  sizeof(fWline));

	BYTE alfa_color=0, red_color=0, grean_color=0, blue_color=0;

	mem.read((char*)&alfa_color,  sizeof(alfa_color));
	mem.read((char*)&red_color,  sizeof(red_color));
	mem.read((char*)&grean_color,  sizeof(grean_color));
	mem.read((char*)&blue_color,  sizeof(blue_color));

	pColorLine=GdiGis::Color(alfa_color, red_color, grean_color, blue_color);
	///////////////////////////////////////////////////////////////////////
	mem.read((char*)&iCountNumber,  sizeof(iCountNumber));
	mem.read((char*)&iCountLetter,  sizeof(iCountLetter));

	mem.read((char*)&strNameTkn, sizeof(strNameTkn));
	strNameTkn[150]='\0';

	mem.read((char*)&iIndNumberB31, sizeof(iIndNumberB31));
	mem.read((char*)&iIndLetterB31, sizeof(iIndLetterB31));

	long size=0;
	mem.read((char*)&size, sizeof(size));
	for(long j=0;j<size;j++)
	{
		STR_IDENT_LETTER pSTR;
//		mem.read((char*)&pSTR, sizeof(STR_IDENT_LETTER));

		char str_temp[5];
		mem.read((char*)&(str_temp), sizeof(char)*5);
		pSTR.strMarkingLetter=str_temp;
		mem.read((char*)&(pSTR.iIdentLetter), sizeof(long));

		vectorLetterFromLeft.push_back(pSTR);
	}

	UpdateTrm();
//	iIndNumberB31=iCountNumber/2;
//	iIndLetterB31=iCountLetter/2;]

	if(m_pSquare) delete[] m_pSquare;
	m_pSquare=NULL;

	InitTknPaletka();

	long count_cell=0;
	mem.read((char*)&count_cell, sizeof(count_cell));
	
	for(long lvl=0;lvl<count_cell;lvl++)
	{
		short ident_line=0;
		short ident_column=0;
		short ident_10_10=0;
		short ident_5_5=0;
		bool draw_was_search=false; 
		bool draw_square=false;
		bool draw_marking=false;

		mem.read((char*)&ident_line, sizeof(ident_line));
		mem.read((char*)&ident_column, sizeof(ident_column));
		mem.read((char*)&ident_10_10, sizeof(ident_10_10));
		mem.read((char*)&ident_5_5, sizeof(ident_5_5));
		mem.read((char*)&draw_was_search, sizeof(draw_was_search));
		mem.read((char*)&draw_square, sizeof(draw_square));
		mem.read((char*)&draw_marking, sizeof(draw_marking));

		SetStatusFill(ident_line, ident_column, ident_10_10, ident_5_5, draw_was_search);
		SetDrawCell(ident_line, ident_column, ident_10_10, ident_5_5, draw_square);
		SetDrawTextCell(ident_line, ident_column, ident_10_10, ident_5_5, draw_marking, false);
	}

	return true;
}

bool TknPaletka::loadFromBlob_3(std::istream &mem)
{
	ClearAllParamCell();

	mem.read((char*)&fKurs,  sizeof(fKurs));

	double b=0, l=0;
	mem.read((char*)&b, sizeof(b));
	mem.read((char*)&l, sizeof(l));
	Pos2D bl_pos;
	bl_pos.e1=b;
	bl_pos.e2=l;
	setPosition(bl_pos);
	///////////////////////////////////////////////////////////////////////
	mem.read((char*)&fWline,  sizeof(fWline));

	BYTE alfa_color=0, red_color=0, grean_color=0, blue_color=0;

	mem.read((char*)&alfa_color,  sizeof(alfa_color));
	mem.read((char*)&red_color,  sizeof(red_color));
	mem.read((char*)&grean_color,  sizeof(grean_color));
	mem.read((char*)&blue_color,  sizeof(blue_color));

	pColorLine=GdiGis::Color(alfa_color, red_color, grean_color, blue_color);
	///////////////////////////////////////////////////////////////////////
	mem.read((char*)&iCountNumber,  sizeof(iCountNumber));
	mem.read((char*)&iCountLetter,  sizeof(iCountLetter));

	mem.read((char*)&strNameTkn, sizeof(strNameTkn));
	strNameTkn[150]='\0';

	mem.read((char*)&iIndNumberB31, sizeof(iIndNumberB31));
	mem.read((char*)&iIndLetterB31, sizeof(iIndLetterB31));

	long size=0;
	mem.read((char*)&size, sizeof(size));
	for(long j=0;j<size;j++)
	{
		STR_IDENT_LETTER pSTR;
//		mem.read((char*)&pSTR, sizeof(STR_IDENT_LETTER));

		char str_temp[5];
		mem.read((char*)&(str_temp), sizeof(char)*5);
		pSTR.strMarkingLetter=str_temp;
		mem.read((char*)&(pSTR.iIdentLetter), sizeof(long));

		vectorLetterFromLeft.push_back(pSTR);
	}

	UpdateTrm();
//	iIndNumberB31=iCountNumber/2;
//	iIndLetterB31=iCountLetter/2;]

	if(m_pSquare) delete[] m_pSquare;
	m_pSquare=NULL;

	InitTknPaletka();

	long count_cell=0;
	mem.read((char*)&count_cell, sizeof(count_cell));
	
	for(long lvl=0;lvl<count_cell;lvl++)
	{
		short ident_line=0;
		short ident_column=0;
		short ident_10_10=0;
		short ident_5_5=0;
		bool draw_was_search=false; 
		bool draw_square=false;
		bool draw_marking=false;

		mem.read((char*)&ident_line, sizeof(ident_line));
		mem.read((char*)&ident_column, sizeof(ident_column));
		mem.read((char*)&ident_10_10, sizeof(ident_10_10));
		mem.read((char*)&ident_5_5, sizeof(ident_5_5));
		mem.read((char*)&draw_was_search, sizeof(draw_was_search));
		mem.read((char*)&draw_square, sizeof(draw_square));
		mem.read((char*)&draw_marking, sizeof(draw_marking));

		SetStatusFill(ident_line, ident_column, ident_10_10, ident_5_5, draw_was_search);
		SetDrawCell(ident_line, ident_column, ident_10_10, ident_5_5, draw_square);
		SetDrawTextCell(ident_line, ident_column, ident_10_10, ident_5_5, draw_marking, false);
	}

	///////////////////////////////////////////////////////
	mem.read((char*)&fBsvyaz, sizeof(fBsvyaz));
	mem.read((char*)&fLsvyaz, sizeof(fLsvyaz));

	mem.read((char*)&fBnot_svyaz, sizeof(fBnot_svyaz));
	mem.read((char*)&fLnot_svyaz, sizeof(fLnot_svyaz));

	mem.read((char*)&iDeviateSvyaz, sizeof(iDeviateSvyaz));
	mem.read((char*)&iErrorSvyaz, sizeof(iErrorSvyaz));
	/////////////////////////////
	mem.read((char*)&fBvs, sizeof(fBvs));
	mem.read((char*)&fLvs, sizeof(fLvs));

	mem.read((char*)&iErrorVs, sizeof(iErrorVs));
	mem.read((char*)&iHvs, sizeof(iHvs));
	mem.read((char*)&fVvertVs, sizeof(fVvertVs));
	mem.read((char*)&fVvs, sizeof(fVvs));
	mem.read((char*)&fKursVs, sizeof(fKursVs));
	mem.read((char*)&iDeviateVs, sizeof(iDeviateVs));
	/////////////////////////////
	mem.read((char*)&fB0, sizeof(fB0));
	mem.read((char*)&fL0, sizeof(fL0));
	mem.read((char*)&fB1, sizeof(fB1));
	mem.read((char*)&fL1, sizeof(fL1));
	mem.read((char*)&fError_0, sizeof(fError_0));
	mem.read((char*)&fDeviate, sizeof(fDeviate));

	mem.read((char*)&iStateDrawRegionSearch, sizeof(iStateDrawRegionSearch));
	///////////////////////////////////////////////////////

	return true;
}

bool TknPaletka::loadFromBlob_4(std::istream &mem)
{
	ClearAllParamCell();

	mem.read((char*)&fKurs,  sizeof(fKurs));

	double b=0, l=0;
	mem.read((char*)&b, sizeof(b));
	mem.read((char*)&l, sizeof(l));
	Pos2D bl_pos;
	bl_pos.e1=b;
	bl_pos.e2=l;
	setPosition(bl_pos);
	///////////////////////////////////////////////////////////////////////
	mem.read((char*)&fWline,  sizeof(fWline));

	BYTE alfa_color=0, red_color=0, grean_color=0, blue_color=0;

	mem.read((char*)&alfa_color,  sizeof(alfa_color));
	mem.read((char*)&red_color,  sizeof(red_color));
	mem.read((char*)&grean_color,  sizeof(grean_color));
	mem.read((char*)&blue_color,  sizeof(blue_color));

	pColorLine=GdiGis::Color(alfa_color, red_color, grean_color, blue_color);
	///////////////////////////////////////////////////////////////////////
	mem.read((char*)&iCountNumber,  sizeof(iCountNumber));
	mem.read((char*)&iCountLetter,  sizeof(iCountLetter));

	mem.read((char*)&strNameTkn, sizeof(strNameTkn));
	strNameTkn[150]='\0';

	mem.read((char*)&iIndNumberB31, sizeof(iIndNumberB31));
	mem.read((char*)&iIndLetterB31, sizeof(iIndLetterB31));

	long size=0;
	mem.read((char*)&size, sizeof(size));
	for(long j=0;j<size;j++)
	{
		STR_IDENT_LETTER pSTR;
//		mem.read((char*)&pSTR, sizeof(STR_IDENT_LETTER));

		char str_temp[5];
		mem.read((char*)&(str_temp), sizeof(char)*5);
		pSTR.strMarkingLetter=str_temp;
		mem.read((char*)&(pSTR.iIdentLetter), sizeof(long));

		vectorLetterFromLeft.push_back(pSTR);
	}

	UpdateTrm();
//	iIndNumberB31=iCountNumber/2;
//	iIndLetterB31=iCountLetter/2;]

	if(m_pSquare) delete[] m_pSquare;
	m_pSquare=NULL;

	InitTknPaletka();
	/////////////////////////////////////////////////
	mem.read((char*)&bDrawMarking0, sizeof(bDrawMarking0));
	mem.read((char*)&bDrawMarking1, sizeof(bDrawMarking1));
	mem.read((char*)&bDrawMarking2, sizeof(bDrawMarking2));
	mem.read((char*)&bDrawMarkingMain, sizeof(bDrawMarkingMain));

	mem.read((char*)&bIsDrawSquare1, sizeof(bIsDrawSquare1));
	mem.read((char*)&bIsDrawSquare2, sizeof(bIsDrawSquare2));
	/////////////////////////////////////////////////

	long count_cell=0;
	mem.read((char*)&count_cell, sizeof(count_cell));
	
	for(long lvl=0;lvl<count_cell;lvl++)
	{
		short ident_line=0;
		short ident_column=0;
		short ident_10_10=0;
		short ident_5_5=0;
		bool draw_was_search=false; 
		bool draw_square=false;
//		bool draw_marking=false;
//		bool draw_marking_main=false;

		mem.read((char*)&ident_line, sizeof(ident_line));
		mem.read((char*)&ident_column, sizeof(ident_column));
		mem.read((char*)&ident_10_10, sizeof(ident_10_10));
		mem.read((char*)&ident_5_5, sizeof(ident_5_5));
		mem.read((char*)&draw_was_search, sizeof(draw_was_search));
		mem.read((char*)&draw_square, sizeof(draw_square));
//		mem.read((char*)&draw_marking, sizeof(draw_marking));
//		mem.read((char*)&draw_marking_main, sizeof(draw_marking_main));

		SetStatusFill(ident_line, ident_column, ident_10_10, ident_5_5, draw_was_search);
		SetDrawCell(ident_line, ident_column, ident_10_10, ident_5_5, draw_square);
//		SetDrawTextCell(ident_line, ident_column, ident_10_10, ident_5_5, draw_marking, draw_marking_main);
	}

	///////////////////////////////////////////////////////
	mem.read((char*)&fBsvyaz, sizeof(fBsvyaz));
	mem.read((char*)&fLsvyaz, sizeof(fLsvyaz));

	mem.read((char*)&fBnot_svyaz, sizeof(fBnot_svyaz));
	mem.read((char*)&fLnot_svyaz, sizeof(fLnot_svyaz));

	mem.read((char*)&iDeviateSvyaz, sizeof(iDeviateSvyaz));
	mem.read((char*)&iErrorSvyaz, sizeof(iErrorSvyaz));
	/////////////////////////////
	mem.read((char*)&fBvs, sizeof(fBvs));
	mem.read((char*)&fLvs, sizeof(fLvs));

	mem.read((char*)&iErrorVs, sizeof(iErrorVs));
	mem.read((char*)&iHvs, sizeof(iHvs));
	mem.read((char*)&fVvertVs, sizeof(fVvertVs));
	mem.read((char*)&fVvs, sizeof(fVvs));
	mem.read((char*)&fKursVs, sizeof(fKursVs));
	mem.read((char*)&iDeviateVs, sizeof(iDeviateVs));
	/////////////////////////////
	mem.read((char*)&fB0, sizeof(fB0));
	mem.read((char*)&fL0, sizeof(fL0));
	mem.read((char*)&fB1, sizeof(fB1));
	mem.read((char*)&fL1, sizeof(fL1));
	mem.read((char*)&fError_0, sizeof(fError_0));
	mem.read((char*)&fDeviate, sizeof(fDeviate));

	mem.read((char*)&iStateDrawRegionSearch, sizeof(iStateDrawRegionSearch));
	///////////////////////////////////////////////////////
	pFontLvl0.loadFromBlob(mem);
	pFontLvl1.loadFromBlob(mem);
	pFontLvl2.loadFromBlob(mem);
	pFontMain.loadFromBlob(mem);
	///////////////////////////////////////////////////////
	SetDrawTextCell(-1, bDrawMarkingMain);
	SetDrawTextCell(0, bDrawMarking0);
	SetDrawTextCell(1, bDrawMarking1);
	SetDrawTextCell(2, bDrawMarking2);

	SetDrawCell(1, bIsDrawSquare1);
	SetDrawCell(2, bIsDrawSquare2);

	return true;
}

void TknPaletka::SetStatusFill(short ident_line, short ident_column, short ident_10_10, short ident_5_5, bool draw_was_search)
{
	for(long j0=0;j0<iCountSquare;j0++)
	{
		if(m_pSquare[j0].iIdent_1==ident_line && m_pSquare[j0].iIdent_2==ident_column)
		{
			if(ident_10_10==0) 
			{
				m_pSquare[j0].bIsDrawFill=draw_was_search;
			}
			else
			{
				for(long j1=0;j1<m_pSquare[j0].iCountChild;j1++)
				{
					if(m_pSquare[j0].m_pArrayChild[j1].iIdent_1==ident_10_10)
					{
						if(ident_5_5==0) 
						{
							m_pSquare[j0].m_pArrayChild[j1].bIsDrawFill=draw_was_search;
						}
						else
						{
							for(long j2=0;j2<m_pSquare[j0].m_pArrayChild[j1].iCountChild;j2++)
							{
								if(m_pSquare[j0].m_pArrayChild[j1].m_pArrayChild[j2].iIdent_1==ident_5_5)
									m_pSquare[j0].m_pArrayChild[j1].m_pArrayChild[j2].bIsDrawFill=draw_was_search;
							}
						}
					}
				}
			}
		}
	}
}

void TknPaletka::SetDrawCell(short ident_line, short ident_column, short ident_10_10, short ident_5_5, bool draw_square)
{
	for(long j0=0;j0<iCountSquare;j0++)
	{
		if(m_pSquare[j0].iIdent_1==ident_line && m_pSquare[j0].iIdent_2==ident_column)
		{
			if(ident_10_10==0) 
			{
				m_pSquare[j0].bIsDrawSquare=draw_square;
			}
			else
			{
				for(long j1=0;j1<m_pSquare[j0].iCountChild;j1++)
				{
					if(m_pSquare[j0].m_pArrayChild[j1].iIdent_1==ident_10_10)
					{
						if(ident_5_5==0) 
						{
							m_pSquare[j0].m_pArrayChild[j1].bIsDrawSquare=draw_square;
						}
						else
						{
							for(long j2=0;j2<m_pSquare[j0].m_pArrayChild[j1].iCountChild;j2++)
							{
								if(m_pSquare[j0].m_pArrayChild[j1].m_pArrayChild[j2].iIdent_1==ident_5_5)
									m_pSquare[j0].m_pArrayChild[j1].m_pArrayChild[j2].bIsDrawSquare=draw_square;
							}
						}
					}
				}
			}
		}
	}
}

void TknPaletka::SetDrawTextCell(short ident_line, short ident_column, short ident_10_10, short ident_5_5, bool draw_marking, bool draw_marking_main)
{
	for(long j0=0;j0<iCountSquare;j0++)
	{
		if(m_pSquare[j0].iIdent_1==ident_line && m_pSquare[j0].iIdent_2==ident_column)
		{
			if(ident_10_10==0) 
			{
				m_pSquare[j0].bDrawMarking_1=draw_marking;
				m_pSquare[j0].bDrawMarkingLvl0=draw_marking_main;
			}
			else
			{
				for(long j1=0;j1<m_pSquare[j0].iCountChild;j1++)
				{
					if(m_pSquare[j0].m_pArrayChild[j1].iIdent_1==ident_10_10)
					{
						if(ident_5_5==0) 
						{
							m_pSquare[j0].m_pArrayChild[j1].bDrawMarking_1=draw_marking;
						}
						else
						{
							for(long j2=0;j2<m_pSquare[j0].m_pArrayChild[j1].iCountChild;j2++)
							{
								if(m_pSquare[j0].m_pArrayChild[j1].m_pArrayChild[j2].iIdent_1==ident_5_5)
									m_pSquare[j0].m_pArrayChild[j1].m_pArrayChild[j2].bDrawMarking_1=draw_marking;
							}
						}
					}
				}
			}
		}
	}
}

bool TknPaletka::CopyFromTkn(TknPaletka* tkn_from)
{
	std::ostrstream pFile1;
	bool bSave=tkn_from->saveToBlob(pFile1);
	if(!bSave) return false;
	long iLen=pFile1.pcount(); 
	if(iLen<1) return false;
	BYTE* m_pChar=(BYTE*)pFile1.str();		

	/////////////////////////////////////////////

	std::istrstream pFile((const char*)m_pChar, iLen);
	bool bRet=loadFromBlob(pFile);

	return bRet;
}

void TknPaletka::AddLineUp()
{
	iCountNumber++;

	STR_SQUARE* m_pTempStr=m_pSquare;
	long iTempCount=iCountSquare;

//	iCountSquare=iCountNumber*iCountLetter;
//	m_pSquare=new STR_SQUARE[iCountSquare];
	InitTknPaletka();

	CopyParamDrawFill(m_pTempStr, iTempCount);

	delete[] m_pTempStr;

/*	for(long j=0;j<iCountLetter;j++)
	{
		long ind_for_new=j*iCountNumber;
		long ind_for_old=j*(iCountNumber-1);
		memcpy(&(m_pSquare[ind_for_new]), &(m_pTempStr[ind_for_old]), sizeof());
	}*/
/*
	for(long let=0;let<iCountLetter;let++)
	{
		for(long num=0;num<iCountNumber;num++)
		{	

	STR_SQUARE* m_pTempStr=m_pSquare;
	long iTempCount=iCountSquare;

	iCountSquare=iCountLine*iCountColumn;
	m_pSquare=new STR_SQUARE[iCountSquare];
	///////////////////////////////////////////////////////////
	iIndColB31	= iCountColumn/2;
	iIndLineB31	= iCountLine/2;

	CopyParamDrawFill(m_pTempStr, iTempCount);*/
}

void TknPaletka::AddLineDown()
{
	if(iIndNumberB31>=20) return;

	iCountNumber++;
	iIndNumberB31++;

	STR_SQUARE* m_pTempStr=m_pSquare;
	long iTempCount=iCountSquare;

	InitTknPaletka();

	CopyParamDrawFill(m_pTempStr, iTempCount);

	delete[] m_pTempStr;
}

void TknPaletka::DeleteLineUp()
{
	if(iCountNumber<=4) return;
	if(iIndNumberB31>=iCountNumber-1) return;
	iCountNumber--;

	STR_SQUARE* m_pTempStr=m_pSquare;
	long iTempCount=iCountSquare;

//	iCountSquare=iCountNumber*iCountLetter;
//	m_pSquare=new STR_SQUARE[iCountSquare];
	InitTknPaletka();

	CopyParamDrawFill(m_pTempStr, iTempCount);

	delete[] m_pTempStr;
}

void TknPaletka::DeleteLineDown()
{
	if(iCountNumber<=4) return;
	if(iIndNumberB31<=0) return;

	iCountNumber--;
	iIndNumberB31--;

	STR_SQUARE* m_pTempStr=m_pSquare;
	long iTempCount=iCountSquare;

	InitTknPaletka();

	CopyParamDrawFill(m_pTempStr, iTempCount);

	delete[] m_pTempStr;
}

bool TknPaletka::IsNeedLetterForNewColumn(bool for_left)
{
	if(for_left && iIndLetterB31<2) return false;
	else if(!for_left && iCountLetter-iIndLetterB31<3) return false;

	return true;
}

bool TknPaletka::IsLetterBusy(std::string str_letter)
{
	if(str_letter=="К" || str_letter=="А" || str_letter=="Б" || str_letter=="В" || str_letter=="Л") return true;
	for(long j=0;j<(long)vectorLetterFromLeft.size();j++)
	{
		if(vectorLetterFromLeft[j].strMarkingLetter==str_letter) return true;
	}
	return false;
}

void TknPaletka::DeleteColNotDraw()
{
	while(true)
	{
		long copy_count_letter=iCountLetter;
		if(copy_count_letter<3) break;

		long num=0;
		long let=0;
		bool bDraw=false;
		for(num=0;num<iCountNumber;num++)
		{	
			long ind=let*iCountNumber+num;
			if(m_pSquare[ind].bIsDrawSquare)
			{
				bDraw=true;
				break;
			}
		}
		if(!bDraw) DeleteColLeft(); 
		////////////////////////////////////
		let=iCountLetter-1;
		bDraw=false;
		for(num=0;num<iCountNumber;num++)
		{	
			long ind=let*iCountNumber+num;
			if(m_pSquare[ind].bIsDrawSquare)
			{
				bDraw=true;
				break;
			}
		}
		if(!bDraw) DeleteColRight();

		if(copy_count_letter==iCountLetter) break;
	}
}

short TknPaletka::ExchangeLetter(std::string str_letter_exchange, std::string str_letter)
{//0-успешно, 1-идентификатор нельзя менять, 2-новый идентификатор занят, 3-заменяемый идентификатор не найден
	if(str_letter_exchange=="К" || str_letter_exchange=="А" || str_letter_exchange=="Б" || str_letter_exchange=="В" || str_letter_exchange=="Л") 
		return 1;

	if(IsLetterBusy(str_letter)) return 2;

	for(long j=0;j<(long)vectorLetterFromLeft.size();j++)
	{
		if(vectorLetterFromLeft[j].strMarkingLetter==str_letter_exchange) 
		{
			vectorLetterFromLeft[j].strMarkingLetter=str_letter;
			for(long j1=0;j1<iCountSquare;j1++)
			{
				if(m_pSquare[j1].iIdent_1==vectorLetterFromLeft[j].iIdentLetter)
				{
					m_pSquare[j1].strMarking_1=str_letter;
				}
			}

			return 0;
		}
	}
	return 3;
}

void TknPaletka::AddColLeft(std::string str_letter)
{
	if(vectorLetterFromLeft.size()<1) return;

	if(iIndLetterB31==0)
		vectorLetterFromLeft.insert( vectorLetterFromLeft.begin(), STR_IDENT_LETTER("А", vectorLetterFromLeft[0].iIdentLetter-1) );
	else if(iIndLetterB31==1)
		vectorLetterFromLeft.insert( vectorLetterFromLeft.begin(), STR_IDENT_LETTER("К", vectorLetterFromLeft[0].iIdentLetter-1) );
	else
		vectorLetterFromLeft.insert( vectorLetterFromLeft.begin(), STR_IDENT_LETTER(str_letter, vectorLetterFromLeft[0].iIdentLetter-1) );

	iCountLetter++;
	iIndLetterB31++;

	STR_SQUARE* m_pTempStr=m_pSquare;
	long iTempCount=iCountSquare;

	InitTknPaletka();

	CopyParamDrawFill(m_pTempStr, iTempCount);

	delete[] m_pTempStr;
}

void TknPaletka::AddColRight(std::string str_letter)
{
	int size=vectorLetterFromLeft.size();
	if(size<1) return;

	if(iCountLetter-iIndLetterB31==1)
		vectorLetterFromLeft.push_back( STR_IDENT_LETTER("В", vectorLetterFromLeft[size-1].iIdentLetter+1) );
	else if(iCountLetter-iIndLetterB31==2)
		vectorLetterFromLeft.push_back( STR_IDENT_LETTER("Л", vectorLetterFromLeft[size-1].iIdentLetter+1) );
	else
		vectorLetterFromLeft.push_back( STR_IDENT_LETTER(str_letter, vectorLetterFromLeft[size-1].iIdentLetter+1) );

	iCountLetter++;

	STR_SQUARE* m_pTempStr=m_pSquare;
	long iTempCount=iCountSquare;

	InitTknPaletka();

	CopyParamDrawFill(m_pTempStr, iTempCount);

	delete[] m_pTempStr;
}

void TknPaletka::DeleteColLeft()
{
	if(iCountLetter<=3) return;
	if(iIndLetterB31<=0) return;
	if(vectorLetterFromLeft.size()<1) return;

	vectorLetterFromLeft.erase( vectorLetterFromLeft.begin() );

	iCountLetter--;
	iIndLetterB31--;

	STR_SQUARE* m_pTempStr=m_pSquare;
	long iTempCount=iCountSquare;

	InitTknPaletka();

	CopyParamDrawFill(m_pTempStr, iTempCount);

	delete[] m_pTempStr;
}

void TknPaletka::DeleteColRight()
{
	if(iCountLetter<=3) return;
	if(iIndLetterB31>=iCountLetter-1) return;
	int size=vectorLetterFromLeft.size();
	if(size<1) return;

	vectorLetterFromLeft.erase( vectorLetterFromLeft.begin()+(size-1) );

	iCountLetter--;

	STR_SQUARE* m_pTempStr=m_pSquare;
	long iTempCount=iCountSquare;

	InitTknPaletka();

	CopyParamDrawFill(m_pTempStr, iTempCount);

	delete[] m_pTempStr;
}

long TknPaletka::GetIndVectorForIdentLetter(long ident_letter)
{
	for(long j=0;j<(long)vectorLetterFromLeft.size();j++)
	{
		if(vectorLetterFromLeft[j].iIdentLetter==ident_letter)
			return j;
	}
	return -1;
}

void TknPaletka::SetDrawSquareLevel0(BOOL is_draw, long ind_letter, long ind_num)
{
	if(ind_letter<0 || ind_letter>=iCountLetter) return;
	if(ind_num<0 || ind_num>=iCountNumber) return;

	long ind=ind_letter*iCountNumber+ind_num;

	if(is_draw==0) m_pSquare[ind].bIsDrawSquare=false;
	else m_pSquare[ind].bIsDrawSquare=true;
}

bool TknPaletka::IsDrawSquareLevel0(long ind_letter, long ind_num)
{
	if(ind_letter<0 || ind_letter>=iCountLetter) return false;
	if(ind_num<0 || ind_num>=iCountNumber) return false;

	long ind=ind_letter*iCountNumber+ind_num;

	return m_pSquare[ind].bIsDrawSquare;
}

void TknPaletka::SetColorLine(COLORREF color_line)
{
	pColorLine.SetFromCOLORREF(color_line);
//	pColorLine=GetRedValue(rgb), GetGreenValue(rgb), GetBlueValue(rgb)
}

void TknPaletka::SetWline(double w_line)
{
	fWline=w_line;
	if(fWline<0.01) fWline=0.01;
	if(fWline>100) fWline=100;
}