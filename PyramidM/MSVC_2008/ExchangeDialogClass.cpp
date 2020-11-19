#include "stdafx.h"
#include <MSVC_2008\ExchangeDialogClass.h>
ExchangeDialog::ExchangeDialog(QWidget *parent, QString text,short closeTimeSec)
{
	this->setParent(parent);
	setupUi(this);
	setText(text);
	progressBar->setValue(0);
	setGeometry(parent->geometry());
	connect(this->close_tool_but,SIGNAL(clicked()),SLOT(slotClose()));

	if(closeTimeSec > 0)
	{
		timer = new QTimer(this);
		connect(timer, SIGNAL( timeout ()), 
			this, SLOT (slotClose()()));
		timer->start (closeTimeSec * 1000);
		
	}
}
ExchangeDialog::ExchangeDialog()
{
}
ExchangeDialog::~ExchangeDialog()
{
}

void ExchangeDialog::slotClose()
{
	this->close();
}

void ExchangeDialog::setText(QString str)
{
	captionLbl->setText(str);
}

void ExchangeDialog::slotSetProgress(qint64 bytesReceived, qint64 bytesTotal,double speed)
{
	QString unit;
    if (speed < 1024) {
        unit = "bytes/sec";
    } else if (speed < 1024*1024) {
        speed /= 1024;
        unit = "kB/s";
    } else {
        speed /= 1024*1024;
        unit = "MB/s";
    }
	this->speedLbl->setText(QString::fromLatin1("%1 %2").arg(speed, 3, 'f', 1).arg(unit));
	progressBar->setMaximum( bytesTotal);
	progressBar->setValue(bytesReceived);



	

}

