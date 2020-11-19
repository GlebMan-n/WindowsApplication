#include "stdafx.h"

class BreakLenLine;
class LineGetInterface;
class MapBaseWidget;
class LineDialogClass;
class LassoTool;
class GridBLData;

class MapToolsClass : public QObject
{
	Q_OBJECT
public:
	MapToolsClass(QWidget* parent,MapBaseWidget* map);
	~MapToolsClass();
private:
	BreakLenLine* _breakLineTool;
	LineGetInterface* _lenLineTool;
	MapBaseWidget* _map;
	QWidget* _parent;
	LineDialogClass* lineDialog;
	LassoTool* toolLasso;
	GridBLData *gridLayer;
private slots:
	void _slotBreakLine();
	void _slotLenLine();
	void _slotLassoTool();
	void _slotLineDialog();
	void _slotDelTools();
	void _slotDayNight();
	void _slotGrid();
signals:
	void signUpdateMap();
};