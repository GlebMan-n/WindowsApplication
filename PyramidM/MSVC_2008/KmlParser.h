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


//����� �������� �������������� KML ������
//��� ����������� ��� �������� GSM �����
class KmlClass : public QThread
{
	Q_OBJECT
public:
	KmlClass(StoreVectorData<FastVectorData>* layer, GdiGis::Color color, QObject* parent = 0, short numbOfThread = 0);
	~KmlClass();
	//���������� ���� � ����� KML ������
	void setPathToDirKml(QString path);
	//��������� ����� � ������� ��� �������� ���������� ������
	void setNumOfThread(short numbOfThread);
	//�������� ���� �� ���������
	void clear();
	//���������� �����
	void stop();
	//���������� �� �����
	bool isStopped();
	
private:
	volatile bool _stopped;
	short _numbOfThread;
	QString _path;	
	//���� ��� ���������
	StoreVectorData<FastVectorData>* _layer;
	//��� �������� (����, �������)
	TypePolygonObj* m_pTypePoly;

private:
	//��������� ����
	void _loadPolyFromKmlFile(QString path);
	//������ ��������(<document>), ���������� ������ �����
	void _docParser(QDomNode nextDocNode);
	//��������� ������������ ���������
	bool _isPosGood(Pos pos);
	//��������� ���������� � �������
	void _doWorkWithCoordinates(QString coordinates);
protected:
	void run();
signals:
	void signStatus(QString status, short numbOfThread);
	void signFinish(short numbOfThread);
};

//����� ���������� ��������� KML ������
//��� ����������� ������ � �������� GSM
class LoadKmlFilesClass : public QWidget
{
	Q_OBJECT
public:
	LoadKmlFilesClass(
		//����
		StoreVectorData<FastVectorData>* layer,
		//����
		GdiGis::Color color,
		//���� � ���������� � �������
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
		//���������� �����
		void _initKmlClass(short numbOfThread);
signals:
	void signWorkCompleate();

};