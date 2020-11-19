#include <PyramidWidgets\DialogGpsSelect.h>
#include <PyramidM\MSVC_2008\CalcFunctions.h>
#include <PyramidWidgets\SelectDialogClass.h>

SelectGpsDial::SelectGpsDial(QWidget *parent,QStringList listOfGps,QStringList typesOfGps,bool trBackg)
{
	this->setParent(parent);
	_parent = parent;
	setupUi(this);
	result = 0;
	connect(this->okButton,SIGNAL(clicked()),this,SLOT(_slotButtonClickSound()));
	connect(this->cancelButton,SIGNAL(clicked()),this,SLOT(_slotButtonClickSound()));
	connect(this->okButton,SIGNAL(clicked()),this,SLOT(_slotOk()));
	connect(this->cancelButton,SIGNAL(clicked()),this,SLOT(_slotCancel()));
	this->setGeometry(parent->geometry());
	if(!trBackg)
	{
		this->setStyleSheet(QString("#SelectGpsDialog{border-image: url(:/Pyramid/background.png);}"));
	}

	_slotComList(listOfGps,typesOfGps);
}

void SelectGpsDial::_slotOk()
{
	_slotCancel();
	result = 1;
	_saveSettings();
	emit singOk();
	this->close();
}
void SelectGpsDial::_slotCancel()
{
	result = 0;
	this->close();
}

void SelectGpsDial::_slotButtonClickSound()
{
	CalcFunctions::soundPressedBut(5);
}

void SelectGpsDial::_slotComList(QStringList comList, QStringList comTypesList )
{
	com_cb->clear();
	com_cb->addItems(comTypesList);
	comPorts = comList;
	
	if(comList.size() > 1)
		this->exec();

	if(comList.size() == 1)
	{
		QString tmp( tr("Найден: ") + comTypesList.at(0) + tr("\nпорт единственный,\nподключаемся к нему") );
		SelectDialog dlg(_parent,tr("Мастер"),tmp,false);
		dlg.okButton->hide();
		dlg.exec();
		_slotOk();
		
	}

	if(comList.size() < 1)
	{
			SelectDialog dlg1(_parent,tr("Мастер"),	tr("Gps не найдены, подключите антенну и\nзайдете в меню настройки GPS"),false);
			dlg1.okButton->hide();
			dlg1.exec();
			_slotCancel();
	}
	
}

void SelectGpsDial::_saveSettings()
{
	QSettings akps_settings (QCoreApplication::applicationDirPath () + "/settings_akps.ini", QSettings::IniFormat);
	QString comPort = comPorts.at(com_cb->currentIndex());
	QString com(comPort);
	akps_settings.setValue("gps/qport", com);
	akps_settings.sync();
}