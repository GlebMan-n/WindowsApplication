#include "stdafx.h"
#include "MSVC_2008\HotTabFunctions.h"
#include "MSVC_2008\WMIO2.h"
#include "MSVC_2008\ExchangeDataClass.h"


HotTabClass::HotTabClass(QWidget *parent)
{
	if(parent)
		this->setParent(parent);
	_saveBrightness = 0;
	_unlockDevKeyboard();

	if(!_getStatusWiFi())
	{
		_wiFiOn(true);
	}

	if(!_getStatus3G())
	{
		_3GOn(true);
	}

	if(!_getStatusCam())
	{
		_camOn(true);
	}

	if(!_getStatusBit3())
	{
		_3bitOn(true);
	}

	if(!_getStatusInd())
	{
		_indicatorOn(true);
	}

	if(!_getStatusIpWireless())
	{
		_ipWirelessOn(true);
	}

	if(!_getStatusBluetooth())
	{
		_bluetoothOn(true);
	}
}



QString HotTabClass::getDateFirmware()
{
	QString result;
	result.clear();
	char screen[128];

	if (GetSMBIOSInfo("/ID", screen))
		result += "/ID = " + QString(screen) + "\r\n";

	return result;
}

QString HotTabClass::getScreenInfo()
{
			QString result;
			result.clear();
			

			char screen[128];

			////разрешение экрана
			if (GetSMBIOSInfo("/SS", screen))
                result += "/SS = " + QString(screen)  + "\r\n";

	
			return result;
}


QString HotTabClass::getVer()
{
			QString result;
			result.clear();
			
			char ver[128];
			//версия
		    if (GetSMBIOSInfo("/IV", ver))
                result += "/IV = " + QString(ver) + "\r\n";
			////дата прошивки
   //         if (GetSMBIOSInfo("/ID", date))
   //             result += "/ID = " + QString(date) + "\r\n";
			////разрешение экрана
			//if (GetSMBIOSInfo("/SS", screen))
   //             result += "/SS = " + QString(screen)  + "\r\n";

			//delete[] ver;
			//value = new char[128];
   //         if (GetSMBIOSInfo("/SM", value))
   //             result += "/SM = " + QString(value)  + "\r\n";

            //if (GetSMBIOSInfo("/SP", value))
            //    result += "/SP = " +QString(value)  + "\r\n";

			//delete[] value;
			//value = new char[128];
   //         if (GetSMBIOSInfo("/SV", value))
   //             result += "/SV = " + QString(value)  + "\r\n";

			//разрешение


			//delete[] value;
			//value = new char[128];
   //         if (GetSMBIOSInfo("/SU", value))
   //             result += "/SU = " +QString(value) + "\r\n";

           /* if (GetSMBIOSInfo("/SK", value))
                result += "/SK = " + QString(value)  + "\r\n";

            if (GetSMBIOSInfo("/SF", value))
                result += "/SF = " + QString(value)  + "\r\n";

            if (GetSMBIOSInfo("/BM", value))
                result += "/BM = " + QString(value)  + "\r\n";

            if (GetSMBIOSInfo("/BP", value))
                result += "/BP = " + QString(value)  + "\r\n";

            if (GetSMBIOSInfo("/BV", value))
                result += "/BV = " + QString(value)  + "\r\n";

            if (GetSMBIOSInfo("/BS", value))
                result += "/BS = " + QString(value)  + "\r\n";

            if (GetSMBIOSInfo("/BT", value))
                result += "/BT = " + QString(value)  + "\r\n";

            if (GetSMBIOSInfo("/CM", value))
                result += "/CM = " + QString(value)  + "\r\n";*/
			//delete[] value;
			//value = new char[128];
   //         if (GetSMBIOSInfo("/CT", value))
   //             result += "/CT = " + QString(value)  + "\r\n";

           /* if (GetSMBIOSInfo("/CV", value))
                result += "/CV = " + QString(value)  + "\r\n";

            if (GetSMBIOSInfo("/CS", value))
                result += "/CS = " + QString(value)  + "\r\n";

            if (GetSMBIOSInfo("/CA", value))
                result += "/CA = " + QString(value)  + "\r\n";*/
			//delete[] value;
			//value = new char[128];
   //         if (GetSMBIOSInfo("/CO", value))
   //             result += "/CO = " + QString(value)  + "\r\n";
			return result;
}

HotTabClass::~HotTabClass()
{
}

short HotTabClass::_getBright()
{
	
	bool ret;
	unsigned char Value;
	ret = GetBrightness(&Value);
	
	if (!ret)
		return false;
	else
		return (short)Value;
}

bool HotTabClass::_getStatusInd()
{
	bool ret;
	unsigned char Value;
	ret = GetDevice(&Value);
	if (!ret)
	return false;
	else
	{
		if ((Value & 0x80)== 0x80)
		return true;
	else
		 return false;
	}
}

bool HotTabClass::_getStatusIpWireless()
{
	bool ret;
	unsigned char Value;
	ret = GetDevice(&Value);
	if (!ret)
	return false;
	else
	{
		if ((Value & 0x40)== 0x40)
		return true;
	else
		 return false;
	}
}


bool HotTabClass::_getStatusBit3()
{
	bool ret;
	unsigned char Value;
	ret = GetDevice(&Value);
	if (!ret)
	return false;
	else
	{
		if ((Value & 0x08)== 0x08)
		return true;
	else
		 return false;
	}
}

bool HotTabClass::_getStatusBluetooth()
{
	bool ret;
	unsigned char Value;
	ret = GetDevice(&Value);
	if (!ret)
	return false;
	else
	{
		if ((Value & 0x04)== 0x04)
		return true;
	else
		 return false;
	}
}

bool HotTabClass::_getStatusGps()
{
	bool ret;
	unsigned char Value;
	ret = GetDevice(&Value);
	if (!ret)
		return false;
	else
	{
		if ((Value & 0x10)== 0x10)
		return true;
	else
		 return false;
	}
}

bool HotTabClass::_getStatus3G()
{
	bool ret;
	unsigned char Value;
	ret = GetDevice(&Value);
	if (!ret)
	return false;
	else
	{
		if ((Value & 0x02)== 0x02)
		return true;
	else
		 return false;
	}
}

bool HotTabClass::_getStatusWiFi()
{
	bool ret;
	unsigned char Value;
	ret = GetDevice(&Value);
	if (!ret)
	return false;
	else
	{
		if ((Value & 0x01)== 0x01)
		return true;
	else
		 return false;
	}
}

bool HotTabClass::_getStatusCam()
{
	bool ret;
	unsigned char Value;
	ret = GetDevice(&Value);
	if (!ret)
	return false;
	else
	{
		if ((Value & 0x20)== 0x20)
		return true;
	else
		 return false;
	}
}

bool HotTabClass::_setBright(short bright)
{
	return SetBrightness((byte)bright);
}

void HotTabClass::_screenOff()
{
	SendMessage(HWND_BROADCAST, WM_SYSCOMMAND,SC_MONITORPOWER, 2);
}
//
bool HotTabClass::_deviceOn(bool gps,  
							bool sim, 
							bool wifi, 
							bool bluetooth,	
							bool indicator,
							bool ipWireless,	
							bool threeBit,
							bool cam)									
{
	unsigned char setvalue = 0;
    bool ret;
	//устанавливаем статус для GPS
	if (gps)
		setvalue += 0x10;
        

	//получаем статусы остальных устройств и устанавливаем аналогичные
	//--//

	if(wifi)
		setvalue += 0x01;

	if(cam)
		setvalue += 0x20;

	if(sim)
		setvalue += 0x02;

	if(bluetooth)
		setvalue += 0x04;

	if(threeBit)
		setvalue += 0x08;

	if(indicator)
		setvalue += 0x80;

	if(ipWireless)
		setvalue += 0x40;

	//--//

	ret = SetDevice(setvalue);


	return ret;
}

bool HotTabClass::_gpsOn(bool on)
{
	return _deviceOn(on,_getStatus3G(),_getStatusWiFi(),_getStatusBluetooth(),_getStatusInd(),_getStatusIpWireless(),_getStatusBit3(),_getStatusCam());
}

bool HotTabClass::_3GOn(bool on)
{
	return _deviceOn(_getStatusGps(),on,_getStatusWiFi(),_getStatusBluetooth(),_getStatusInd(),_getStatusIpWireless(),_getStatusBit3(),_getStatusCam());
}

bool HotTabClass::_wiFiOn(bool on)
{
	return _deviceOn(_getStatusGps(),_getStatus3G(),on,_getStatusBluetooth(),_getStatusInd(),_getStatusIpWireless(),_getStatusBit3(),_getStatusCam());
}

bool HotTabClass::_bluetoothOn(bool on)
{
	return _deviceOn(_getStatusGps(),_getStatus3G(),_getStatusWiFi(),on,_getStatusInd(),_getStatusIpWireless(),_getStatusBit3(),_getStatusCam());
}

bool HotTabClass::_indicatorOn(bool on)
{
	return _deviceOn(_getStatusGps(),_getStatus3G(),_getStatusWiFi(),_getStatusBluetooth(),on,_getStatusIpWireless(),_getStatusBit3(),_getStatusCam());
}

bool HotTabClass::_ipWirelessOn(bool on)
{
	return _deviceOn(_getStatusGps(),_getStatus3G(),_getStatusWiFi(),_getStatusBluetooth(),_getStatusInd(),on,_getStatusBit3(),_getStatusCam());
}

bool HotTabClass::_3bitOn(bool on)
{
	bool res =  _deviceOn(_getStatusGps(),_getStatus3G(),_getStatusWiFi(),_getStatusBluetooth(),_getStatusInd(),_getStatusIpWireless(),on,_getStatusCam());
	return res;
}

bool HotTabClass::_camOn(bool on)
{
	return _deviceOn(_getStatusGps(),_getStatus3G(),_getStatusWiFi(),_getStatusBluetooth(),_getStatusInd(),_getStatusIpWireless(),_getStatusBit3(),on);
}

void HotTabClass::_slotBright(bool plus)
{
	if(plus)
	{
		short bright = _getBright();
		
		if(bright > 7)
		{
			emit signBrightStatus(bright);
			return;
		}

		bright++;
		_setBright (bright);
		emit signBrightStatus(_getBright());
	}
	else
	{
		short bright = _getBright();

		if(bright < 1)
		{
			emit signBrightStatus(bright);
			return;
		}

		bright--;
		_setBright (bright);
		emit signBrightStatus(_getBright());
	}
}

void HotTabClass::_slotBrightMax()
{
	_setBright(_saveBrightness);
	emit signBrightStatus(_getBright());
}

void HotTabClass::_slotBrightMin()
{
	_saveBrightness = _getBright();
	_setBright(0);
	emit signBrightStatus(_getBright());
}

void HotTabClass::_slotScreenOff()
{
	_screenOff();
}

void HotTabClass::_slotGpsOn(bool on)
{
	_gpsOn(on);
	emit signGpsStatus(_getStatusGps());
}

void HotTabClass::_slot3GOn(bool on)
{
	_3GOn(on);
	emit sign3gStatus(_getStatus3G());
}

void HotTabClass::_slotWiFiOn(bool on)
{
	_wiFiOn(on);
	emit signWiFiStatus(_getStatusWiFi());
}

void HotTabClass::_slotBluetoothOn(bool on)
{
	_bluetoothOn(on);
	emit signBluetoothStatus(_getStatusBluetooth());
}

void HotTabClass::_slotIndicatorOn(bool on)
{
	_indicatorOn(on);
	emit signIndicatorStatus(_getStatusInd());
}

void HotTabClass::_slotIpWirelessOn(bool on)
{
	_ipWirelessOn(on);
	emit signIpWirelessStatus(_getStatusIpWireless());
}

void HotTabClass::_slotCamOn(bool on)
{
	_camOn(on);
	emit signCamStatus(_getStatusCam());
}

void HotTabClass::_slotStatusBluetooth()
{
	emit signBluetoothStatus(_getStatusBluetooth());
}

void HotTabClass::_slotStatusGps()
{
	emit signGpsStatus(_getStatusGps());
}

void HotTabClass::_slotStatus3G()
{
	emit sign3gStatus(_getStatus3G());
}

void HotTabClass::_slotStatusWiFi()
{
	emit signWiFiStatus(_getStatusWiFi());
}

void HotTabClass::_slotStatusCam()
{
	emit signCamStatus(_getStatusCam());
}

void HotTabClass::_slotStatusBit3()
{
	emit signBit3Stat(_getStatusBit3());
}

void HotTabClass::_slotStatusInd()
{
	emit signIndicatorStatus(_getStatusInd());
}

void HotTabClass::_slotStatusIpWireless()
{
	emit signIpWirelessStatus(_getStatusIpWireless());
}

void HotTabClass::_slotStatusBrightness()
{
	emit signBrightStatus(_getBright());
}

void HotTabClass::_slotBrightUp()
{
	short bright = _getBright();
	bright++;
	_setBright (bright);
	emit signBrightStatus(_getBright());
}

void HotTabClass::_slotBrightDown()
{
		short bright = _getBright();
		bright--;
		_setBright (bright);
		emit signBrightStatus(_getBright());
}

void HotTabClass::_slotBit3On(bool on)
{
	_3bitOn(on);
	emit signBit3Stat(_getStatusBit3());
}

bool HotTabClass::_unlockDevKeyboard()
{
	return ModeOpen(0);
}