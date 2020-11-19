#include "ui_MapMap.h"
#include <QtGui>

class MapMapClass : public QWidget, public Ui::MapMap
{
	Q_OBJECT
public:
	MapMapClass(QWidget* parent);
	~MapMapClass();
	private slots:
		void _slotButtonClickSound();
signals:
	void signSelectMap();
	void signLasso();
	void signDayNight();
	void signGoToPoint();
	void signLayers();
};