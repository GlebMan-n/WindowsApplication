#include "ui_MapSelectTool.h"
#include <QtGui>

class MapSelectToolClass : public QWidget, public Ui::SelectToolZamer
{
	Q_OBJECT
public:
	MapSelectToolClass(QWidget* parent);
	~MapSelectToolClass();
	private slots:
		void _slotButtonClickSound();
signals:
	void signLineTool();
	void signBreakLineTool();
	void signManualRoute();
};