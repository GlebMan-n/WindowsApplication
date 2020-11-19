//Класс отображения навигационных объектов на карте
#include "stdafx.h"
#include <QObject>
#include <gis\GrUnits\StoreVectorData.h>

class TknSimpleLines;
class DrawNaviClass : public QObject
{
	Q_OBJECT
public:
	DrawNaviClass(QObject* parent,StoreVectorData<FastVectorData>* layer);
	~DrawNaviClass();
	void hide();
private:
	void _init();
	bool _showNavi;
private:
	StoreVectorData<FastVectorData>* _workLayer;

	/*TknSimpleLines* _endRoutePoint;*/
	TknSimpleLines* _naviLine;
private slots:
	void _slotEndPoint(double lat, double lon);
	void _slotNaviLine(double posLat, double posLon, double pointLat, double pointLon);
	void _slotClearNavi();
};
