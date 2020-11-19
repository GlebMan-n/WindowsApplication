#include <PyramidWidgets\SelectDialogClass.h>
#include <PyramidM\MSVC_2008\CalcFunctions.h>

SelectDialog::SelectDialog(QWidget *parent,QString caption, QString text,bool trBackg,short closeTimeSec)
{
	this->setParent(parent);
	setupUi(this);
	setText(text);
	setWindowCaption(caption);
	result = 0;
	timer = NULL;
	connect(this->okButton,SIGNAL(clicked()),this,SLOT(_slotButtonClickSound()));
	connect(this->cancelButton,SIGNAL(clicked()),this,SLOT(_slotButtonClickSound()));
	connect(this->okButton,SIGNAL(clicked()),this,SLOT(slotOk()));
	connect(this->cancelButton,SIGNAL(clicked()),this,SLOT(slotCancel()));
	this->setGeometry(parent->geometry());
	if(!trBackg)
	{
		this->setStyleSheet(QString("#SelectDialogClass{border-image: url(:/Pyramid/background.png);}"));
	}

	if(closeTimeSec > 0)
	{
		timer = new QTimer(this);
		connect(timer, SIGNAL( timeout ()), 
			this, SLOT (slotOk()));
		timer->start (closeTimeSec * 1000);
		
	}

}

void SelectDialog::slotOk()
{
	result = 1;
	emit signalOk();
	this->close();
}
void SelectDialog::slotCancel()
{
	result = 0;
	emit signalCancel();
	this->close();
}

void SelectDialog::setText(QString str)
{
	this->label->setText(str);
}

void SelectDialog::setWindowCaption(QString str)
{
	//this->setCaption(str);
}
void SelectDialog::_slotButtonClickSound()
{
	CalcFunctions::soundPressedBut(5);
}
