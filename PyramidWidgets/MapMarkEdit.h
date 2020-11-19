#include "ui_MapMarkEdit.h"
#include <QtGui>

struct MARK 
{
	MARK()
	{
		type = "";
		date = "";
		time = "";
		info  = "";
		lon = "";
		lat = "";
		height = "";
	}
	QString type;
	QString date;
	QString time;
	QString info;
	QString lon;
	QString lat;
	QString height;
};

class MapMarkEdit : public QWidget, public Ui::MapMarkEdit
{
	Q_OBJECT
public:
	MapMarkEdit(QWidget* parent);
	~MapMarkEdit();
private:
	QWidget* _parent;
	QString _pathToMark;
	//вектор для хранения отметок
	std::vector<MARK> marks;
	short _currentIndex;
private:
	bool _loadSettings();
	bool _loadMarksFromFile(QString path);
	bool _saveMarksToFile(QString path);
	void _addMarkToVector(QString str);
	void _showMarkData(short index);
	short _nextMark();
	short _pervMark();
	void _cancel();
	bool _save();
	bool _deleteCurrentMark();
	bool _saveChangesToVector();
private slots:
	void _slotKeyboard();
	void _slotDelete();
	void _slotNextMark();
	void _slotPervMark();
	void _slotSave();
	void _slotCancel();
	
signals:
	//обновить отметки на карте
	void signUpdateMarks();
};