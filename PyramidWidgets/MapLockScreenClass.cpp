#include <PyramidWidgets\MapLockScreenClass.h>
#include <PyramidM\MSVC_2008\CalcFunctions.h>

MapLockScreenClass::MapLockScreenClass(QWidget *parent)
{
	this->setParent(parent);
	setupUi(this);
	this->setGeometry(parent->geometry());
	b_btnPressed = false;
	_x = 0;	
	_y = but->y();
	_xPred = but->x() + lockWgt->x() + 290;
	_xPred1 = but->x() + lockWgt->x() ;
}

void MapLockScreenClass::init()
{
	emit signBrightMin();
}

void MapLockScreenClass::mousePressEvent ( QMouseEvent * event )
{
	if(checkButtonClicked(event->pos ().x(),event->pos ().y()))
		b_btnPressed = true;
	else
		b_btnPressed = false;
}

void MapLockScreenClass::mouseReleaseEvent ( QMouseEvent * event )
{
	b_btnPressed = false;
	this->but->setGeometry(30,43,120,83);
	_x = 0;
	_slotButtonClickSound();
}

void MapLockScreenClass::mouseMoveEvent(QMouseEvent* pe)
{

	if(b_btnPressed)
	{
		double butX;
		
		double delta = pe->pos ().x() - (lockWgt->x() + 30);
			this->but->setGeometry(delta-42,_y,120,83);

		butX = but->x();
		if(butX >  315)
		{
			_slotButtonClickSound();
			emit signBrightMax();
			this->close();
		}

		if(butX < 30)
			this->but->setGeometry(30,43,120,83);

		but->show();
	}	
}

bool MapLockScreenClass::checkButtonClicked(double x, double y)
{
	double bx = lockWgt->x() + but->x();
	double by = lockWgt->y() +  but->y();
	double _bx = bx + but->width();
	double _by = by + but->height();

	if( x < _bx && x > bx &&  y < _by && y > by )
		return true;
	else
		return false;
}

void MapLockScreenClass::_slotButtonClickSound()
{
	CalcFunctions::soundPressedBut(5);
}