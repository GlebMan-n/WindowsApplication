#include <MapControlsClass.h>
#include <PyramidM\MSVC_2008\CalcFunctions.h>
#include <PyramidWidgets\SelectDialogClass.h>

MapControlsClass::MapControlsClass(QWidget* parent,PyramidWidgets *widgets)
{
	_parent = parent;
	_widgets = widgets;
	_leftMenuHidden = false;
	_scaleLbl = NULL;
	_noGpsLbl = NULL;
	_noLogTrackLbl = NULL;
	_isGpsOk = true;
	_gpsCounter = 0;
	gpsStat = 0;
	this->setParent(parent);
	setupUi(this);
	this->setGeometry(parent->geometry());
	//сначала звуки
	connect(this->main_menu_but,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(this->map_menu_but,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(this->hide_but,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(this->left_menu_but,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(this->zoom_in_but,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(this->zoom_out_but,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(this->centerPosBut,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(this->breakTrackBut,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(this->lock_but,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(this->photo_but,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(this->mess_but,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(this->mark_but_l,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(kmch_tm_lbl,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(SatBut,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(connectLbl,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(rose_wnd_lbl,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(day_night_tm_lbl,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(noTrackLog,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	////функционал 

	connect(this->main_menu_but,SIGNAL(clicked()),SIGNAL(signBackToMainMenu()));
	connect(this->map_menu_but,SIGNAL(clicked()),SIGNAL(signMapMenu()));
	connect(this->hide_but,SIGNAL(clicked()),SIGNAL(signHideApp()));
	connect(this->left_menu_but,SIGNAL(clicked()),SLOT(slotMapLeftMenu()));
	connect(this->zoom_in_but,SIGNAL(clicked()),SIGNAL(signZoomIn()));
	connect(this->zoom_out_but,SIGNAL(clicked()),SIGNAL(signZoomOut()));
	connect(this->centerPosBut,SIGNAL(clicked()),SLOT(_slotPosBut()));
	connect(this->breakTrackBut,SIGNAL(clicked()),SLOT(_slotBreakTrack()));
	connect(this->lock_but,SIGNAL(clicked()),SIGNAL(signHideMapLeftMenu()));
	connect(this->lock_but,SIGNAL(clicked()),SIGNAL(signMapLock()));
	connect(this->photo_but,SIGNAL(clicked()),SIGNAL(signPhotoMenu()));
	connect(this->mess_but,SIGNAL(clicked()),SIGNAL(signMessageMenu()));
	connect(this->mark_but_l,SIGNAL(clicked()),SIGNAL(signMarkMenu()));
	connect(this,SIGNAL(signHideMapLeftMenu()),SLOT(_slotHideLeftMenu()));
	connect(kmch_tm_lbl,SIGNAL(clicked()),SLOT(_slotKmBut()));
	connect(SatBut,SIGNAL(clicked()),SIGNAL(signUpdateGpsPorts()));
	connect(rose_wnd_lbl,SIGNAL(clicked()),SIGNAL(signalMapRotate()));
	connect(connectLbl,SIGNAL(clicked()),SIGNAL(signConnectionSettings()));
	connect(day_night_tm_lbl,SIGNAL(clicked()),SIGNAL(signDayNight()));
	connect(noTrackLog,SIGNAL(clicked()),SLOT(_slotNoGpsInfo()));

	_initTime();
	_addLeftMenuAnim();
	_addScaleLbl();
	slotMapLeftMenu();	
	_speedKm = true;
}

void MapControlsClass::_slotBreakTrack()
{
	emit signTrackMenu();
}

void MapControlsClass::_addScaleLbl()
{
	if(!_scaleLbl)
	{
		QRect rect(_parent->geometry());
		short x = rect.width() - 270;
		short y = rect.height() - 35;
		_scaleLbl = new QLabel(_parent);
		_scaleLbl->setGeometry(x,y,180,20);
		QFont font = _scaleLbl->font(); 
		font.setPointSize(20);
		_scaleLbl->setFont(font);
		_scaleLbl->show();
	}
}

void MapControlsClass::_addNoGpsLbl(bool hide)
{
	if(hide)
	{
		if(_noGpsLbl)
		{
			delete _noGpsLbl;
			_noGpsLbl = NULL;
		}
		return;
	}

	if(!_noGpsLbl)
	{
		QRect rect(_parent->geometry());
		short x = rect.width() - 590;
		short y = rect.height() - 35;
		_noGpsLbl = new QLabel(_parent);
		_noGpsLbl->setGeometry(x,y,300,20);
		QFont font = _noGpsLbl->font(); 
		font.setPointSize(18);
		font.setBold(true);
		_noGpsLbl->setFont(font);
		_noGpsLbl->setText(tr("НЕ ПОДКЛЮЧЕН GPS!"));
		_noGpsLbl->show();
	}	
}

void MapControlsClass::_addNoLogTrackLbl(bool hide)
{
	if(hide)
	{
		if(_noLogTrackLbl)
		{
			delete _noLogTrackLbl;
			_noLogTrackLbl = NULL;
		}
		return;
	}

	if(!_noLogTrackLbl)
	{
		QRect rect(_parent->geometry());
		short x = rect.width() - 890;
		short y = rect.height() - 35;
		_noLogTrackLbl = new QLabel(_parent);
		_noLogTrackLbl->setGeometry(x,y,300,20);
		QFont font = _noLogTrackLbl->font(); 
		font.setPointSize(18);
		font.setBold(true);
		_noLogTrackLbl->setFont(font);
		_noLogTrackLbl->setText(tr("НЕТ ЗАПИСИ ТРЕКА"));
		_noLogTrackLbl->show();
	}
}

void MapControlsClass::_addLeftMenuAnim()
{
	QState* st1 ;
	QState * st2 ;

	st1  = new QState();
	st2  = new QState();

	st1->assignProperty(left_menu_but, "geometry", QRect( 0,  0, 110, 96));
	st1->assignProperty(mapMenuLeftN, "minimumSize", QSize(110, 96));
	st1->assignProperty(mapMenuLeftN, "maximumSize", QSize(110, 96));

	st2->assignProperty(left_menu_but, "geometry", QRect(0,  414, 110, 96));
	st2->assignProperty(mapMenuLeftN, "maximumSize", QSize(110, 510));
	st2->assignProperty(mapMenuLeftN, "minimumSize", QSize(110, 510));


	st1->addTransition(this->left_menu_but,SIGNAL(clicked()),st2);
    st2->addTransition(this->left_menu_but,SIGNAL(clicked()),st1);
	st2->addTransition(this,SIGNAL(signHideMapLeftMenu()),st1);
 	machine_.addState(st1);
    machine_.addState(st2);
    machine_.setInitialState(st1);

 	QPropertyAnimation* an3 = new QPropertyAnimation(left_menu_but, "geometry");
	machine_.addDefaultAnimation(an3);
	an3->setEasingCurve(QEasingCurve::OutQuint);
	an3->setDuration(250);

	QPropertyAnimation* an1 = new QPropertyAnimation(mapMenuLeftN, "minimumSize");
	machine_.addDefaultAnimation(an1);
	an1->setEasingCurve(QEasingCurve::OutQuint);
	an1->setDuration(250);

	QPropertyAnimation* an2 = new QPropertyAnimation(mapMenuLeftN, "maximumSize");
	machine_.addDefaultAnimation(an2);
	an2->setEasingCurve(QEasingCurve::OutQuint);
	an2->setDuration(250);

	machine_.start();
}

MapControlsClass::~MapControlsClass()
{

}
void MapControlsClass::_slotTime(QString day, QString sdate, QString stime)
{
	this->map_time->setText(stime);
}

void MapControlsClass::slotMapLeftMenu()
{

	if(_leftMenuHidden)
	{
		
		if(_parent->height()<940)
		{
			this->zoom_in_but->hide();
			this->zoom_out_but->hide();
			this->centerPosBut->hide();
		}

		this->lock_but->show();
		this->mark_but_l->show();
		this->mess_but->show();
		this->photo_but->show();
		mapMenuLeftN->setStyleSheet("border-image: url(:/Pyramid/fon_left_menu.png)");
		_leftMenuHidden = false;
	}
	else
	{
		_slotHideLeftMenu();
	}

}
void MapControlsClass::_slotHideLeftMenu()
{



	this->lock_but->hide();
	this->mark_but_l->hide();
	this->mess_but->hide();
	this->photo_but->hide();
	this->zoom_in_but->show();
	this->zoom_out_but->show();
	this->centerPosBut->show();
	mapMenuLeftN->setStyleSheet("border-image: url(:/Pyramid/l_menu_fon.png)");
	_leftMenuHidden = true;
}

void MapControlsClass::slotLogTrack(bool logOk)
{
	if(logOk)
	{
		breakTrackBut->setStyleSheet(QString("QPushButton{border-image: url(:/Pyramid/brakeMarshButG.png);}"));
	}
	else
	{
		breakTrackBut->setStyleSheet(QString("QPushButton{border-image: url(:/Pyramid/brakeMarshButR.png);}"));
	}
}


void MapControlsClass::slotScale(double scale)
{
	if(_scaleLbl)
		_scaleLbl->setText(tr("1:") + QString::number((int)scale));
}

void MapControlsClass::_slotCalc(QTime sunrise, QTime sunset, short akb,int timePso, int avspeed,short timeOfStop,double rasstOb)
{
	_setSunrise(sunrise,sunset);
	_setBat(akb);
}

void MapControlsClass::_slotGpsData(QTime time, QDate date, bool status, double lat, double lon, double speed, double angle,short sat, float height)
{

	_slotGpsStatus(2);
	_setLat(lat);
	_setLon(lon);
	_setSat(sat);
	_setSpeed(speed);

	
	if(!_isGpsOk)
	{
		_gpsCounter--;
		if(_gpsCounter < 1)
		{
			CalcFunctions::soundPressedBut(10);
			_isGpsOk = true;
			_gpsCounter = 5;
			
		}
	}
}

void MapControlsClass::_slotSetCurrentData(QString string,bool check)
{
	if(gpsStat == 1)
	{
		if(string.startsWith("$GPGGA"))
		{
			float sat = string.section(',', 7, 7).toInt();
			if(sat < 4)
				_setSat((short)sat);
		}
	}
}

void MapControlsClass::_setSunrise(QTime sunrise,QTime sunset)
{
	//если до восхода больше чем до заката
	if(sunrise.hour() < sunset.hour() )
	{
		day_night_tm_lbl->setIcon(QPixmap(QString::fromUtf8(":/Pyramid/m.png")));
		time_zakat_lbl->setText(sunrise.toString("HH:mm"));
	}
	else
	{
		day_night_tm_lbl->setIcon(QPixmap(QString::fromUtf8(":/Pyramid/s.png")));
		time_zakat_lbl->setText(sunset.toString("HH:mm"));
	}
}


void MapControlsClass::_setLat(double lat)
{
	if(lat>0)
	{
		QString tmp;
		tmp = CalcFunctions::dDToDDMMSS(lat);
		tmp += tr("N");
		lat_tm_lbl->setText(tmp);
	}
	else
	{
		QString tmp;
		tmp = CalcFunctions::dDToDDMMSS(lat);
		tmp += tr("S");
		lat_tm_lbl->setText(tmp);
	}
}

void MapControlsClass::_setLon(double lon)
{
	if(lon>0)
	{
		QString tmp;
		tmp =CalcFunctions::dDToDDMMSS(lon);
		tmp += tr("E");
		lon_tm_lbl->setText(tmp);
	}
	else
	{		
		QString tmp;
		tmp = CalcFunctions::dDToDDMMSS(lon);
		tmp += tr("W");
		lon_tm_lbl->setText(tmp);
	}
}

void MapControlsClass::_setSat(short sat)
{
	sat_tm_lbl->setText(QString::number(sat));
}


void MapControlsClass::_setSpeed(int speed)
{
	if(_speedKm)
	{
		kmch_tm_lbl_->setText(tr("км/ч"));
		speed_tm_lbl->setText(QString::number(speed));
	}
	else
	{
		kmch_tm_lbl_->setText(tr("м/c"));
		int sp = speed * 0.277777778 ;
		speed_tm_lbl->setText(QString::number(sp));
	}
}


void MapControlsClass::_setBat(int akb)
{

		if(akb < 5)
			batLbl->setPixmap(QPixmap(QString::fromUtf8(":/Pyramid/bat_0.png")));

		if(akb < 21)
			batLbl->setPixmap(QPixmap(QString::fromUtf8(":/Pyramid/bat_20.png")));

		if(akb < 41)
			batLbl->setPixmap(QPixmap(QString::fromUtf8(":/Pyramid/bat_40.png")));

		if(akb < 61)
			batLbl->setPixmap(QPixmap(QString::fromUtf8(":/Pyramid/bat_60.png")));

		if(akb < 85)
			batLbl->setPixmap(QPixmap(QString::fromUtf8(":/Pyramid/bat_80.png")));

		if(akb > 84)
			batLbl->setPixmap(QPixmap(QString::fromUtf8(":/Pyramid/bat_100.png")));


	batLblText->setText(QString::number(akb) + QString("%"));
}

void MapControlsClass::_initTime()
{
	QString day, sdate, stime;
	SYSTEMTIME st; 
	GetLocalTime(&st);
	QTime time(st.wHour,st.wMinute);
	QDate date(st.wYear, st.wMonth, st.wDay);
	stime = time.toString("HH:mm");
	sdate = date.toString("dd.MM.yyyy");
	day = date.toString("dddd");
	day[0] = day[0].toUpper(); 
	_slotTime(day, sdate, stime);
}

void MapControlsClass::_slotChangeForSearch()
{
	if(map_menu_but->isHidden())
	{
		hide_but->show();
		map_menu_but->show();
		main_menu_but->show();
		map_time->show();
		mapMenuLeftN->show();
		centerPosBut->show();
		zoom_out_but->show();
		zoom_in_but->show();
		rose_wnd_lbl->show();
	}
	else
	{
		hide_but->hide();
		map_menu_but->hide();
		main_menu_but->hide();
		map_time->hide();
		mapMenuLeftN->hide();
		centerPosBut->hide();
		zoom_out_but->hide();
		zoom_in_but->hide();
		rose_wnd_lbl->hide();
	}
}

void MapControlsClass::_slotCorrentDataSended(bool sended)
{
	if(sended)
	{
		connectLbl->setIcon(QPixmap(QString::fromUtf8(":/Pyramid/send_data_good.png")));
	}
	else
	{
		connectLbl->setIcon(QPixmap(QString::fromUtf8(":/Pyramid/send_data_bad.png")));
	}
}

void MapControlsClass::_slotRoseWndAngle(double angle)
{
	rose_wnd_lbl->setIcon(QPixmap( "" ));
	rose_wnd_lbl->show();
	rose_wnd_lbl->setIcon(QPixmap( CalcFunctions::rotatePixmap(angle*CCT_180_PI,":/Pyramid/roza.png")));
	rose_wnd_lbl->show();
}

void MapControlsClass::_slotButtonClickSound()
{
	CalcFunctions::soundPressedBut(5);
}

void MapControlsClass::_slotBadSign()
{
	//_setSat(0);
	_slotGpsStatus(1);
	lat_tm_lbl->setText(tr("плохие"));
	lon_tm_lbl->setText(tr("условия приема"));
	
	if(_isGpsOk)
	{
		_gpsCounter--;
		if(_gpsCounter < 1)
		{
			CalcFunctions::soundPressedBut(12);
			_isGpsOk = false;
			_gpsCounter = 5;
		}
	}	
}
void MapControlsClass::_slotGpsErrors(bool noError)
{
	if(!noError)
	{
		_setSat(0);
		_slotGpsStatus(0);
		lat_tm_lbl->setText(tr("ошибка "));
		lon_tm_lbl->setText(tr("GPS"));
		

		if(_isGpsOk)
		{
			_gpsCounter--;
			if(_gpsCounter < 1)
			{
				CalcFunctions::soundPressedBut(11);
				_isGpsOk = false;
				_gpsCounter = 5;
			}
		}
	}
}

void MapControlsClass::_slotKmBut()
{
	
	_speedKm = !_speedKm;

	if(_speedKm)
	{
		kmch_tm_lbl_->setText(tr("км/ч"));
		speed_tm_lbl->setText(QString::number(0));
	}
	else
	{
		kmch_tm_lbl_->setText(tr("м/c"));
		speed_tm_lbl->setText(QString::number(0));
	}

}

//расчет позиционирования экрана от текущей позиции
void MapControlsClass::_slotPosAngle(short angle)
{
	short position = 0;
	
	if ( (angle > -1 && angle < 24) || (angle > 339 && angle < 361))
	{
		//текущая позиция сверху
		position = 1;
		centerPosBut->setStyleSheet(QString("QPushButton:hover:pressed {border-image: url(:/Pyramid/otmetka_n.png);}QPushButton{border-image: url(:/Pyramid/posUp.png);}QPushButton:hover {	border-image: url(:/Pyramid/posUp.png);}"));
	}

	if ( (angle > 23 && angle < 70))
	{
		//текущая позиция сверху справа
		position = 2;
		centerPosBut->setStyleSheet(QString("QPushButton:hover:pressed {border-image: url(:/Pyramid/otmetka_n.png);}QPushButton{border-image: url(:/Pyramid/posUpRight.png);}QPushButton:hover {	border-image: url(:/Pyramid/posUpRight.png);}"));
	}

	if ( (angle > 70 && angle < 116))
	{
		//текущая позиция справа
		position = 3;
		centerPosBut->setStyleSheet(QString("QPushButton:hover:pressed {border-image: url(:/Pyramid/otmetka_n.png);}QPushButton{border-image: url(:/Pyramid/posRight.png);}QPushButton:hover {	border-image: url(:/Pyramid/posRight.png);}"));
	}


	if ( (angle > 115 && angle < 162))
	{
		//текущая позиция снизу справа
		position = 4;
		centerPosBut->setStyleSheet(QString("QPushButton:hover:pressed {border-image: url(:/Pyramid/otmetka_n.png);}QPushButton{border-image: url(:/Pyramid/posDownRight.png);}QPushButton:hover {	border-image: url(:/Pyramid/posDownRight.png);}"));
	}

	if ( (angle > 161 && angle < 208))
	{
		//текущая позиция снизу
		position = 5;
		centerPosBut->setStyleSheet(QString("QPushButton:hover:pressed {border-image: url(:/Pyramid/otmetka_n.png);}QPushButton{border-image: url(:/Pyramid/posDown.png);}QPushButton:hover {	border-image: url(:/Pyramid/posDown.png);}"));
	}

	if ( (angle > 207 && angle < 254))
	{
		//текущая позиция снизу слева
		position = 6;
		centerPosBut->setStyleSheet(QString("QPushButton:hover:pressed {border-image: url(:/Pyramid/otmetka_n.png);}QPushButton{border-image: url(:/Pyramid/posDownLeft.png);}QPushButton:hover {	border-image: url(:/Pyramid/posDownLeft.png);}"));
	}

	if ( (angle > 253 && angle < 300))
	{
		//текущая позиция слева
		position = 7;
		centerPosBut->setStyleSheet(QString("QPushButton:hover:pressed {border-image: url(:/Pyramid/otmetka_n.png);}QPushButton{border-image: url(:/Pyramid/posLeft.png);}QPushButton:hover {	border-image: url(:/Pyramid/posLeft.png);}"));
	}

	if ( (angle > 299 && angle < 340))
	{
		//текущая позиция слева
		position = 8;
		centerPosBut->setStyleSheet(QString("QPushButton:hover:pressed {border-image: url(:/Pyramid/otmetka_n.png);}QPushButton{border-image: url(:/Pyramid/posUpLeft.png);}QPushButton:hover {	border-image: url(:/Pyramid/posUpLeft.png);}"));
	}
}

void MapControlsClass::_slotPosBut()
{
	centerPosBut->setStyleSheet(QString("QPushButton:hover:pressed {border-image: url(:/Pyramid/otmetka_n.png);}QPushButton{border-image: url(:/Pyramid/otmetka.png);}QPushButton:hover {	border-image: url(:/Pyramid/otmetka.png);}"));
	emit signCenterPos();
}

void MapControlsClass::_slotCloseFeature(bool on)
{
	if(!on)
		map_menu_but->setStyleSheet(QString("QPushButton:hover:pressed {border-image: url(:/Pyramid/close_fw_n.png);}" 
											"QPushButton{	border-image: url(:/Pyramid/close_fw.png);}"
											"QPushButton:hover {	border-image: url(:/Pyramid/close_fw.png);}"));
	else
		map_menu_but->setStyleSheet(QString("QPushButton:hover:pressed {border-image: url(:/Pyramid/show_menu_n.png);}" 
											"QPushButton{border-image: url(:/Pyramid/show_menu.png);}"
											"QPushButton:hover {border-image: url(:/Pyramid/show_menu.png);}"));
}

void MapControlsClass::_slotGpsStatus(short status)
{
	gpsStat = status;
	switch(status)
	{
	case 0:
		SatBut->setStyleSheet(QString("QPushButton{	border-image: url(:/Pyramid/sputn_r.png);}"));
		noTrackLog->show();
		break;
	case 1:
		SatBut->setStyleSheet(QString("QPushButton{	border-image: url(:/Pyramid/sputn_e.png);}"));
		noTrackLog->show();
		break;
	case 2:
		SatBut->setStyleSheet(QString("QPushButton{	border-image: url(:/Pyramid/sputn_g.png);}"));
		noTrackLog->hide();
		break;
	}
}

void MapControlsClass::_slotNoGPS()
{
	lat_tm_lbl->setText(tr("НЕТ ЗАПИСИ"));
	lon_tm_lbl->setText(tr("ТРЕКА"));

}

void MapControlsClass::_slotNoGpsInfo()
{
	emit signCloseMenus();
	QString srtDlg1;
	QString srtDlg2;

	if(_numbOfGps == 1)
	{
		srtDlg1 = QString::number(_numbOfGps)+tr(" GPS устройство")
			+	tr("\nПодключите GPS модуль\nдалее поиск GPS");
		
	}

	if(_numbOfGps < 1)
	{
		srtDlg1 = QString::number(_numbOfGps)+tr(" GPS устройств")
			+	tr("\nПодключите GPS модуль\nдалее поиск GPS");
	}

	if(_numbOfGps > 1)
	{
		srtDlg1 = QString::number(_numbOfGps)+tr(" GPS устройства")
			+	tr("\nПодключите GPS модуль\nдалее поиск GPS");
	}

	if(_numbOfGps == 1)
	{
		srtDlg2 = QString::number(_numbOfGps)+tr(" GPS устройство")
			+	tr("\nПоместите устройство в зону\nпокрытия GPS");
		
	}

	if(_numbOfGps < 1)
	{
				srtDlg2 = QString::number(_numbOfGps)+tr(" GPS устройств")
			+	tr("\nПоместите устройство в зону\nпокрытия GPS");
	}

	if(_numbOfGps > 1)
	{
		srtDlg2 = QString::number(_numbOfGps)+tr(" GPS устройства")
			+	tr("\nПоместите устройство в зону\nпокрытия GPS");
	}


	SelectDialog dlg1(_parent,tr("Мастер"),srtDlg1,true);
	SelectDialog dlg2(_parent,tr("Мастер"),srtDlg2,true);

	switch(gpsStat)
	{

	case 0:
		dlg1.okButton->hide();
		dlg1.exec();
	break;

	case 1:
		dlg2.okButton->hide();
		dlg2.exec();
	break;

	case 2:
	break;
	}

}

void MapControlsClass::_slotNumbOfGps(short numb)
{
	_numbOfGps = numb;
	//SatBut->setText(QString::number(numb));
}

void MapControlsClass::init()
{
	emit signGetNumbOfGps();
}

