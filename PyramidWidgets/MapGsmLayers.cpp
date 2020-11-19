#include <PyramidWidgets\MapGsmLayers.h>	
#include <PyramidM\MSVC_2008\CalcFunctions.h>

MapGsmLayers::MapGsmLayers(QWidget* parent)
{
	this->setParent(parent);
	setupUi(this);
	this->setGeometry(CalcFunctions::calcWindowPos(parent->geometry(),this->geometry()) );

	connect(beeline,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(megafon,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(mts,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(tele2,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));

	connect(beeline,SIGNAL(clicked()),SLOT(_slotShowBeeline()));
	connect(megafon,SIGNAL(clicked()),SLOT(_slotShowMegafon()));
	connect(mts,SIGNAL(clicked()),SLOT(_slotShowMts()));
	connect(tele2,SIGNAL(clicked()),SLOT(_slotShowTele2()));
	
}

void MapGsmLayers::init()
{
	emit signGetStatusGsmLayers();
}

void MapGsmLayers::_slotStatusGsmLayers(bool megafon_, 
										bool beeline_,
										bool mts_, 
										bool tele2_)
{
	megafon->setChecked(megafon_);
	beeline->setChecked(beeline_);
	mts->setChecked(mts_);
	tele2->setChecked(tele2_);

}

MapGsmLayers::~MapGsmLayers()
{
}

void MapGsmLayers::_slotButtonClickSound()
{
	CalcFunctions::soundPressedBut(5);
}

void MapGsmLayers::_slotShowMegafon()
{
	emit signShowMegafon(megafon->isChecked());
}

void MapGsmLayers::_slotShowBeeline()
{
	emit signShowBeeline(beeline->isChecked());
}

void MapGsmLayers::_slotShowMts()
{
	emit signShowMts(mts->isChecked());
}

void MapGsmLayers::_slotShowTele2()
{
	emit signShowTele2(tele2->isChecked());
}
