#include "stdafx.h"
#include <QObject>
#include <gis\GrUnits\StoreVectorData.h>

class TknPaletka;
class MapBaseWidget;

class PaletkaClass : public QObject
{
	Q_OBJECT
public:
	PaletkaClass(QWidget *parent, MapBaseWidget* map, StoreVectorData<FastVectorData>** gridLayer);
	~PaletkaClass();
	void drawGrid(QString path);
public:
	QString renamePal(QString pathDir);
	bool drawPaletkaOnMap(TknPaletka* tknPal,MapBaseWidget *m_mapWidget, StoreVectorData<FastVectorData>* m_pLayer = NULL,bool clearLayer = false/*чистить ли слой?*/);
	bool drawPaletkaOnMap(QString path, MapBaseWidget *m_mapWidget,StoreVectorData<FastVectorData>** m_pLayer = NULL,bool clearLayer = false);
private:
	//палетка
	TknPaletka* _tknPaletka;
	MapBaseWidget* _map;
	StoreVectorData<FastVectorData>* _gridLayer;
	QWidget* _parent;
	QString _pathToGrid;
private:
	//получить палетку по пути
	bool _palFromFile(QString path);
	bool _loadSettings();

private slots:
	void _slotGridLoaded(bool loaded);
	void _slotDrawGrid(QString path);
signals:
	void signPathToGrid(QString fullPath);
	
};
