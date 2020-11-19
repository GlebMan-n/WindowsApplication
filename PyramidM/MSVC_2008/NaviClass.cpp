#include "stdafx.h"
#include <PyramidM\MSVC_2008\NaviClass.h>	
#include <PyramidM\MSVC_2008\CalcFunctions.h>
#include <PyramidM\MSVC_2008\TknRoute.h>
#include <PyramidM\MSVC_2008\RouteClass.h>
#include <gis_tool_qt\GisInterfaceQT\MapBaseWidget.h>
#include <PyramidM\MSVC_2008\CalcClass.h>

//���������� �� �������
NaviClass::NaviClass(QObject* parent, MapBaseWidget* map,QString pathToRoute)
{
	this->setParent(parent);
	_map = map;

	_distNavi = 0.1; //(��)
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
//���������� �� �����
NaviClass::NaviClass(QObject* parent, MapBaseWidget* map, double latNavi, double lonNavi)
{
	this->setParent(parent);
	_map = map;

	_distNavi = 0.1; //(��)
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
	//��������� � ������������ ������
	//���������� �� �������� (��)
	double routeDist = 0;
	//���������� �� ����� (��)
	double pointDist = 0;
	//��������� ���������� (��)
	double calcDist = 0;
	//���������� �� �������� (�������)
	double routeAngle = 0;
	//���������� �� ����� (�������)
	double pointAngle = 0;
	//������� ����������� (�������)
	double currentAngle = 0;
	//����� ��������� ����� 
	short pointNumber = 0;
	//��������� ����� � ���� (����� ��� �������)
	double calcTime = 0;
	
	//������ ������������
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
	
	//�������� ������� ����� ������� ����� ��� ����������, ����� ���������� �� ������� �� ������
	if(_pointIsSet)
	{
		//������� ������ �� ����� ���������
		anglToBNT = CalcFunctions::calcTrueAngleRad(lat,_latNavi,lon,_lonNavi);
		//���������� ������ ��������������
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

		//������� �� �����
		routeDist = 0;
		routeAngle = 0;

		pointDist = rasstFromBNT;
		//���� ������� ������� �� ���������� �� ���������� �� �������� �����
		calcDist = pointDist;
		//setData(0,rasstFromBNT*1000,0,angle2,angleFact*CCT_180_PI,0,time,distanceOfMarsh);
		pointAngle = angle2*CCT_PI_180;
		currentAngle = angle;
		//����� ������� � �������������
		pointNumber = 0;
		//���������� ������ �� ���������� �� �����
		emit signNaviData(routeDist,pointDist,calcDist,routeAngle,pointAngle,currentAngle,pointNumber,calcTime);
		//������ �� ��������� ��������������� �����
		emit signDrawOrientLine(lat,lon,_latNavi,_lonNavi);
		return;
	}
	//�������� ������� ������� ��������


	//�������������
	//���������� ��������� ������������ �����
	neerestPt = _neerestPoint(&_vectorPoints,lat,lon,_distNavi);

	if ((neerestPt == -2 || neerestPt == _vectorPoints.size()-1))
	{
		//������� �������
		emit signNaviSuccess();
		return;
	}
	
	//������� ����������� �� ������� ������������ ����� ����� ����� ������������ ������
	tmp_point = _findFirstUPoint(&_vectorPoints,neerestPt);

	//���� �� ������������ ����� �� ��������� ������������ �����
	if(!(tmp_point == neerestPt) )
	{
		//���� ���� ���������� ��������� ��� ����������� ����� �� ������������ ������� �� ����������� ������������ ����� 
		//� ������� ��������
		if(tmp_point > -1)
		{
			if(_unchNavi)
				//��������������� �� �� ������������ �����
				neerestPt = tmp_point;
			
			else
				_vectorPoints.at(tmp_point).checked = true;
		}
	}

	//���������� �� ��������� ������������ ����� �� ����� ��������
	rasstOfMarshAfterBTN = _findRasstFromPointToPointMarsh(&_vectorPoints,neerestPt,_vectorPoints.size()-1);
	//���������� ����������� � ��������� ������������ ����� (� ��������)
	anglToBNT = CalcFunctions::calcTrueAngleRad(lat, _vectorPoints.at(neerestPt).lat, lon, _vectorPoints.at(neerestPt).lon);
	//���������� ���������� �� ��������� ������������ ����� (� ��)
	rasstFromBNT = CalcFunctions::findRasstRadians(lat, _vectorPoints.at(neerestPt).lat, lon, _vectorPoints.at(neerestPt).lon);
	//���������� ����������� � �������� ������� �������� (�������)
	//���� ������� ��� ������ ��� ��������� �� ���������� �� �������� �� ���������
	if(neerestPt == 0)
		angleToTOM = anglToBNT;
	else
	{
		//���� ����������� ������������ ����
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
	
	//���������� ���������� �� �������� ������� �������� (��)
	//���� ����� ������� (�� ��� ������ ���)
	if(neerestPt == 0)
	{
		rasstFromTOM = 0 ;
		distanceOfMarsh = rasstFromBNT + rasstOfMarshAfterBTN;		
	}
	else
	{
		//���� ����� �� �������
		PosBL tek,first,second;
		tek.b = lat;
		tek.l = lon;
		first.b = _vectorPoints.at(neerestPt-1).lat;
		first.l = _vectorPoints.at(neerestPt-1).lon;
		second.b = _vectorPoints.at(neerestPt).lat;
		second.l = _vectorPoints.at(neerestPt).lon;

		rasstFromTOM = CalcClass::calcRasstFromLine(tek,first,second);
		PosBL neerestPointOfTOM = _calcPosOfPt(tek,first,second);
		//���������� �� ������� ���������� �� ����� ����� � �������;
		double rasstFromTekToTVM = CalcFunctions::findRasstRadians(neerestPointOfTOM.b,_vectorPoints.at(neerestPt).lat,neerestPointOfTOM.l,_vectorPoints.at(neerestPt).lon);
		//���������� �� ������� ������� �� ��������� ������������ ����� �������� ���������� �� ����� �� ������� �������� � ���������� ���������� �� ������������ �����
		double rasstFromTvmToBnt = rasstFromTOM + rasstFromTekToTVM;//_getRasstTwmBnt(lat,lon, _vectorPoints->at(neerestPt-1).lat, _vectorPoints->at(neerestPt-1).lon, _vectorPoints->at(neerestPt).lat, _vectorPoints->at(neerestPt).lon);
		distanceOfMarsh = rasstFromTvmToBnt  + rasstOfMarshAfterBTN;
				
		if( rasstFromTOM > 0 && rasstFromTOM < _distNavi)
			angle1 = angle2;
	}
	//�
	if(speed > 0)
		calcTime = distanceOfMarsh/speed;
	if (calcTime <1) calcTime = 0;
	//���� ���������� �� ��������
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
		
		//������� �� �����
		routeDist = rasstFromTOM;
		routeAngle = angleToTOM;

		pointDist = rasstFromBNT;
		//���� ������� ������� �� ���������� �� ���������� �� �������� �����
		calcDist = distanceOfMarsh;
		pointAngle = angle2*CCT_PI_180;
		currentAngle = angle;
		//����� ������� � �������������
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
		//�������� ���������� ��������� ����� ��������
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
		//����� �������� �� ������ �������
		for(unsigned int i = 0; i < vectorPoints->size(); i++)
		{
			if(!vectorPoints->at(i).checked)
			{
				//���������� �� ������ �����
				double rasst_tmp = CalcFunctions::findRasstRadians(lat,vectorPoints->at(i).lat,lon,vectorPoints->at(i).lon);

				//���� ����� ����� ���������
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

short NaviClass::_findFirstUPoint(std::vector<RoutePoints> *vectorPoints,short numb/*������� ����� �� ������� ����*/)
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


//������ ����������� �� �������
double NaviClass::_calcNaprOfPt(PosBL tekPoint,PosBL firstPoint,PosBL secondPoint,bool trueAngle)
{
	//����� �
	double a = CalcClass::calcRasstFromLine(tekPoint,firstPoint,secondPoint);
	//���������� �
	double c = CalcFunctions::findRasstRadians(tekPoint.b,secondPoint.b,tekPoint.l,secondPoint.l);
	//����� b
	//������� ��������
	double b = sqrt( (c*c) - (a*a) );
	//����������� ������� �������� cos(c) = cos(a)*cos(b)
	//b = cos(c) / cos(a) ;
		double rasstTo0;
	double rasstTo1;
	double rasstFrom0To1;
	//������� �������� ���������� ����� �������

	//���������� �� ������� ����� �� ������ ����� �������
	//a
	rasstTo0 = CalcFunctions::findRasstRadians(tekPoint.b,firstPoint.b,tekPoint.l,firstPoint.l);
	rasstTo0 *= rasstTo0;
	//���������� �� ������� ����� �� ������ ����� �������
	//b
	rasstTo1 =  CalcFunctions::findRasstRadians(tekPoint.b,secondPoint.b,tekPoint.l,secondPoint.l);
	rasstTo1 *= rasstTo1;
	//���������� �� ������ �� ������ ����� �������
	//c
	rasstFrom0To1 =  CalcFunctions::findRasstRadians(firstPoint.b,secondPoint.b,firstPoint.l,secondPoint.l);
	rasstFrom0To1 *= rasstFrom0To1;


	//����������� ��������� ����� �� ������ ����� �������
	double zeroUnk = CalcFunctions::findRasstRadians(firstPoint.b,secondPoint.b,firstPoint.l,secondPoint.l) - b;
	//����������� (�����)
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
	//����� �
	double a =  CalcClass::calcRasstFromLine(tekPoint,firstPoint,secondPoint);
	//���������� �
	double c =  CalcFunctions::findRasstRadians(tekPoint.b,secondPoint.b,tekPoint.l,secondPoint.l);
	//����� b
	double b = sqrt( (c*c) - (a*a) );

	//b = sqrt ( mySqr ( acos ( cos(c) / cos(a) ) ) ) ;

	double rasstTo0;
	double rasstTo1;
	double rasstFrom0To1;
	//������� �������� ���������� ����� �������

	//���������� �� ������� ����� �� ������ ����� �������
	//a
	rasstTo0 =  CalcFunctions::findRasstRadians(tekPoint.b,firstPoint.b,tekPoint.l,firstPoint.l);
	rasstTo0 *= rasstTo0;
	//���������� �� ������� ����� �� ������ ����� �������
	//b
	rasstTo1 =   CalcFunctions::findRasstRadians(tekPoint.b,secondPoint.b,tekPoint.l,secondPoint.l);
	rasstTo1 *= rasstTo1;
	//���������� �� ������ �� ������ ����� �������
	//c
	rasstFrom0To1 =   CalcFunctions::findRasstRadians(firstPoint.b,secondPoint.b,firstPoint.l,secondPoint.l);
	rasstFrom0To1 *= rasstFrom0To1;

	//����������� ��������� ����� �� ������ ����� �������
	double zeroUnk =  CalcFunctions::findRasstRadians(firstPoint.b,secondPoint.b,firstPoint.l,secondPoint.l) - b;
	//����������� (�����)
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