#include "ui_MapObjects.h"
#include <QtGui>

class MapObjects : public QWidget, public Ui::MapObjects
{
	Q_OBJECT
public:
	MapObjects(QWidget* parent);
	~MapObjects();
	void init();
private slots:
	void _slotButtonClickSound();

	void _slotShowRoute();
	void _slotShowGreed();
	void _slotShowMarks();
	void _slotShowTracks();
	void _slotShowCurrentPos();
	void _slotShowManualRoute();

	void _slotStatusObjects(bool route_, bool greed_,bool marks_, bool tracks_, bool currentPos_,bool manualRoute_);
signals:
	void signGetStatusObjects();

	void signShowRoute(bool show);
	void signShowGreed(bool show);
	void signShowMarks(bool show);
	void signShowTracks(bool show);
	void signShowCurrentPos(bool show);
	void signShowManualRoute(bool show);

};