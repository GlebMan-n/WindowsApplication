
#include "ui_DialogSelect.h"
#include <QDialog>
#include <QTimer>
class SelectDialog : public QDialog, public Ui::SelectDialogClass
{
	Q_OBJECT
public:
	SelectDialog(QWidget *parent,QString caption, QString text,bool trBackg = true,short closeTimeSec = 0);
	int result;
	void setText(QString str);
	void setWindowCaption(QString str);
	
private:
	QTimer* timer;
private slots:
	void slotOk();
	void slotCancel();
	void _slotButtonClickSound();
signals:
	void signalOk();
	void signalCancel();

};