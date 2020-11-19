#include <PyramidWidgets\MapSendPhoto.h>	
#include <PyramidM\MSVC_2008\CalcFunctions.h>
#include <PyramidWidgets\FileExplorerClass.h>
#include <PyramidWidgets\SelectDialogClass.h>

MapSendPhoto::MapSendPhoto(QWidget* parent)
{
	_parent = parent;
	this->setParent(parent);
	setupUi(this);
	this->setGeometry( CalcFunctions::calcWindowPos(parent->geometry(),this->geometry()) );
	connect(right_photo_but,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(left_photo_but,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(send_foto_but,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(delPhotoBut,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(serv,SIGNAL(highlighted (int)),SLOT(_slotButtonClickSound()));

	connect(right_photo_but,SIGNAL(clicked()),SLOT(_slotNextFile()));
	connect(left_photo_but,SIGNAL(clicked()),SLOT(_slotPervFile()));
	connect(send_foto_but,SIGNAL(clicked()),SLOT(_slotSendPhoto()));
	connect(delPhotoBut,SIGNAL(clicked()),SLOT(_slotDeleteFile()));
	connect(serv,SIGNAL(currentIndexChanged (int)),SLOT(_slotToKcpsChanged(int)));
	_pathToPhotos = "";
	_toKcps = 0;
	_init();
}
MapSendPhoto::~MapSendPhoto()
{
}

void MapSendPhoto::_init()
{
	if(_loadSettings())
	{
		_fileExplorer = new FileExplorerClass(this,_pathToPhotos,QStringList() << "*.jpg");
		serv->setCurrentIndex(_toKcps);
		_updateFileInfo();
	}
}
bool MapSendPhoto::_loadSettings()
{
	QString path_to_ini = QCoreApplication::applicationDirPath () + "/settings_akps.ini";
	if (QFile::exists(path_to_ini) ) 
	{
		QSettings akps_settings (path_to_ini, QSettings::IniFormat);
		_pathToPhotos = akps_settings.value("programm/path_to_photo").toString();
		_toKcps = akps_settings.value("programm/def_serv").toInt();
		return true;
	}
	return false;
}
bool MapSendPhoto::_deleteFile()
{
	if(_fileExplorer->getCountOfFiles() > 0)
	{
		if(_fileExplorer->deleteCurrentFile())
		{
			_updateFileInfo();
			return true;
		}
		else
			status_send_photo->setText(tr("удалить не удалось"));
	}
	else
	{
		_updateFileInfo();
	}
	return false;
}
void MapSendPhoto::_updateFileInfo()
{
	if(_fileExplorer->getCountOfFiles() > 0)
	{
		status_send_photo->setText(QString::number(_fileExplorer->getCurrentIndex()) + tr(" из ") + QString::number(_fileExplorer->getCountOfFiles()));
		data_photo_lbl->setText(_namePhoto(_fileExplorer->getCurrentFile(true)));
		photo_lbl->setPixmap(QPixmap(_fileExplorer->getCurrentFile(true)));

		delPhotoBut->setEnabled(true);
		send_foto_but->setEnabled(true);
		right_photo_but->setEnabled(true);
		left_photo_but->setEnabled(true);
	}
	else
	{
		status_send_photo->setText(tr("файлов нет"));
		photo_lbl->setPixmap(QPixmap(""));
		data_photo_lbl->setText("");
		delPhotoBut->setEnabled(false);
		send_foto_but->setEnabled(false);
		right_photo_but->setEnabled(false);
		left_photo_but->setEnabled(false);
	}
}
QString MapSendPhoto::_namePhoto(QString fullPath)
{
	//,18,3,2013,16,59,41,0.97553,0.655337,.jpg
	QString result("null");
	QFileInfo file(fullPath);
	QString name = file.fileName();
	QString day = name.section(',', 1, 1);
	QString month = name.section(',', 2, 2);
	QString year = name.section(',', 3, 3);
	QString hour = name.section(',', 4, 4);
	QString min = name.section(',', 5, 5);
	QString sec = name.section(',', 6, 6);
	QString lat = name.section(',', 7, 7);
	QString lon = name.section(',',8 , 8);
	QTime qtime(hour.toInt(),min.toInt(),sec.toInt());
	QDate date(year.toInt(), month.toInt(), day.toInt());
	lat = CalcFunctions::dDToDDMMSS(lat.toDouble());
	lon = CalcFunctions::dDToDDMMSS(lon.toDouble());
	result = qtime.toString("HH:mm:ss") + " " + date.toString("dd.MM.yyyy")+" "+ lat +" "+ lon;
	return result;
}

void MapSendPhoto::_slotNextFile()
{
	_fileExplorer->getNextFile();
	_updateFileInfo();
}

void MapSendPhoto::_slotPervFile()
{
	_fileExplorer->getPervFile();
	_updateFileInfo();
}

void MapSendPhoto::_slotDeleteFile()
{
	SelectDialog dlgPhoto(_parent,tr("Трек"),
	tr("<br>Удалить фотографию?</br>"));
		dlgPhoto.exec();

	if(dlgPhoto.result == 1)
		_deleteFile();
}

void MapSendPhoto::_slotSendPhoto()
{
	if(_fileExplorer->getCountOfFiles() > 0)
		emit signSendPhoto(_fileExplorer->getCurrentFile(true),_toKcps);
	else
	{
		status_send_photo->setText(tr("файлов нет"));
		data_photo_lbl->setText("");
	}
}

void MapSendPhoto::_slotSendPhotoStatus(bool status)
{
	if(status)
	{
		status_send_photo->setText(tr("отправлено"));
	}
	else
	{
		status_send_photo->setText(tr("не отправлено"));
	}
}

void MapSendPhoto::_slotToKcpsChanged(int toKcps)
{
	_toKcps = toKcps;
}

void MapSendPhoto::_slotButtonClickSound()
{
	CalcFunctions::soundPressedBut(5);
}