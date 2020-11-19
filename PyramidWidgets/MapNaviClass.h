#include "ui_MapSelectOrient.h"
#include <QtGui>

class MapNaviClass : public QWidget, public Ui::SelectOrientDlg
{
	Q_OBJECT
public:
	MapNaviClass(QWidget* parent);
	~MapNaviClass();
private:
	bool _loadSettings();
private:
	double _lat;
	double _lon;
	QWidget* _parent;
private slots:
	void _slotInputCoord();
	void _slotMapCoord();
	void _slotButtonClickSound();
signals:
	void signRouteNavi();
	void signCoordNavi(double lat,double lon);
	void signPointNavi();
};