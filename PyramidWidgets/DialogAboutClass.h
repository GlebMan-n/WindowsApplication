#include "ui_DialogAbout.h"
#include <QtGui>

class DialogAboutClass : public QDialog, public Ui::AboutDlg
{
	Q_OBJECT
public:
	DialogAboutClass(QWidget* parent,double version);
	~DialogAboutClass();

private:
	void _setVer(double version);
private slots:
	void _slotBack();
};