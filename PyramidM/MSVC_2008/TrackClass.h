#include "stdafx.h"
#include <QtGui>
#include <gis\GrUnits\StoreVectorData.h>


class TknSimpleLines;
class TknRoute;

class TrackClass : public QObject
{
	Q_OBJECT
public:
	TrackClass(QObject* parent, StoreVectorData<FastVectorData>* layer);
	~TrackClass();	
	void setLayer(StoreVectorData<FastVectorData>* layer);
	/*�������, 
	����:
	0 - ������;
	1 - �������;
	2 - �����;
	3 - �������;
	4 - ����������;
	*/
	void setTrackParam(double width, short color = 0);
private:
	bool _logTrack(QString string, QString path);
	bool _logTrack(QTime time, QDate date, bool status, double lat, double lon, double speed, double angle,short sat, float height, QString path,short com);
	bool _loadSettings();
	void _saveSettings();
	bool _isCurrentTrackCreated();
	QString _nameOfCurrentTrack();
	QString _createCurrentTrack();
	bool _finishCurrentTrack();
	void _clearTrack();
	bool _addTrackPoint(double lat, double lon);
	bool _addTrackPoint(double lat, double lon, TknSimpleLines* track, TknRoute* tknTrack,QTime time, QDate date, double altitude, double speed);
	void _drawTrack(QString path, double discret = 0,bool drawTime = true); //������������ � ������
	void _drawTrack(QString path,TknSimpleLines* track,double discret = 0);//������������ � ������
	void _showTrack();
	bool _timeToBreak(int minsToBreak);
	QTime _timeOfCreateCurrentTrack(QString fullPath);
	QDate _dateOfCreateCurrentTrack(QString fullPath);
private:
	QString _pathToTrack;
	QString _nameOfPc;
	TknSimpleLines* _track;
	TknSimpleLines* _trackTmp;

	TknRoute* _tknTrack;
	TknRoute* _tknTrackTmp;

	StoreVectorData<FastVectorData>* _trackLayer;
	double _lat;
	double _lon;
	//short _color;
	short _trackFormat;
	short _badSign;
	short _format;
	short _color;
	double _width;
	short _discretType;
	short _discret;
	short _autoBreak;
	int _pointsSaved;
	short _isDataCurrent;
private slots:
	void _slotGpsData(QTime time, QDate date, bool status, double lat, double lon, double speed, double angle,short sat, float height,short com);
	void _slotGpsData(QString string, bool isChecked);
	void _slotUpdateSettings();
	void _slotBreakTrack(bool clear);
	void _slotDrawTrack(QString trackPath,int discret);
	void _slotClearTrack();	
signals:
	void signTrackLogged(bool logged);
	void signFullRedrawMap();
};