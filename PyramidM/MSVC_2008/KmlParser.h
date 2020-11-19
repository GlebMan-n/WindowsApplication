#include "stdafx.h"
#include <QObject>
#include <QDomDocument>
#include <gis\GrUnits\StoreVectorData.h>

class TknPolygon;
class TypePolygonObj;

struct Pos
{
	Pos()
	{
		lat = 0;
		lon = 0;
	}
	Pos(double _lat, double _lon)
	{
		lat = _lat;
		lon = _lon;
	}

	double lat;
	double lon;
};

struct PolygonStyle
{
	PolygonStyle()
	{
		color = "";
		visible = 100;
	}
	QString color;
	short visible;

};

struct LineStyle
{
	LineStyle()
	{
		color = "";
		visible = 100;
		width = 1;
	}
	QString color;
	short visible;
	short width;
};

struct KmlStyle
{
	KmlStyle()
	{
		id = "";
		threahold_value = 0;
		polygonStyle = PolygonStyle();
		lineStyle = LineStyle();
	}	

	QString id;
	double threahold_value;
	PolygonStyle polygonStyle;
	LineStyle lineStyle;
};


// ласс загрузки подготовленных KML файлов
//ƒл€ отображени€ зон покрыти€ GSM сетей
class KmlClass : public QThread
{
	Q_OBJECT
public:
	KmlClass(StoreVectorData<FastVectorData>* layer, GdiGis::Color color, QObject* parent = 0, short numbOfThread = 0);
	~KmlClass();
	//установить путь к папке KML файлов
	void setPathToDirKml(QString path);
	//установим номер в очереди дл€ текущего экземпл€ра класса
	void setNumOfThread(short numbOfThread);
	//очистить слой от полигонов
	void clear();
	//остановить поток
	void stop();
	//остановлен ли поток
	bool isStopped();
	
private:
	volatile bool _stopped;
	short _numbOfThread;
	QString _path;	
	//слой дл€ полигонов
	StoreVectorData<FastVectorData>* _layer;
	//тип полигона (цвет, заливка)
	TypePolygonObj* m_pTypePoly;

private:
	//загружаем файл
	void _loadPolyFromKmlFile(QString path);
	//парсим документ(<document>), возвращаем вектор точек
	void _docParser(QDomNode nextDocNode);
	//провер€ем корректность координат
	bool _isPosGood(Pos pos);
	//добавл€ем координату в полигон
	void _doWorkWithCoordinates(QString coordinates);
protected:
	void run();
signals:
	void signStatus(QString status, short numbOfThread);
	void signFinish(short numbOfThread);
};

//класс управлени€ загрузкой KML файлов
//дл€ отображени€ данных о покрытии GSM
class LoadKmlFilesClass : public QWidget
{
	Q_OBJECT
public:
	LoadKmlFilesClass(
		//слой
		StoreVectorData<FastVectorData>* layer,
		//цвет
		GdiGis::Color color,
		//путь к директории с файлами
		QString dirPath,
		QObject* parent = 0);

	~LoadKmlFilesClass();
private:
	void _saveSettings();
	bool _loadSettings();
private:
	short _numOfCore;

	QVector <KmlClass*> klmClasses;
	QFileInfoList fileList;
	StoreVectorData<FastVectorData>* _layer;
	GdiGis::Color _color;

	QString _dirPath;
	struct MyThread : public QThread { using QThread::msleep;};	
	public slots:
		void _slotBegin();
		//инициируем поток
		void _initKmlClass(short numbOfThread);
signals:
	void signWorkCompleate();

};