#include "ui_DialogLock.h"
#include <QtGui>

class MapLockScreenClass: public QDialog, public Ui::LockScreenDlg
{
	Q_OBJECT
public:
	MapLockScreenClass(QWidget *parent = 0);
	void init();
private:
	bool b_btnPressed;
	double _x;
	double _y;
	double _xPred;
	double _xPred1;
	bool checkButtonClicked(double x, double y);
private slots:
	void _slotButtonClickSound();

protected:
	void mousePressEvent ( QMouseEvent * event );
	virtual void mouseMoveEvent(QMouseEvent* pe);
	void mouseReleaseEvent ( QMouseEvent * event );

signals:
	void signBrightMax();
	void signBrightMin();
};