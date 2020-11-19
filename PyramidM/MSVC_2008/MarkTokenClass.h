#include "stdafx.h"
#include <QObject>
#include <gis\GrUnits\StoreVectorData.h>
#include <gis\GisBase\cppgis\TypeGisToken.h>

class TknNonScale;
class MapBaseWidget;

class MarkTokenClass : public QObject
{
	Q_OBJECT
public:
	MarkTokenClass(QObject* parent,MapBaseWidget* map, StoreVectorData<FastVectorData>* tokenLayer);
	~MarkTokenClass();
	//инициализация знака
	
	//удаление знака
	bool removeTokens();
private:
	bool _addMark(short typeOfToken,double lat,double lon);
	bool _loadSettings();
	//загрузка отметок из файла
	bool _loadMarks();
private:
	MapBaseWidget* _map;
	
	StoreVectorData<FastVectorData>* _marksLayer;
	QString _pathToImages;
	QString _pathToMarks;

private slots:
	void _slotMark(short type, double lat, double lon);
	void _slotUpdateMarks();
};