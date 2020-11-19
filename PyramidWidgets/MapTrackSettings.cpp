#include <PyramidWidgets\MapTrackSettings.h>	
#include <PyramidM\MSVC_2008\CalcFunctions.h>
#include <PyramidWidgets\SelectDialogClass.h>

MapTrackSettings::MapTrackSettings(QWidget* parent)
{
	_parent = parent;
	this->setParent(parent);
	setupUi(this);
	this->setGeometry( CalcFunctions::calcWindowPos(parent->geometry(),this->geometry()) );

	connect(trackFormat,SIGNAL(currentIndexChanged (int)),SLOT(_slotChangeFormat(int)));
	connect(color,SIGNAL(currentIndexChanged (int)),SLOT(_slotChangeColor(int)));
	connect(widtLine,SIGNAL(currentIndexChanged (int)),SLOT(_slotChangeWidth(int)));
//	connect(discr,SIGNAL(currentIndexChanged (int)),SLOT(_slotChangeDiscr(int)));
//	connect(discrType,SIGNAL(currentIndexChanged (int)),SLOT(_slotChangeDiscrType(int)));
	connect(autobreak,SIGNAL(currentIndexChanged (int)),SLOT(_slotChangeAutobreak(int)));
	connect(breakTrack,SIGNAL(clicked()),SLOT(_slotBreakTrack()));
	connect(apply,SIGNAL(clicked()),SLOT(_slotApply()));
	
	_format = 0;
	_color = 0;
	_width = 1;
	_discretType = 0;
	_discret = 0;
	_autoBreak = 0;
	_changingType = false;
	if(_loadSettings())
		_init();
}
MapTrackSettings::~MapTrackSettings()
{}

void MapTrackSettings::_slotChangeFormat(int index)
{
	_format = index;
}

void MapTrackSettings::_slotChangeColor(int index)
{
	_color = index;
}

void MapTrackSettings::_slotChangeWidth(int index)
{
	double width = widtLine->currentText().toDouble();
	_width = width/10;
}

void MapTrackSettings::_slotChangeDiscr(int index)
{
	
	//if(_changingType)
	//{
	//	_changingType = false;
	//	return;
	//}
	//switch(discrType->currentIndex())
	//{
	//case 0:
	//	//все
	//	_discretType = 0;
	//	_discret = 0;
	//	break;
	//case 1:
	//	//расстояние
	//	_discretType = 1;
	//	_discret = discr->currentText().toInt();
	//	break;
	//case 2:
	//	//время
	//	_discretType = 2;
	//	_discret = discr->currentText().toInt();
	//	break;
	//case 3:
	//	//скорость
	//	_discretType = 3;
	//	_discret = discr->currentText().toInt();
	//	break;
	//}	
}

void MapTrackSettings::_slotChangeAutobreak(int index)
{
	_autoBreak = index;
}

void MapTrackSettings::_slotChangeDiscrType(int index)
{
	//discr->setEnabled(false);
	//_discretType = index;
	//QStringList itemsList;
	//_changingType = true;

	//switch(discrType->currentIndex())
	//{
	//case 0:
	//	//все
	//	
	//	itemsList.clear();
	//	discr->clear();
	//	//discr->addItems(itemsList);
	//	break;
	//case 1:
	//	//расстояние

	//	itemsList << "50" << "100" << "250" << "500" << "1000" << "1500" << "2000" << "5000" << "7000" << "10000";
	//	discr->clear();
	//	discr->addItems(itemsList);
	//	discr->setCurrentIndex(0);
	//	break;
	//case 2:
	//	//время
	//	itemsList << "0" << "1" << "2" << "3" << "5" << "7" << "10" << "15";
	//	discr->clear();
	//	discr->addItems(itemsList);
	//	discr->setCurrentIndex(0);
	//	break;
	//case 3:
	//	//скорость

	//	itemsList << "1" << "3" << "5" << "10";
	//	discr->clear();
	//	discr->addItems(itemsList);
	//	discr->setCurrentIndex(0);
	//	break;
	//}
	//discr->setEnabled(true);

}

void MapTrackSettings::_slotBreakTrack()
{
	SelectDialog dlgTrack(_parent,tr("Трек"),
		tr("<br>Разбить трек?</br>"));
	dlgTrack.exec();
	if(dlgTrack.result == 1)
	{
		SelectDialog dlg(_parent,tr("Трек"),
			tr("Очистить слой треков?"));
		dlg.exec();
		if(dlg.result == 1)
		{
			status->setText(tr("трек разбит(очищен)"));
			emit signBreakTrack(true);
		}
		else
		{
			status->setText(tr("трек разбит"));
			emit signBreakTrack(false);
		}
	}
}

bool MapTrackSettings::_loadSettings()
{
	QString path_to_ini = QCoreApplication::applicationDirPath () + "/settings_akps.ini";
	if (QFile::exists(path_to_ini) ) 
	{
		QSettings akps_settings (path_to_ini, QSettings::IniFormat);
		_format = akps_settings.value("track/format").toInt();
		_color = akps_settings.value("track/color").toInt();
 		_width = akps_settings.value("track/width").toDouble();
		//_discretType = akps_settings.value("track/discretType").toInt();
	//	_discret = akps_settings.value("track/discret").toInt();
		_autoBreak = akps_settings.value("track/autobreak").toInt();
		status->setText(tr("загружено"));

		if(_width < 0.3 || _width > 3)
			_width = 0.7;
			
		return true;
		
	}
	status->setText(tr("базовые настройки"));
	return false;		
}

bool MapTrackSettings::_saveSettings()
{
	QSettings akps_settings (QCoreApplication::applicationDirPath () + "/settings_akps.ini", QSettings::IniFormat);
	akps_settings.setValue("track/format", _format);
	akps_settings.setValue("track/color", _color);
	akps_settings.setValue("track/width",_width);
//	akps_settings.setValue("track/discretType", _discretType);
//	akps_settings.setValue("track/discret", _discret);
	akps_settings.setValue("track/autobreak", _autoBreak);
	akps_settings.sync();
	status->setText(tr("сохранено"));
	return true;
}

void MapTrackSettings::_init()
{
	trackFormat->setCurrentIndex(_format);
	color->setCurrentIndex(_color);
	short width = _width * 10;
	widtLine->setCurrentIndex(widtLine->findText(QString::number(width)));
//	discrType->setCurrentIndex(_discretType);
	//discr->setCurrentIndex(discr->findText(QString::number(_discret)));
	autobreak->setCurrentIndex(_autoBreak);
}


void MapTrackSettings::_slotApply()
{
	_saveSettings();
	emit signUpdateSettings();	
	
}