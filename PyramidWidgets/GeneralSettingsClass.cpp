#include "PyramidWidgets\GeneralSettingsClass.h"
#include <windows.h>
#include <PyramidM\MSVC_2008\CalcFunctions.h>
GeneralSettingsClass::GeneralSettingsClass(QWidget *parent)
{
	this->setParent(parent);
	setupUi(this);
	this->setGeometry(parent->geometry());

	_pathToPhoto = QCoreApplication::applicationDirPath() + "/data/photo/" ;
	_pathToTracks = QCoreApplication::applicationDirPath() + "/data/tracks/" ;
	_timeZone = 0;


	if(!_loadSettings())
	{
		_saveSettings();
		_loadSettings();
	}

	trackFormat->setCurrentIndex(_trackFormat);
	connect(this->GenSetMenu_apply_but,SIGNAL(clicked()),SLOT(_slotApply() ) );
	connect(this->GenSetMenu_back_but,SIGNAL(clicked()),SLOT(_slotBack() ) );
	connect(this->geo_browse,SIGNAL(clicked()),SIGNAL(signGeoDataSettings() ) );
	connect(this->GenSetMenu_cancel_but,SIGNAL(clicked()),SLOT(_slotCancel() ) );
	connect(this->keyboardBut,SIGNAL(clicked()),SLOT(_slotKeyboard()));
	connect(trackFormat,SIGNAL(currentIndexChanged (int)),SLOT(_slotTrackFormatChanged(int)));


	connect(this->GenSetMenu_apply_but,SIGNAL(clicked()),SLOT(_slotButtonClickSound() ) );
	connect(this->GenSetMenu_back_but,SIGNAL(clicked()),SLOT(_slotButtonClickSound() ) );
	connect(this->geo_browse,SIGNAL(clicked()),SLOT(_slotButtonClickSound() ) );
	connect(this->GenSetMenu_cancel_but,SIGNAL(clicked()),SLOT(_slotButtonClickSound() ) );
	connect(this->keyboardBut,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(trackFormat,SIGNAL(highlighted (int)),SLOT(_slotButtonClickSound()));

	_initTime();
	
}
GeneralSettingsClass::~GeneralSettingsClass()
{

}

bool GeneralSettingsClass::_loadSettings()
{
	QString path_to_ini = QCoreApplication::applicationDirPath () + "/settings_akps.ini";
	if (QFile::exists(path_to_ini) ) 
	{
		QSettings akps_settings (path_to_ini, QSettings::IniFormat);

		_pathToPhoto = akps_settings.value("programm/path_to_photo").toString();
		_pathToTracks = akps_settings.value("programm/path_to_track").toString();
		_timeZone = akps_settings.value("programm/time_zone").toInt();
		_trackFormat = akps_settings.value("track/format").toInt(); 

		//пытаемся создать директории
		QDir dir;
		bool ok = dir.mkpath(_pathToTracks);
		if(!ok) qDebug() << "can`t create directory";

		ok = dir.mkpath(_pathToPhoto);
		if(!ok) qDebug() << "can`t create directory";


		_setSettings(_pathToPhoto, _pathToTracks, _timeZone);
		return true;
	}
	return false;
}
void GeneralSettingsClass::_saveSettings()
{
	QSettings akps_settings (QCoreApplication::applicationDirPath () + "/settings_akps.ini", QSettings::IniFormat);
	_pathToTracks = PathToTrack_LE->text();
	_pathToPhoto = PathToPhoto_LE->text();
	_timeZone = timeZone_chb->currentText ().toInt();
	akps_settings.setValue("programm/path_to_track", _pathToTracks);
	akps_settings.setValue("programm/path_to_photo", _pathToPhoto);
	akps_settings.setValue("programm/time_zone", _timeZone);
	akps_settings.setValue("track/format", _trackFormat);
	akps_settings.sync();
}

void GeneralSettingsClass::_initTime()
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

void GeneralSettingsClass::_slotTime(QString day, QString sdate, QString stime)
{
	this->day_gen_set_lbl->setText(day);
	this->date_gen_set_lbl->setText(sdate);
	this->time_gen_set_lbl->setText(stime);
}
void GeneralSettingsClass::_slotApply()
{
	_saveSettings();
	emit singNewTimeZone();
}

void GeneralSettingsClass::_slotCancel()
{
	_loadSettings();
}

void GeneralSettingsClass::_slotBack()
{
	this->close();
}

void GeneralSettingsClass::_setSettings(QString pathToPhoto, QString pathToTracks, short timeZone)
{
	int index = timeZone_chb->findText(QString::number(timeZone));
	if ( index != -1 )
		timeZone_chb->setCurrentIndex(index);
	
	PathToTrack_LE->setText(pathToTracks);
	PathToPhoto_LE->setText(pathToPhoto);
	
}

void GeneralSettingsClass::_slotChangeTimeZone(QString timeZone)
{
	_timeZone = timeZone.toInt();
}

void GeneralSettingsClass::_slotTrackFormatChanged(int trackFormat)
{
	_trackFormat = trackFormat;
	//обновление настроек
	
}

void GeneralSettingsClass::_slotKeyboard()
{
	CalcFunctions::runKeyboard(this);
}

void GeneralSettingsClass::_slotButtonClickSound()
{
	CalcFunctions::soundPressedBut(5);
}