#include <PyramidWidgets\MapVideoClass.h>	
#include <PyramidWidgets\FileExplorerClass.h>
#include <PyramidWidgets\SelectDialogClass.h>
#include <PyramidWidgets\MapPhotoClass.h>
#include <PyramidWidgets\camera.h>
#include <PyramidM\MSVC_2008\CalcFunctions.h>
#include <QFile>
#include <QSettings>
#include <QCoreApplication>
#include <QLibrary>
#include <QDir>
#include <QTimer>

MapVideoClass::MapVideoClass(QWidget* parent)
{
	_parent = parent;
	this->setParent(parent);
	setupUi(this);
	this->setGeometry( CalcFunctions::calcWindowPos(parent->geometry(),this->geometry()) );
	connect(right_video_but,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(left_video_but,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(send_video_but,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(playBut,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(delVideoBut,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(serv,SIGNAL(highlighted (int)),SLOT(_slotButtonClickSound()));


	connect(right_video_but,SIGNAL(clicked()),SLOT(_slotNextFile()));
	connect(left_video_but,SIGNAL(clicked()),SLOT(_slotPervFile()));
	connect(send_video_but,SIGNAL(clicked()),SLOT(_slotSendVideo()));
	connect(playBut,SIGNAL(clicked()),SLOT(_slotPlayVideo()));
	connect(delVideoBut,SIGNAL(clicked()),SLOT(_slotDeleteFile()));
	connect(serv,SIGNAL(currentIndexChanged (int)),SLOT(_slotToKcpsChanged(int)));

	m_VideoTimer = new QTimer(this);
	connect(m_VideoTimer, SIGNAL( timeout ()),
	this, SLOT (_slotVideoScreen()));
	capture = NULL;
	b_demo = false;
	_pathToVideo = "";
	_toKcps = 0;
	_init();
}

MapVideoClass::~MapVideoClass()
{
	if(capture)
		cvReleaseCapture( &capture );
	if(m_VideoTimer)	
		m_VideoTimer->stop();
}

void MapVideoClass::_init()
{
	if(_loadSettings())
	{
		_fileExplorer = new FileExplorerClass(this,_pathToVideo,QStringList() << "*.avi");
		serv->setCurrentIndex(_toKcps);
		_updateFileInfo();
	}
}
bool MapVideoClass::_loadSettings()
{
	QString path_to_ini = QCoreApplication::applicationDirPath () + "/settings_akps.ini";
	if (QFile::exists(path_to_ini) ) 
	{
		QSettings akps_settings (path_to_ini, QSettings::IniFormat);
		_pathToVideo =  akps_settings.value("programm/path_to_video").toString();
		_toKcps = akps_settings.value("programm/def_serv").toInt();
		return true;
	}
	return false;
}

bool MapVideoClass::_deleteFile()
{
	if(_fileExplorer->getCountOfFiles() > 0)
	{
		if(_fileExplorer->deleteCurrentFile())
		{
			_updateFileInfo();
			return true;
		}
		else
			statusVideo->setText(tr("удалить не удалось"));
	}
	else
	{
		_updateFileInfo();
	}
	return false;
}
void MapVideoClass::_updateFileInfo()
{
	if(_fileExplorer->getCountOfFiles() > 0)
	{
		statusVideo->setText(QString::number(_fileExplorer->getCurrentIndex()) + tr(" из ") + QString::number(_fileExplorer->getCountOfFiles()));
		data_video_lbl->setText(_nameVideo(_fileExplorer->getCurrentFile(true)));
		b_demo = true;
		_slotPlayVideo();

		send_video_but->setEnabled(true);
		playBut->setEnabled(true);
		left_video_but->setEnabled(true);
		right_video_but->setEnabled(true);
		delVideoBut->setEnabled(true);
	}
	else
	{
		statusVideo->setText(tr("файлов нет"));
		captureLbl->setPixmap(QPixmap(""));
		data_video_lbl->setText("");

		send_video_but->setEnabled(false);
		playBut->setEnabled(false);
		left_video_but->setEnabled(false);
		right_video_but->setEnabled(false);
		delVideoBut->setEnabled(false);
	}
}
QString MapVideoClass::_nameVideo(QString fullPath)
{
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
	lat = QString::number(lat.toDouble() * CCT_180_PI);
	lon = QString::number(lon.toDouble() * CCT_180_PI);
	result = qtime.toString("HH:mm:ss") + " " + date.toString("dd.MM.yyyy")+" "+ lat +" "+ lon;
	return result;
}

void MapVideoClass::_slotNextFile()
{
	_fileExplorer->getNextFile();
	_updateFileInfo();
}

void MapVideoClass::_slotPervFile()
{
	_fileExplorer->getPervFile();
	_updateFileInfo();
}

void MapVideoClass::_slotDeleteFile()
{
	SelectDialog dlgVideo(_parent,tr("Трек"),
	tr("<br>Удалить видеозапись?</br>"));
		dlgVideo.exec();

	if(dlgVideo.result == 1)
		_deleteFile();
}

void MapVideoClass::_slotSendVideo()
{
	if(_fileExplorer->getCountOfFiles() > 0)
		emit signSendVideo(_fileExplorer->getCurrentFile(true),_toKcps);
	else
	{
		statusVideo->setText(tr("файлов нет"));
		data_video_lbl->setText("");
	}
}

void MapVideoClass::_slotSendVideoStatus(bool status)
{
	if(status)
	{
		statusVideo->setText(tr("отправлено"));
	}
	else
	{
		statusVideo->setText(tr("не отправлено"));
	}
}

//runVideo
void MapVideoClass::_slotPlayVideo()
{
	if(_fileExplorer->getCountOfFiles() < 1)
	{
		statusVideo->setText(tr("нет файлов видео"));
		return;
	}

	if(m_VideoTimer->isActive())
	{
		if(capture)
			cvReleaseCapture( &capture );
		m_VideoTimer->stop();
		return;

	}
	QByteArray ar = _fileExplorer->getCurrentFile(true).toLocal8Bit();
	char* filename = ar.data();
	capture = cvCreateFileCapture( filename );
	
	if (capture)
		m_VideoTimer->start(165);
}

void MapVideoClass::_slotVideoScreen()
{
	if (capture == NULL) return; 
	IplImage* frame;
    assert( capture );
    frame = 0;
    frame = cvQueryFrame( capture );
	if( !frame ) 
	{
		if(capture)
			cvReleaseCapture( &capture );
		m_VideoTimer->stop();
		this->playBut->setChecked(false);
		return;
    }

	QImage img;
	img = *MapPhotoClass::IplImage2QImage(frame);
	frame = 0;
	captureLbl->setPixmap(QPixmap::fromImage(img));
	img.~QImage(); 
	captureLbl->show();
	
	if(b_demo)
	{
			b_demo = false;
			_slotPlayVideo();
	}
}

void MapVideoClass::_slotToKcpsChanged(int toKcps)
{
	_toKcps = toKcps;
}


void MapVideoClass::_slotButtonClickSound()
{
	CalcFunctions::soundPressedBut(5);
}