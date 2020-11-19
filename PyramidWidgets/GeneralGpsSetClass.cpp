#include <windows.h>
#include "GeneralGpsSetClass.h"
#include <PyramidM\MSVC_2008\CalcFunctions.h>
#include <PyramidWidgets\SelectDialogClass.h>

GeneralGpsSetClass::GeneralGpsSetClass(QWidget *parent)
{
	this->setParent(parent);
	_parent = parent;

	firstRun = true;
	setupUi(this);
	_dlg = NULL;
	this->setGeometry(parent->geometry());
	_loadSettings();
	
	//звуки
	//connect(this->apply_but,SIGNAL(clicked()),SLOT(_slotButtonClickSound() ) );
	connect(this->back_but,SIGNAL(clicked()),SLOT(_slotButtonClickSound() ) );
	//connect(this->test_con_but,SIGNAL(clicked()),SLOT(_slotButtonClickSound() ) );
//	connect(this->cancel_but,SIGNAL(clicked()),SLOT(_slotButtonClickSound() ) );
	//connect(this->gpsStringsBut,SIGNAL(clicked()),SLOT(_slotButtonClickSound() ) );

	//connect(this->apply_but,SIGNAL(clicked()),SLOT(_slotApply() ) );
	connect(this->back_but,SIGNAL(clicked()),SIGNAL(signBack() ) );
	//connect(this->test_con_but,SIGNAL(clicked()),SLOT(_slotTest() ) );
	//connect(this->cancel_but,SIGNAL(clicked()),SLOT(_slotCancel() ) );
	//connect(this->gpsStringsBut,SIGNAL(clicked()),SIGNAL(signCurrentData() ) );
	this->setEnabled(false); 
	this->show();
	//инициализация
	m_timer = new QTimer(this);
	connect(m_timer, SIGNAL( timeout ()), 
		this, SLOT (_slotSearchGps ()));
	m_timer->start (250);

	_initTime();
	emit signGetNumbOfGps();

}
void GeneralGpsSetClass::_slotSearchGps()
{
	if(firstRun)
	{
		firstRun = false;
	}
	else
	{

		m_timer->stop();
		delete m_timer;
		m_timer = NULL;
		this->setEnabled(true); 
		_searchGps();
	}
}
void GeneralGpsSetClass::_slotApply()
{
	emit signUpdateSettings();
}

void GeneralGpsSetClass::_slotTest()
{
	emit signGpsRestart();
}

GeneralGpsSetClass::~GeneralGpsSetClass()
{

}

void GeneralGpsSetClass::_slotTime(QString day, QString sdate, QString stime)
{
	this->gps_sett_day_lbl->setText(day);
	this->gps_sett_date_lbl->setText(sdate);
	this->gps_sett_time_lbl->setText(stime);
	
}



//получаем статус GPS
void  GeneralGpsSetClass::_slotIsGps(bool gps)
{
	if(gps)
		testConGpsLbl->setStyleSheet(QString::fromUtf8(" image: url(:/Pyramid/yes_gps.png);"));
	else
	{
		testConGpsLbl->setStyleSheet(QString::fromUtf8(" image: url(:/Pyramid/no_gps.png);"));
		this->gpsQuality->setText(tr("нет"));
		satNumb->setText(QString::number(0));
	}
}

void  GeneralGpsSetClass::_setGpsSettings(QString com, QString brate, QString bsize, QString par, QString stopb)
{
	//int index = com_cb->findText(com);
	////if ( index != -1 ) { // -1 for not found
	////  this->com_cb->setCurrentIndex(index);
	////}

	//index = baudrate_cb->findText(brate);
	//if ( index != -1 ) { // -1 for not found
	//   baudrate_cb->setCurrentIndex(index);
	//}


	//index = bytesyte_cb->findText(bsize);
	//if ( index != -1 ) { // -1 for not found
	//   bytesyte_cb->setCurrentIndex(index);
	//}


	//index = parity_cb->findText(par);
	//if ( index != -1 ) { // -1 for not found
	//   parity_cb->setCurrentIndex(index);
	//}

	//index = stopbits_cb->findText(stopb);
	//if ( index != -1 ) { // -1 for not found
	//  stopbits_cb->setCurrentIndex(index);
	//}
}

bool GeneralGpsSetClass::_loadSettings()
{
	QString path_to_ini = QCoreApplication::applicationDirPath () + "/settings_akps.ini";
	if (QFile::exists(path_to_ini) ) 
	{
		QSettings akps_settings (path_to_ini, QSettings::IniFormat);
		QString com, brate, bsize, par, stopb;
		
		com = akps_settings.value("gps/qport").toString();
		brate = akps_settings.value("gps/qbrate").toString();
		bsize  = akps_settings.value("gps/qbsize").toString();
		par = akps_settings.value("gps/qparity").toString();
		stopb = akps_settings.value("gps/qstopbits").toString();
		_setGpsSettings(com, brate, bsize, par, stopb);
		return true;
	}
	return false;
}

void GeneralGpsSetClass::_saveSettings()
{
	//QSettings akps_settings (QCoreApplication::applicationDirPath () + "/settings_akps.ini", QSettings::IniFormat);

	//short ind = com_cb->currentIndex();
	//QString comPort = comPorts.at(ind);
	//QString com(comPort);
	//QString brate(baudrate_cb->currentText());
	//QString bsize(bytesyte_cb->currentText());
	//QString par(parity_cb->currentText());
	//QString stopb(stopbits_cb->currentText());

	//akps_settings.setValue("gps/qport", com);
	//akps_settings.setValue("gps/qbrate", brate);
	//akps_settings.setValue("gps/qbsize", bsize);
	//akps_settings.setValue("gps/qparity", par);
	//akps_settings.setValue("gps/qstopbits", stopb);
	//akps_settings.sync();
}

void GeneralGpsSetClass::_slotCancel()
{
	_loadSettings();
}

void GeneralGpsSetClass::_initTime()
{
	QString day, sdate, stime;
	SYSTEMTIME st; 
	GetLocalTime(&st);
	QTime time(st.wHour,st.wMinute);
	QDate date(st.wYear, st.wMonth, st.wDay);
	stime = time.toString("HH:mm");
	sdate = date.toString("dd.MM.yyyy");
	day = date.toString("dddd");
	day[0] = day[0].toUpper(); 
	_slotTime(day, sdate, stime);
	
}

void GeneralGpsSetClass::_slotButtonClickSound()
{
	CalcFunctions::soundPressedBut(5);
}

void GeneralGpsSetClass::_slotComList(QStringList comList, QStringList comTypesList )
{
	/*if(_dlg)
	{
		delete _dlg;
		_dlg = NULL;
	}
	
	if(!comList.isEmpty())
	{

		com_cb->clear();
		com_cb->addItems(comTypesList);
		comPorts = comList;
		_slotApply();
		_slotTest();
	}
	else
	{
		SelectDialog dlg(_parent,tr("Мастер"),
		tr("Подключите GPS устройство, \nили завершите другие программы\nего использующие"));
		dlg.exec();
		this->close();
		signBack();
	}*/
}

void GeneralGpsSetClass::_slotBadSign()
{
	//this->gpsQuality->setText(tr("низкое"));
	testConGpsLbl->setStyleSheet(QString::fromUtf8(" image: url(:/Pyramid/yes_gps.png);"));
}
void GeneralGpsSetClass::_searchGps()
{
	//emit signScanCom();
}

void GeneralGpsSetClass::_slotSetCurrentData(QString string,bool check)
{
	QString tmp_;
	tmp_ = this->typeMessageCB->currentText();
	
	if(string.startsWith("$GPGGA"))
	{
		float sat = string.section(',', 7, 7).toInt();
		satNumb->setText(QString::number(sat));
		if (sat > 3)
		{
			this->gpsQuality->setText(tr("отличное"));
		}

		else
		{
			this->gpsQuality->setText(tr("низкое"));
		}
	}


	if (check && tmp_ == "$ALL")
	{
		QString tmp = string + this->curr->toPlainText ();
		if (tmp.length() > 1150) tmp = string;
		this->curr->setPlainText(tmp);
	}
	else
	{
		if(check && string.startsWith(tmp_))
		{
			QString tmp = string + this->curr->toPlainText ();
			if (tmp.length() > 1150) tmp = string;
			this->curr->setPlainText(tmp);
		}
	}

}

void GeneralGpsSetClass::_slotSetCurrentDataErrors(QString string)
{
		QString tmp = string + this->curr->toPlainText ();
		if (tmp.length() > 1150) tmp = string;
		this->curr->setPlainText(tmp);
}

void GeneralGpsSetClass::_slotNumbOfGps(short numb)
{
	this->gpsCount->setText(QString::number(numb));
}

void GeneralGpsSetClass::init()
{
	emit signGetNumbOfGps();
}