#include "stdafx.h"
#include "ui_DialogExchange.h"


class ExchangeDialog : public QDialog, public Ui::ExchangeDialogClass
{
	Q_OBJECT
public:
	ExchangeDialog(QWidget *parent, QString text,short closeTimeSec = 0);
	ExchangeDialog();
	~ExchangeDialog();

private:
	void setText(QString str);
	QTimer* timer;
private slots:
	void slotSetProgress(qint64 bytesReceived, qint64 bytesTotal, double speed);
	void slotClose();


};