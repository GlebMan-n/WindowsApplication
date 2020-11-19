#include <PyramidWidgets\MapScaleClass.h>	
#include <PyramidM\MSVC_2008\CalcFunctions.h>

MapScaleClass::MapScaleClass(QWidget* parent)
{
	this->setParent(parent);
	setupUi(this);
	this->setGeometry( CalcFunctions::calcWindowPos(parent->geometry(),this->geometry()) );
	k = 1000;
	kk = 1000000;
	connect(this->scale1k_but,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(this->scale10k_but,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(this->scale25k_but,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(this->scale50k_but,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));

	connect(this->scale100k_but,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(this->scale200k_but,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(this->scale500k_but,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(this->scale1mln_but,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));

	connect(this->scale4mln_but,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(this->scale5mln_but,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(this->scale8mln_but,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(this->scale40mln_but,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));

	connect(this->scale1k_but,SIGNAL(clicked()),SLOT(_slot1k()));
	connect(this->scale10k_but,SIGNAL(clicked()),SLOT(_slot10k()));
	connect(this->scale25k_but,SIGNAL(clicked()),SLOT(_slot25k()));
	connect(this->scale50k_but,SIGNAL(clicked()),SLOT(_slot50k()));

	connect(this->scale100k_but,SIGNAL(clicked()),SLOT(_slot100k()));
	connect(this->scale200k_but,SIGNAL(clicked()),SLOT(_slot200k()));
	connect(this->scale500k_but,SIGNAL(clicked()),SLOT(_slot500k()));
	connect(this->scale1mln_but,SIGNAL(clicked()),SLOT(_slot1kk()));

	connect(this->scale4mln_but,SIGNAL(clicked()),SLOT(_slot4kk()));
	connect(this->scale5mln_but,SIGNAL(clicked()),SLOT(_slot5kk()));
	connect(this->scale8mln_but,SIGNAL(clicked()),SLOT(_slot8kk()));
	connect(this->scale40mln_but,SIGNAL(clicked()),SLOT(_slot40kk()));

}

MapScaleClass::~MapScaleClass()
{

}

void MapScaleClass::_slot1k(){emit signScale(1*k);};
void MapScaleClass::_slot10k(){emit signScale(10*k);};
void MapScaleClass::_slot25k(){emit signScale(25*k);};
void MapScaleClass::_slot50k(){emit signScale(50*k);};

void MapScaleClass::_slot100k(){emit signScale(100*k);};
void MapScaleClass::_slot200k(){emit signScale(200*k);};
void MapScaleClass::_slot500k(){emit signScale(500*k);};
void MapScaleClass::_slot1kk(){emit signScale(1*kk);};

void MapScaleClass::_slot4kk(){emit signScale(4*kk);};
void MapScaleClass::_slot5kk(){emit signScale(5*kk);};
void MapScaleClass::_slot8kk(){emit signScale(8*kk);};
void MapScaleClass::_slot40kk(){emit signScale(40*kk);};

void MapScaleClass::_slotButtonClickSound()
{
	CalcFunctions::soundPressedBut(5);
}