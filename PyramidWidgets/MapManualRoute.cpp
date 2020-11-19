#include <PyramidWidgets\MapManualRoute.h>	
#include <PyramidM\MSVC_2008\CalcFunctions.h>
#include <PyramidWidgets\DialInpCoordClass.h>
#include <PyramidWidgets\SelectDialogClass.h>

MapManualRoute::MapManualRoute(QWidget* parent)
{
	this->setParent(parent);
	setupUi(this);
	this->setGeometry(CalcFunctions::calcWindowPos(parent->geometry(),this->geometry()) );
	_defLat = 0;
	_defLon = 0;
	_loadSettings();

	_parent = parent;
	connect(mapInput,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(mapInput,SIGNAL(clicked()),SIGNAL(signManualRoute()));
	connect(mapInput,SIGNAL(clicked()),SLOT(close()));

	connect(coordInput,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(coordInput,SIGNAL(clicked()),SLOT(_slotCoordMRoute()));

	connect(clear,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(clear,SIGNAL(clicked()),SIGNAL(signClearMRoute()));
}

MapManualRoute::~MapManualRoute()
{

}

void MapManualRoute::_slotButtonClickSound()
{
	CalcFunctions::soundPressedBut(5);
}

bool MapManualRoute::_loadSettings()
{
	QString path_to_ini = QCoreApplication::applicationDirPath () + "/settings_akps.ini";
	if (QFile::exists(path_to_ini) ) 
	{
		QSettings akps_settings (path_to_ini, QSettings::IniFormat);
		
		_defLat = akps_settings.value("programm/lastLat").toDouble();
		_defLon = akps_settings.value("programm/lastLon").toDouble();
		return true;
	}
	return false;
}

void MapManualRoute::_slotCoordMRoute()
{
	this->hide();
	InputCoordinatesClass inputCoordDlg(_parent,_defLat,_defLon);
	inputCoordDlg.exec();
	
	if(inputCoordDlg.result)
	{
		emit signMRouteCoord(inputCoordDlg.Lat,inputCoordDlg.Lon);
	}
	else
		return;

	SelectDialog dlg(_parent,tr("Мастер"),
	tr("Добавить еще точку?"));
	dlg.exec();
	if(dlg.result == 1)
	{
		_slotCoordMRoute();
		return;
	}
	
	this->show();
}