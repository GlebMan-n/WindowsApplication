#include "ui_MapRotate.h"
#include <QtGui>

class MapRotateClass : public QWidget, public Ui::RotateMenu
{
	Q_OBJECT
public:
	MapRotateClass(QWidget* parent);
	~MapRotateClass();
	private slots:
		void _slotButtonClickSound();
signals:
	void signNothOrient();
	void signSouthOrient();
	void signMoveOrient();
};