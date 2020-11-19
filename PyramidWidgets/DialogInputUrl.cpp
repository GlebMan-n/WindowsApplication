#include <PyramidWidgets\DialogInputUrl.h>	
#include <PyramidM\MSVC_2008\CalcFunctions.h>

DialogInputUrl::DialogInputUrl(QWidget* parent, QStringList listOfAdr)
{
	if(parent)
		this->setParent(parent);
	setupUi(this);
	
	//if(parent)
	//	this->setGeometry(CalcFunctions::calcWindowPos(parent->geometry(),this->geometry()) );
	//else
		this->setGeometry(parent->geometry());
	_setAdress(listOfAdr);
	connect(this->apply,SIGNAL(clicked()),SLOT(_slotApply() ) );
	connect(this->cancel,SIGNAL(clicked()),SLOT(_slotCancel() ) );
	connect(this->keyboard,SIGNAL(clicked()),SLOT(_slotKeyboard() ) );
	
}

DialogInputUrl::~DialogInputUrl()
{
}

void DialogInputUrl::_setAdress(QStringList listOfAdr)
{
	this->kcpsOUT->setText(listOfAdr.at(0));
	this->kcpsIN->setText(listOfAdr.at(1));
	this->shtabOUT->setText(listOfAdr.at(2));
	this->shtabIN->setText(listOfAdr.at(3));
	this->vpuOUT->setText(listOfAdr.at(4));
	this->vpuIN->setText(listOfAdr.at(5));
	this->gkcpsOUT->setText(listOfAdr.at(6));
	this->gkcpsIN->setText(listOfAdr.at(7));
}

void DialogInputUrl::_slotApply()
{
	result 
		<< kcpsOUT->text() 
		<< kcpsIN->text() 
		<< shtabOUT->text() 
		<< shtabIN->text()
		<< vpuOUT->text()
		<< vpuIN->text()
		<< gkcpsOUT->text()
		<< gkcpsIN->text();
	this->close();
}

void DialogInputUrl::_slotCancel()
{
	result.clear();
	this->close();
}
void DialogInputUrl::_slotKeyboard()
{
	CalcFunctions::runKeyboard(this);
}
