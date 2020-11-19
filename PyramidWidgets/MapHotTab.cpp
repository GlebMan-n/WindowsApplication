#include <PyramidWidgets\MapHotTab.h>	
#include <PyramidM\MSVC_2008\CalcFunctions.h>

MapHotTab::MapHotTab(QWidget* parent)
{
	this->setParent(parent);
	setupUi(this);
	this->setGeometry(CalcFunctions::calcWindowPos(parent->geometry(),this->geometry()) );

	sim->hide();
	ipWireless->hide();
	connect(gps,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(wifi,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(indicators,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(bluetooth,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(sim,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(ipWireless,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));


	connect(gps,SIGNAL(clicked()),SLOT(_slotSetGps()));
	connect(wifi,SIGNAL(clicked()),SLOT(_slotSetWiFi()));
	connect(indicators,SIGNAL(clicked()),SLOT(_slotSetIndicator()));
	connect(bluetooth,SIGNAL(clicked()),SLOT(_slotSet3g()));
	connect(sim,SIGNAL(clicked()),SLOT(_slotSetBluetooth()));
	connect(ipWireless,SIGNAL(clicked()),SLOT(_slotSetIpWireless()));
	
}

void MapHotTab::init()
{
	emit signGetGpsStatus();
	emit signGet3gStatus();
	emit signGetWiFiStatus();
	emit signGetBluetoothStatus();
	emit signGetIndicatorStatus();
	emit signGetIpWirelessStatus();
	emit signGetCamStatus();
	emit signGetBit3Stat();
}
MapHotTab::~MapHotTab()
{
}

void MapHotTab::_slotButtonClickSound()
{
	CalcFunctions::soundPressedBut(5);
}

////////////////////////////
//получение ответа
void MapHotTab::_slotStatusBluetooth(bool status)
{
	this->bluetooth->setChecked(status);
}

void MapHotTab::_slotStatusGps(bool status)
{
	this->gps->setChecked(status);
}

void MapHotTab::_slotStatus3G(bool status)
{
	this->sim->setChecked(status);
}

void MapHotTab::_slotStatusWiFi(bool status)
{
	this->wifi->setChecked(status);
}

void MapHotTab::_slotStatusCam(bool status)
{
	//this->bluetooth->setChecked(status);
}

void MapHotTab::_slotStatusBit3(bool status)
{
	//this->bluetooth->setChecked(status);
}

void MapHotTab::_slotStatusInd(bool status)
{
	this->indicators->setChecked(status);
}

void MapHotTab::_slotStatusIpWireless(bool status)
{
	this->ipWireless->setChecked(status);
}
//////////////////////////////
//по нажатию кнопки
void MapHotTab::_slotSetGps()
{
	emit signSetGps(gps->isChecked());
}

void MapHotTab::_slotSet3g()
{
	emit signSet3g(sim->isChecked());
}

void MapHotTab::_slotSetWiFi()
{
	emit signSetWiFi(wifi->isChecked());
}

void MapHotTab::_slotSetBluetooth()
{
	emit signSetBluetooth(bluetooth->isChecked());
}

void MapHotTab::_slotSetIndicator()
{
	emit signSetIndicator(indicators->isChecked());
}

void MapHotTab::_slotSetIpWireless()
{
	emit signSetIpWireless(ipWireless->isChecked());
}
