#include <PyramidWidgets\MapMarkClass.h>
#include <PyramidM\MSVC_2008\CalcFunctions.h>
#include <PyramidWidgets\DialInpCoordClass.h>
#include <PyramidWidgets\SelectDialogClass.h>

MapMarkClass::MapMarkClass(QWidget* parent)
{
	this->setParent(parent);
	setupUi(this);
	this->setGeometry( CalcFunctions::calcWindowPos(parent->geometry(),this->geometry()) );
	_parent = parent;
	//звуки
	connect(this->markTekPos,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(this->keyboardBut,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(this->markToMap,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(this->markEdit,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(this->markToCoord,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));

	connect(this->markTekPos,SIGNAL(clicked()),SLOT(_slotMark()));
	connect(this->keyboardBut,SIGNAL(clicked()),SLOT(_slotKeyboard()));
	connect(this->markToMap,SIGNAL(clicked()),SLOT(_slotFromMap()));
	connect(this->markToCoord,SIGNAL(clicked()),SLOT(_slotFromCoord()));
	connect(this->markEdit,SIGNAL(clicked()),SIGNAL(signMarkEdit()));
	_lat = 0;
	_lon = 0;
	_height = 197.5;//высота по умолчанию
	_pathToMark = "null";
	_loadSettings();
	_fromMap = false;
}

MapMarkClass::~MapMarkClass()
{

}
void MapMarkClass::_slotFromCoord()
{
		InputCoordinatesClass inputCoordDlg(_parent,_lat,_lon);
		this->hide();
		inputCoordDlg.exec();
		if(inputCoordDlg.result == 1)
		{
			_lat = inputCoordDlg.Lat;
			_lon = inputCoordDlg.Lon;
			_slotMark();
			
		}
		this->show();
}
bool MapMarkClass::_loadSettings()
{
	QString path_to_ini = QCoreApplication::applicationDirPath () + "/settings_akps.ini";

	if (QFile::exists(path_to_ini) ) 
	{
		QSettings akps_settings (path_to_ini, QSettings::IniFormat);
		_pathToMark = akps_settings.value("programm/path_to_marks").toString();
		_lat = akps_settings.value("programm/lastLat").toDouble();
		_lon = akps_settings.value("programm/lastLon").toDouble();
		_pathToMark += "\\marks.file";
		return true;
	}
	return false;
}

bool MapMarkClass::_saveMarkToFile(QString type,QString date, QString time, QString description, QString lat, QString lon,QString height)
{
	//формат файла отметок
	//тип,дата,время,описание,
	//координаты,высота, marks\n
	QFile fileMarks(_pathToMark);
	if(!fileMarks.open(QIODevice::Append | QIODevice::Text)) 
		return false;
	QTextStream out(&fileMarks);

	QString string = type + "," + date + "," + time + "," + 
					 description +"," + lat + "," + lon + "," + height + ",marks\n";

	out << string;
	fileMarks.close();
	return true;
}

void MapMarkClass::_slotGpsData(QTime time, QDate date, bool status, double lat, double lon, double speed, double angle,short sat, float height)
{
	if(_fromMap)
		return;
	_lat = lat;
	_lon = lon;
	_height = height;
}

void MapMarkClass::_slotMark()
{
	short index;
	if(typeComboBox->currentText() == tr("Прочее"))
		index = 100;
	else
		index = typeComboBox->currentIndex() + 1;
	if(!_saveMarkToFile(QString::number(index),QDate::currentDate().toString("dd.MM.yyyy"), QTime::currentTime().toString("hh:mm:ss") , typeComboBox->currentText() + " " + infoPlainText->toPlainText(), QString::number(_lat), QString::number(_lon), QString::number(_height)))
		return;

	//отобразить на карте
	emit signMark(index, _lat, _lon);
	status->setText(tr("Отметка нанесена"));
}

void MapMarkClass::_slotKeyboard()
{
	CalcFunctions::runKeyboard(this);
}

void MapMarkClass::_slotButtonClickSound()
{
	infoPlainText->setFocus();
	CalcFunctions::soundPressedBut(5);
}

void MapMarkClass::_slotCoordMap(double lat, double lon)
{
	if(_fromMap)
	{
		_lat = lat;
		_lon = lon;
		_slotMark();
		_fromMap = false;
	}
}

void MapMarkClass::_slotFromMap()
{
		_fromMap = true;
		this->hide();
}