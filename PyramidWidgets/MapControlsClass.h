
#include "ui_MapButtons.h"
#include <QtGui>
#include "pyramidwidgets.h"

class MapControlsClass : public QWidget, public Ui::Map
{
	Q_OBJECT
public:
	MapControlsClass(QWidget* parent,PyramidWidgets *widgets);
	~MapControlsClass();	
	void init();
private:
	QWidget* _parent;
	PyramidWidgets *_widgets;
	QStateMachine machine_;
	QLabel* _scaleLbl;
	QLabel* _noGpsLbl;
	QLabel* _noLogTrackLbl;
	bool _speedKm;
	bool _isGpsOk;
	short _gpsCounter;
	short gpsStat;
	short _numbOfGps;
private:
	bool _leftMenuHidden;
	void _addLeftMenuAnim();
	void _addScaleLbl();
	void _addNoGpsLbl(bool hide = false);
	void _addNoLogTrackLbl(bool hide = false);
	void _initTime();
	

	void _setSunrise(QTime sunrise,QTime sunset);
	void _setLat(double lat);
	void _setLon(double lon);
	void _setSat(short sat);
	void _setSpeed(int speed);
	void _setBat(int akb);


private slots:
	void _slotTime(QString day, QString sdate, QString stime);
	void slotMapLeftMenu();
	void slotLogTrack(bool logOk);
	void slotScale(double scale);	
	void _slotCalc(QTime sunrise, QTime sunset, short akb,int timePso, int avspeed,short timeOfStop,double rasstOb);
	void _slotGpsData(QTime time, QDate date, bool status, double lat, double lon, double speed, double angle,short sat, float height);
	void _slotSetCurrentData(QString string,bool check);
	void _slotBadSign();
	void _slotChangeForSearch();
	void _slotCorrentDataSended(bool sended);
	void _slotRoseWndAngle(double angle);
	void _slotBreakTrack();
	void _slotButtonClickSound();
	//использовать сигнал!!!
	void _slotHideLeftMenu();
	void _slotGpsErrors(bool isError);
	void _slotKmBut();
	void _slotPosAngle(short angle);
	void _slotPosBut();
	void _slotCloseFeature(bool on);
	void _slotGpsStatus(short status);
	void _slotNoGPS();
	void _slotNoGpsInfo();
	void _slotNumbOfGps(short numb);
signals:
	void signBackToMainMenu();
	void signMapMenu();
	void signHideApp();
	void signMapLeftMenu();
	void signZoomIn();
	void signZoomOut();
	void signCenterPos();
	void signTrackMenu();
	void signMapLock();
	void signPhotoMenu();
	void signMessageMenu();
	void signMarkMenu();
	//скрывать левое меню этим!
	void signHideMapLeftMenu();
	void signSatellites();
	void signUpdateGpsPorts();
	void signalMapRotate();
	void signConnectionSettings();
	void signDayNight();
	void signGetNumbOfGps();
	void signCloseMenus();
};