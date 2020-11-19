#include <QtGui>
#include "ui_MapSendTrack.h"

class FileExplorerClass;
class MapSendTrack : public QDialog, public Ui::SendTrackMenu
{
	Q_OBJECT
public:
	MapSendTrack(QWidget* parent);
	~MapSendTrack();	
private:
	void _init();
	bool _loadSettings();
	bool _deleteFile();
	void _updateFileInfo();
	QString _firstNameTrack(QString fullPath);
	QString _lastNameTrack(QString fullPath);
	QString _dateTimeFormat(QString dateTime);
	int _getDiscret();
	QString _sizeToStr(qint64 size);
private:
	FileExplorerClass* _fileExplorer;
	QString _pathToTracks;
	short _toKcps;
	QWidget * _parent;

private slots:
	void _slotNextFile();
	void _slotPervFile();
	void _slotDeleteFile();
	void _slotSendTrack();
	void _slotDrawTrack();
	void _slotToKcpsChanged(int toKcps);
	void _slotSendTrackStatus(bool status);

	void _slotButtonClickSound();

signals:
	//путь к файлу для отправки, дискретность точек трека
	void signSendTrack(QString path, int discret, short toKcps);
	void signDrawTrack(QString path,int discret);
	void signClearTrack();
};