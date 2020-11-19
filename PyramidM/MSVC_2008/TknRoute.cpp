#include "stdafx.h"
#include <MSVC_2008\TknRoute.h>
#include <sstream>
//#include <vld/vld.h>

//////////////////////////////////////////////////////////////////////МАРШРУТ ТРЕК
//////////////////////////////////////////////////////////////////////
// TknRoute
TknRoute::TknRoute() 
: TknMapNonScaleBase()
{
	m_className		= "TknRoute";
	iVersion		= 2;
	fWline			= 1;
	pColorLine		= GdiGis::Color(255, 0, 0, 0);
	fRpoint			= 2;
	time_t time_temp;
	time(&time_temp);
	iTimeBegin		= time_temp;
	strNameTkn[0]	= '\0';
	strNoteTkn[0]	= '\0';
	_showText = true;
	_showDate = true;
	_showTime = true;
	_showAltitude = true;
	_showNumber = true;
	_showSpeed = true;

}

/** Отображение геопространственного объекта системой визуализации */
CctError* TknRoute::draw(GisLayer* gisLayer, GdiGis::ImageDevice* dev)
{
	double w_line_pt=fWline*dev->m_mm2pt;
	double r_pt=fRpoint*dev->m_mm2pt;
	double delta_y=(fRpoint+2.5)*dev->m_mm2pt;
	double x0=0, y0=0, x1=0, y1=0;

	GdiGis::Pen pen(dev->factory(), pColorLine, w_line_pt);
	GdiGis::SolidBrush brush(dev->factory(), pColorLine);

	GdiGis::Font* font = dev->buildFont(gis_font_arial, 3, GdiGis::FontStyleBold);
	GdiGis::StringFormat stringFormat(dev->factory());
	stringFormat.SetAlignment(GdiGis::StringAlignmentCenter);
	stringFormat.SetLineAlignment(GdiGis::StringAlignmentCenter);

	if(!vectorPoints.size()) 
		return NULL;		

	for(unsigned long j=0;j<vectorPoints.size();j++)
	{
		if(j>0)
		{
			BLtoWnd(gisLayer, vectorPoints[j].fB, vectorPoints[j].fL, x1, y1);
			dev->DrawLine(&pen, (float)x0, (float)y0, (float)x1, (float)y1);
			x0=x1;
			y0=y1;
		}
		else
			BLtoWnd(gisLayer, vectorPoints[0].fB, vectorPoints[0].fL, x0, y0);
		
		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		GdiGis::PointF pointText;
		pointText.X=x0;
		pointText.Y=y0-delta_y;

		GdiGis::PointF pointAltitude;
		pointAltitude.X=x0;
		pointAltitude.Y=y0+delta_y;

		std::string str_text = "";
		std::string str_altitude = "";

		if(_showTime)
			str_text += " " + GetStrTimeForIndPoint(j);

		if(_showDate)
			str_text += " " + GetStrDateForIndPoint(j);

		if(_showNumber)
		{
			std::stringstream strstream;
			strstream << std::string(" №") << j+1;//
			strstream >> str_altitude;
		}

		if(_showAltitude)
			str_altitude += " h=" + GetStrAltitudeForIndPoint(j);

		if(_showSpeed)
			str_altitude += " s=" + GetStrSpeedForIndPoint(j);

		WCHAR* bstr = NULL;
		WCHAR* bstr_altitude = NULL;
		cct_char_to_wchar(str_text.c_str(), &bstr);
		cct_char_to_wchar(str_altitude.c_str(), &bstr_altitude);

		int len = wcslen((wchar_t*)bstr);
		int len_altitude = wcslen((wchar_t*)bstr_altitude);

		if(_showText)
		{
			dev->DrawString(bstr, len, font, pointText, &stringFormat, &brush);
			dev->DrawString(bstr_altitude, len_altitude, font, pointAltitude, &stringFormat, &brush);
		}
		
		if(bstr) 
			cct_free_string(bstr);
		if(bstr_altitude) 
			cct_free_string(bstr_altitude);

		dev->FillEllipse(&brush, (float)(x0 - w_line_pt*0.6), (float)(y0 - w_line_pt*0.6), (float)(1.2*w_line_pt), (float)(1.2*w_line_pt));
		dev->DrawEllipse(&pen, (float)(x0 - r_pt), (float)(y0 - r_pt), (float)(2*r_pt), (float)(2*r_pt));
	}

	if(font) delete font;

	return NULL;
} 
void TknRoute::SetHiddenText(bool hide)
{
	_showText = !hide;
}
//время
std::string TknRoute::GetStrTimeForIndPoint(long ind)
{
	std::string strRet="00:00";

	char strTemp[20];

	if(vectorPoints[ind].iMinute>9) sprintf(strTemp, "%d:%d", vectorPoints[ind].iHour, vectorPoints[ind].iMinute);
		else sprintf(strTemp, "%d:0%d", vectorPoints[ind].iHour, vectorPoints[ind].iMinute);

	strTemp[19]='\0';
	strRet=strTemp;

	return strRet;
}

std::string TknRoute::GetStrDateForIndPoint(long ind)
{
	std::string strRet="01.01.2013";

	char strTemp[20];


	sprintf(strTemp, "%d.%d.%d", vectorPoints[ind].iDay, vectorPoints[ind].iMonth, vectorPoints[ind].iYear);
		

	strTemp[19]='\0';
	strRet=strTemp;

	return strRet;
}

std::string TknRoute::GetStrAltitudeForIndPoint(long ind)
{
	std::string strRet="1";

	char strTemp[20];


	sprintf(strTemp, "%d", vectorPoints[ind].iAltitude);


	strTemp[19]='\0';
	strRet=strTemp;

	return strRet;
}

std::string TknRoute::GetStrSpeedForIndPoint(long ind)
{
	std::string strRet="1";

	char strTemp[20];


	sprintf(strTemp, "%d", vectorPoints[ind].iSpeed);


	strTemp[19]='\0';
	strRet=strTemp;

	return strRet;
}

//переводим координаты в формат карты
void TknRoute::BLtoWnd(GisLayer* gisLayer, double b, double l, double& x, double& y)
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

//сохраняем в поток
bool TknRoute::saveToBlob(std::ostream &mem) const
{

	mem.write((char*)&iVersion, sizeof(iVersion));

	mem.write((char*)&strNameTkn, 51);
	mem.write((char*)&strNoteTkn, 256);

	mem.write((char*)&fWline, sizeof(fWline));
	mem.write((char*)&fRpoint, sizeof(fRpoint));
	mem.write((char*)&iTimeBegin, sizeof(iTimeBegin));


	BYTE alfa_color=pColorLine.GetAlpha();
	BYTE red_color=pColorLine.GetRed();
	BYTE grean_color=pColorLine.GetGreen();
	BYTE blue_color=pColorLine.GetBlue();

	mem.write((char*)&alfa_color, sizeof(alfa_color));
	mem.write((char*)&red_color, sizeof(red_color));
	mem.write((char*)&grean_color, sizeof(grean_color));
	mem.write((char*)&blue_color, sizeof(blue_color));

	long size=vectorPoints.size();
	mem.write((char*)&size, sizeof(size));

	for(long j=0;j<size;j++)
	{
		STR_POINT_ROUTE pSTR=vectorPoints[j];
		mem.write((char*)&(pSTR.fB), sizeof(pSTR.fB));
		mem.write((char*)&(pSTR.fL), sizeof(pSTR.fL));
		mem.write((char*)&(pSTR.iHour), sizeof(pSTR.iHour));
		mem.write((char*)&(pSTR.iMinute), sizeof(pSTR.iMinute));
		mem.write((char*)&(pSTR.iSecond), sizeof(pSTR.iSecond));
		if(iVersion > 1)
		{
			mem.write((char*)&(pSTR.iDay), sizeof(pSTR.iDay));
			mem.write((char*)&(pSTR.iMonth), sizeof(pSTR.iMonth));
			mem.write((char*)&(pSTR.iYear), sizeof(pSTR.iYear));
			mem.write((char*)&(pSTR.iAltitude), sizeof(pSTR.iAltitude));
			mem.write((char*)&(pSTR.iSpeed), sizeof(pSTR.iSpeed));
		}
	}	

	return true;
}

bool TknRoute::loadFromBlob(std::istream &mem)
{
	unsigned char version;
	mem.read((char*)&version, sizeof(version));

	bool flag = true;

	switch(version)
	{
		case 1:
			SetHiddenDate(true);
			SetHiddenAltitude(true);
			SetHiddenNumber(true);
			SetHiddenSpeed(true);
 			flag = loadFromBlob_1(mem);
			break;
		case 2:
			SetHiddenDate(false);
			SetHiddenAltitude(false);
			SetHiddenNumber(false);
			SetHiddenSpeed(false);
 			flag = loadFromBlob_2(mem);
			break;
		default: flag = false;
	}
	return flag;
}


//загружаем из потока
bool TknRoute::loadFromBlob_1(std::istream &mem)
{
	//читаем заголовок

	//очистили вектор
	ClearTkn();
	//крайний символ  \0 имя
	mem.read((char*)&strNameTkn,  51);
	strNameTkn[50]='\0';

	//крайний символ  \0 заметка
	mem.read((char*)&strNoteTkn,  256);
	strNoteTkn[255]='\0';
	//толщина
	mem.read((char*)&fWline,  sizeof(fWline));
	//радиус
	mem.read((char*)&fRpoint,  sizeof(fRpoint));
	//время начала
	mem.read((char*)&iTimeBegin,  sizeof(iTimeBegin));
	//цвет
	BYTE alfa_color=0, red_color=0, grean_color=0, blue_color=0;

	mem.read((char*)&alfa_color,  sizeof(alfa_color));
	mem.read((char*)&red_color,  sizeof(red_color));
	mem.read((char*)&grean_color,  sizeof(grean_color));
	mem.read((char*)&blue_color,  sizeof(blue_color));

	pColorLine=GdiGis::Color(alfa_color, red_color, grean_color, blue_color);
	//длинна
	long size=0;
	mem.read((char*)&size,  sizeof(size));
	//читаем массив координат
	for(long j=0;j<size;j++)
	{
		STR_POINT_ROUTE pSTR;
		mem.read((char*)&(pSTR.fB), sizeof(pSTR.fB));
		mem.read((char*)&(pSTR.fL), sizeof(pSTR.fL));
		mem.read((char*)&(pSTR.iHour), sizeof(pSTR.iHour));
		mem.read((char*)&(pSTR.iMinute), sizeof(pSTR.iMinute));
		mem.read((char*)&(pSTR.iSecond), sizeof(pSTR.iSecond));
		vectorPoints.push_back(pSTR);
	}
	return true;
}

//загружаем из потока
bool TknRoute::loadFromBlob_2(std::istream &mem)
{
	//читаем заголовок

	//очистили вектор
	ClearTkn();
	//крайний символ  \0 имя
	mem.read((char*)&strNameTkn,  51);
	strNameTkn[50]='\0';

	//крайний символ  \0 заметка
	mem.read((char*)&strNoteTkn,  256);
	strNoteTkn[255]='\0';
	//толщина
	mem.read((char*)&fWline,  sizeof(fWline));
	//радиус
	mem.read((char*)&fRpoint,  sizeof(fRpoint));
	//время начала
	mem.read((char*)&iTimeBegin,  sizeof(iTimeBegin));
	//цвет
	BYTE alfa_color=0, red_color=0, grean_color=0, blue_color=0;

	mem.read((char*)&alfa_color,  sizeof(alfa_color));
	mem.read((char*)&red_color,  sizeof(red_color));
	mem.read((char*)&grean_color,  sizeof(grean_color));
	mem.read((char*)&blue_color,  sizeof(blue_color));

	pColorLine=GdiGis::Color(alfa_color, red_color, grean_color, blue_color);
	//длинна
	long size=0;
	mem.read((char*)&size,  sizeof(size));
	//читаем массив координат
	for(long j=0;j<size;j++)
	{
		STR_POINT_ROUTE pSTR;
		mem.read((char*)&(pSTR.fB), sizeof(pSTR.fB));
		mem.read((char*)&(pSTR.fL), sizeof(pSTR.fL));
		mem.read((char*)&(pSTR.iHour), sizeof(pSTR.iHour));
		mem.read((char*)&(pSTR.iMinute), sizeof(pSTR.iMinute));
		mem.read((char*)&(pSTR.iSecond), sizeof(pSTR.iSecond));
		mem.read((char*)&(pSTR.iDay), sizeof(pSTR.iDay));
		mem.read((char*)&(pSTR.iMonth), sizeof(pSTR.iMonth));
		mem.read((char*)&(pSTR.iYear), sizeof(pSTR.iYear));
		mem.read((char*)&(pSTR.iAltitude), sizeof(pSTR.iAltitude));
		mem.read((char*)&(pSTR.iSpeed), sizeof(pSTR.iSpeed));
		vectorPoints.push_back(pSTR);
	}
	return true;
}

//устанавливаем имя для слоя(?)
void TknRoute::SetNameTkn(std::string str_name)
{
	int len=str_name.size();
	if(len>50) str_name.resize(50);
	sprintf(strNameTkn, "%s", str_name.c_str());
}

//Делаем пометку
void TknRoute::SetNoteTkn(std::string str_note)
{
	int len=str_note.size();
	if(len>255) str_note.resize(255);
	sprintf(strNoteTkn, "%s", str_note.c_str());
}

//очищаем вектор
void TknRoute::ClearTkn()
{
	vectorPoints.clear();
}

//добавляем точку в вектор
void TknRoute::AddPoint(double b, double l, short hour, short minute, short second)
{
	iVersion = 1;
	SetHiddenDate(true);
	SetHiddenAltitude(true);
	SetHiddenNumber(true);
	STR_POINT_ROUTE pSTR;
	pSTR.fB=b;
	pSTR.fL=l;
	pSTR.iHour=hour;
	pSTR.iMinute=minute;
	pSTR.iSecond=second;
	vectorPoints.push_back(pSTR);
}

void TknRoute::AddPoint(double b, double l, short hour, 
			  short minute, short second, 
			  short day, short month, short year, short altitude, short speed)
{
	iVersion = 2;
	STR_POINT_ROUTE pSTR;
	pSTR.fB = b;
	pSTR.fL = l;
	pSTR.iHour = hour;
	pSTR.iMinute = minute;
	pSTR.iSecond = second;
	pSTR.iDay = day;
	pSTR.iMonth = month;
	pSTR.iYear = year;
	pSTR.iAltitude = altitude;
	pSTR.iSpeed = speed;
	vectorPoints.push_back(pSTR);
}

//устанавливаем толщину линии
void TknRoute::SetWline(double w_line)
{
	fWline=w_line;
	if(fWline<0.01) fWline=0.01;
	if(fWline>100) fWline=100;
}

//устанавливаем радиус точки
void TknRoute::SetRadius(double r_point)
{
	fRpoint=r_point;
	if(fRpoint<0.01) fRpoint=0.01;
	if(fRpoint>100) fRpoint=100;
}

//устанавливаем цвет линии
void TknRoute::SetColorLine(COLORREF color_line)
{
	pColorLine.SetFromCOLORREF(color_line);
//	pColorLine=GetRedValue(rgb), GetGreenValue(rgb), GetBlueValue(rgb)
}

//копируем маршрут из карты в файл
bool TknRoute::CopyFromTkn(TknRoute* tkn_from)
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

void TknRoute::SetHiddenTime(bool hidden)
{
	_showTime = !hidden;
}
void TknRoute::SetHiddenDate(bool hidden)
{
	_showDate = !hidden;
}
void TknRoute::SetHiddenAltitude(bool hidden)
{
	_showAltitude = !hidden;
}

void TknRoute::SetHiddenNumber(bool hidden)
{
	_showNumber = !hidden;
}

void TknRoute::SetHiddenSpeed(bool hidden)
{
	_showSpeed = !hidden;
}

std::string TknRoute::GetNameTkn()
{
	return std::string(strNameTkn);
}

char* TknRoute::GetCharNameTkn()
{
	return strNameTkn;
}

std::string  TknRoute::GetNoteTkn()
{
	return std::string(strNoteTkn);
}

__int64 TknRoute::GetTimeBegin()
{
	return iTimeBegin;
}

void TknRoute::SetTimeBegin(__int64 timeBegin)
{
	iTimeBegin = timeBegin;
}

double TknRoute::GetWLine()
{
	return fWline;
}

double TknRoute::GetFRpoint()
{
	return fRpoint;
}

GdiGis::Color TknRoute::GetColor()
{
	return pColorLine;
}

bool TknRoute::isHiddenText()
{
	return _showText;
}

bool TknRoute::isHiddenTime()
{
	return _showTime;
}

bool TknRoute::isHiddenDate()
{
	return _showDate;
}

bool TknRoute::isHiddenNumber()
{
	return _showNumber;
}

bool TknRoute::isHiddenAltitude()
{
	return _showAltitude;
}

bool TknRoute::isHiddenSpeed()
{
	return _showSpeed;
}

unsigned char TknRoute::GetVersion()
{
	return iVersion;
}