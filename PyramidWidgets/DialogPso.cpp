#include <PyramidWidgets\DialogPso.h>
#include <PyramidM\MSVC_2008\CalcFunctions.h>
#include <PyramidWidgets\SelectDialogClass.h>

PsoDialog::PsoDialog(QWidget *parent)
{
	this->setParent(parent);
	_parent = parent;
	setupUi(this);
	result = 0;

	connect(this->newPso,SIGNAL(clicked()),this,SLOT(_slotButtonClickSound()));
	connect(this->continuePso,SIGNAL(clicked()),this,SLOT(_slotButtonClickSound()));
	connect(this->newPso,SIGNAL(clicked()),this,SLOT(slotNew()));
	connect(this->continuePso,SIGNAL(clicked()),this,SLOT(slotCont()));
	this->setGeometry(parent->geometry());
	this->setStyleSheet(QString("#psoDlg{border-image: url(:/Pyramid/background.png);}"));
}

void PsoDialog::slotNew()
{
	SelectDialog dlg1(_parent,tr("Мастер"),
		tr("\nНачать новую ПСО\nстарые треки, фото\nи прочие данные будут удалены"),true);
	this->hide();
	dlg1.exec();
	
	if(dlg1.result == 1)
	{
		result = 1;
		emit signalNew();
		this->close();
	}
	emit signalCont();
	this->show();
}
void PsoDialog::slotCont()
{
	result = 0;
	emit signalCont();
	this->close();
}

void PsoDialog::_slotButtonClickSound()
{
	CalcFunctions::soundPressedBut(5);
}

void PsoDialog::setDateTime(QString date)
{
	this->datePsoLabel->setText(date);
}