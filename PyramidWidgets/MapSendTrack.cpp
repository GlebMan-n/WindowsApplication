#include <PyramidWidgets\SelectDialogClass.h>
#include <PyramidWidgets\MapSendTrack.h>
#include <PyramidWidgets\FileExplorerClass.h>
#include <PyramidM\MSVC_2008\CalcFunctions.h>

MapSendTrack::MapSendTrack(QWidget* parent)
{
	_parent = parent;
	this->setParent(parent);
	setupUi(this);
	this->setGeometry( CalcFunctions::calcWindowPos(parent->geometry(),this->geometry()) );

	connect(right_track_but,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(left_track_but,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(send_track_but,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(delBut,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(drawTrack,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(clearTrack,SIGNAL(clicked()),SIGNAL(_slotButtonClickSound()));
	connect(serv,SIGNAL(highlighted (int)),SLOT(_slotButtonClickSound()));
	
	connect(right_track_but,SIGNAL(clicked()),SLOT(_slotNextFile()));
	connect(left_track_but,SIGNAL(clicked()),SLOT(_slotPervFile()));
	connect(send_track_but,SIGNAL(clicked()),SLOT(_slotSendTrack()));
	connect(delBut,SIGNAL(clicked()),SLOT(_slotDeleteFile()));
	connect(drawTrack,SIGNAL(clicked()),SLOT(_slotDrawTrack()));
	connect(clearTrack,SIGNAL(clicked()),SIGNAL(signClearTrack()));
	connect(serv,SIGNAL(currentIndexChanged (int)),SLOT(_slotToKcpsChanged(int)));
	_pathToTracks = "";
	_toKcps = 0;
	_init();
}
MapSendTrack::~MapSendTrack()
{
}

void MapSendTrack::_init()
{
	if(_loadSettings())
	{
		_fileExplorer = new FileExplorerClass(this,_pathToTracks,QStringList() << "*.current" << "*.finished");
		serv->setCurrentIndex(_toKcps);
		_updateFileInfo();
	}
}
bool MapSendTrack::_loadSettings()
{
	QString path_to_ini = QCoreApplication::applicationDirPath () + "/settings_akps.ini";
	if (QFile::exists(path_to_ini) ) 
	{
		QSettings akps_settings (path_to_ini, QSettings::IniFormat);
		_pathToTracks = akps_settings.value("programm/path_to_track").toString();
		_toKcps = akps_settings.value("programm/def_serv").toInt();
		return true;
	}
	return false;
}
bool MapSendTrack::_deleteFile()
{
	if(_fileExplorer->getCountOfFiles() > 0)
	{
		if(_fileExplorer->deleteCurrentFile())
		{
			_updateFileInfo();
			return true;
		}
		else
			status_track->setText(tr("удалить не удалось"));
	}
	else
	{
		status_track->setText(tr("файлов нет"));
		date_of_create->setText("");
		date_of_finished->setText("");
	}
	return false;
}

void MapSendTrack::_slotNextFile()
{
	_fileExplorer->getNextFile();
	_updateFileInfo();

}
void MapSendTrack::_slotPervFile()
{
	_fileExplorer->getPervFile();
	_updateFileInfo();
}
void MapSendTrack::_slotDeleteFile()
{
	SelectDialog dlgTrack(_parent,tr("Трек"),
	tr("<br>Удалить файл трека?</br>"));
		dlgTrack.exec();

	if(dlgTrack.result == 1)
		_deleteFile();
}
void MapSendTrack::_slotSendTrack()
{
	if(_fileExplorer->getCountOfFiles() > 0)
		emit signSendTrack(_fileExplorer->getCurrentFile(true),_getDiscret(),_toKcps);
	else
	{
		status_track->setText(tr("файлов нет"));
		date_of_create->setText("");
		date_of_finished->setText("");
	}
}

void MapSendTrack::_updateFileInfo()
{
	if(_fileExplorer->getCountOfFiles() > 0)
	{
		status_track->setText(QString::number(_fileExplorer->getCurrentIndex()) + tr(" из ") + QString::number(_fileExplorer->getCountOfFiles()));
		date_of_create->setText(_firstNameTrack(_fileExplorer->getCurrentFile(true)));
		date_of_finished->setText(_lastNameTrack(_fileExplorer->getCurrentFile(true)));
		
		QFileInfo file(_fileExplorer->getCurrentFile(true));
		sizeLbl->setText(_sizeToStr(file.size()));

		send_track_but->setEnabled(true);
		delBut->setEnabled(true);
	}
	else
	{
		status_track->setText(tr("файлов нет"));
		date_of_create->setText("");
		date_of_finished->setText("");
		send_track_but->setEnabled(false);
		delBut->setEnabled(false);
	}
}

QString MapSendTrack::_sizeToStr(qint64 size)
{
	QString result;

	if(size > 1024 && size < 1048576)
	{
		size = size / 1024;
		result = QString::number(size) + tr(" КБ");
	}

	if(size > 1048576)
	{
		size = size / 1048576;
		result = QString::number(size) + tr(" МБ");
	}

	return result;
}

QString MapSendTrack::_firstNameTrack(QString fullPath)
{
	QString result;
	QString name = fullPath;
	QFileInfo file(fullPath);
	QString ext = file.completeSuffix();
	result = file.baseName();
	
	if(ext == "finished")
	{
		//обрезаем расширение
		name.chop(24);
		QString dateTimeOfCreated;
		result = name.right(15);
		result = _dateTimeFormat(result);
		return result;

	}

	if(ext == "current")
	{
		//обрезаем расширение
		name.chop(8);
		QString dateTimeOfCreated;
		result = name.right(15);
		result = _dateTimeFormat(result);
		return result;
	}
	return result;
}

QString MapSendTrack::_lastNameTrack(QString fullPath)
{
	QString result;
	QFileInfo file(fullPath);
	result = file.baseName();
	QString name = file.fileName();
	QString ext = file.completeSuffix();

	if(ext == "finished")
	{
		//обрезаем расширение
		name.chop(9);
		result = name.right(15);
		result = _dateTimeFormat(result);
		return result;

	}

	if(ext == "current")
	{
		//обрезаем расширение
		return tr("трек текущий(еще не закончен)");
	}

	return result;
}

QString MapSendTrack::_dateTimeFormat(QString dateTime)
{
	QString tmp;
	tmp = dateTime;
	QString stime;
	QString sdate;
	stime = dateTime.section('_', 0, 0); 
	sdate = dateTime.section('_', 1, 1); 
	QTime time;
	time = QTime::fromString(stime, "hhmmss");
	QDate date;
	date = QDate::fromString(sdate,"ddMMyyyy");
	tmp = time.toString("hh:mm:ss") + " " + date.toString("dd.MM.yyyy");
	return tmp;
}

int MapSendTrack::_getDiscret()
{
	short meters = 0; 
	switch(this->meters->currentIndex())
	{
		case 0:
			meters = 0;
			break;
		case 1:
			meters = 10;
			break;
		case 2:
			meters = 50;
			break;
		case 3:
			meters = 100;
			break;
		case 4:
			meters = 500;
			break;
		case 5:
			meters = 1000;
			break;
		case 6:
			meters = 3000;
			break;
		case 7:
			meters = 5000;
			break;
		case 8:
			meters = 10000;
			break;
		case 9:
			meters = 15000;
			break;
	}
	return meters;
}

void MapSendTrack::_slotDrawTrack()
{
	emit signClearTrack();
	emit signDrawTrack(_fileExplorer->getCurrentFile(true),_getDiscret());
}

void MapSendTrack::_slotSendTrackStatus(bool status)
{
	if(status)
	{
		status_track->setText(tr("отправлено"));
	}
	else
	{
		status_track->setText(tr("не отправлено"));
	}
}

void MapSendTrack::_slotToKcpsChanged(int toKcps)
{
	_toKcps = toKcps;
}

void MapSendTrack::_slotButtonClickSound()
{
	CalcFunctions::soundPressedBut(5);
}