#include "ui_MapVideo.h"
#include <QDialog>
#include <QThread>
#include <QTime>
#include <PyramidWidgets\opencv\build\include\opencv2\core\types_c.h>

class CvCapture;
class CvVideoWriter;
class FileExplorerClass;

class MapVideoClass: public QDialog, public Ui::ViewVideoClass
{
	Q_OBJECT
public:
	MapVideoClass(QWidget *parent = 0);
	~MapVideoClass();
	
private:
	struct MyThread : public QThread { using QThread::msleep;};

	void _runVideo();
	void _init();
	bool _loadSettings();
	bool _deleteFile();
	void _updateFileInfo();
	QString _nameVideo(QString fullPath);
	
private:
	CvCapture* capture;
	QTimer* m_VideoTimer;
	QString _pathToVideo;
	QWidget * _parent;
	short _toKcps;
	FileExplorerClass* _fileExplorer;
	bool b_demo;

private slots:

	void _slotNextFile();
	void _slotPervFile();
	void _slotDeleteFile();
	void _slotSendVideo();
	void _slotPlayVideo();
	void _slotSendVideoStatus(bool status);
	void _slotVideoScreen();
	void _slotToKcpsChanged(int toKcps);
	void _slotButtonClickSound();

signals:
	//путь к файлу для отправки, дискретность точек трека
	void signSendVideo(QString path, short toKcps);

};