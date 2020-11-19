#include <PyramidWidgets\MapViewNaviClass.h>	
#include <PyramidM\MSVC_2008\CalcFunctions.h>


MapViewNaviClass::MapViewNaviClass(QWidget* parent)
{
	this->setParent(parent);
	setupUi(this);
	this->setGeometry( CalcFunctions::calcWindowPos(parent->geometry(),this->geometry()) );
	_parent = parent;
	_maximize = true;	
	_routeNavi = (bool) selectOrient->currentIndex();
	connect(selectOrient,SIGNAL(highlighted (int)),SLOT(_slotButtonClickSound()));
	connect(uncheckedOrientBut,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(checkDist,SIGNAL(highlighted (int)),SLOT(_slotButtonClickSound()));
	connect(minimBut,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));

	connect(selectOrient,SIGNAL(currentIndexChanged (int)),SLOT(_slotRouteNavi(int)));
	connect(uncheckedOrientBut,SIGNAL(released()),SLOT(_slotUncheckedNavi()));
	connect(checkDist,SIGNAL(currentIndexChanged (int)),SLOT(_slotCheckDist(int)));
	connect(minimBut,SIGNAL(clicked()),SLOT(_slotMinMax()));
	_minToTime(1.25);
}

MapViewNaviClass::~MapViewNaviClass()
{

}

void MapViewNaviClass::_slotMinMax()
{
	if(_maximize)
	{
		this->setGeometry(QRect(this->x(), this->y(), 460, 260));
		this->background->setGeometry(QRect(0, 0, 460, 260));
		this->minimBut->setGeometry(QRect(405, 10, 50, 230));
		_maximize = false;
		this->setGeometry( CalcFunctions::calcWindowPos(_parent->geometry(),this->geometry()) );
	}
	else
	{
		this->setGeometry(QRect(this->x(), this->y(), 770, 470));
		this->background->setGeometry(QRect(0, 0, 770, 470));
		this->minimBut->setGeometry(QRect(700, 10, 60, 450));
		_maximize = true;
		this->setGeometry( CalcFunctions::calcWindowPos(_parent->geometry(),this->geometry()) );
	}
}

//Расстояние до маршрута (км)
//Номер ближайшей точки 
//Расстояние до точки (км)
//Расчетное время в пути (часах или минутах)
//Расчетное расстояние (км)
//Отклонение от маршрута (радианы)
//Отклонение от точки (радианы)
//Текущее направление (радианы)
void MapViewNaviClass::_slotNaviData(double routeDist, 
								 double pointDist,
								 double calcDist,
								 double routeAngle,
								 double pointAngle,
								 double currentAngle,
								 short pointNumber,
								 double calcTime
								 )
{
	//получаем расстояния в виде км и м из км.км
	//устанавливаем расстояние до маршрута
	_setRouteDist(routeDist);
	//устанавливаем расстояние до точки
	_setPointDist(pointDist,pointNumber);

	//устанавливаем расчетное расстояние
	_setCalcDist(calcDist);
	//устанавливаем расчетное время
	_setCalcTime(calcTime);
	//устанавливаем азимут на маршрут
	_setRouteAngle(routeAngle);
	//устанавливаем азимут на точку
	_setPointAngle(pointAngle);
	//устанавливаем изимут текущего движения
	_setTrueAngle(currentAngle);

	//Угол указателя (радианы в градусы)
	if(_routeNavi)
		_setNaviPixmap(routeAngle);
	else
		_setNaviPixmap(pointAngle);

}

void MapViewNaviClass::_slotRouteNavi(int index)
{
	_routeNavi = !(bool)index;
	emit signRouteNavi(_routeNavi);

}
//получаем км и м из км.км
Dist MapViewNaviClass::_kmToDist(double km)
{
	Dist result;
	result.km = (int)km;
	result.m = (int)((km - result.km)*1000);
	return result;
}

QTime MapViewNaviClass::_minToTime(double timeInHour)
{
	int h = (int)timeInHour;
	int m = (timeInHour - h)*60;
	QTime time(h, m,0);
	return time;
}
void MapViewNaviClass::_slotUncheckedNavi()
{
	bool res = uncheckedOrientBut->isChecked();
	emit signUncheckedNavi(res);
}

void MapViewNaviClass::_slotCheckDist(int index)
{
	switch(index)
	{
	case 0:
		emit signDistNavi(100);
		break;
	case 1:
		emit signDistNavi(500);
		break;
	case 2:
		emit signDistNavi(1000);
		break;
	case 3:
		emit signDistNavi(3000);
		break;
	case 4:
		emit signDistNavi(5000);
		break;
	case 5:
		emit signDistNavi(7000);
		break;
	case 6:
		emit signDistNavi(10000);
		break;
	case 7:
		emit signDistNavi(15000);
		break;
	default:
		emit signDistNavi(500);
		break;
	}
}

void MapViewNaviClass::_slotEndNavi()
{
	//навигация окончена
}


void MapViewNaviClass::_setRouteDist(double dist)
{
	Dist routeDist_= _kmToDist(dist);
	marshKm->display(routeDist_.km);
	marshM->display(routeDist_.m);
}

void MapViewNaviClass::_setPointDist(double dist,short pointNumber)
{
	Dist pointDist_= _kmToDist(dist);
	pointKm->display(pointDist_.km);
	pointM->display(pointDist_.m);
	numbPointLbl->setText(QString::number(pointNumber));
}

void MapViewNaviClass::_setCalcDist(double dist)
{
	Dist calcDist_= _kmToDist(dist);
	marshDistKm->display(calcDist_.km);
	marshDistM->display(calcDist_.m);
}

void MapViewNaviClass::_setCalcTime(double time)
{
	QTime calcTime_ = _minToTime(time);
	timeToArrive_h->display(calcTime_.hour());
	timeToArrive_m->display(calcTime_.minute());
}

void MapViewNaviClass::_setRouteAngle(double angle)
{
	short routeAngle = angle*CCT_180_PI;
	if (routeAngle > 180)
		routeAngle -= 360;
	marshDegr->display(QString::number(routeAngle));
}

void MapViewNaviClass::_setPointAngle(double angle)
{
	short pointAngle = angle*CCT_180_PI;
	if (pointAngle > 180)
		pointAngle -= 360;
	pointDegr->display(QString::number(pointAngle));

}

void MapViewNaviClass::_setTrueAngle(double angle)
{
	short currentAngle = angle*CCT_180_PI;
	currentDegr->display(QString::number(currentAngle));
}

void  MapViewNaviClass::_setNaviPixmap(double angle)
{
	short anglePixmap(angle*CCT_180_PI);
	pixLblNapr->setPixmap(QPixmap(CalcFunctions::rotatePixmap(anglePixmap,":/Pyramid/arrow.png")));
}

void MapViewNaviClass::_slotButtonClickSound()
{
	CalcFunctions::soundPressedBut(5);
}