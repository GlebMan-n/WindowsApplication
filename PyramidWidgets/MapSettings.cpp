#include <PyramidWidgets\MapSettings.h>	
#include <PyramidM\MSVC_2008\CalcFunctions.h>

MapSettingsClass::MapSettingsClass(QWidget* parent)
{
	this->setParent(parent);
	setupUi(this);
	this->setGeometry(CalcFunctions::calcWindowPos(parent->geometry(),this->geometry()) );

	connect(trackSettings,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(searchGps,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(kmMetr,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(deviceManager,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(brightness,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));

	connect(trackSettings,SIGNAL(clicked()),SIGNAL(signTrackSettings()));
	connect(searchGps,SIGNAL(clicked()),SIGNAL(signSearchGps()));
	connect(kmMetr,SIGNAL(clicked()),SIGNAL(signSpeedFormat()));
	connect(deviceManager,SIGNAL(clicked()),SIGNAL(signDeviceManager()));
	connect(brightness,SIGNAL(clicked()),SIGNAL(signBrightness()));
}

MapSettingsClass::~MapSettingsClass()
{
}

void MapSettingsClass::_slotButtonClickSound()
{
	CalcFunctions::soundPressedBut(5);
}