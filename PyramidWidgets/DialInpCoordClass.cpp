
#include <PyramidWidgets\DialInpCoordClass.h>
#include <PyramidM\MSVC_2008\CalcFunctions.h>
InputCoordinatesClass::InputCoordinatesClass(QWidget *parent,double lat, double lon)
{
	this->setParent(parent);
	setupUi(this);
	this->setGeometry(parent->geometry());
	
	//звуки
	connect(this->okButton,SIGNAL(clicked()),this,SLOT(_slotButtonClickSound()));
	connect(this->cancelButton,SIGNAL(clicked()),this,SLOT(_slotButtonClickSound()));
	connect(this->plusDegrLat,SIGNAL(clicked()),this,SLOT(_slotButtonClickSound()));
	connect(this->plusMinLat,SIGNAL(clicked()),this,SLOT(_slotButtonClickSound()));
	connect(this->plusLatSec,SIGNAL(clicked()),this,SLOT(_slotButtonClickSound()));
	connect(this->minusLatDegr,SIGNAL(clicked()),this,SLOT(_slotButtonClickSound()));
	connect(this->minusLatMin,SIGNAL(clicked()),this,SLOT(_slotButtonClickSound()));
	connect(this->minusLatSec,SIGNAL(clicked()),this,SLOT(_slotButtonClickSound()));
	connect(this->plusLonDegr,SIGNAL(clicked()),this,SLOT(_slotButtonClickSound()));
	connect(this->plusLonMin,SIGNAL(clicked()),this,SLOT(_slotButtonClickSound()));
	connect(this->plusLonSec,SIGNAL(clicked()),this,SLOT(_slotButtonClickSound()));
	connect(this->minusLonDegr,SIGNAL(clicked()),this,SLOT(_slotButtonClickSound()));
	connect(this->minusLonMin,SIGNAL(clicked()),this,SLOT(_slotButtonClickSound()));
	connect(this->minusLonSec,SIGNAL(clicked()),this,SLOT(_slotButtonClickSound()));
	connect(this->lonEastCheck,SIGNAL(stateChanged(int)),this,SLOT(_slotButtonClickSound()));
	connect(this->latNothCheck,SIGNAL(stateChanged(int)),this,SLOT(_slotButtonClickSound()));


	connect(this->okButton,SIGNAL(clicked()),this,SLOT(slotOk()));
	connect(this->cancelButton,SIGNAL(clicked()),this,SLOT(slotCancel()));
	connect(this->plusDegrLat,SIGNAL(pressed()),this,SLOT(slotPlusLatDegr()));
	connect(this->plusMinLat,SIGNAL(pressed()),this,SLOT(slotPlusLatMin()));
	connect(this->plusLatSec,SIGNAL(pressed()),this,SLOT(slotPlusLatSec()));
	connect(this->minusLatDegr,SIGNAL(pressed()),this,SLOT(slotMinusLatDegr()));
	connect(this->minusLatMin,SIGNAL(pressed()),this,SLOT(slotMinusLatMin()));
	connect(this->minusLatSec,SIGNAL(pressed()),this,SLOT(slotMinusLatSec()));
	connect(this->plusLonDegr,SIGNAL(pressed()),this,SLOT(slotPlusLonDegr()));
	connect(this->plusLonMin,SIGNAL(pressed()),this,SLOT(slotPlusLonMin()));
	connect(this->plusLonSec,SIGNAL(pressed()),this,SLOT(slotPlusLonSec()));
	connect(this->minusLonDegr,SIGNAL(pressed()),this,SLOT(slotMinusLonDegr()));
	connect(this->minusLonMin,SIGNAL(pressed()),this,SLOT(slotMinusLonMin()));
	connect(this->minusLonSec,SIGNAL(pressed()),this,SLOT(slotMinusLonSec()));

	connect(this->plusDegrLat,SIGNAL(released()),this,SLOT(slotPlusLatDegr_()));
	connect(this->plusMinLat,SIGNAL(released()),this,SLOT(slotPlusLatMin_()));
	connect(this->plusLatSec,SIGNAL(released()),this,SLOT(slotPlusLatSec_()));
	connect(this->minusLatDegr,SIGNAL(released()),this,SLOT(slotMinusLatDegr_()));
	connect(this->minusLatMin,SIGNAL(released()),this,SLOT(slotMinusLatMin_()));
	connect(this->minusLatSec,SIGNAL(released()),this,SLOT(slotMinusLatSec_()));
	connect(this->plusLonDegr,SIGNAL(released()),this,SLOT(slotPlusLonDegr_()));
	connect(this->plusLonMin,SIGNAL(released()),this,SLOT(slotPlusLonMin_()));
	connect(this->plusLonSec,SIGNAL(released()),this,SLOT(slotPlusLonSec_()));
	connect(this->minusLonDegr,SIGNAL(released()),this,SLOT(slotMinusLonDegr_()));
	connect(this->minusLonMin,SIGNAL(released()),this,SLOT(slotMinusLonMin_()));
	connect(this->minusLonSec,SIGNAL(released()),this,SLOT(slotMinusLonSec_()));

	connect(this->lonEastCheck,SIGNAL(stateChanged(int)),this,SLOT(slotCheckEast(int)));
	connect(this->latNothCheck,SIGNAL(stateChanged(int)),this,SLOT(slotCheckNoth(int)));



	
	b_east = this->latNothCheck->isChecked();
	b_noth = this->lonEastCheck->isChecked();

	setPropertyes(lat,lon);
	Lat = 0;
	Lon = 0;
	result = false;
	
}

void InputCoordinatesClass::slotCheckEast(int x)
{
	b_noth = this->lonEastCheck->isChecked();
}
void InputCoordinatesClass::slotCheckNoth(int x)
{
	b_east = this->latNothCheck->isChecked();
}

void InputCoordinatesClass::slotCancel()
{
	
	this->close();
}

void InputCoordinatesClass::slotOk()
{
	double latDd = degrLat->intValue();
	double latMm = minLat->intValue();
	double latSs = secLat->intValue();

	double lonDd = degrLon->intValue();
	double lonMm = minLon->intValue();
	double lonSs = secLon->intValue();

	double lat = latDd + (latMm*60 + latSs)/3600;
	double lon = lonDd + (lonMm*60 + lonSs)/3600;
	if (!b_noth) lat*=-1;
	if (!b_east) lon*=-1;
	Lat = lat*CCT_PI_180;
	Lon = lon*CCT_PI_180;
	result = true;
	this->close();
}

void InputCoordinatesClass::slotPlusLatDegr()
{
	int val = this->degrLat->intValue() + 1;
	if (val>90) val = 0;
	if (val<0) val = 90;
	this->degrLat->display(val);
		
}
void InputCoordinatesClass::slotPlusLatMin()
{
	int val = this->minLat->intValue() + 1;
	if (val>60) val = 0;
	if (val<0) val = 60;
	this->minLat->display(val);  
}

void InputCoordinatesClass::slotPlusLatSec()
{
	int val = this->secLat->intValue() + 1;
	if (val>60) val = 0;
	if (val<0) val = 60;
	this->secLat->display(val);  
}

void InputCoordinatesClass::slotMinusLatDegr()
{
	int val = this->degrLat->intValue() - 1;
	if (val>90) val = 0;
	if (val<0) val = 90;
	this->degrLat->display(val); 
}
void InputCoordinatesClass::slotMinusLatMin()
{
	int val = this->minLat->intValue() - 1;
	if (val>60) val = 0;
	if (val<0) val = 60;
	this->minLat->display(val);  
}
void InputCoordinatesClass::slotMinusLatSec()
{
	int val = this->secLat->intValue() - 1;
	if (val>60) val = 0;
	if (val<0) val = 60;
	this->secLat->display(val);  
}

void InputCoordinatesClass::slotPlusLonDegr()
{
	int val = this->degrLon->intValue() + 1;
	if (val>180) val = 0;
	if (val<0) val = 180;
	this->degrLon->display(val);  
}
void InputCoordinatesClass::slotPlusLonMin()
{
	int val = this->minLon->intValue() + 1;
	if (val>60) val = 0;
	if (val<0) val = 60;
	this->minLon->display(val);  
}
void InputCoordinatesClass::slotPlusLonSec()
{
	int val = this->secLon->intValue() + 1;
	if (val>60) val = 0;
	if (val<0) val = 60;
	this->secLon->display(val);  
}

void InputCoordinatesClass::slotMinusLonDegr()
{
	int val = this->degrLon->intValue() - 1;
	if (val>180) val = 0;
	if (val<0) val = 180;
	this->degrLon->display(val);  
}
void InputCoordinatesClass::slotMinusLonMin()
{
	int val = this->minLon->intValue() - 1;
	if (val>60) val = 0;
	if (val<0) val = 60;
	this->minLon->display(val);  
}
void InputCoordinatesClass::slotMinusLonSec()
{
	int val = this->secLon->intValue() - 1;
	if (val>60) val = 0;
	if (val<0) val = 60;
	this->secLon->display(val);  
}

void InputCoordinatesClass::setPropertyes(double lat, double lon)
{
	double _lat = lat*CCT_180_PI;
	double _lon = lon*CCT_180_PI;
	double DD = (int)_lat;
	double MM = (int)((_lat - DD)*60);
	double SS = (((_lat - DD)*60)-MM)*60;

	double DD_ = (int)_lon;
	double MM_ = (int)((_lon - DD_)*60);
	double SS_ = (((_lon - DD_)*60)-MM_)*60;

	this->degrLat->display(DD);
	this->minLat->display(MM);
	this->secLat->display(SS);

	this->degrLon->display(DD_);
	this->minLon->display(MM_);
	this->secLon->display(SS_);

}

void InputCoordinatesClass::_slotButtonClickSound()
{
	CalcFunctions::soundPressedBut(5);
}