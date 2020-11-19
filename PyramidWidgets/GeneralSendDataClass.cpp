#include <windows.h>
#include "PyramidWidgets\GeneralSendDataClass.h"
#include <PyramidWidgets\FileExplorerClass.h>
#include <PyramidM\MSVC_2008\CalcFunctions.h>
#include <PyramidWidgets\SelectDialogClass.h>

GeneralSendDataClass::GeneralSendDataClass(QWidget *parent)
{
	this->setParent(parent);
	setupUi(this);
	_parent = parent;
	this->setGeometry(parent->geometry());
	connect(toVpuBut,SIGNAL(clicked()),SLOT(_slotAllToVpu() ) );
	connect(toKcpsBut,SIGNAL(clicked()),SLOT(_slotAllToKcps() ) );
	connect(backBut,SIGNAL(clicked()),SLOT(_slotBack() ) );

	connect(delMarks,SIGNAL(clicked()),SLOT(_slotDelMarks() ) );
	connect(delPhoto,SIGNAL(clicked()),SLOT(_slotDelPhoto() ) );
	connect(delTrack,SIGNAL(clicked()),SLOT(_slotDelTrack() ) );
	connect(delVideo,SIGNAL(clicked()),SLOT(_slotDelVideo() ) );

	connect(toVpuBut,SIGNAL(clicked()),SLOT(_slotButtonClickSound() ) );
	connect(toKcpsBut,SIGNAL(clicked()),SLOT(_slotButtonClickSound() ) );
	connect(backBut,SIGNAL(clicked()),SLOT(_slotButtonClickSound() ) );

	connect(delMarks,SIGNAL(clicked()),SLOT(_slotButtonClickSound() ) );
	connect(delPhoto,SIGNAL(clicked()),SLOT(_slotButtonClickSound() ) );
	connect(delTrack,SIGNAL(clicked()),SLOT(_slotButtonClickSound() ) );
	connect(delVideo,SIGNAL(clicked()),SLOT(_slotButtonClickSound() ) );
	
	_initTime();
	_loadSettings();

	_tracksSended = false;
	_photosSended = false;
	_marksSended = false;
	_videosSended = false;
}

GeneralSendDataClass::~GeneralSendDataClass()
{
}

bool GeneralSendDataClass::_loadSettings()
{
	QString path_to_ini = QCoreApplication::applicationDirPath () + "/settings_akps.ini";
	if (QFile::exists(path_to_ini) ) 
	{
		QSettings akps_settings (path_to_ini, QSettings::IniFormat);		
		_pathToTracks = akps_settings.value("programm/path_to_track").toString();
		_pathToPhoto = akps_settings.value("programm/path_to_photo").toString();
		_pathToVideo = akps_settings.value("programm/path_to_video").toString();
		_pathToMark = akps_settings.value("programm/path_to_marks").toString();
		return true;
	}
	return false;
}

void GeneralSendDataClass::_initTime()
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

void GeneralSendDataClass::_slotTime(QString day, QString sdate, QString stime)
{
	this->day_lbl->setText(day);
	this->date_lbl->setText(sdate);
	this->time_lbl->setText(stime);
}

	//0 - КЦПС, 1 - ВПУ , 2 - ? ...
void GeneralSendDataClass::_slotAllToVpu()
{
	tracksStat->setText(tr("..."));
	photosStat->setText(tr("..."));
	marksStat->setText(tr("..."));
	videosStat->setText(tr("..."));

	sendTracksProgress->setMaximum(0);
	sendTracksProgress->setValue(0);
	sendPhotosProgress->setMaximum(0);
	sendPhotosProgress->setValue(0);
	sendMarksProgress->setMaximum(0);
	sendMarksProgress->setValue(0);
	sendVideoProgress->setMaximum(0);
	sendVideoProgress->setValue(0);

	QStringList tracks;
	tracks = _getListOfTracks();
	
	if (tracks.size() > 0)
		emit signSendAllTracks(tracks,1);
	else
		tracksStat->setText(tr("файлов нет"));

	QStringList photos;
	photos = _getListOfPhotos();

	if (photos.size() > 0)
		emit signSendAllPhotos(photos,1);
	else
		photosStat->setText(tr("файлов нет"));
	
	if(_isMarks())
		emit signSendAllMarks(1);
	else
		marksStat->setText(tr("файлов нет"));

	QStringList videos;
	videos = _getListOfVideos();

	if (videos.size() > 0)
		emit signSendAllVideos(videos,1);
	else
		videosStat->setText(tr("файлов нет"));


}

	//0 - КЦПС, 1 - ВПУ , 2 - ? ...
void GeneralSendDataClass::_slotAllToKcps()
{
	tracksStat->setText(tr("..."));
	photosStat->setText(tr("..."));
	marksStat->setText(tr("..."));
	videosStat->setText(tr("..."));

	sendTracksProgress->setMaximum(0);
	sendTracksProgress->setValue(0);
	sendPhotosProgress->setMaximum(0);
	sendPhotosProgress->setValue(0);
	sendMarksProgress->setMaximum(0);
	sendMarksProgress->setValue(0);
	sendVideoProgress->setMaximum(0);
	sendVideoProgress->setValue(0);

	QStringList tracks;
	tracks = _getListOfTracks();
	
	if (tracks.size() > 0)
		emit signSendAllTracks(tracks,0);
	else
		tracksStat->setText(tr("файлов нет"));

	QStringList photos;
	photos = _getListOfPhotos();

	if (photos.size() > 0)
		emit signSendAllPhotos(photos,0);
	else
		photosStat->setText(tr("файлов нет"));


	

	if(_isMarks())
		emit signSendAllMarks(0);
	else
		marksStat->setText(tr("файлов нет"));


	QStringList videos;
	videos = _getListOfVideos();

	if (videos.size() > 0)
		emit signSendAllVideos(videos,0);
	else
		videosStat->setText(tr("файлов нет"));
}

void GeneralSendDataClass::_slotSetSendProgressTracks(short progress,short maximum)
{
	sendTracksProgress->setMaximum(maximum);
	sendTracksProgress->setValue(progress);

	if(progress == maximum)
	{
		if(_tracksSended && delTrack->isChecked())
			_deleteFiles(_getListOfTracks());
	}
}

void  GeneralSendDataClass::_slotSetSendProgressPhotos(short progress,short maximum)
{
	sendPhotosProgress->setMaximum(maximum);
	sendPhotosProgress->setValue(progress);

	if(progress == maximum)
	{
		if(_photosSended && delPhoto->isChecked())
			_deleteFiles(_getListOfPhotos());
	}
}

void GeneralSendDataClass::_slotSetSendProgressMarks(bool sended)
{
	if(sended)
	{
		sendMarksProgress->setMaximum(1);
		sendMarksProgress->setValue(1);

		if(delMarks->isChecked())
		{
			QString markPath = _pathToMark +"\\marks.file";
			QFile fileMarks(markPath);
			if(fileMarks.exists ())
				fileMarks.remove ();			
		}
	}
	else
	{
		sendMarksProgress->setMaximum(1);
		sendMarksProgress->setValue(0);
	}
}

void GeneralSendDataClass::_slotSetSendProgressVideos(short progress,short maximum)
{
	sendVideoProgress->setMaximum(maximum);
	sendVideoProgress->setValue(progress);
	
	if(progress == maximum)
	{
		if(_videosSended && delVideo->isChecked())
			_deleteFiles(_getListOfVideos());	
	}
}

void GeneralSendDataClass::_slotSetSendStatusTracks(bool status)
{
	_tracksSended = status;

	if(status)
	{
		tracksStat->setText(tr("доставлено"));		
	}
	else
	{
		tracksStat->setText(tr("не доставлено"));
	}
}

void GeneralSendDataClass::_slotSetSendStatusPhotos(bool status)
{
	_photosSended = status;

	if(status)
	{
		photosStat->setText(tr("доставлено"));
	}
	else
	{
		photosStat->setText(tr("не доставлено"));
	}
}

void GeneralSendDataClass::_slotSetSendStatusMarks(bool status)
{
	if(status)
	{
		marksStat->setText(tr("доставлено"));
	}
	else
	{
		marksStat->setText(tr("не доставлено"));
	}
}

void GeneralSendDataClass::_slotSetSendStatusVideos(bool status)
{
	_videosSended = status;

	if(status)
	{
		videosStat->setText(tr("доставлено"));
	}
	else
	{
		videosStat->setText(tr("не доставлено"));
	}
}

void GeneralSendDataClass::_slotBack()
{
	this->close();
}

QStringList GeneralSendDataClass::_getListOfTracks()
{
	FileExplorerClass _fileExplorer(this,_pathToTracks,QStringList() << "*.current" << "*.finished");
	return _fileExplorer.getStringListOfFiles();
}

bool GeneralSendDataClass::_isMarks()
{
	QFile marks(_pathToMark+"//marks.file");
	if(marks.exists())
		return true;
	else
		return false;
}

QStringList GeneralSendDataClass::_getListOfPhotos()
{
	FileExplorerClass _fileExplorer(this,_pathToPhoto,QStringList() << "*.jpg");
	return _fileExplorer.getStringListOfFiles();
}

QStringList GeneralSendDataClass:: _getListOfVideos()
{
	FileExplorerClass _fileExplorer(this,_pathToVideo,QStringList() << "*.avi");
	return _fileExplorer.getStringListOfFiles();
}

void GeneralSendDataClass::_slotButtonClickSound()
{
	CalcFunctions::soundPressedBut(5);
}

void GeneralSendDataClass::_deleteFiles(QStringList listOfFiles)
{
	for (int i = 0; i < listOfFiles.size(); ++i)
	{
		QFile file(listOfFiles.at(i));
		if(file.exists())
		{
			file.copy(QCoreApplication::applicationDirPath ()+ "/tmp/" + listOfFiles.at(i));
			file.remove();

		}
	}
}

void GeneralSendDataClass::_slotDelMarks()
{
	if(!delMarks->isChecked())
		return;

	SelectDialog dlg1(_parent,tr("Мастер"),
	tr("\nУдалить данные\nпосле успешной отправки?"));
	dlg1.exec();
	if(dlg1.result == 0)
		delMarks->setChecked(false);
}

void GeneralSendDataClass::_slotDelPhoto()
{
	if(!delPhoto->isChecked())
		return;
	SelectDialog dlg1(_parent,tr("Мастер"),
	tr("\nУдалить данные\nпосле успешной отправки?"));
	dlg1.exec();
	if(dlg1.result == 0)
		delPhoto->setChecked(false);
}

void GeneralSendDataClass::_slotDelTrack()
{
	if(!delTrack->isChecked())
		return;
	SelectDialog dlg1(_parent,tr("Мастер"),
	tr("\nУдалить данные\nпосле успешной отправки?"));
	dlg1.exec();
	if(dlg1.result == 0)
		delTrack->setChecked(false);
}

void GeneralSendDataClass::_slotDelVideo()
{
	if(!delVideo->isChecked())
		return;
	SelectDialog dlg1(_parent,tr("Мастер"),
	tr("\nУдалить данные\nпосле успешной отправки?"));
	dlg1.exec();
	if(dlg1.result == 0)
		delVideo->setChecked(false);
}