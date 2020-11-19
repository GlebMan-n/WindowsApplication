#include "stdafx.h"
#include <PyramidM\MSVC_2008\NaviClass.h>	
#include <PyramidM\MSVC_2008\CalcFunctions.h>
#include <PyramidM\MSVC_2008\TknRoute.h>
#include <PyramidM\MSVC_2008\RouteClass.h>
#include <gis_tool_qt\GisInterfaceQT\MapBaseWidget.h>
#include <PyramidM\MSVC_2008\CalcClass.h>

//ориентация на маршрут
NaviClass::NaviClass(QObject* parent, MapBaseWidget* map,QString pathToRoute)
{
	this->setParent(parent);
	_map = map;

	_distNavi = 0.1; //(км)
	_latNavi = 0;
	_lonNavi = 0;
	_pointIsSet = false;
	_naviOn = false;
	_unchNavi = true;
	_naviOn = false;
	initResult = false;

	if(_routePrepare(pathToRoute))
	{
		_naviOn = true;
		initResult = true;
	}
	
}
//ориентация на точку
NaviClass::NaviClass(QObject* parent, MapBaseWidget* map, double latNavi, double lonNavi)
{
	this->setParent(parent);
	_map = map;

	_distNavi = 0.1; //(км)
	initResult = false;
	if(latNavi == 0 && lonNavi == 0)
	{
		_pointIsSet = false;
		_naviOn = false;
		return;
	}
	initResult = true;
	_latNavi = latNavi;
	_lonNavi = lonNavi;
	_pointIsSet = true;
	_naviOn = true;
	_unchNavi = true;
}

NaviClass::~NaviClass()
{

}


bool NaviClass::_loadSettings()
{
	return true;
}
void NaviClass::_slotGpsData(QTime time, QDate date, bool status, double lat, double lon, double speed, double angle,short sat, float height)
{
	if(!_naviOn)
		return;
	//принимаем и обрабатываем данные
	//Расстояние до маршрута (км)
	double routeDist = 0;
	//Расстояние до точки (км)
	double pointDist = 0;
	//Расчетное расстояние (км)
	double calcDist = 0;
	//Отклонение от маршрута (радианы)
	double routeAngle = 0;
	//Отклонение от точки (радианы)
	double pointAngle = 0;
	//Текущее направление (радианы)
	double currentAngle = 0;
	//Номер ближайшей точки 
	short pointNumber = 0;
	//Расчетное время в пути (часах или минутах)
	double calcTime = 0;
	
	//радиус посещенности
	//_distNavi
	/*************************************/
	short neerestPt;
	short tmp_point;
	double anglToBNT;
	double rasstFromBNT;
	double angleToTOM;
	double rasstFromTOM;
	double distanceOfMarsh;
	double rasstOfMarshAfterBTN;
	/*************************************/
	
	//возможен вариант когда указана точка для ориентации, тогда ориентация на маршрут не нужена
	if(_pointIsSet)
	{
		//находим азимут на точку навигации
		anglToBNT = CalcFunctions::calcTrueAngleRad(lat,_latNavi,lon,_lonNavi);
		//определяем азимут ориентирования
		double angle2 = anglToBNT*CCT_180_PI + _map->getGisView()->angle()*CCT_180_PI;
		rasstFromBNT = CalcFunctions::findRasstRadians(lat,_latNavi, lon,_lonNavi);
		
		if (_distNavi > rasstFromBNT)
		{
			_naviOn = false;
			emit signNaviSuccess();
			return;
			
		}

		distanceOfMarsh = rasstFromBNT;

		if(speed > 0)
			calcTime = distanceOfMarsh/speed;

		if (calcTime <1) 
			calcTime = 0;

		if (angle2 > 360)
			angle2 -= 360;
		if (angle2 < -360)
			angle2 += 360;

		//маршрут не задан
		routeDist = 0;
		routeAngle = 0;

		pointDist = rasstFromBNT;
		//весь маршрут состоит из расстояния от устройства до заданной точки
		calcDist = pointDist;
		//setData(0,rasstFromBNT*1000,0,angle2,angleFact*CCT_180_PI,0,time,distanceOfMarsh);
		pointAngle = angle2*CCT_PI_180;
		currentAngle = angle;
		//точка нулевая и единстрвенная
		pointNumber = 0;
		//отправляем данные об ориентации на точку
		emit signNaviData(routeDist,pointDist,calcDist,routeAngle,pointAngle,currentAngle,pointNumber,calcTime);
		//сигнал на отрисовку ориентировачной линии
		emit signDrawOrientLine(lat,lon,_latNavi,_lonNavi);
		return;
	}
	//возможен вариант маршрут загружен


	//инициализация
	//определяем ближайшую непосещенную точку
	neerestPt = _neerestPoint(&_vectorPoints,lat,lon,_distNavi);

	if ((neerestPt == -2 || neerestPt == _vectorPoints.size()-1))
	{
		//маршрут пройден
		emit signNaviSuccess();
		return;
	}
	
	//находим минимальную по индексу непосещенную точку перед нашей непосещенной точкой
	tmp_point = _findFirstUPoint(&_vectorPoints,neerestPt);

	//есть ли непосещенные точки до ближайшей непосещенной точки
	if(!(tmp_point == neerestPt) )
	{
		//если есть используем настройки для определения будет ли направляться маршрут на неближайшую непосещенную точку 
		//с меньшим индексом
		if(tmp_point > -1)
		{
			if(_unchNavi)
				//ориентироваться ли на непосещенную точку
				neerestPt = tmp_point;
			
			else
				_vectorPoints.at(tmp_point).checked = true;
		}
	}

	//расстояние от ближайшей непосещенной точки до конца маршрута
	rasstOfMarshAfterBTN = _findRasstFromPointToPointMarsh(&_vectorPoints,neerestPt,_vectorPoints.size()-1);
	//определяем направление к ближайшей непосещенной точке (в радианах)
	anglToBNT = CalcFunctions::calcTrueAngleRad(lat, _vectorPoints.at(neerestPt).lat, lon, _vectorPoints.at(neerestPt).lon);
	//определяем расстояние от ближайшей непосещенной точке (в км)
	rasstFromBNT = CalcFunctions::findRasstRadians(lat, _vectorPoints.at(neerestPt).lat, lon, _vectorPoints.at(neerestPt).lon);
	//определяем направление к текущему отрезку маршрута (радианы)
	//если текущая бнт первая или последняя то расстояние от маршрута не считается
	if(neerestPt == 0)
		angleToTOM = anglToBNT;
	else
	{
		//угол фактический относительно нуля
		PosBL tek1;
		PosBL first1;
		PosBL second1;
		tek1.b = lat;
		tek1.l = lon;
		first1.b = _vectorPoints.at(neerestPt-1).lat;
		first1.l = _vectorPoints.at(neerestPt-1).lon;
		second1.b = _vectorPoints.at(neerestPt).lat;
		second1.l = _vectorPoints.at(neerestPt).lon;
		
		angleToTOM = _calcNaprOfPt(tek1,first1,second1,true);

		
	}
		double angle1 = angleToTOM*CCT_180_PI + _map->getGisView()->angle()*CCT_180_PI;
		double angle2 = anglToBNT*CCT_180_PI + _map->getGisView()->angle()*CCT_180_PI;
	
	//определяем расстояние от текущего отрезка маршрута (км)
	//если точка нулевая (до нее ничего нет)
	if(neerestPt == 0)
	{
		rasstFromTOM = 0 ;
		distanceOfMarsh = rasstFromBNT + rasstOfMarshAfterBTN;		
	}
	else
	{
		//если точка не нулевая
		PosBL tek,first,second;
		tek.b = lat;
		tek.l = lon;
		first.b = _vectorPoints.at(neerestPt-1).lat;
		first.l = _vectorPoints.at(neerestPt-1).lon;
		second.b = _vectorPoints.at(neerestPt).lat;
		second.l = _vectorPoints.at(neerestPt).lon;

		rasstFromTOM = CalcClass::calcRasstFromLine(tek,first,second);
		PosBL neerestPointOfTOM = _calcPosOfPt(tek,first,second);
		//расстояние от текущей координаты до точки входа в маршрут;
		double rasstFromTekToTVM = CalcFunctions::findRasstRadians(neerestPointOfTOM.b,_vectorPoints.at(neerestPt).lat,neerestPointOfTOM.l,_vectorPoints.at(neerestPt).lon);
		//расстояния от текущей позиции до ближайшей непосещенной точки маршрута расстояние от линии до отрезка маршрута и оставшееся расстояние до непосещенной точки
		double rasstFromTvmToBnt = rasstFromTOM + rasstFromTekToTVM;//_getRasstTwmBnt(lat,lon, _vectorPoints->at(neerestPt-1).lat, _vectorPoints->at(neerestPt-1).lon, _vectorPoints->at(neerestPt).lat, _vectorPoints->at(neerestPt).lon);
		distanceOfMarsh = rasstFromTvmToBnt  + rasstOfMarshAfterBTN;
				
		if( rasstFromTOM > 0 && rasstFromTOM < _distNavi)
			angle1 = angle2;
	}
	//ч
	if(speed > 0)
		calcTime = distanceOfMarsh/speed;
	if (calcTime <1) calcTime = 0;
	//угол отклонения от маршрута
	if (angle1 > 360)
		angle1 -= 360;
	if (angle2 > 360)
		angle2 -= 360;

	if (angle1 < -360)
		angle1 += 360;
	if (angle2 < -360)
		angle2 += 360;

	if(neerestPt > 0 && angle1 != angle2)
	{
		PosBL tek,first,second;
		tek.b = lat;
		tek.l = lon;
		first.b = _vectorPoints.at(neerestPt-1).lat;
		first.l = _vectorPoints.at(neerestPt-1).lon;
		second.b = _vectorPoints.at(neerestPt).lat;
		second.l = _vectorPoints.at(neerestPt).lon;
		PosBL neerestPointOfTOM  = _calcPosOfPt(tek,first,second);
		emit signDrawOrientLine(lat, lon, neerestPointOfTOM.b, neerestPointOfTOM.l); 
	}
	else
		emit signDrawOrientLine(lat, lon, _vectorPoints.at(neerestPt).lat, _vectorPoints.at(neerestPt).lon); 
		
		//маршрут не задан
		routeDist = rasstFromTOM;
		routeAngle = angleToTOM;

		pointDist = rasstFromBNT;
		//весь маршрут состоит из расстояния от устройства до заданной точки
		calcDist = distanceOfMarsh;
		pointAngle = angle2*CCT_PI_180;
		currentAngle = angle;
		//точка нулевая и единстрвенная
		pointNumber = tmp_point;
	emit signNaviData(routeDist,pointDist,calcDist,  routeAngle,pointAngle,currentAngle,pointNumber,calcTime);
}

void NaviClass::_slotDistNavi(short dist)
{
	_distNavi = (double)dist/1000;
}

void NaviClass::_slotUncheckedNavi(bool unchNavi)
{
	_unchNavi = unchNavi;
}

bool NaviClass::_routePrepare(QString pathToRoute)
{
	TknRoute* route = RouteClass::routeFromFile(pathToRoute);
	if(_trackToVector(route))
	{
		//сообщаем координаты последней точки маршрута
		double lat = _vectorPoints.at(_vectorPoints.size()-1).lat;
		double lon = _vectorPoints.at(_vectorPoints.size()-1).lon;
		emit signLastPointRoute(lat,lon);
		return true;
	}
	return false;
}

bool NaviClass::_trackToVector(TknRoute* tknRt)
{
	if(!tknRt)
		return false;
	int vectorSize = tknRt->vectorPoints.size();
	if (vectorSize)
	{
		RoutePoints point;
		_vectorPoints.clear();
		for(int i = 0; i < vectorSize; i++)
		{
			point.lat = tknRt->vectorPoints.at(i).fB;
			point.lon = tknRt->vectorPoints.at(i).fL;
			_vectorPoints.push_back(point);
		}
	}
	emit signLastPointRoute(_vectorPoints.at(_vectorPoints.size()-1).lat,_vectorPoints.at(_vectorPoints.size()-1).lon);
	return true;
}


short NaviClass::_neerestPoint(std::vector<RoutePoints> *vectorPoints, double lat, double lon,double checkDist)
{
		double result = -2;
		double rasstRes = 999999999;
		//нужна проверка на размер вектора
		for(unsigned int i = 0; i < vectorPoints->size(); i++)
		{
			if(!vectorPoints->at(i).checked)
			{
				//расстояние до первой точки
				double rasst_tmp = CalcFunctions::findRasstRadians(lat,vectorPoints->at(i).lat,lon,vectorPoints->at(i).lon);

				//если точка ближе идеальной
				if(rasst_tmp > checkDist)
				{
					if( rasstRes > rasst_tmp)
					{
						rasstRes = rasst_tmp;
						result = i;
					}
				}
				else
				{
					vectorPoints->at(i).checked = true;
				}
			}	
		}
		return result;
}

short NaviClass::_findFirstUPoint(std::vector<RoutePoints> *vectorPoints,short numb/*текущая точка до которой ищем*/)
{
	for(short i =0; i<numb+1;i++)
	{
		if(!vectorPoints->at(i).checked)
			return i;
	}
	return -2;
}


double NaviClass::_findRasstFromPointToPointMarsh(std::vector<RoutePoints> *vectorPoints,unsigned short firstPoint,unsigned short lastPoint)
{
	double result = 0;
	
	int vectorSize = vectorPoints->size();
	
	if(vectorSize < firstPoint + 1 || vectorSize < lastPoint + 1)
		return 0;


		RoutePoints point;
		RoutePoints point2;
		int i = firstPoint;
		while(i < lastPoint)
		{
			point.lat = vectorPoints->at(i).lat;
			point.lon = vectorPoints->at(i).lon;
			
			if (i < lastPoint)
			{
				point2.lat = vectorPoints->at(i+1).lat;
				point2.lon = vectorPoints->at(i+1).lon;
				result += CalcFunctions::findRasstRadians(point.lat,point2.lat ,point.lon,point2.lon);
			}
			else
				break;
			i++;
		}
	return result;
}


//расчет направления на маршрут
double NaviClass::_calcNaprOfPt(PosBL tekPoint,PosBL firstPoint,PosBL secondPoint,bool trueAngle)
{
	//катет а
	double a = CalcClass::calcRasstFromLine(tekPoint,firstPoint,secondPoint);
	//гипотенуза с
	double c = CalcFunctions::findRasstRadians(tekPoint.b,secondPoint.b,tekPoint.l,secondPoint.l);
	//катет b
	//теорема пифагора
	double b = sqrt( (c*c) - (a*a) );
	//сферическая теорема пифагора cos(c) = cos(a)*cos(b)
	//b = cos(c) / cos(a) ;
		double rasstTo0;
	double rasstTo1;
	double rasstFrom0To1;
	//находим квадраты расстояний между точками

	//Расстояние от текущей точки до первой точки отрезка
	//a
	rasstTo0 = CalcFunctions::findRasstRadians(tekPoint.b,firstPoint.b,tekPoint.l,firstPoint.l);
	rasstTo0 *= rasstTo0;
	//Расстояние от текущей точки до второй точки отрезка
	//b
	rasstTo1 =  CalcFunctions::findRasstRadians(tekPoint.b,secondPoint.b,tekPoint.l,secondPoint.l);
	rasstTo1 *= rasstTo1;
	//Расстояние от первой до второй точки отрезка
	//c
	rasstFrom0To1 =  CalcFunctions::findRasstRadians(firstPoint.b,secondPoint.b,firstPoint.l,secondPoint.l);
	rasstFrom0To1 *= rasstFrom0To1;


	//удаленность начальной точки от первой точки отрезка
	double zeroUnk = CalcFunctions::findRasstRadians(firstPoint.b,secondPoint.b,firstPoint.l,secondPoint.l) - b;
	//соотношение (лямда)
	zeroUnk = zeroUnk / CalcFunctions::findRasstRadians(firstPoint.b,secondPoint.b,firstPoint.l,secondPoint.l);
	
	PosBL coord;
	coord.b = firstPoint.b + (secondPoint.b - firstPoint.b )* zeroUnk;
	coord.l = firstPoint.l + (secondPoint.l - firstPoint.l )* zeroUnk;


	double lat = coord.b * CCT_180_PI;
	double lon = coord.l * CCT_180_PI;
	double result = 0;

	if(rasstTo0 >= rasstTo1 + rasstFrom0To1) 
	{
		if(trueAngle)
			result = CalcFunctions::calcTrueAngleRad(tekPoint.b,secondPoint.b,tekPoint.l,secondPoint.l);
		else
			result = CalcClass::find_angle(tekPoint.b*CCT_180_PI,secondPoint.b*CCT_180_PI,tekPoint.l*CCT_180_PI,secondPoint.l*CCT_180_PI);

		return result;
	}

    if(rasstTo1>=rasstTo0+rasstFrom0To1)
	{
		if(trueAngle)
			result = CalcFunctions::calcTrueAngleRad(tekPoint.b,firstPoint.b,tekPoint.l,firstPoint.l);
		else
			result = CalcClass::find_angle(tekPoint.b*CCT_180_PI,firstPoint.b*CCT_180_PI,tekPoint.l*CCT_180_PI,firstPoint.l*CCT_180_PI);

		return result;
	}




	if(trueAngle)
		result = CalcFunctions::calcTrueAngleRad(tekPoint.b,coord.b,tekPoint.l,coord.l);
	else
		result = CalcClass::find_angle(tekPoint.b*CCT_180_PI,coord.b*CCT_180_PI,tekPoint.l*CCT_180_PI,coord.l*CCT_180_PI);
		
	double resDegr = result * CCT_180_PI;
	return result;
}


PosBL NaviClass::_calcPosOfPt(PosBL tekPoint,PosBL firstPoint,PosBL secondPoint)
{
	//катет а
	double a =  CalcClass::calcRasstFromLine(tekPoint,firstPoint,secondPoint);
	//гипотенуза с
	double c =  CalcFunctions::findRasstRadians(tekPoint.b,secondPoint.b,tekPoint.l,secondPoint.l);
	//катет b
	double b = sqrt( (c*c) - (a*a) );

	//b = sqrt ( mySqr ( acos ( cos(c) / cos(a) ) ) ) ;

	double rasstTo0;
	double rasstTo1;
	double rasstFrom0To1;
	//находим квадраты расстояний между точками

	//Расстояние от текущей точки до первой точки отрезка
	//a
	rasstTo0 =  CalcFunctions::findRasstRadians(tekPoint.b,firstPoint.b,tekPoint.l,firstPoint.l);
	rasstTo0 *= rasstTo0;
	//Расстояние от текущей точки до второй точки отрезка
	//b
	rasstTo1 =   CalcFunctions::findRasstRadians(tekPoint.b,secondPoint.b,tekPoint.l,secondPoint.l);
	rasstTo1 *= rasstTo1;
	//Расстояние от первой до второй точки отрезка
	//c
	rasstFrom0To1 =   CalcFunctions::findRasstRadians(firstPoint.b,secondPoint.b,firstPoint.l,secondPoint.l);
	rasstFrom0To1 *= rasstFrom0To1;

	//удаленность начальной точки от первой точки отрезка
	double zeroUnk =  CalcFunctions::findRasstRadians(firstPoint.b,secondPoint.b,firstPoint.l,secondPoint.l) - b;
	//соотношение (лямда)
	zeroUnk = zeroUnk /  CalcFunctions::findRasstRadians(firstPoint.b,secondPoint.b,firstPoint.l,secondPoint.l);
	
	PosBL coord;
	coord.b = firstPoint.b + (secondPoint.b - firstPoint.b )* zeroUnk;
	coord.l = firstPoint.l + (secondPoint.l - firstPoint.l )* zeroUnk;

	PosBL result;

	if(rasstTo0 >= rasstTo1 + rasstFrom0To1) 
	{

		return secondPoint;
	}

    if(rasstTo1>=rasstTo0+rasstFrom0To1)
	{
		return firstPoint;
	}

	
	result.b = coord.b;
	result.l = coord.l;
	return result;
}