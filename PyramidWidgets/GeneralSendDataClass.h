
#include "ui_GenSendData.h"
#include <QtGui>

class GeneralSendDataClass : public QDialog, public Ui::SendAllData
{
	Q_OBJECT
public:
	GeneralSendDataClass(QWidget *parent = 0);
	~GeneralSendDataClass();
private:
	void _initTime();
	bool _loadSettings();
	QStringList _getListOfTracks();
	QStringList _getListOfPhotos();
	QStringList _getListOfVideos();
	void _deleteFiles(QStringList listOfFiles);
	bool _isMarks();
	QWidget* _parent;
	
private:
	QString _pathToPhoto;
	QString _pathToVideo;
	QString _pathToTracks;
	QString _pathToMark;
	bool _tracksSended;
	bool _photosSended;
	bool _marksSended;
	bool _videosSended;
private slots:
	void _slotBack();
	void _slotTime(QString day, QString sdate, QString stime);
	void _slotAllToVpu();
	void _slotAllToKcps();

	void _slotSetSendProgressTracks(short progress,short maximum);
	void _slotSetSendProgressPhotos(short progress,short maximum);
	void _slotSetSendProgressMarks(bool sended);
	void _slotSetSendProgressVideos(short progress,short maximum);

	void _slotSetSendStatusTracks(bool status);
	void _slotSetSendStatusPhotos(bool status);
	void _slotSetSendStatusMarks(bool status);
	void _slotSetSendStatusVideos(bool status);
	void _slotButtonClickSound();
	void _slotDelMarks();
	void _slotDelPhoto();
	void _slotDelTrack();
	void _slotDelVideo();

signals:
	//0 - КЦПС, 1 - ВПУ , 2 - ? ...
	void signSendAllTracks(QStringList listOfTracks,short source);
	void signSendAllPhotos(QStringList listOPhotos,short source);
	void signSendAllMarks(short source);
	void signSendAllVideos(QStringList listOfVideos,short source);
	
	//удаление после отправки
	void signDelAllSendedPhotos();
	void signDelAllSendedTracks();
	void signDelAllSendedMarks();
	void signDelAllSendedVideos();
};