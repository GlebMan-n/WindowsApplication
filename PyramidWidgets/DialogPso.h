
#include "ui_DialogPSO.h"
#include <QDialog>
#include <QTimer>

class PsoDialog : public QDialog, public Ui::psoDlg
{
	Q_OBJECT
public:
	PsoDialog(QWidget *parent);
	int result;
	void setDateTime(QString date);
private:
	QWidget* _parent;
private slots:
	void slotNew();
	void slotCont();
	void _slotButtonClickSound();
signals:
	void signalNew();
	void signalCont();

};