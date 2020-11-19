
#include "ui_GenMenu.h"
#include <QtGui>

class GeneralMenuForm : public QDialog, public Ui::GeneralMenuForm
{
	Q_OBJECT
public:
	GeneralMenuForm(QWidget *parent = 0,double version = 1.0);
	~GeneralMenuForm();
private:
	void _setVers(double version);
	void _initTime();
private slots:
	void _slotTime(QString day, QString sdate, QString stime);
	void _slotButtonClickSound();
signals:
	void signBeginMission();
	void signSearch();
	void signSattelites();
	void signSync();
	void signGenSettings();
	void signGpsSettings();
	void signConnectionSettings();
	void signOmens();
	void signExit();
};