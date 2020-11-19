#include "stdafx.h"
#include <QObject>

#include <gis\GrUnits\StoreVectorData.h>

class TknRoute;
class MapBaseWidget;
class RouteClass : public QObject
{
	Q_OBJECT
public:
	RouteClass(QWidget *parent, MapBaseWidget* map, StoreVectorData<FastVectorData>* routeLayer);
	~RouteClass();
	bool drawRoute(QString path);
	static QString nmeaToTkn(QString pathToNmea, int meters);
public:
	bool drawRouteOnMap(TknRoute* tknRoute,MapBaseWidget *m_mapWidget, StoreVectorData<FastVectorData>* m_pLayer = NULL,bool clearLayer = false/*чистить ли слой?*/);
	bool drawRouteOnMap(QString path, MapBaseWidget *m_mapWidget,StoreVectorData<FastVectorData>* m_pLayer = NULL,bool clearLayer = false);
	QString renameRoute(QString pathDir);
private:
	TknRoute* _tknRoute;
	MapBaseWidget* _map;
	StoreVectorData<FastVectorData>* _routeLayer;
	QWidget* _parent;
	QString _pathToRoute;
private:
	//переименуем файл палетки в имя палетки
	bool _renameRoute(QString pathDir);
	bool _loadSettings();
	bool _routeFromFile(QString path);
public:
	static TknRoute* routeFromFile(QString path);
	static bool routeToFile(TknRoute* tknRoute,QString path);
private slots:
	void _slotRouteLoaded(bool loaded);
	void _slotDrawRoute(QString path);
signals:
	void signPathToRoute(QString fullPath);
	
};
