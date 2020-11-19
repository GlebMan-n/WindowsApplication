#include <PyramidWidgets\GenRaportsClass.h>
#include <PyramidWidgets\SelectDialogClass.h>
#include <PyramidM\MSVC_2008\CalcFunctions.h>

GenRaportsClass::GenRaportsClass(QWidget *parent)	
{
	this->setParent(parent);
	setupUi(this);
	this->setGeometry(parent->geometry());
	message.clear();
	_parent = parent;
		/*/*//*/*//*//*//*/*//////////////////
	connect(this->yellowBut_21,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(this->blueBut_3,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(this->redBut_29,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(this->blueBut_4,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(this->blueBut_2,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(this->blueBut_6,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(this->yellowBut_24,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(this->blueBut_7,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(this->redBut_28,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(this->redBut_25,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));

	connect(this->redBut_23,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(this->redBut_21,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(this->redBut_19,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(this->blueBut_8,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(this->yellowBut_23,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(this->yellowBut_22,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(this->yellowBut_20,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(this->yellowBut_18,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(this->redBut_22,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(this->redBut_24,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));

	connect(this->blueBut_1,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(this->redBut_27,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(this->yellowBut_19,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(this->blueBut_5,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(this->yellowBut_17,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));

	connect(this->closeBut,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(this->eraseBut,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(this->sendBut,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	
	connect(this->blueCat,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(this->redCat,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(this->yellowCat,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));

	/*/*//*/*///*//*/*/

	connect(this->yellowBut_21,SIGNAL(clicked()),SLOT(slotSendNaprSled()));
	connect(this->blueBut_3,SIGNAL(clicked()),SLOT(slotNeedFuel()));
	connect(this->redBut_29,SIGNAL(clicked()),SLOT(slotNeedKompas()));
	connect(this->blueBut_4,SIGNAL(clicked()),SLOT(slotNeedLamp()));
	connect(this->blueBut_2,SIGNAL(clicked()),SLOT(slotNeedDoctor()));
	connect(this->blueBut_6,SIGNAL(clicked()),SLOT(slotNeedFood()));
	connect(this->yellowBut_24,SIGNAL(clicked()),SLOT(slotNeedMeh()));
	connect(this->blueBut_7,SIGNAL(clicked()),SLOT(slotNeedDrugs()));
	connect(this->redBut_28,SIGNAL(clicked()),SLOT(slotYes()));
	connect(this->redBut_25,SIGNAL(clicked()),SLOT(slotNo()));

	connect(this->redBut_23,SIGNAL(clicked()),SLOT(slotNotAnders()));
	connect(this->redBut_21,SIGNAL(clicked()),SLOT(slotWeAFindAllPeople()));
	connect(this->redBut_19,SIGNAL(clicked()),SLOT(slotFindNotAllPeople()));
	connect(this->blueBut_8,SIGNAL(clicked()),SLOT(slotCantMove()));
	connect(this->yellowBut_23,SIGNAL(clicked()),SLOT(slotOllOk()));
	connect(this->yellowBut_22,SIGNAL(clicked()),SLOT(slotTryToFly()));
	connect(this->yellowBut_20,SIGNAL(clicked()),SLOT(slotGetNaprSled()));
	connect(this->yellowBut_18,SIGNAL(clicked()),SLOT(slotVSInThisNapr()));
	connect(this->redBut_22,SIGNAL(clicked()),SLOT(slotOperationsEnded()));
	connect(this->redBut_24,SIGNAL(clicked()),SLOT(slotCantContinueReturn()));

	connect(this->blueBut_1,SIGNAL(clicked()),SLOT(slotVSBrouken()));
	connect(this->redBut_27,SIGNAL(clicked()),SLOT(slotOnTwoGroup()));
	connect(this->yellowBut_19,SIGNAL(clicked()),SLOT(slotNothingFindHere()));
	connect(this->blueBut_5,SIGNAL(clicked()),SLOT(slotNeedWeapons()));
	connect(this->yellowBut_17,SIGNAL(clicked()),SLOT(slotSafeToSit()));

	connect(this->closeBut,SIGNAL(clicked()),SLOT(slotClose()));
	connect(this->eraseBut,SIGNAL(clicked()),SLOT(slotErase()));
	connect(this->sendBut,SIGNAL(clicked()),SLOT(slotSendMessage()));
	
	connect(this->blueCat,SIGNAL(clicked()),SLOT(slotBlueCat()));
	connect(this->redCat,SIGNAL(clicked()),SLOT(slotRedCat()));
	connect(this->yellowCat,SIGNAL(clicked()),SLOT(slotYellowCat()));
	connect(this->keyboardButton,SIGNAL(clicked()),SLOT(_slotKeyboard()));


	redCatWgt->hide();
	yellowCatWgt->hide();
	slotBlueCat();
	_loadSettings();
}

GenRaportsClass::~GenRaportsClass()
{

}

void GenRaportsClass::slotBlueCat()
{
	this->RootWgt->hide();
		this->RootWgt->setStyleSheet(QString::fromUtf8(" QWidget{ \n"
				"	border-image: url(:/Pyramid/z_si_current.png); }"	));
	this->RootWgt->show();
}

void GenRaportsClass::slotRedCat()
{
	this->RootWgt->hide();
	this->RootWgt->setStyleSheet(QString::fromUtf8(" QWidget{ \n"
				"	border-image: url(:/Pyramid/z_kr_current.png); }"	));
	this->RootWgt->show();
}

void GenRaportsClass::slotYellowCat()
{
	this->RootWgt->hide();
	this->RootWgt->setStyleSheet(QString::fromUtf8(" QWidget{ \n"
				"	border-image: url(:/Pyramid/z_g_current.png); }"	));
				
	this->RootWgt->show();
}

//слоты разные
//слот для приема ответа после отправки сообщения
void GenRaportsClass::_slotRaportResult(bool result)
{
		if (result) 
		{
			//messEdit->setPlainText(tr("Отправлено"));
			this->messageTextBox->setPlainText(tr("отправлено"));
		}
		else
		{
			this->messageTextBox->setPlainText(tr("не отправлено"));
		}
}

//слот для закрытия окна
void GenRaportsClass::slotClose()
{
	emit signDeleteThis();
}
//слот для очистки поля сообщений
void GenRaportsClass::slotErase()
{
	this->messageTextBox->clear();
	message = "";
}


void GenRaportsClass::setMessage(QString message)
{
	slotErase();
	QString tmp(this->messageTextBox->toPlainText());
	tmp += " " + message;
	this->messageTextBox->setPlainText(tmp);
}



void GenRaportsClass::slotGetAzimuth(double azimuth)
{
	QString tmp(this->messageTextBox->toPlainText ());
	tmp += QString::number((int)azimuth) +tr("°") ;
	this->messageTextBox->setPlainText(tmp);
	this->show();
	emit signDelNavi();
}

void GenRaportsClass::slotSendMessage()
{
	message = this->messageTextBox->toPlainText ();
	sendReport(message);	
}


void GenRaportsClass::slotGetNaprSled(){setMessage(tr("сообщите направление движения")+_devicePosition());}

void GenRaportsClass::slotSendNaprSled()
{
	
	SelectDialog dlg(_parent,tr("Мастер"),
		tr("Щелкните по карте,\nчтобы задать координаты\nГотовы?"),false);
	dlg.exec();	
	if(dlg.result == 1)
	{
		setMessage(tr("последние данные о моем положении ш:") + CalcFunctions::dDToDDMMSS(_lat) + tr("д:") + CalcFunctions::dDToDDMMSS(_lon) + tr("двигаемся на азимут: ")); 
		this->hide(); 
		emit signMapPoint();
	}	

}

void GenRaportsClass::slotTryToFly(){setMessage(tr("попытаемся взлететь")+_devicePosition());}
void GenRaportsClass::slotSafeToSit(){setMessage(tr("здесь можно приземлиться")+_devicePosition());}
void GenRaportsClass::slotNeedMeh(){setMessage(tr("нам нужен механик")+_devicePosition());}
void GenRaportsClass::slotOllOk(){setMessage(tr("все в порядке")+_devicePosition());}
void GenRaportsClass::slotNothingFindHere(){setMessage(tr("ничего не нашли")+_devicePosition());}
void GenRaportsClass::slotNo(){setMessage(tr("ответ нет")+_devicePosition());}
void GenRaportsClass::slotVSInThisNapr()
{
	SelectDialog dlg(_parent,tr("Мастер"),
		tr("Щелкните по карте,\nчтобы задать координаты\nГотовы?"),false);
	dlg.exec();	
	if(dlg.result == 1)
	{
		setMessage(tr("последние данные о моем положении: ") + CalcFunctions::dDToDDMMSS(_lat)  + tr("д:") +  CalcFunctions::dDToDDMMSS(_lon) + tr(", воздушное судно в этом направлении: ")); 
		this->hide(); 
		emit signMapPoint();
	}	
}
void GenRaportsClass::slotYes(){setMessage(tr("ответ да")+_devicePosition());}
void GenRaportsClass::slotWeAFindAllPeople(){setMessage(tr("мы нашли всех людей")+_devicePosition());}
void GenRaportsClass::slotNotAnders(){setMessage(tr("не понял")+_devicePosition());}
void GenRaportsClass::slotFindNotAllPeople(){setMessage(tr("мы нашли группу людей")+_devicePosition());}
void GenRaportsClass::slotOperationsEnded(){setMessage(tr("ПСО закончено")+_devicePosition());}
void GenRaportsClass::slotCantContinueReturn(){setMessage(tr("мы не можем продолжать ПСО, возвращаемся на базу")+_devicePosition());}
void GenRaportsClass::slotOnTwoGroup(){setMessage(tr("мы разделились на две группы")+_devicePosition());}
void GenRaportsClass::slotNeedLamp(){setMessage(tr("нужна сигнальная лампа с батареей и радиостанцией")+_devicePosition());}
void GenRaportsClass::slotNeedFuel(){setMessage(tr("требуется топливо и масло")+_devicePosition());}
void GenRaportsClass::slotVSBrouken(){setMessage(tr("судно серьезно повреждено")+_devicePosition());}
void GenRaportsClass::slotNeedKompas(){setMessage(tr("требуются карта и компас")+_devicePosition());}
void GenRaportsClass::slotNeedWeapons(){setMessage(tr("требуются оружие и боеприпасы")+_devicePosition());}
void GenRaportsClass::slotNeedFood(){setMessage(tr("нужны пища и вода")+_devicePosition());}
void GenRaportsClass::slotCantMove(){setMessage(tr("неспособны двигаться")+_devicePosition());}
void GenRaportsClass::slotNeedDrugs(){setMessage(tr("нужны медикаменты")+_devicePosition());}
void GenRaportsClass::slotNeedDoctor(){setMessage(tr("нужен доктор")+_devicePosition());}

void GenRaportsClass::_slotGpsData(QTime time, QDate date, bool status, double lat, double lon, double speed, double angle,short sat, float height)
{
	_lat = lat;
	_lon = lon;
}

void GenRaportsClass::sendReport(QString report)
{
	emit signMessage(report,_toKcps);
}

bool GenRaportsClass::_loadSettings()
{
	QString path_to_ini = QCoreApplication::applicationDirPath () + "/settings_akps.ini";
	
	if (QFile::exists(path_to_ini) ) 
	{
		QSettings akps_settings (path_to_ini, QSettings::IniFormat);
		_lat = akps_settings.value("programm/lastLat").toDouble();
		_lon = akps_settings.value("programm/lastLon").toDouble();
		_toKcps = akps_settings.value("programm/def_serv").toInt();
		return true;
	}

	return false;
}

void GenRaportsClass::_slotMapPoint(double lat, double lon)
{

}

void GenRaportsClass::_slotAzimuth(double azimuth)
{
	QString tmp(this->messageTextBox->toPlainText ());
	tmp += QString::number((int)azimuth)  +tr("°");
	this->messageTextBox->setPlainText(tmp);
	this->show();
	emit signDelNavi();
}

void GenRaportsClass::_slotButtonClickSound()
{
	CalcFunctions::soundPressedBut(5);
}

QString GenRaportsClass::_devicePosition()
{
	return QString( tr(" (мои координаты: ") + tr("ш:") + CalcFunctions::dDToDDMMSS(_lat)  + tr("д:") +  CalcFunctions::dDToDDMMSS(_lon) + tr(")"));
}

void GenRaportsClass::_slotKeyboard()
{
	
	messageTextBox->clear();
    messageTextBox->setFocus();
	CalcFunctions::runKeyboard(this);
}