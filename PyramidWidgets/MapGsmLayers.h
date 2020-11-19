#include "ui_MapGsmLayers.h"
#include <QtGui>

class MapGsmLayers : public QWidget, public Ui::MapGsmLayers
{
	Q_OBJECT
public:
	MapGsmLayers(QWidget* parent);
	~MapGsmLayers();
	void init();
private slots:
	void _slotButtonClickSound();

	void _slotShowMegafon();
	void _slotShowBeeline();
	void _slotShowMts();
	void _slotShowTele2();


	void _slotStatusGsmLayers(bool megafon_, bool beeline_,bool mts_, bool tele2_);
signals:
	void signGetStatusGsmLayers();

	void signShowMegafon(bool show);
	void signShowBeeline(bool show);
	void signShowMts(bool show);
	void signShowTele2(bool show);

};