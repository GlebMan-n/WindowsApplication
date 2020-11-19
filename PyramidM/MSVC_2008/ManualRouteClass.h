#include "stdafx.h"
#include <QtGui>
#include <gis\GrUnits\StoreVectorData.h>

class TknRoute;
class MapBaseWidget;

class ManualRoute : public QObject
{
	Q_OBJECT
public:
	ManualRoute(QObject* parent, StoreVectorData<FastVectorData>* layer,MapBaseWidget* map);
	~ManualRoute();	
	void setTrackParam(double width, short color = 0);
	void clear();
private:
	void _addPoint(double lat,double lon,QTime time = QTime(0,0,0));
	bool _saveManualRoute(QString path);
	bool _loadManualRoute(QString path);
	bool _loadSettings();
private:
	StoreVectorData<FastVectorData>* _mRouteLayer;
	TknRoute* _mRoute;

	short _color;
	MapBaseWidget* _map;
	QString _pathToMRoute;
private slots:
	void _slotAddPoint(double lat, double lon, QTime time = QTime(0,0,0));
	void _slotClearMRoute();
};	


