#include "ui_MapPhotoMenu.h"
#include <QDialog>
#include <QThread>
#include <QTime>
#include <PyramidWidgets\opencv\build\include\opencv2\core\types_c.h>
class CvCapture;
class CvVideoWriter;


class MapPhotoClass: public QDialog, public Ui::PhotoMenu
{
	Q_OBJECT
public:
	MapPhotoClass(QWidget *parent = 0);
	~MapPhotoClass();
	static QImage*  IplImage2QImage(IplImage *iplImg);
private:
	
	struct MyThread : public QThread { using QThread::msleep;};
	void _runVideo();
	bool _isVideoWriting();
	void _stopWriteVideo();
	bool _loadSettings();
	bool _lightOn;

private:
	CvCapture* capture;
	CvVideoWriter *writer;
	QTimer* m_VideoTimer;

	bool lightOn;
	bool b_vidio;
	bool b_demo;
	QString _pathToVideo;
	QString _pathToPhoto;

	double _lat;
	double _lon;

	bool _noVideoDir;
	bool _noPhotoDir;
	short _videoSource;

private slots:
	void _slotGpsData(QTime time, QDate date, bool status, double lat, double lon, double speed, double angle,short sat, float height);
	void _slotVideo();
	void _slotMakePhoto();
	void _slotLight();
	void _slotSaveVideo();
	void _slotCloseThis();
	void _slotVideoScreen();
	void _slotButtonClickSound();
	void _slotPhotoSound();
signals:
	void signClose();
};