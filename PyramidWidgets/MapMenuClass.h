
#include "ui_MapMenu.h"
#include <QtGui>

class MapMenuClass : public QWidget, public Ui::MapMenu
{
	Q_OBJECT
public:
	MapMenuClass(QWidget* parent);
	~MapMenuClass();
private:
	QWidget* _parent;
	QStateMachine machine_;
private:
	QRect _calcWindowPos();
	void _addWindowAnim();
private slots:
	void _slotButtonClickSound();
public slots:
	void slotShow();
signals:
	void signShow();
};