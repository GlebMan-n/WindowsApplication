#include <PyramidWidgets\MapExchangeClass.h>
#include <PyramidM\MSVC_2008\CalcFunctions.h>

MapExchangeClass::MapExchangeClass(QWidget* parent)
{
	this->setParent(parent);
	setupUi(this);
	this->setGeometry( CalcFunctions::calcWindowPos(parent->geometry(),this->geometry()) );
	_toKcps = 0;
	_loadSettings();
	connect(get_track_but,SIGNAL(clicked()),SLOT(_slotLoadRoute()));
	connect(get_paletka_but,SIGNAL(clicked()),SLOT(_slotLoadGrid()));

	connect(send_track_but,SIGNAL(clicked()),SIGNAL(signSendTrack()));
	connect(viewVideoBut,SIGNAL(clicked()),SIGNAL(signSendVideo()));
	connect(send_photo_but,SIGNAL(clicked()),SIGNAL(signSendPhoto()));
	connect(send_photo_but,SIGNAL(clicked()),SIGNAL(signSendPhoto()));
	connect(sendMarks,SIGNAL(clicked()),SLOT(_slotSendMarks()));
	//звуки
	connect(get_track_but,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(get_paletka_but,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(send_track_but,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(viewVideoBut,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(send_photo_but,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(sendMarks,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
}

MapExchangeClass::~MapExchangeClass()
{
	
}

void MapExchangeClass::_slotStatusRoute(QString status)
{
	if(status == "null")
		statusLbl->setText(tr("не принято"));
	else
	{
		QFileInfo finfo(status);
		statusLbl->setText(tr("загружено ") + finfo.fileName());
		emit signDrawRoute(status);
	}
}

void MapExchangeClass::_slotStatusGrid(QString status)
{
	if(status == "null")
		statusLbl->setText(tr("не принято"));
	else
	{
		QFileInfo finfo(status);
		statusLbl->setText(tr("загружено ") + finfo.fileName());
		emit signDrawGrid(status);
	}
}

void MapExchangeClass::_slotLoadRoute()
{
	emit signLoadRoute(_toKcps);
}
void MapExchangeClass::_slotLoadGrid()
{
	emit signLoadGrid(_toKcps);
}

void MapExchangeClass::_slotButtonClickSound()
{
	CalcFunctions::soundPressedBut(5);
}

bool MapExchangeClass::_loadSettings()
{
	QString path_to_ini = QCoreApplication::applicationDirPath () + "/settings_akps.ini";
	if (QFile::exists(path_to_ini) ) 
	{
		QSettings akps_settings (path_to_ini, QSettings::IniFormat);
		_toKcps = akps_settings.value("programm/def_serv").toInt();
		return true;
	}
	return false;
}

void MapExchangeClass::_slotSendMarks()
{
	emit signSendMarks(_toKcps);
}

void MapExchangeClass::_slotSetSendStatusMarks(bool result)
{
	if(result)
	{
		statusLbl->setText(tr("отправлено"));
	}
	else
	{
		statusLbl->setText(tr("не отправлено"));
	}
}