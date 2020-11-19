#include "stdafx.h"
#include "gis\GisBase\cppgis\TknBase.h"
#include "gis\GisBase\cgis\str.h"


//TknPaletka
class TypeLinesObj;
class TknPaletka : public TknMapNonScaleBase  
{
public: 
  /** Конструктор */
	TknPaletka();//CPyramidFrame* par_frame);
	virtual ~TknPaletka();

	/** Отображение геопространственного объекта системой визуализации */
	virtual CctError* draw(GisLayer *gisLayer, GdiGis::ImageDevice *dev);

	/** запись знака в выходной поток в двоичном виде
     @return успешно ли выполнена запись
	*/
	virtual bool saveToBlob(std::ostream &mem) const;

	/** чтение знака из двоичного входного потока
     @return успешно ли выполнено чтение
	*/
	virtual bool loadFromBlob(std::istream &mem);

	bool loadFromBlob_1(std::istream &mem);
	bool loadFromBlob_2(std::istream &mem);
	bool loadFromBlob_3(std::istream &mem);
	bool loadFromBlob_4(std::istream &mem);

	void DrawKvadrat(GisLayer* gisLayer, GdiGis::ImageDevice *dev, GdiGis::Pen* pen_draw, double x_bottom, double y_left, double x_top, double y_right);
	void DrawText(GisLayer* gisLayer, GdiGis::ImageDevice *dev, double ray_polar, double size_mm, double x_center, double y_center, std::string str_text, BYTE transpare);
	void DrawRegionSearch(GisLayer* gisLayer, GdiGis::ImageDevice* dev);
	void BLtoWnd(GisLayer* gisLayer, double b, double l, double& x, double& y);
	double GetKursOrtodrom(double b0, double l0, double b1, double l1);
	double GetLenToPoint(double b0, double l0, double b1, double l1);
	void GetBLforPointKursLenOrtodrom(double b0, double l0, double kurs, double per_len, double& b, double& l);

	void SetCountLineAndColumn(short count_line, short count_column);

	void UpdateTrm();

	void GeoToKsa(double b, double l, double& x, double& y);
	void KsaToGeo(double x, double y, double& b, double& l);

	void SetParamPaletka(double b, double l, double kurs);
	void SetPositionPaletka(double b, double l);

	void SetStatusFill(short level, short ident_line, short ident_column, short ident_10_10, short ident_5_5, bool show);
	void ClearAllParamCell();

	void SetDrawCell(short level, bool show);
	void SetDrawTextCell(short level, bool show);

	void saveParamCellToBlob(std::ostream &mem, short ident_line, short ident_column, short ident_10_10, short ident_5_5,
							 bool draw_was_search, bool draw_square, bool draw_marking, bool draw_marking_main) const;

	void SetStatusFill(short ident_line, short ident_column, short ident_10_10, short ident_5_5, bool draw_was_search);
	void SetDrawCell(short ident_line, short ident_column, short ident_10_10, short ident_5_5, bool draw_square);
	void SetDrawTextCell(short ident_line, short ident_column, short ident_10_10, short ident_5_5, bool draw_marking, bool draw_marking_main);

	bool CopyFromTkn(TknPaletka* tkn_from);

	void SetNameTkn(std::string str_name);

	void InitTknPaletka();
	void AddLineUp();
	void AddLineDown();
	void DeleteLineUp();
	void DeleteLineDown();
	void AddColLeft(std::string str_letter);
	void AddColRight(std::string str_letter);
	void DeleteColLeft();
	void DeleteColRight();

	long GetIndVectorForIdentLetter(long ident_letter);

	void SetDrawSquareLevel0(BOOL is_draw, long ind_letter, long ind_num);
	bool IsDrawSquareLevel0(long ind_letter, long ind_num);

	void SetColorLine(COLORREF color_line);
	void SetWline(double w_line);

	bool IsNeedLetterForNewColumn(bool for_left);
	bool IsLetterBusy(std::string str_letter);
	void DeleteColNotDraw();

	///////////////////////////////////////////////////////////////////////////////
	////return: 0-успешно, 1-идентификатор нельзя менять, 2-новый идентификатор занят, 3-заменяемый идентификатор не найден
	short ExchangeLetter(std::string str_letter_exchange, std::string str_letter);

public:
	unsigned char iVersion;
	double fKurs;

	short iCountNumber;//left-right - цифры column
	short iCountLetter;//bottom-top - буквы line

	char strNameTkn[151];

	long iIndNumberB31;
	long iIndLetterB31;
//	long iSizeLeft;
//	long iSizeRight;
//	double fSizeTop;
//	double fSizeBottom;

//	CPyramidFrame* m_pParentFrame;


	//////////////////////////////////////////////////////
	//район поиска
	bool bIsDraw;
	BYTE iStateDrawRegionSearch;//0-не задан, 1-не отображать, 2-отображать

	double fB0;
	double fL0;
	double fB1;
	double fL1;
	double fError_0;
	double fDeviate;
	//////////////////////////////////////////////////////
	//рассчетные
	TrmGeo2Ksa* m_pGeoToKsa;
	TrmKsa2Geo* m_pKsaToGeo;

	double fSizeKvadrWnd;

	PosEllpsExt pEllpsExt;

	TypeLinesObj* m_pTypeLineLvl0;

	double fWline;
	GdiGis::Color pColorLine;

	GisFontExt pFontLvl0;
	GisFontExt pFontLvl1;
	GisFontExt pFontLvl2;
	GisFontExt pFontMain;

	bool bDrawMarking0;
	bool bDrawMarking1;
	bool bDrawMarking2;
	bool bDrawMarkingMain;

	bool bIsDrawSquare1;
	bool bIsDrawSquare2;
	///////////////////////////////////////////////////////
	//параметры для расчетов палетки
	double fBsvyaz;
	double fLsvyaz;

	double fBnot_svyaz;
	double fLnot_svyaz;

	long iDeviateSvyaz;
	long iErrorSvyaz;
	/////////////////////////////
	double fBvs;
	double fLvs;

	long	iErrorVs;
	long	iHvs;
	double	fVvertVs;
	double	fVvs;
	double	fKursVs;
	long	iDeviateVs;
	///////////////////////////////////////////////////////

public:
	struct STR_SQUARE
	{
		STR_SQUARE()
		{
			m_pParent		= NULL;

			fXksaBottom		= 0;
			fXksaTop		= 0;
			fYksaLeft		= 0;
			fYksaRight		= 0;

			m_pArrayChild	= NULL;
			iCountChild		= 0;

			bDrawMarkingMain= true;

			bDrawMarking_1	= false;
			fXmarking_1		= 0;
			fYmarking_1		= 0;
			iIdent_1		= 0;

			bDrawMarking_2	= false;
			fXmarking_2		= 0;
			fYmarking_2		= 0;
			iIdent_2		= 0;	

			bDrawMarkingLvl0= false;
			fXmarkingLvl0	= 0;
			fYmarkingLvl0	= 0;

			bIsDrawSquare	= false;
			bIsDrawFill		= false;
//			bIsDrawText		= true;

			iLvlSquare		= 0;
		}
		~STR_SQUARE()
		{
			if(m_pArrayChild) delete[] m_pArrayChild;
			m_pArrayChild=NULL;
		}
		STR_SQUARE(STR_SQUARE* parent, std::string str_marking, double x_bottom, double x_top, double y_left, double y_right)
		{
			m_pParent	= parent;

//			strMarking	= str_marking;

			fXksaBottom	= x_bottom;
			fXksaTop	= x_top;
			fYksaLeft	= y_left;
			fYksaRight	= y_right;

			m_pArrayChild= NULL;
			iCountChild	= 0;
		}
		std::string GetNameSquareMenu()
		{
			std::string strRet;

			if(iLvlSquare==0) strRet=strMarking_1+"-"+strMarking_2;
			else if(iLvlSquare==1 && m_pParent) strRet=strMarking_1+"("+m_pParent->strMarking_1+"-"+m_pParent->strMarking_2+")";
			else if(iLvlSquare==2 && m_pParent && m_pParent->m_pParent) 
				strRet=m_pParent->strMarking_1+"-"+strMarking_1+"("+m_pParent->m_pParent->strMarking_1+"-"+m_pParent->m_pParent->strMarking_2+")";

			return strRet;
		}
		void SetDrawMarking_1(bool is_draw, double x, double y)
		{
			bDrawMarking_1	= is_draw;
			fXmarking_1		= x;
			fYmarking_1		= y;
		}
		void SetDrawMarking_2(bool is_draw, double x, double y)
		{
			bDrawMarking_2	= is_draw;
			fXmarking_2		= x;
			fYmarking_2		= y;
		}
		void SetDrawMarkingLvl0(bool is_draw, double x, double y)
		{
			bDrawMarkingLvl0= is_draw;
			fXmarkingLvl0	= x;
			fYmarkingLvl0	= y;
		}
		void DivideSquareLvl1()
		{
			m_pArrayChild=new STR_SQUARE[4];
			iCountChild=4;

			double x_center=(fXksaBottom+fXksaTop)/2;
			double y_center=(fYksaLeft+fYksaRight)/2;

			m_pArrayChild[0].fXksaBottom=fXksaBottom;
			m_pArrayChild[0].fXksaTop=x_center;
			m_pArrayChild[0].fYksaLeft=fYksaLeft;
			m_pArrayChild[0].fYksaRight=y_center;

			m_pArrayChild[1].fXksaBottom=x_center;
			m_pArrayChild[1].fXksaTop=fXksaTop;
			m_pArrayChild[1].fYksaLeft=fYksaLeft;
			m_pArrayChild[1].fYksaRight=y_center;

			m_pArrayChild[2].fXksaBottom=x_center;
			m_pArrayChild[2].fXksaTop=fXksaTop;
			m_pArrayChild[2].fYksaLeft=y_center;
			m_pArrayChild[2].fYksaRight=fYksaRight;

			m_pArrayChild[3].fXksaBottom=fXksaBottom;
			m_pArrayChild[3].fXksaTop=x_center;
			m_pArrayChild[3].fYksaLeft=y_center;
			m_pArrayChild[3].fYksaRight=fYksaRight;


/*			m_pArrayChild[0].fXksaBottom=x_center;
			m_pArrayChild[0].fXksaTop=fXksaTop;
			m_pArrayChild[0].fYksaLeft=fYksaLeft;
			m_pArrayChild[0].fYksaRight=y_center;

			m_pArrayChild[1].fXksaBottom=x_center;
			m_pArrayChild[1].fXksaTop=fXksaTop;
			m_pArrayChild[1].fYksaLeft=y_center;
			m_pArrayChild[1].fYksaRight=fYksaRight;

			m_pArrayChild[2].fXksaBottom=fXksaBottom;
			m_pArrayChild[2].fXksaTop=x_center;
			m_pArrayChild[2].fYksaLeft=y_center;
			m_pArrayChild[2].fYksaRight=fYksaRight;

			m_pArrayChild[3].fXksaBottom=fXksaBottom;
			m_pArrayChild[3].fXksaTop=x_center;
			m_pArrayChild[3].fYksaLeft=fYksaLeft;
			m_pArrayChild[3].fYksaRight=y_center;*/

			m_pArrayChild[0].strMarking_1="г";
			m_pArrayChild[1].strMarking_1="д";
			m_pArrayChild[2].strMarking_1="е";
			m_pArrayChild[3].strMarking_1="ж";

			m_pArrayChild[0].iIdent_1=1;
			m_pArrayChild[1].iIdent_1=2;
			m_pArrayChild[2].iIdent_1=3;
			m_pArrayChild[3].iIdent_1=4;

			m_pArrayChild[0].DevideSquareLvl2();
			m_pArrayChild[1].DevideSquareLvl2();
			m_pArrayChild[2].DevideSquareLvl2();
			m_pArrayChild[3].DevideSquareLvl2();

			m_pArrayChild[0].SetDrawMarking_1(true, (m_pArrayChild[0].fXksaBottom+m_pArrayChild[0].fXksaTop)/2, (m_pArrayChild[0].fYksaLeft+m_pArrayChild[0].fYksaRight)/2);
			m_pArrayChild[1].SetDrawMarking_1(true, (m_pArrayChild[1].fXksaBottom+m_pArrayChild[1].fXksaTop)/2, (m_pArrayChild[1].fYksaLeft+m_pArrayChild[1].fYksaRight)/2);
			m_pArrayChild[2].SetDrawMarking_1(true, (m_pArrayChild[2].fXksaBottom+m_pArrayChild[2].fXksaTop)/2, (m_pArrayChild[2].fYksaLeft+m_pArrayChild[2].fYksaRight)/2);
			m_pArrayChild[3].SetDrawMarking_1(true, (m_pArrayChild[3].fXksaBottom+m_pArrayChild[3].fXksaTop)/2, (m_pArrayChild[3].fYksaLeft+m_pArrayChild[3].fYksaRight)/2);

			m_pArrayChild[0].bIsDrawSquare=true;
			m_pArrayChild[1].bIsDrawSquare=true;
			m_pArrayChild[2].bIsDrawSquare=true;
			m_pArrayChild[3].bIsDrawSquare=true;

			m_pArrayChild[0].iLvlSquare=1;
			m_pArrayChild[1].iLvlSquare=1;
			m_pArrayChild[2].iLvlSquare=1;
			m_pArrayChild[3].iLvlSquare=1;

			m_pArrayChild[0].m_pParent=this;
			m_pArrayChild[1].m_pParent=this;
			m_pArrayChild[2].m_pParent=this;
			m_pArrayChild[3].m_pParent=this;
		}
		void DevideSquareLvl2()
		{
			m_pArrayChild=new STR_SQUARE[4];
			iCountChild=4;

			double x_center=(fXksaBottom+fXksaTop)/2;
			double y_center=(fYksaLeft+fYksaRight)/2;

			m_pArrayChild[0].fXksaBottom=fXksaBottom;
			m_pArrayChild[0].fXksaTop=x_center;
			m_pArrayChild[0].fYksaLeft=fYksaLeft;
			m_pArrayChild[0].fYksaRight=y_center;

			m_pArrayChild[1].fXksaBottom=x_center;
			m_pArrayChild[1].fXksaTop=fXksaTop;
			m_pArrayChild[1].fYksaLeft=fYksaLeft;
			m_pArrayChild[1].fYksaRight=y_center;

			m_pArrayChild[2].fXksaBottom=x_center;
			m_pArrayChild[2].fXksaTop=fXksaTop;
			m_pArrayChild[2].fYksaLeft=y_center;
			m_pArrayChild[2].fYksaRight=fYksaRight;

			m_pArrayChild[3].fXksaBottom=fXksaBottom;
			m_pArrayChild[3].fXksaTop=x_center;
			m_pArrayChild[3].fYksaLeft=y_center;
			m_pArrayChild[3].fYksaRight=fYksaRight;

/*			m_pArrayChild[0].fXksaBottom=x_center;
			m_pArrayChild[0].fXksaTop=fXksaTop;
			m_pArrayChild[0].fYksaLeft=fYksaLeft;
			m_pArrayChild[0].fYksaRight=y_center;

			m_pArrayChild[1].fXksaBottom=x_center;
			m_pArrayChild[1].fXksaTop=fXksaTop;
			m_pArrayChild[1].fYksaLeft=y_center;
			m_pArrayChild[1].fYksaRight=fYksaRight;

			m_pArrayChild[2].fXksaBottom=fXksaBottom;
			m_pArrayChild[2].fXksaTop=x_center;
			m_pArrayChild[2].fYksaLeft=y_center;
			m_pArrayChild[2].fYksaRight=fYksaRight;

			m_pArrayChild[3].fXksaBottom=fXksaBottom;
			m_pArrayChild[3].fXksaTop=x_center;
			m_pArrayChild[3].fYksaLeft=fYksaLeft;
			m_pArrayChild[3].fYksaRight=y_center;*/

			m_pArrayChild[0].strMarking_1="1";
			m_pArrayChild[1].strMarking_1="2";
			m_pArrayChild[2].strMarking_1="3";
			m_pArrayChild[3].strMarking_1="4";

			m_pArrayChild[0].iIdent_1=1;
			m_pArrayChild[1].iIdent_1=2;
			m_pArrayChild[2].iIdent_1=3;
			m_pArrayChild[3].iIdent_1=4;

			m_pArrayChild[0].SetDrawMarking_1(true, (m_pArrayChild[0].fXksaBottom+m_pArrayChild[0].fXksaTop)/2, (m_pArrayChild[0].fYksaLeft+m_pArrayChild[0].fYksaRight)/2);
			m_pArrayChild[1].SetDrawMarking_1(true, (m_pArrayChild[1].fXksaBottom+m_pArrayChild[1].fXksaTop)/2, (m_pArrayChild[1].fYksaLeft+m_pArrayChild[1].fYksaRight)/2);
			m_pArrayChild[2].SetDrawMarking_1(true, (m_pArrayChild[2].fXksaBottom+m_pArrayChild[2].fXksaTop)/2, (m_pArrayChild[2].fYksaLeft+m_pArrayChild[2].fYksaRight)/2);
			m_pArrayChild[3].SetDrawMarking_1(true, (m_pArrayChild[3].fXksaBottom+m_pArrayChild[3].fXksaTop)/2, (m_pArrayChild[3].fYksaLeft+m_pArrayChild[3].fYksaRight)/2);

			m_pArrayChild[0].iLvlSquare=2;
			m_pArrayChild[1].iLvlSquare=2;
			m_pArrayChild[2].iLvlSquare=2;
			m_pArrayChild[3].iLvlSquare=2;

			m_pArrayChild[0].m_pParent=this;
			m_pArrayChild[1].m_pParent=this;
			m_pArrayChild[2].m_pParent=this;
			m_pArrayChild[3].m_pParent=this;
		}
		void GetPolySquareInBL(TrmKsa2Geo* ksa_to_geo, double kurs, PosBL* poly_bl)
		{
//			double PI=3.1415926535897932;

		//	Color colorBlack(m_Klight, 0, 0, 0);
		//	Pen pen(dev->factory(), colorBlack, 0.3*dev->m_mm2pt);

			PosXY posXYleft_top, posXYleft_bottom, posXYright_top, posXYright_bottom;
			
			double r=sqrt(fXksaBottom*fXksaBottom+fYksaLeft*fYksaLeft);
			double angle=atan2( fYksaLeft, fXksaBottom )+kurs;

			posXYleft_bottom.x=r*cos(angle);
			posXYleft_bottom.y=r*sin(angle);
			/////////////
			r=sqrt(fXksaBottom*fXksaBottom+fYksaRight*fYksaRight);
			angle=atan2( fYksaRight, fXksaBottom )+kurs;

			posXYright_bottom.x=r*cos(angle);
			posXYright_bottom.y=r*sin(angle);
			/////////////
			r=sqrt(fXksaTop*fXksaTop+fYksaLeft*fYksaLeft);
			angle=atan2( fYksaLeft, fXksaTop )+kurs;

			posXYleft_top.x=r*cos(angle);
			posXYleft_top.y=r*sin(angle);
			/////////////
			r=sqrt(fXksaTop*fXksaTop+fYksaRight*fYksaRight);
			angle=atan2( fYksaRight, fXksaTop )+kurs;

			posXYright_top.x=r*cos(angle);
			posXYright_top.y=r*sin(angle);
			////////////////////////////////////////////////////////////////////////
			double fBleft_top=0, fLleft_top=0;
			double fBleft_bottom=0, fLleft_bottom=0;
			double fBright_top=0, fLright_top=0;
			double fBright_bottom=0, fLright_bottom=0;

			KsaToGeo(ksa_to_geo, posXYleft_top.x,		posXYleft_top.y, 	fBleft_top,		fLleft_top);
			KsaToGeo(ksa_to_geo, posXYleft_bottom.x,	posXYleft_bottom.y,	fBleft_bottom,	fLleft_bottom);
			KsaToGeo(ksa_to_geo, posXYright_top.x,		posXYright_top.y,	fBright_top,	fLright_top);
			KsaToGeo(ksa_to_geo, posXYright_bottom.x,	posXYright_bottom.y,fBright_bottom, fLright_bottom);

			poly_bl[0].b=fBleft_top;
			poly_bl[0].l=fLleft_top;

			poly_bl[1].b=fBright_top;
			poly_bl[1].l=fLright_top;

			poly_bl[2].b=fBright_bottom;
			poly_bl[2].l=fLright_bottom;

			poly_bl[3].b=fBleft_bottom;
			poly_bl[3].l=fLleft_bottom;
		}
		void DrawSquare(GisLayer* gisLayer, GdiGis::ImageDevice* dev, GdiGis::Pen* pen_0, GdiGis::Pen* pen_1, GdiGis::Pen* pen_2, 
						GdiGis::SolidBrush* brush, TrmKsa2Geo* ksa_to_geo, double kurs, double ray_angle,
						GisFontExt font_lvl0, GisFontExt font_lvl1, GisFontExt font_lvl2, GisFontExt font_main)
		{
//			double PI=3.1415926535897932;

		//	Color colorBlack(m_Klight, 0, 0, 0);
		//	Pen pen(dev->factory(), colorBlack, 0.3*dev->m_mm2pt);

			PosXY posXYleft_top, posXYleft_bottom, posXYright_top, posXYright_bottom;
			
			double r=sqrt(fXksaBottom*fXksaBottom+fYksaLeft*fYksaLeft);
			double angle=atan2( fYksaLeft, fXksaBottom )+kurs;

			posXYleft_bottom.x=r*cos(angle);
			posXYleft_bottom.y=r*sin(angle);
			/////////////
			r=sqrt(fXksaBottom*fXksaBottom+fYksaRight*fYksaRight);
			angle=atan2( fYksaRight, fXksaBottom )+kurs;

			posXYright_bottom.x=r*cos(angle);
			posXYright_bottom.y=r*sin(angle);
			/////////////
			r=sqrt(fXksaTop*fXksaTop+fYksaLeft*fYksaLeft);
			angle=atan2( fYksaLeft, fXksaTop )+kurs;

			posXYleft_top.x=r*cos(angle);
			posXYleft_top.y=r*sin(angle);
			/////////////
			r=sqrt(fXksaTop*fXksaTop+fYksaRight*fYksaRight);
			angle=atan2( fYksaRight, fXksaTop )+kurs;

			posXYright_top.x=r*cos(angle);
			posXYright_top.y=r*sin(angle);
			////////////////////////////////////////////////////////////////////////
			double fBleft_top=0, fLleft_top=0;
			double fBleft_bottom=0, fLleft_bottom=0;
			double fBright_top=0, fLright_top=0;
			double fBright_bottom=0, fLright_bottom=0;

			KsaToGeo(ksa_to_geo, posXYleft_top.x,		posXYleft_top.y, 	fBleft_top,		fLleft_top);
			KsaToGeo(ksa_to_geo, posXYleft_bottom.x,	posXYleft_bottom.y,	fBleft_bottom,	fLleft_bottom);
			KsaToGeo(ksa_to_geo, posXYright_top.x,		posXYright_top.y,	fBright_top,	fLright_top);
			KsaToGeo(ksa_to_geo, posXYright_bottom.x,	posXYright_bottom.y,fBright_bottom, fLright_bottom);

			polyKsaLastDraw[0]=posXYleft_top;
			polyKsaLastDraw[1]=posXYleft_bottom;
			polyKsaLastDraw[2]=posXYright_bottom;
			polyKsaLastDraw[3]=posXYright_top;
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

			if(bIsDrawFill && (iLvlSquare!=0 || bIsDrawSquare)) dev->FillPolygon(brush, polyTemp, 4);

			if(bIsDrawSquare && iLvlSquare==0) dev->DrawPolygon(pen_0, polyTemp, 4);
			else if(bIsDrawSquare && iLvlSquare==1) dev->DrawPolygon(pen_1, polyTemp, 4);
			else if(bIsDrawSquare && iLvlSquare==2) dev->DrawPolygon(pen_2, polyTemp, 4);
			//////////////////////////////////////////////
			if(m_pArrayChild && (iLvlSquare!=0 || bIsDrawSquare))
			{
				m_pArrayChild[0].DrawSquare(gisLayer, dev, pen_0, pen_1, pen_2, brush, ksa_to_geo, kurs, ray_angle, font_lvl0, font_lvl1, font_lvl2, font_main);
				m_pArrayChild[1].DrawSquare(gisLayer, dev, pen_0, pen_1, pen_2, brush, ksa_to_geo, kurs, ray_angle, font_lvl0, font_lvl1, font_lvl2, font_main);
				m_pArrayChild[2].DrawSquare(gisLayer, dev, pen_0, pen_1, pen_2, brush, ksa_to_geo, kurs, ray_angle, font_lvl0, font_lvl1, font_lvl2, font_main);
				m_pArrayChild[3].DrawSquare(gisLayer, dev, pen_0, pen_1, pen_2, brush, ksa_to_geo, kurs, ray_angle, font_lvl0, font_lvl1, font_lvl2, font_main);
			}

			///////////////////////////////////////////////////////
			double size_square_wnd=(pWndleft_top.x-pWndright_top.x)*(pWndleft_top.x-pWndright_top.x)+
						(pWndright_top.y-pWndleft_top.y)*(pWndright_top.y-pWndleft_top.y);
			size_square_wnd=sqrt(size_square_wnd);
			size_square_wnd=dev->pt2mm((float)size_square_wnd)/2;

			double size_square_wnd_copy=size_square_wnd;

			if(iLvlSquare==0) size_square_wnd=font_main.m_font.fontHeight/20.0*size_square_wnd;
			else if(iLvlSquare==1) size_square_wnd=font_lvl1.m_font.fontHeight/10.0*size_square_wnd;
			else if(iLvlSquare==2) size_square_wnd=font_lvl2.m_font.fontHeight/5.0*size_square_wnd;

			GisFontExt font_cur;
			if(iLvlSquare==0) font_cur=font_main;
			else if(iLvlSquare==1) font_cur=font_lvl1;
			else if(iLvlSquare==2) font_cur=font_lvl2;

			if(bDrawMarkingMain || iLvlSquare!=0)
			{
				if(bDrawMarking_1)
					DrawText(gisLayer, dev, ray_angle, size_square_wnd, fXmarking_1, fYmarking_1, strMarking_1, ksa_to_geo, kurs, font_cur);

				if(bDrawMarking_2)
					DrawText(gisLayer, dev, ray_angle, size_square_wnd, fXmarking_2, fYmarking_2, strMarking_2, ksa_to_geo, kurs, font_cur);
			}

			if(iLvlSquare==0 && bDrawMarkingLvl0 && bIsDrawSquare)
			{
				size_square_wnd=size_square_wnd_copy;
				size_square_wnd=font_lvl0.m_font.fontHeight/20.0*size_square_wnd;
				DrawText(gisLayer, dev, ray_angle, size_square_wnd, fXmarkingLvl0, fYmarkingLvl0, strMarking_1+strMarking_2, ksa_to_geo, kurs, font_lvl0);
			}
		}

		void DrawText(GisLayer* gisLayer, GdiGis::ImageDevice *dev, double ray_polar, double size_mm, 
					  double x_center, double y_center, std::string str_text, TrmKsa2Geo* ksa_to_geo, double kurs, GisFontExt font_text)
		{
//			double PI=3.1415926535897932;

			GdiGis::Color colorBlack(255, 0, 0, 0);
			GdiGis::SolidBrush brush_black(dev->factory(), font_text.m_font.fontColor);//colorBlack);

			double r=sqrt(x_center*x_center+y_center*y_center);
			double angle=atan2( y_center, x_center )+kurs;

			x_center=r*cos(angle);
			y_center=r*sin(angle);

			double fBcenter=0, fLcenter=0;
			KsaToGeo(ksa_to_geo, x_center, y_center, fBcenter, fLcenter);

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
			////////////////////////////////////////////////////
/*			Matrix matrix(dev->factory()), oldMatrix(dev->factory());
			dev->GetTransform(&oldMatrix);

			double cosA = cos(ray_polar);
			double sinA = sin(ray_polar);

			matrix.SetElements( (REAL)cosA, (REAL)sinA, -(REAL)sinA, (REAL)cosA,
								(REAL)(pointText.X*(1-cosA) + pointText.Y*sinA),
								(REAL)(pointText.Y*(1-cosA) - pointText.X*sinA));

			dev->SetTransform(&matrix);*/
			////////////////////////////////////////////////////
			if(str_text.size()>0)
			{
				GdiGis::PointF pointText;
				pointText.X=pWndcenter.x;
				pointText.Y=pWndcenter.y;

//				GdiGis::Font* font = dev->buildFont(font_text.m_font.fontSet, float(font_text.m_font.fontHeight/2 * scale), GdiGis::FontStyleBold);
				font_text.m_font.fontHeight=size_mm;
				GdiGis::Font* font = dev->buildFont(font_text, (float)scale);

				WCHAR* bstr = NULL;
				cct_char_to_wchar(str_text.c_str(), &bstr);
	//			BSTR pBSTRname = str_text.AllocSysString();
				int len = wcslen((wchar_t*)bstr);

				dev->DrawString(bstr, len, font, pointText, &stringFormat, &brush_black);

	//			if(bstr) SysFreeString(bstr);
				if(bstr) cct_free_string(bstr);
				if(font) delete font;
			}

//			dev->SetTransform(&oldMatrix);
		}

		void KsaToGeo(TrmKsa2Geo* ksa_to_geo, double x, double y, double& b, double& l)
		{
			if(!ksa_to_geo) return;

			PosBL bl;
			PosXY xy;

			xy.x=x;
			xy.y=y;
			trm_transform(ksa_to_geo, &xy, &bl, 1);
			b=bl.b;
			l=bl.l;
		}

//		Pen pPenDraw;

		double fXksaBottom;//в системе КСА с центром в getPosition()
		double fXksaTop;//в системе КСА с центром в getPosition()
		double fYksaLeft;//в системе КСА с центром в getPosition()
		double fYksaRight;//в системе КСА с центром в getPosition()

		STR_SQUARE* m_pParent;
		STR_SQUARE* m_pArrayChild;
		long iCountChild;

		bool bDrawMarkingMain;

		bool bDrawMarking_1;

		std::string strMarking_1;
		double fXmarking_1;
		double fYmarking_1;
		long iIdent_1;

		bool bDrawMarking_2;

		std::string strMarking_2;
		double fXmarking_2;
		double fYmarking_2;
		long iIdent_2;

		bool bDrawMarkingLvl0;
		double fXmarkingLvl0;
		double fYmarkingLvl0;

		bool bIsDrawSquare;
		short iLvlSquare;//0-1-2
		bool bIsDrawFill;
//		bool bIsDrawText;

		PosXY polyKsaLastDraw[4];

	};
	STR_SQUARE* m_pSquare;
	long iCountSquare;

	struct STR_IDENT_LETTER
	{
		STR_IDENT_LETTER()
		{
			iIdentLetter	= 0;
		}
		STR_IDENT_LETTER(std::string str_letter, long ident_letter)
		{
			strMarkingLetter= str_letter;
			iIdentLetter	= ident_letter;
		}
		std::string strMarkingLetter;
		long iIdentLetter;
	};
	std::vector<STR_IDENT_LETTER> vectorLetterFromLeft;

/*	struct STR_LINE_SQUARE
	{
		STR_SQUARE* m_pSquare;
		long iCountColumns;
	}
	STR_LINE_SQUARE* m_pLineSquare;
	long iCountLines;*/

	void GetVectorSelectedSquare(std::vector<STR_SQUARE*>& vector_square, double b, double l);
	void CopyParamDrawFill(STR_SQUARE* square, long count);

	////////////////////////////////////////////
	//CSquarePolyHandler
	class CSquarePolyHandler : public TknMapScaleBase::MouseHandler
	{
	public:
		CSquarePolyHandler(STR_SQUARE* square)
		{
			m_pSquare = square;
		}
		virtual ~CSquarePolyHandler(){}

		/** обработка мыши */
		virtual bool operator()(const Mouse& me, ScaleSensor& sensor,
			GisLayer *layer, TknMapScaleBase* obj){return true;}
		virtual bool isDo(const Mouse& me){return true;}

	//	virtual bool hasPoint(const PointF& position, GisLayer *layer,
	//						  const std::vector<PosXY> &map_points); 

	public:
		STR_SQUARE* m_pSquare; 

	};
	std::vector<CSquarePolyHandler*> vectorHandler;
};

/////////////////////////////////////////////////
//TknPaletkaPlugin

class TknPaletkaPlugin : public TokenPlugin
{
public:
	TknPaletkaPlugin() : TokenPlugin()
	{
		m_keys.push_back("TknPaletka");
	}
	virtual ~TknPaletkaPlugin() 
	{
	
	}
	virtual TknMapBase* create ( const std::string &key ) const
	{
		TknMapBase *ptToken = NULL;

		if(key.compare("TknPaletka") == 0)
			ptToken = new TknPaletka();
		
		return ptToken;
	}
	virtual TknMapBase* clone ( const TknMapBase *pSrcTkn ) const
	{ 
/*		TknMapBase *ptToken = NULL;
		std::string key = pSrcTkn->className();

		if(key.compare("TknIron") == 0)
		{
			ptToken = new TknIron();
			((TknIron*)pSrcTkn)->clone(ptToken)
		}

		return ptToken;*/
		return NULL;
	}

};