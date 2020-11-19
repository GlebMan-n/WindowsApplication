#ifndef PiramidMT_H
#define PiramidMT_H

#include <QtGui/QMainWindow>
#include "ui_PyramidM.h"
#include <gis\GrUnits\StoreVectorData.h>

class QAction;
class QDockWidget;
class QSettings;
class GisAccess;
class MapBaseWidget;
class Magnifier;
class MapStatusBar;
class ToolBarMap;
class WndData;
class QSettingsUtf8;
class PyramidMediator;
class TknRoute;
#ifndef QT_NO_SPLASHSCREEN
class QSplashScreen;
#endif
//класс главного окна с картой
class PiramidM : public QMainWindow
{
	Q_OBJECT

public:
	PiramidM(
#ifndef QT_NO_SPLASHSCREEN
		QSplashScreen *splash = 0, 
#endif
		QWidget *parent = 0, Qt::WFlags flags = 0);
	~PiramidM();
	bool initMapWidget ();
	MapBaseWidget* getMap();
	bool test;
protected:
    //void keyPressEvent(QKeyEvent* event);
	bool event(QEvent *event) ;

public slots:
	bool connectGisBase();
private slots:
	void init();
	void _slotInitMapWidget();
	void _slotRedrawMap();

private:
	Ui::PiramidMClass ui;
	MapBaseWidget *m_mapWidget;
	GdiGis::GisGdiFactory *m_factory;
	QSettingsUtf8 *m_settings;
	QTimer *m_timer;

	GisInputInterface* m_gisTool;
	PyramidMediator* mediator;
	//test
	//QTimer *m_timer1;
	void drawPal();
	void drawRoute();

	void drawEllips();
	void drawText();
	void drawLine();
	bool routeToFile(TknRoute* tknRoute,QString path);

	StoreVectorData<FastVectorData> *gridLayer;
	bool InitLayerForKalka(StoreVectorData<FastVectorData>** layer);



#ifndef QT_NO_SPLASHSCREEN
	QSplashScreen *m_splashScreen;
#endif
signals:
	void signOk();
	void signMenu();
	void signF1();
	void signF2();
	void signF3();
	void signKeyUp();
	void signKeyDown();
	void signKeyLeft();
	void signKeyRight();

};
#endif // PiramidMT_H
