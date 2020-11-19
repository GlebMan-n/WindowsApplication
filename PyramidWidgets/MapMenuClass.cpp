#include <PyramidWidgets\MapMenuClass.h>
#include <PyramidM\MSVC_2008\CalcFunctions.h>

MapMenuClass::MapMenuClass(QWidget* parent)
{
	_parent = parent;
	this->setParent(parent);
	setupUi(this);

	QRect initRect =_calcWindowPos();
	initRect.setY(_parent->height());

	this->setGeometry(initRect);
	_addWindowAnim();	

	connect(this->rotate_but,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(this->dist_but,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(this->scale_but,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(this->search_but,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(this->exchange_but,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(this->naviBut,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(this->referance,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
}

MapMenuClass::~MapMenuClass()
{
	
}

void MapMenuClass::slotShow()
{
	this->show();
	emit signShow();
	
}

QRect MapMenuClass::_calcWindowPos()
{
	QRect pRect = _parent->geometry();
	QRect mRect = this->geometry();
	QRect result;
	result.setX(pRect.width()/2-mRect.width()/2);
	result.setY(pRect.height()/2-mRect.height()/2);
	result.setWidth(mRect.width());
	result.setHeight(mRect.height());
	return result;
}

void MapMenuClass::_addWindowAnim()
{
	QRect initRect =_calcWindowPos();
	initRect.setY(_parent->height());

	QState* st1;
	QState* st2;
	st1  = new QState();
	st2 =  new QState();

	QRect initRect1 =_calcWindowPos();
	initRect1.setY(_parent->height());

	st1->assignProperty(this, "geometry", _calcWindowPos());
	st2->assignProperty(this, "geometry", initRect1);

	st1->addTransition(this,SIGNAL(signShow()),st2);
    st2->addTransition(this,SIGNAL(signShow()),st1);
  
	machine_.addState(st1);
    machine_.addState(st2);
    machine_.setInitialState(st1);
	QPropertyAnimation* an1 = new QPropertyAnimation(this, "geometry");
	machine_.addDefaultAnimation(an1);
	an1->setEasingCurve(QEasingCurve::OutQuint);
	short duration = 1200 - _parent->height();	
	an1->setDuration(duration);
	machine_.start();
}

void MapMenuClass::_slotButtonClickSound()
{
	CalcFunctions::soundPressedBut(5);
}