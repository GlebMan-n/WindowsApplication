
#include "ui_StartScreen.h"
#include <windows.h>
#include <QObject>
#include <QDialog>
#include <QTimer>
#include <QTime>
#include <QGraphicsDropShadowEffect>

//START SCREEN
class StartScreen : public QDialog, public Ui::StartScreen
{
	Q_OBJECT
public:
	StartScreen(QWidget *parent = 0,double version = 1.0);
	~StartScreen();

private:
	void _setVers(double version);
	void _initTime();
private:
	QTimer *timer;
	short sec;
	QGraphicsDropShadowEffect *effect;
	QGraphicsDropShadowEffect *effect1;	
	QGraphicsDropShadowEffect *effect2; 

private slots:
	void showLoadProgress();
	void slotTime(QString day, QString sdate, QString stime);
signals:
	void isClosed();
};