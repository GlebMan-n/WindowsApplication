#include <CurrentGpsData.h>
#include <PyramidM\MSVC_2008\CalcFunctions.h>

CurrentGpsData::CurrentGpsData(QWidget *parent)
{
	this->setParent(parent);
	setupUi(this);
	this->setGeometry(parent->geometry());
	connect(this->pauseBut,SIGNAL(clicked()),SLOT(slotPause()));

	connect(typeMessageCB,SIGNAL(highlighted(int)),SLOT(_slotButtonClickSound()));
	connect(pauseBut,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(cancelButton,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	this->setGeometry(parent->geometry());
	_counter = 10;
	_show_data = !pauseBut->isChecked();
}

void CurrentGpsData::slotSetCurrentData(QString string,bool check)
{
	QString tmp_;
	tmp_ = this->typeMessageCB->currentText();
	
	if (_show_data && check && tmp_ == "$ALL")
	{
		QString tmp = string + this->curr->toPlainText ();
		if (tmp.length() > 1150) tmp = string;
		this->curr->setPlainText(tmp);
	}
	else
	{
		if(_show_data && check && string.startsWith(tmp_))
		{
			QString tmp = string + this->curr->toPlainText ();
			if (tmp.length() > 1150) tmp = string;
			this->curr->setPlainText(tmp);
		}
	}
}

void CurrentGpsData::slotPause()
{
	_show_data = !pauseBut->isChecked();
}

void CurrentGpsData::slotSetCurrentDataErrors(QString string)
{
		QString tmp = string + this->curr->toPlainText ();
		if (tmp.length() > 1150) tmp = string;
		this->curr->setPlainText(tmp);
}

void CurrentGpsData::_slotButtonClickSound()
{
	CalcFunctions::soundPressedBut(5);
}