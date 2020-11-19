//����� ������������� ��������
#include "stdafx.h"
#include <QObject>
#include <gis\GisBase\cgis\pos.h>

struct RoutePoints
{
	RoutePoints()
	{
		lat = 0;
		lon = 0;
		checked = false;
		dist = 0;
	}
	double lat;
	double lon;
	double dist;
	bool checked;

};

class TknRoute;
class MapBaseWidget;

class NaviClass : public QObject
{
	Q_OBJECT
public:
	//���������� �� �������
	NaviClass(QObject* parent, MapBaseWidget* map,QString pathToRoute);
	//���������� �� �����
	NaviClass(QObject* parent, MapBaseWidget* map,double latNavi, double lonNavi);
	~NaviClass();
public:
	//��������� �������������
	bool initResult;
private:
	bool _loadSettings();
	bool _trackToVector(TknRoute* tknRt);
	bool _routePrepare(QString pathToRoute);
	short _neerestPoint(std::vector<RoutePoints> *vectorPoints, double lat, double lon,double checkDist);
	short _findFirstUPoint(std::vector<RoutePoints> *vectorPoints,short numb/*������� ����� �� ������� ����*/);
	PosBL _calcPosOfPt(PosBL tekPoint,PosBL firstPoint,PosBL secondPoint);
	double _calcNaprOfPt(PosBL tekPoint,PosBL firstPoint,PosBL secondPoint,bool trueAngle);
	double _findRasstFromPointToPointMarsh(std::vector<RoutePoints> *vectorPoints,unsigned short firstPoint,unsigned short lastPoint);
private:
	MapBaseWidget* _map;
	//���� ��������� �� �������

	bool _pointIsSet;
	bool _naviOn;
	//���������� �� ������������ �����
	bool _unchNavi;
	//��������� ��������� ����� � ��
	double _distNavi;
	//���������� ��������� (� ������ ��������� �� �����)
	double _latNavi;
	double _lonNavi;
	//������ ����� ��������
	std::vector<RoutePoints> _vectorPoints;
private slots:
	void _slotGpsData(QTime time, QDate date, bool status, double lat, double lon, double speed, double angle,short sat, float height);
	//������������� �� ����� �������� ��� �� ��� �������
	void _slotDistNavi(short dist);
	void _slotUncheckedNavi(bool unchNavi);	

signals:
	void signNaviData(double routeDist,double pointDist,double calcDist,
			   double routeAngle,double pointAngle,double currentAngle,
			   short pointNumber,double calcTime);
	void signLastPointRoute(double lat, double lon);
	void signDrawOrientLine(double firstLat, double firstLon, double secondLat, double secondLon); 
	void signNaviSuccess();
};