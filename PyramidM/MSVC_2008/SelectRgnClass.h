#include "stdafx.h"
#include "ui_MapSelectTopo.h"
#include <QtGui>

class MapBaseWidget;
class ToolBarRegion;

class SelectRgnClass : public QDialog, public Ui::SelectMapClass
{
	Q_OBJECT
public:

	SelectRgnClass(QMainWindow *parent, MapBaseWidget* mapWidget);
	~SelectRgnClass();
private:
	ToolBarRegion *_toolBarRegion;
private:
	bool _createRegion(MapBaseWidget* m_mapWidget);
	QRect _calcWindowPos(QWidget* widget);
};