#include <PyramidWidgets\camera.h>
#include <PyramidWidgets\MapPhotoClass.h>
#include <PyramidM\MSVC_2008\CalcFunctions.h>
#include <QFile>
#include <QSettings>
#include <QCoreApplication>
#include <QLibrary>
#include <QDir>
#include <QTimer>

MapPhotoClass::MapPhotoClass(QWidget* parent)
{
	this->setParent(parent);
	setupUi(this);
	this->setGeometry(CalcFunctions::calcWindowPos(parent->geometry(),this->geometry()) );
	_lightOn = false;
	b_vidio = false;
	capture = NULL;
	writer = NULL;
	_pathToVideo = QCoreApplication::applicationDirPath () + "\\data\\video";
	_pathToPhoto = QCoreApplication::applicationDirPath () + "\\data\\photo";
	
	_lat = 0;
	_lon = 0;
	_videoSource = 1;
	_loadSettings();

	QDir dir;
	_noVideoDir = dir.mkpath(_pathToVideo);
	_noPhotoDir = dir.mkpath(_pathToPhoto);	
	
	m_VideoTimer = new QTimer(this);
	connect(m_VideoTimer, SIGNAL( timeout ()),
	this, SLOT (_slotVideoScreen()));


	connect(light_on_but,SIGNAL(clicked()),this,SLOT(_slotLight()));
	connect(make_photo_but,SIGNAL(clicked()),this,SLOT(_slotMakePhoto()));
	connect(make_vidio_but,SIGNAL(clicked()),this,SLOT(_slotSaveVideo()));


	connect(light_on_but,SIGNAL(clicked()),this,SLOT(_slotButtonClickSound()));
	connect(make_photo_but,SIGNAL(clicked()),this,SLOT(_slotPhotoSound()));
	connect(make_vidio_but,SIGNAL(clicked()),this,SLOT(_slotButtonClickSound()));
	this->save_photo_lbl->hide();
	_runVideo();	
}

MapPhotoClass::~MapPhotoClass()
{
	_slotCloseThis();
}

void MapPhotoClass::_runVideo()
{
	if(capture) 
	{
		m_VideoTimer->stop();
		cvReleaseCapture( &capture );
		capture = NULL;
		if(writer)
			cvReleaseVideoWriter(&writer);
		writer = NULL;
		this->save_photo_lbl->hide();
		return;
	}
	
	capture = cvCreateCameraCapture(0);  
	if (capture)
		m_VideoTimer->start(5);
	else
	{
		this->video_lbl->setPixmap(QPixmap(":/Pyramid/no_video.png"));
		MyThread::msleep(5000);
		this->close();
	}
}

bool MapPhotoClass::_isVideoWriting()
{
	return true;
}

void MapPhotoClass::_stopWriteVideo()
{
}

bool MapPhotoClass::_loadSettings()
{
	QString path_to_ini = QCoreApplication::applicationDirPath () + "/settings_akps.ini";
	
	if (QFile::exists(path_to_ini) ) 
	{
		QSettings akps_settings (path_to_ini, QSettings::IniFormat);

		_pathToPhoto = akps_settings.value("programm/path_to_photo").toString();
		_pathToVideo = akps_settings.value("programm/path_to_video").toString();
		_lat = akps_settings.value("programm/lastLat").toDouble();
		_lon = akps_settings.value("programm/lastLon").toDouble();
		return true;
	}

	return false;
}
void MapPhotoClass::_slotGpsData(QTime time, QDate date, bool status, double lat, double lon, double speed, double angle,short sat, float height)
{
	_lat = lat;
	_lon = lon;
}

void MapPhotoClass::_slotVideo()
{
}

void MapPhotoClass::_slotMakePhoto()
{
	if(capture)
	{
		save_photo_lbl->show();
		SYSTEMTIME st;
		GetLocalTime(&st);
		QString filename;
		filename = _pathToPhoto + "/," + QString::number(st.wDay) + "," + QString::number(st.wMonth) + "," + QString::number(st.wYear) + "," + QString::number(st.wHour) + "," + QString::number(st.wMinute) + "," + QString::number(st.wSecond) + "," + QString::number(_lat) + "," +  QString::number(_lon) + ",.jpg";
		QByteArray ar = filename.toLocal8Bit();
		char* path=ar.data();
		IplImage* frame;
		frame = 0;
		frame = cvQueryFrame( capture );
		cvSaveImage(path, frame);
	}
}

void MapPhotoClass::_slotLight()
{
	QFile file("dll/BLLDLL.dll");
	if(!file.exists ())
		return;

	QLibrary myLib("dll/BLLDLL.dll");
	typedef  bool (* SetPortVal)(int wPortAddr, int dwPortVal, byte bSize);
	SetPortVal setPortVal = (SetPortVal) myLib.resolve("SetPortValExV");
	if (setPortVal)
	{
		if(!_lightOn)
		{
			_lightOn = true; 
			setPortVal(0x6C, 0x41, 1);
		}
		else 
		{
			_lightOn = false; 
			setPortVal(0x6C, 0x40, 1);
		}
	}
}

void MapPhotoClass::_slotSaveVideo()
{
	if(capture)
	{
		//vidio
		if(!b_vidio)
		{			
			QString tmp(QCoreApplication::applicationDirPath () + "//tmp//tmp.avi");
			
			QByteArray ar = tmp.toLocal8Bit();
			char* path=ar.data();
			double fps = 5;
			CvSize size = cvSize(640, 480);
			writer = cvCreateVideoWriter(path, CV_FOURCC('X','V','I','D'), fps, size,1);
			b_vidio = true;
			save_photo_lbl->show();
			videoStat->setText(tr("идет запись видео..."));
		}
		else
		{
			
			cvReleaseVideoWriter(&writer);
			save_photo_lbl->hide();
			b_vidio = false;

			QString filename;

			SYSTEMTIME st;
			GetLocalTime(&st);
			filename = _pathToVideo + "/," + QString::number(st.wDay) + "," + QString::number(st.wMonth) + "," + QString::number(st.wYear) + "," + QString::number(st.wHour) + "," + QString::number(st.wMinute) + "," + QString::number(st.wSecond) + "," + QString::number(_lat) + "," +  QString::number(_lon) + ",.avi";
			QByteArray ar = filename.toLocal8Bit();

			QFile video(QCoreApplication::applicationDirPath () + "//tmp//tmp.avi");
			videoStat->setText(tr(""));
			if(video.open(QIODevice::ReadWrite))
			{
				video.rename(filename);
				video.close();
			}
		}
	}
}

void MapPhotoClass::_slotCloseThis()
{
	if(b_vidio)
		_slotSaveVideo();
	if(this->isVisible())
		_runVideo();

	this->close();
}

QImage*  MapPhotoClass::IplImage2QImage(IplImage *iplImg)
{
	int h = iplImg->height;
	int w = iplImg->width;
	int channels = iplImg->nChannels;
	QImage *qimg = new QImage(w, h, QImage::Format_ARGB32);
	char *data = NULL;
	data = iplImg->imageData;

	for (int y = 0; y < h; y++, data += iplImg->widthStep)
	{
		for (int x = 0; x < w; x++)
		{

			char r, g, b, a = 0;

			if (channels == 1)
			{
				r = data[x * channels];
				g = data[x * channels];
				b = data[x * channels];
			}
			else 
				if (channels == 3 || channels == 4)
				{
					r = data[x * channels + 2];
					g = data[x * channels + 1];
					b = data[x * channels];
				}

			if (channels == 4)
			{
				a = data[x * channels + 3];
				qimg->setPixel(x, y, qRgba(r, g, b, a));
			}
			else qimg->setPixel(x, y, qRgb(r, g, b));
			
		}
	}
	data = NULL;
	return qimg;

}
void MapPhotoClass::_slotVideoScreen()
{
	if (capture == NULL) return; 
	
	IplImage* frame;
    assert( capture );
    frame = 0;
    frame = cvQueryFrame( capture );
	QImage img;
	
	//vidio
	if(b_vidio)
	{
		cvWriteFrame(writer, frame);
	}

	img = *IplImage2QImage(frame);
	frame = 0;
	video_lbl->setPixmap(QPixmap::fromImage(img));
	img.~QImage(); 

	video_lbl->show();
	if(save_photo_lbl->isVisible())
	{
		if(!b_vidio)
		{
			MyThread::msleep(1500);
			save_photo_lbl->hide();
		}
	}
}

void MapPhotoClass::_slotButtonClickSound()
{
	CalcFunctions::soundPressedBut(5);
}

void MapPhotoClass::_slotPhotoSound()
{
	CalcFunctions::soundPressedBut(0);
}
