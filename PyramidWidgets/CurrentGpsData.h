
#include "ui_DialogCurrentGpsData.h"
#include <QtGui>

class CurrentGpsData : public QDialog, public Ui::CurrentDataGps
{
	Q_OBJECT
public:
	CurrentGpsData(QWidget *parent);
private:
	short _counter;
	bool _show_data;
private slots:
	void slotSetCurrentData(QString string,bool check);
	void slotSetCurrentDataErrors(QString string);
	void slotPause();
	void _slotButtonClickSound();

};