#include <PyramidWidgets\MapObjects.h>	
#include <PyramidM\MSVC_2008\CalcFunctions.h>

MapObjects::MapObjects(QWidget* parent)
{
	this->setParent(parent);
	setupUi(this);
	this->setGeometry(CalcFunctions::calcWindowPos(parent->geometry(),this->geometry()) );

	connect(routes,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(greeds,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(marks,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(tracks,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(currentPos,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(manualRoute,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));

	connect(routes,SIGNAL(clicked()),SLOT(_slotShowRoute()));
	connect(greeds,SIGNAL(clicked()),SLOT(_slotShowGreed()));
	connect(marks,SIGNAL(clicked()),SLOT(_slotShowMarks()));
	connect(tracks,SIGNAL(clicked()),SLOT(_slotShowTracks()));
	connect(currentPos,SIGNAL(clicked()),SLOT(_slotShowCurrentPos()));
	connect(manualRoute,SIGNAL(clicked()),SLOT(_slotShowManualRoute()));
		
}

void MapObjects::init()
{
	emit signGetStatusObjects();
}

void MapObjects::_slotStatusObjects(bool route_, bool greed_,bool marks_, bool tracks_, bool currentPos_,bool manualRoute_)
{
	routes->setChecked(route_);
	greeds->setChecked(greed_);
	marks->setChecked(marks_);
	tracks->setChecked(tracks_);
	//currentPos->setChecked(currentPos_);
	manualRoute->setChecked(manualRoute_);
}

MapObjects::~MapObjects()
{
}

void MapObjects::_slotButtonClickSound()
{
	CalcFunctions::soundPressedBut(5);
}

void MapObjects::_slotShowRoute()
{
	emit signShowRoute(routes->isChecked());
}

void MapObjects::_slotShowGreed()
{
	emit signShowGreed(greeds->isChecked());
}

void MapObjects::_slotShowMarks()
{
	emit signShowMarks(marks->isChecked());
}

void MapObjects::_slotShowTracks()
{
	emit signShowTracks(tracks->isChecked());
}

void MapObjects::_slotShowCurrentPos()
{
	emit signShowCurrentPos(currentPos->isChecked());
}

void MapObjects::_slotShowManualRoute()
{
	emit signShowManualRoute(manualRoute->isChecked());
}