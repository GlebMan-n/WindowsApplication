#include <PyramidWidgets\MapNaviClass.h>	
#include <PyramidM\MSVC_2008\CalcFunctions.h>
#include <PyramidWidgets\DialInpCoordClass.h>
#include <PyramidWidgets\SelectDialogClass.h>

MapNaviClass::MapNaviClass(QWidget* parent)
{
	this->setParent(parent);
	setupUi(this);
	this->setGeometry( CalcFunctions::calcWindowPos(parent->geometry(),this->geometry()) );
	_parent = parent;
	connect(marshOrientBut,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(tchkMapBut,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(tchkHandBut,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	
	connect(marshOrientBut,SIGNAL(clicked()),SIGNAL(signRouteNavi()));
	connect(tchkHandBut,SIGNAL(clicked()),SLOT(_slotInputCoord()));
	connect(tchkMapBut,SIGNAL(clicked()),SLOT(_slotMapCoord()));
	_lat = 0;
	_lon = 0;
	_loadSettings();
}

MapNaviClass::~MapNaviClass()
{
}

bool MapNaviClass::_loadSettings()
{
	QString path_to_ini = QCoreApplication::applicationDirPath () + "/settings_akps.ini";
	if (QFile::exists(path_to_ini) ) 
	{
		QSettings akps_settings (path_to_ini, QSettings::IniFormat);
		_lat = akps_settings.value("programm/lastLat").toDouble();
		_lon = akps_settings.value("programm/lastLon").toDouble();
		
		return true;
	}
	return false;
}

void MapNaviClass::_slotInputCoord()
{
	this->hide();
	InputCoordinatesClass inputCoordDlg(_parent,_lat,_lon);
	inputCoordDlg.exec();

	if(inputCoordDlg.result)
	{
		emit signCoordNavi(inputCoordDlg.Lat,inputCoordDlg.Lon);
	}
	else
		this->show();
	
}

void MapNaviClass::_slotMapCoord()
{
	this->hide();
	SelectDialog dlg(_parent,tr("Мастер"),
		tr("Щелкнете по карте,\nчто бы задать координаты\nГотовы?"));
	dlg.exec();	
	
	if(dlg.result == 1)
	{
		emit signPointNavi();
		return;
	}

	this->show();
}

void MapNaviClass::_slotButtonClickSound()
{
	CalcFunctions::soundPressedBut(5);
}
