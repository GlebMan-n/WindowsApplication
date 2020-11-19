#include <PyramidWidgets\DialogAboutClass.h>	
#include <PyramidM\MSVC_2008\CalcFunctions.h>

DialogAboutClass::DialogAboutClass(QWidget* parent,double version)
{
	this->setParent(parent);
	setupUi(this);
	this->setGeometry(parent->geometry());
	_setVer(version);
	connect(this->back_but,SIGNAL(clicked()),SLOT(_slotBack() ) );
	
}

DialogAboutClass::~DialogAboutClass()
{
}

void DialogAboutClass::_setVer(double version)
{
	versionLbl->setText(tr("версия ")+QString::number(version));
}

void DialogAboutClass::_slotBack()
{
	this->close();
}
