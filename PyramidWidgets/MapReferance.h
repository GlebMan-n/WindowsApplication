#include "ui_MapReferance.h"
#include <QtGui>

class MapReferanceClass : public QWidget, public Ui::MapReferance
{
	Q_OBJECT
public:
	MapReferanceClass(QWidget* parent);
	~MapReferanceClass();
	QFileInfoList _fileInfo;
	void setPathList(QFileInfoList fileInfo);
	void init();
private:
	short _currentFileIndex;
private:
	void _showCurrentFile();

private slots:
	void _slotButtonClickSound();
	void _nextRef();
	void _pervRef();


};