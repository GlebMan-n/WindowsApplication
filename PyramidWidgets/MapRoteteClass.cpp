#include <PyramidWidgets\MapRoteteClass.h>	
#include <PyramidM\MSVC_2008\CalcFunctions.h>

MapRotateClass::MapRotateClass(QWidget* parent)
{
	this->setParent(parent);
	setupUi(this);
	this->setGeometry( CalcFunctions::calcWindowPos(parent->geometry(),this->geometry()) );
	connect(rotate_noth_but,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(rotate_south_but,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(rotate_move_but,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));

	connect(rotate_noth_but,SIGNAL(clicked()),SIGNAL(signNothOrient()));
	connect(rotate_south_but,SIGNAL(clicked()),SIGNAL(signSouthOrient()));
	connect(rotate_move_but,SIGNAL(clicked()),SIGNAL(signMoveOrient()));
}

MapRotateClass::~MapRotateClass()
{
}

void MapRotateClass::_slotButtonClickSound()
{
	CalcFunctions::soundPressedBut(5);
}