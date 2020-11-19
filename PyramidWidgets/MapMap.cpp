#include <PyramidWidgets\MapMap.h>
#include <PyramidM\MSVC_2008\CalcFunctions.h>

MapMapClass::MapMapClass(QWidget* parent)
{
	this->setParent(parent);
	setupUi(this);
	this->setGeometry(CalcFunctions::calcWindowPos(parent->geometry(),this->geometry()) );
	connect(selectMap,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(lasso,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(dayNight,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(gotoPoint,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(layers,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(gridBut,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(selectMap,SIGNAL(clicked()),SIGNAL(signSelectMap()));
	connect(lasso,SIGNAL(clicked()),SIGNAL(signLasso()));
	connect(dayNight,SIGNAL(clicked()),SIGNAL(signDayNight()));
	connect(gotoPoint,SIGNAL(clicked()),SIGNAL(signGoToPoint()));
	connect(layers,SIGNAL(clicked()),SIGNAL(signLayers()));
}

MapMapClass::~MapMapClass()
{
}

void MapMapClass::_slotButtonClickSound()
{
	CalcFunctions::soundPressedBut(5);
}