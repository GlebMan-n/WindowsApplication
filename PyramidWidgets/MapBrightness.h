#include "ui_MapBrightness.h"
#include <QtGui>

class MapBrightness : public QWidget, public Ui::MapBrightness
{
	Q_OBJECT
public:
	MapBrightness(QWidget* parent);
	~MapBrightness();
	void init();
private slots:
	void _slotButtonClickSound();
	void _slotBrightMinus();
	void _slotBrightPlus();
	void _slotGetBright(short bright);
signals:
	void signGetBright();
	void signBrightUp();
	void signBrightDown();
};