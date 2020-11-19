#include "ui_MapMark.h"
#include <QtGui>

class MapMarkClass : public QWidget, public Ui::MarkMenu
{
	Q_OBJECT
public:
	MapMarkClass(QWidget* parent);
	~MapMarkClass();
private:
	QWidget* _parent;
	double _lat;
	double _lon;
	double _height;
	QString _pathToMark;
	bool _fromMap;
private:
	bool _loadSettings();
	bool _saveMarkToFile(QString type,QString date, QString time, QString description, QString lat, QString lon,QString height);
private slots:
	void _slotGpsData(QTime time, QDate date, bool status, double lat, double lon, double speed, double angle,short sat, float height);
	void _slotCoordMap(double lat, double lon);
	void _slotMark();
	void _slotKeyboard();
	void _slotButtonClickSound();
	void _slotFromMap();
	void _slotFromCoord();
signals:
	//сообщаем о типе и координатах отметки
	void signMark(short type, double lat, double lon);
	void signMarkEdit();

};