#include <PyramidWidgets\MapBrightness.h>	
#include <PyramidM\MSVC_2008\CalcFunctions.h>

MapBrightness::MapBrightness(QWidget* parent)
{
	this->setParent(parent);
	setupUi(this);
	this->setGeometry(CalcFunctions::calcWindowPos(parent->geometry(),this->geometry()) );

	connect(brightDown,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(brightUp,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));

	connect(brightDown,SIGNAL(clicked()),SLOT(_slotBrightMinus()));
	connect(brightUp,SIGNAL(clicked()),SLOT(_slotBrightPlus()));	
}
void MapBrightness::init()
{
	emit signGetBright();
}
MapBrightness::~MapBrightness()
{
}

void MapBrightness::_slotButtonClickSound()
{
	CalcFunctions::soundPressedBut(5);
}

void MapBrightness::_slotBrightMinus()
{
	emit signBrightDown();
}

void MapBrightness::_slotBrightPlus()
{
	emit signBrightUp();
}

void MapBrightness::_slotGetBright(short bright)
{
	switch (bright)
	{
		case 0:
			brightnessInd->setPixmap(QPixmap(QString::fromUtf8(":/Pyramid/brightness1.png")));
			break;
		case 1:
			brightnessInd->setPixmap(QPixmap(QString::fromUtf8(":/Pyramid/brightness2.png")));
			break;
		case 2:
			brightnessInd->setPixmap(QPixmap(QString::fromUtf8(":/Pyramid/brightness3.png")));
			break;
		case 3:
			brightnessInd->setPixmap(QPixmap(QString::fromUtf8(":/Pyramid/brightness4.png")));
			break;
		case 4:
			brightnessInd->setPixmap(QPixmap(QString::fromUtf8(":/Pyramid/brightness5.png")));
			break;
		case 5:
			brightnessInd->setPixmap(QPixmap(QString::fromUtf8(":/Pyramid/brightness6.png")));
			break;
		case 6:
			brightnessInd->setPixmap(QPixmap(QString::fromUtf8(":/Pyramid/brightness7.png")));
			break;
		case 7:
			brightnessInd->setPixmap(QPixmap(QString::fromUtf8(":/Pyramid/brightness8.png")));
			break;
		case 8:
			brightnessInd->setPixmap(QPixmap(QString::fromUtf8(":/Pyramid/brightness9.png")));
			break;
		default:
			brightnessInd->setPixmap(QPixmap(QString::fromUtf8(":/Pyramid/brightness4.png")));
	}
}