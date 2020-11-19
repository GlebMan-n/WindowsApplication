#include "stdafx.h"
#include <QObject>


class HotTabClass : public QObject
{
	Q_OBJECT
public:
	HotTabClass(QWidget *parent = 0);
	~HotTabClass();
	bool _gpsOn(bool on);
	bool _getStatusGps();
private:
	short _saveBrightness;
private:

	short _getBright(); /*0-8*/
	bool _setBright(short bright); /*0-8*/
	void _screenOff();
	bool _unlockDevKeyboard();
	
	QString getVer();
	QString getDateFirmware();
	QString getScreenInfo();

	bool _deviceOn(bool gps = false,  
							bool sim = false, 
							bool wifi = false, 
							bool bluetooth = false,	
							bool indicator = false,
							bool ipWireless = false,	
							bool threeBit = false,
							bool cam = false);
	/*ложь по умолчанию для всех устройств*/

	
	bool _3GOn(bool on);
	bool _wiFiOn(bool on);
	bool _bluetoothOn(bool on);
	bool _indicatorOn(bool on);
	bool _ipWirelessOn(bool on);
	bool _3bitOn(bool on);
	bool _camOn(bool on);

	bool _getStatusBluetooth();
	
	bool _getStatus3G();
	bool _getStatusWiFi();
	bool _getStatusCam();
	bool _getStatusBit3();
	bool _getStatusInd();
	bool _getStatusIpWireless();

	private slots:
		void _slotBright(bool plus);
		void _slotBrightUp();
		void _slotBrightDown();
		void _slotBrightMax();
		void _slotBrightMin();
		void _slotScreenOff();

		void _slotGpsOn(bool on);
		void _slot3GOn(bool on);
		void _slotWiFiOn(bool on);
		void _slotBluetoothOn(bool on);
		void _slotIndicatorOn(bool on);
		void _slotIpWirelessOn(bool on);
		void _slotCamOn(bool on);
		void _slotBit3On(bool on);

		void _slotStatusBluetooth();
		void _slotStatusGps();
		void _slotStatus3G();
		void _slotStatusWiFi();
		void _slotStatusCam();
		void _slotStatusBit3();
		void _slotStatusInd();
		void _slotStatusIpWireless();
		void _slotStatusBrightness();

signals:
		void signBrightStatus(short status);
		void signGpsStatus(bool status);
		void sign3gStatus(bool status);
		void signWiFiStatus(bool status);
		void signBluetoothStatus(bool status);
		void signIndicatorStatus(bool status);
		void signIpWirelessStatus(bool status);
		void signCamStatus(bool status);
		void signBit3Stat(bool status);
		void signMenuPressed();
};