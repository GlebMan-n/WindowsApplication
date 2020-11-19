#include "ui_MapHotTab.h"
#include <QtGui>

class MapHotTab : public QWidget, public Ui::MapHotTab
{
	Q_OBJECT
public:
	MapHotTab(QWidget* parent);
	~MapHotTab();
	void init();
private slots:
	void _slotButtonClickSound();

	void _slotStatusBluetooth(bool status);
	void _slotStatusGps(bool status);
	void _slotStatus3G(bool status);
	void _slotStatusWiFi(bool status);
	void _slotStatusCam(bool status);
	void _slotStatusBit3(bool status);
	void _slotStatusInd(bool status);
	void _slotStatusIpWireless(bool status);


	void _slotSetGps();
	void _slotSet3g();
	void _slotSetWiFi();
	void _slotSetBluetooth();
	void _slotSetIndicator();
	void _slotSetIpWireless();
	
	//void signSetCam(bool status);
	//void signSetBit3(bool status);

signals:
		//запрос на получение статуса
		void signGetGpsStatus();
		void signGet3gStatus();
		void signGetWiFiStatus();
		void signGetBluetoothStatus();
		void signGetIndicatorStatus();
		void signGetIpWirelessStatus();
		void signGetCamStatus();
		void signGetBit3Stat();
	//установка состояния устройств
		void signSetGps(bool status);
		void signSet3g(bool status);
		void signSetWiFi(bool status);
		void signSetBluetooth(bool status);
		void signSetIndicator(bool status);
		void signSetIpWireless(bool status);
		void signSetCam(bool status);
		void signSetBit3(bool status);


};