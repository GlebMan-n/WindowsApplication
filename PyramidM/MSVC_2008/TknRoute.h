#include "stdafx.h"
#include <QtGui>
#include <QtDebug>
#include <gis_tool_qt/GisInterfaceQT/QToolBarMap.h>
#include <gis_tool_qt/GisInterfaceQT/QToolBarScale.h>
#include <gis_tool_qt/GisInterfaceQT/QToolBarRegion.h>
#include <gis_tool_qt/GisInterfaceQT/QToolBarRoseWind.h>
#include <gis_tool_qt/GisInterfaceQT/MapStatusBar.h>
#include <gis_tool_qt/GisInterfaceQT/PropertyDlg.h>
#include <gis_tool_qt/GisInterfaceQT/MapBaseWidget.h>
#include <gis_tool_qt/GisInterfaceQT/GisMapQtInterface.h>
#include <gis_tool_qt/GisWidgets/GisAccessDlg.h>
#include <gis/GrUnits/TknMapNonScale.h>
#include <gis/GrUnits/TknTerminator.h>
#include "gis/GisDataInterface/ControlSelector.h"
#include <gis/GrUnits/StoreVectorData.h>
#include <gis/GisBase/cgis/str.h>
#include <gis/GisBase/cppgis/beepBase.h>
#include <gis/GisBase/cppgis/unm.h>
#include <gis/GisBase/cppgis/UNNRegion.h>
#include <gis_tool_qt/GisInterfaceQT/GisFindTopoData.h>
#include <gis_tool_qt/GisInterfaceQT/MapBaseWidget.h>
#include <gis_tool_qt/GisInterfaceQT/EditPosBLWgt.h>
#include <gis_tool_qt/GisInterfaceQT/MapScaleDlg.h>
#include <gis_tool_qt/GisInterfaceQT/LayerWidget.h>
#include <gis/GisBase/cppgis/WndData.h>
#include <gis_tool_qt/GisInterfaceQT/QComboBoxScale.h>
#include <gis_tool_qt/GisInterfaceQT/LayerDlg.h>
#include <gis_tool_qt/GisInterfaceQT/MapBaseWidget.h>
#include <gis_tool_qt/PropertyBrowserQT/qtpropertybrowser.h>
#include "gis_tool_qt/GisInterfaceQT/MovePointDlg.h"
#include <gis/GisBase/cgis/pos.h>
#include <gis_tool_qt/GisInterfaceQT/ui/ui_MovePointDlg.h>
#include <gis/GisBase/cppgis/PellicleData.h>
#include <gis_tool_qt/GisInterfaceQT/MapParamDlg.h>
#include <gis/GrUnits/TknWnd.h>
#include <gis_tool_qt/GisInterfaceQT/typeOpenFromFile.h>
#include <gis/GisBase/cppgis/TypeGisToken.h>
#include  "gis\GisBase\cppgis\TknBase.h"
#include "time.h"

class TknRoute;
class TknNonScale;
class GisAccess;
class MapBaseWidget;
class Magnifier;
class MapStatusBar;
class ToolBarMap;
class TknTerminator;
class TknSimpleLines;
class TknBarrier;
class SelectInterface;
class TypePolygonTemplate;
class TokensClassification;
class LassoTool;
class GisFindTopoData;
class QWidget;
class EditPosBLWgt;
class MapScaleDlg;
class LayerWidget;
class QToolBarScale;
class ToolBarMap;
class ToolBarRegion;
class GridBLData;
class LayerDlg;
class MapBaseWidget;
class QtProperty;
class MovePointDlg;
class PellicleData;
class MapParamDlg;
class LineGetInterface;
class PointGetInterface;
class TknRoseWind;
class TknPhoto;
class ToolBarRoseWind;
class TypeGisToken;
class TknNonScale;

class TknRoute : public TknMapNonScaleBase  
{
public: 
  /** Конструктор */
	TknRoute();//
	/** Отображение геопространственного объекта системой визуализации */
	
	/** запись знака в выходной поток в двоичном виде
     @return успешно ли выполнена запись
	*/
	virtual bool saveToBlob(std::ostream &mem) const;

	/** чтение знака из двоичного входного потока
     @return успешно ли выполнено чтение
	*/
	virtual bool loadFromBlob(std::istream &mem);

	void ClearTkn();//очистить слой
	void AddPoint(double b, double l, short hour, short minute, short second);//добавляем точки
	void AddPoint(double b, double l, short hour, short minute, short second, short day, short month, short year, short altitude, short speed);
	bool CopyFromTkn(TknRoute* tkn_from);
	
	void SetWline(double w_line);
	void SetRadius(double r_point);
	void SetColorLine(COLORREF color_line);
	void SetNameTkn(std::string str_name);
	void SetNoteTkn(std::string str_note);
	void SetTimeBegin(__int64 timeBegin);

	void SetHiddenText(bool hide);
	void SetHiddenTime(bool hidden);
	void SetHiddenDate(bool hidden);
	void SetHiddenNumber(bool hidden);
	void SetHiddenAltitude(bool hidden);
	void SetHiddenSpeed(bool hidden);	

	std::string GetNameTkn();
	char* GetCharNameTkn();
	GdiGis::Color GetColor();
	std::string  GetNoteTkn();
	__int64 GetTimeBegin();
	double GetWLine();
	double GetFRpoint();
	unsigned char GetVersion();

	bool isHiddenText();
	bool isHiddenTime();
	bool isHiddenDate();
	bool isHiddenNumber();
	bool isHiddenAltitude();
	bool isHiddenSpeed();

private:
	std::string GetStrTimeForIndPoint(long ind);
	std::string GetStrDateForIndPoint(long ind);
	std::string GetStrAltitudeForIndPoint(long ind);
	std::string GetStrSpeedForIndPoint(long ind);
	bool loadFromBlob_1(std::istream &mem);
	bool loadFromBlob_2(std::istream &mem);
	virtual CctError* draw(GisLayer *gisLayer, GdiGis::ImageDevice *dev);//
	void BLtoWnd(GisLayer* gisLayer, double b, double l, double& x, double& y);//

private:
	unsigned char iVersion;

	double fWline;
	GdiGis::Color pColorLine;
	double fRpoint;

	__int64 iTimeBegin;

	char strNameTkn[51];
	char strNoteTkn[256];

	bool _showText;
	bool _showDate;
	bool _showTime;
	bool _showAltitude;
	bool _showNumber;
	bool _showSpeed;

public:
	struct STR_POINT_ROUTE
	{
		STR_POINT_ROUTE()
		{
			fB			= 0;
			fL			= 0;

			iHour		= 0;
			iMinute		= 0;
			iSecond		= 0;
			iDay		= 1;
			iMonth		= 1;
			iYear		= 2013;
			iAltitude	= 0;
			iSpeed		= 0;
		}

		double fB;
		double fL;
		short iHour;
		short iMinute;
		short iSecond;
		short iDay;
		short iMonth;
		short iYear;
		short iAltitude;
		short iSpeed;

	};
	std::vector<STR_POINT_ROUTE> vectorPoints;
	struct STR_ZAG_KALKA
	{
		STR_ZAG_KALKA()
		{
			iByte1			= 245;
			iVersion		= 2;
			iIsForPrint		= 0;

			iTypeSyst		= 0;
			iEllpsType		= 0; //5+3
			
			fBparam			= 0;//16
			
			fLparam			= 0;
			fBparam2		= 0;
			fLparam2		= 0;//40
			
			strReserve[0]	= '\0';//40 + 18 = 58 (округляется до большего кратного 8) = 64
			
			iByte2			= 10; //58 + 1 = 59 округлено до 64 
			
		}
	unsigned char iByte1;//байт проверки на корректность файла
	unsigned char iVersion;

	unsigned char iIsForPrint;//0-not print;1-for print;

	unsigned char iTypeSyst;
	unsigned char iEllpsType;
	
	double fBparam;
	double fLparam;
	double fBparam2;
	double fLparam2;

	
	char strReserve[18];
	
	unsigned char iByte2; //байт проверки на корректность файла
	
	};


};