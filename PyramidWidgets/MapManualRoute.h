#include "ui_MapManualRoute.h"
#include <QtGui>

class MapManualRoute : public QWidget, public Ui::MapManualRoute
{
	Q_OBJECT
public:
	MapManualRoute(QWidget* parent);
	~MapManualRoute();
private:
	bool _loadSettings();
private:
	double _defLat;
	double _defLon;
	QWidget* _parent;
private slots:
	void _slotButtonClickSound();
	void _slotCoordMRoute();	
signals:
	void signManualRoute();
	void signMRouteCoord(double lat, double lon);
	void signClearMRoute();
};