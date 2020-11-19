#include "stdafx.h"
#include <QObject>
#include <gis\GrUnits\StoreVectorData.h>
#include <gis\GisBase\cppgis\TypeGisToken.h>

class TknNonScale;
class MapBaseWidget;
class CurrentPosToken : public QObject
{
	Q_OBJECT
public:
	CurrentPosToken(QObject* parent,MapBaseWidget* map, StoreVectorData<FastVectorData>* tokenLayer);
	~CurrentPosToken();
	//инициализация знака
	bool initToken();
	//удаление знака
	void removeToken();
private:
	bool _init(QString pathToImages = "null",double lat = 0,double lon = 0);
	
private:
	TknNonScale* _myPosToken;
	MapBaseWidget* _map;
	TypeGisToken _typeGisToken;
	StoreVectorData<FastVectorData>* _tokenLayer;

private slots:
	void _slotGpsData(QTime time, QDate date, bool status, double lat, double lon, double speed, double angle,short sat, float height);
	void _slotViewPos(double lat, double lon);
};