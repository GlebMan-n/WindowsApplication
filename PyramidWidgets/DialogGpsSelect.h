#include "ui_DialogSelectGps.h"
#include <QDialog>
#include <QtGui>
class SelectGpsDial : public QDialog, public Ui::SelectGpsDialog
{
	Q_OBJECT
public:
	SelectGpsDial(QWidget *parent,QStringList listOfGps,QStringList typesOfGps,bool trBackg = true);
	int result;
	QString comPortResult;	
private:
	void _saveSettings();
	QStringList comPorts;
	QWidget* _parent;
private slots:
	void _slotOk();
	void _slotCancel();
	void _slotButtonClickSound();
	void _slotComList(QStringList comList, QStringList comTypesList );
signals:
	void singOk();
	void signCancel();
};