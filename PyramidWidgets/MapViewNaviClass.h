#include "ui_MapMarshOrient.h"
#include <QtGui>

struct Dist
{
	Dist()
	{
		km = 0;
		m = 0;
	}
	int km;
	int m;
};

class MapViewNaviClass : public QWidget, public Ui::MarshOrientDlg
{
	Q_OBJECT
public:
	MapViewNaviClass(QWidget* parent);
	~MapViewNaviClass();
private:
	bool _maximize;
	bool _routeNavi;
	QWidget* _parent;
private:
	Dist _kmToDist(double km);
	QTime _minToTime(double timeInHour);
	//отображаем данные
	void _setRouteDist(double dist);
	void _setPointDist(double dist,short pointNumber);
	void _setCalcDist(double dist);
	void _setCalcTime(double time);
	void _setRouteAngle(double angle);
	void _setPointAngle(double angle);
	void _setTrueAngle(double angle);
	void _setNaviPixmap(double angle);

private	slots:
	void _slotButtonClickSound();
	void _slotMinMax();
	void _slotRouteNavi(int index);
	void _slotCheckDist(int index);
	void _slotUncheckedNavi();
	//данные о навигации
	void _slotNaviData(double routeDist,double pointDist,double calcDist,
				   double routeAngle,double pointAngle,double currentAngle,
				   short pointNumber,double calcTime);
	void _slotEndNavi();
signals:
	void signDistNavi(short dist);
	void signUncheckedNavi(bool unchNavi);
	//сигнал смена типа навигации (на маршрут/на точку)
	void signRouteNavi(bool routeNavi);
};