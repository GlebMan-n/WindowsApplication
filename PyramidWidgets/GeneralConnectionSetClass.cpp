
#include <PyramidWidgets\GeneralConnectionSetClass.h>
#include <PyramidM\MSVC_2008\CalcFunctions.h>
#include <PyramidWidgets\DialogInputUrl.h>	

GeneralConnectionSetClass::GeneralConnectionSetClass(QWidget *parent)
{
	this->setParent(parent);
	setupUi(this);
	this->setGeometry(parent->geometry());
	_parent = parent;
	//значения по умолчанию
	_idDevice =  "tablet1";
	_timeToSend = 30;
	_nameOfPc = CalcFunctions::nameOfPc();
	

	if(!_loadSettings())
	{
		_saveSettings();
		_loadSettings();
	}
	
	connect(this->con_set_apply_but,SIGNAL(clicked()),SLOT(_slotButtonClickSound() ) );
	connect(this->con_set_back_but,SIGNAL(clicked()),SLOT(_slotButtonClickSound() ) );
	connect(this->con_set_cancel_but,SIGNAL(clicked()),SLOT(_slotButtonClickSound() ) );
	connect(defSrv,SIGNAL(highlighted(int)),SLOT(_slotButtonClickSound()));
	connect(this->keyboardBut,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));

	connect(this->con_set_apply_but,SIGNAL(clicked()),SLOT(_slotApply() ) );
	connect(this->con_set_back_but,SIGNAL(clicked()),SLOT(_slotBack() ) );
	connect(this->con_set_cancel_but,SIGNAL(clicked()),SLOT(_slotCancel() ) );
	connect(defSrv,SIGNAL(currentIndexChanged (int)),SLOT(_slotDefSrv(int)));
	connect(this->keyboardBut,SIGNAL(clicked()),SLOT(_slotKeyboard()));
	connect(this->urlBrowse,SIGNAL(clicked()),SLOT(_slotUrlDlg()));
	_initTime();

}

GeneralConnectionSetClass::~GeneralConnectionSetClass()
{
}

bool GeneralConnectionSetClass::_loadSettings()
{
	QString path_to_ini = QCoreApplication::applicationDirPath () + "/settings_akps.ini";

	if (QFile::exists(path_to_ini) ) 
	{
		QSettings akps_settings (path_to_ini, QSettings::IniFormat);

		_urlKcpsIN = akps_settings.value("programm/url_to_kcpsIN").toString();
		_urlKcpsOUT = akps_settings.value("programm/url_to_kcpsOUT").toString();

		_urlVpuIN = akps_settings.value("programm/url_to_vpuIN").toString();
		_urlVpuOUT = akps_settings.value("programm/url_to_vpuOUT").toString();

		_url_to_gkcpsIN = akps_settings.value("programm/url_to_gkcpsIN").toString();
		_url_to_gkcpsOUT = akps_settings.value("programm/url_to_gkcpsOUT").toString();

		_url_to_shtabIN = akps_settings.value("programm/url_to_shtabIN").toString();
		_url_to_shtabOUT = akps_settings.value("programm/url_to_shtabOUT").toString();

		_idDevice = akps_settings.value("programm/id").toString();
		_nameOfPc = CalcFunctions::nameOfPc();
		_timeToSend = akps_settings.value("programm/time_to_send_data").toInt();
		_defSrv = akps_settings.value("programm/def_serv").toInt();

		_setSettings(_urlKcpsIN, _urlKcpsOUT, _idDevice,_nameOfPc ,_timeToSend, _defSrv);
		return true;
	}
	return false;
}

void GeneralConnectionSetClass::_saveSettings()
{
	QSettings akps_settings (QCoreApplication::applicationDirPath () + "/settings_akps.ini", QSettings::IniFormat);

	_nameOfPc = CalcFunctions::nameOfPc();
	_idDevice = id_device->text();

	_timeToSend = time_to_exch->text().toInt();

	akps_settings.setValue("programm/pc_name", _nameOfPc);
	akps_settings.setValue("programm/id", _idDevice);
	
	akps_settings.setValue("programm/url_to_kcpsIN", _urlKcpsIN);
	akps_settings.setValue("programm/url_to_kcpsOUT", _urlKcpsOUT);

	akps_settings.setValue("programm/url_to_vpuIN", _urlVpuIN);
	akps_settings.setValue("programm/url_to_vpuOUT", _urlVpuOUT);

	akps_settings.setValue("programm/url_to_gkcpsIN", _url_to_gkcpsIN);
	akps_settings.setValue("programm/url_to_gkcpsOUT",_url_to_gkcpsOUT);

	akps_settings.setValue("programm/url_to_shtabIN", _url_to_shtabIN);
	akps_settings.setValue("programm/url_to_shtabOUT", _url_to_shtabOUT);

	akps_settings.setValue("programm/time_to_send_data", _timeToSend);
	akps_settings.setValue("programm/def_serv", _defSrv);
	akps_settings.sync();
}

void GeneralConnectionSetClass::_initTime()
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

void GeneralConnectionSetClass::_setSettings(QString vpuAdr, QString kcpsAdr, QString idDevice, QString nameOfPc ,short timeToSend, short defSrvV)
{

	id_device->setText(idDevice);
	name_device->setText(nameOfPc);
	time_to_exch->setText(QString::number(timeToSend));
	defSrv->setCurrentIndex (defSrvV);
}

void GeneralConnectionSetClass::_slotTime(QString day, QString sdate, QString stime)
{
	this->day_connect_lbl->setText(day);
	this->date_connect_lbl->setText(sdate);
	this->time_connect_lbl->setText(stime);
}

void GeneralConnectionSetClass::_slotApply()
{
	_saveSettings();
	emit signUpdateSettings();
}

void GeneralConnectionSetClass::_slotCancel()
{
	_loadSettings();
}

void GeneralConnectionSetClass::_slotBack()
{
	emit signDeleteMenu();
	//this->close();
}

void GeneralConnectionSetClass::_slotDefSrv(int serv)
{
	_defSrv = serv;
}

void GeneralConnectionSetClass::_slotKeyboard()
{
	CalcFunctions::runKeyboard(this);
}

void GeneralConnectionSetClass::_slotButtonClickSound()
{
	CalcFunctions::soundPressedBut(5);
}

void GeneralConnectionSetClass::_slotUrlDlg()
{
	QStringList listOfUrls;
	listOfUrls << _urlKcpsOUT << _urlKcpsIN << _url_to_shtabOUT << _url_to_shtabIN << _urlVpuOUT << _urlVpuIN << _url_to_gkcpsOUT << _url_to_gkcpsIN;
	DialogInputUrl dlg(_parent,listOfUrls);
	dlg.exec();
	this->hide();

	if(!dlg.result.isEmpty())
	{
		_urlKcpsOUT = dlg.result.at(0);
		_urlKcpsIN = dlg.result.at(1);
		_url_to_shtabOUT = dlg.result.at(2);
		_url_to_shtabIN = dlg.result.at(3);
		_urlVpuOUT = dlg.result.at(4);
		_urlVpuIN = dlg.result.at(5);
		_url_to_gkcpsOUT = dlg.result.at(6);
		_url_to_gkcpsIN = dlg.result.at(7);
		_slotApply();
	}

	this->show();
}