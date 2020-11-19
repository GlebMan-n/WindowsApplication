#include <QtGui>
#include "ui_MapSendPhoto.h"

class FileExplorerClass;
class MapSendPhoto : public QDialog, public Ui::SendPhotoMenu
{
	Q_OBJECT
public:
	MapSendPhoto(QWidget* parent);
	~MapSendPhoto();	
private:
	void _init();
	bool _loadSettings();
	bool _deleteFile();
	void _updateFileInfo();
	QString _namePhoto(QString fullPath);
private:
	FileExplorerClass* _fileExplorer;
	QString _pathToPhotos;
	short _toKcps;
	QWidget * _parent;

private slots:
	void _slotNextFile();
	void _slotPervFile();
	void _slotDeleteFile();
	void _slotSendPhoto();
	void _slotSendPhotoStatus(bool status);
	void _slotToKcpsChanged(int toKcps);

	void _slotButtonClickSound();

signals:
	//путь к файлу для отправки, дискретность точек трека
	void signSendPhoto(QString path, short toKcps);
};