#include <PyramidWidgets\MapSelectTools.h>	
#include <PyramidM\MSVC_2008\CalcFunctions.h>

MapSelectToolClass::MapSelectToolClass(QWidget* parent)
{
	this->setParent(parent);
	setupUi(this);
	this->setGeometry(CalcFunctions::calcWindowPos(parent->geometry(),this->geometry()) );
	connect(distLine,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(breakDistLine,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	//connect(manualRoute,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));

	connect(distLine,SIGNAL(clicked()),SIGNAL(signLineTool()));
	connect(breakDistLine,SIGNAL(clicked()),SIGNAL(signBreakLineTool()));
	//connect(manualRoute,SIGNAL(clicked()),SIGNAL(signManualRoute()));
}

MapSelectToolClass::~MapSelectToolClass()
{
}

void MapSelectToolClass::_slotButtonClickSound()
{
	CalcFunctions::soundPressedBut(5);
}