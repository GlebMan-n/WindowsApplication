
#include <StartScreenClass.h>
////////////////////////////////////////////////////////////////////////////
//StartScreen
////////////////////////////////////////////////////////////////////////////
StartScreen::StartScreen(QWidget *parent,double version)
{
	this->setParent(parent);
	setupUi(this);
	this->setGeometry(parent->geometry());
	if(parent->geometry().width() > 1024 ||  parent->geometry().height() > 600)
		this->setStyleSheet(QString("#StartScreen{border-image: url(:/Pyramid/fon.png);}"));
	_setVers(version);
	
	effect = new QGraphicsDropShadowEffect(this);
	effect1 = new QGraphicsDropShadowEffect(this);
	effect2 = new QGraphicsDropShadowEffect(this);

	effect->setBlurRadius(0);
	effect->setColor(QColor("#000000"));
	effect->setOffset(2,5);
	effect1->setBlurRadius(0);
	effect1->setColor(QColor("#000000"));
	effect1->setOffset(2,5);
	effect2->setBlurRadius(0);
	effect2->setColor(QColor("#000000"));
	effect2->setOffset(2,5);

	this->start_day_lbl->setGraphicsEffect(effect);
	this->start_date_lbl->setGraphicsEffect(effect1);
	this->time_lbl->setGraphicsEffect(effect2);

	//this->setGeometry(0,0,1024,600);
	this->load_1_lbl->hide();
	this->load_2_lbl->hide();
	this->load_3_lbl->hide();
	this->load_4_lbl->hide();
	this->load_5_lbl->hide();
	timer = NULL;
	timer = new QTimer();
	
	_initTime();

	connect(timer, SIGNAL( timeout ()), 
		this, SLOT (showLoadProgress()));
	sec = 0;
	timer->start(250);

}

void StartScreen::_setVers(double version)
{
	versionLbl->setText(tr("версия ")+QString::number(version));
}

void StartScreen::_initTime()
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
	slotTime(day, sdate, stime);
}

void StartScreen::slotTime(QString day, QString sdate, QString stime)
{
	this->start_day_lbl->setText(day);
	this->start_date_lbl->setText(sdate);
	this->time_lbl->setText(stime);
}

StartScreen::~StartScreen()
{
}


void StartScreen::showLoadProgress()
{
	if (sec == 3) 
		this->load_1_lbl->show();
			
	if (sec == 4) 
		this->load_2_lbl->show();
	
	if (sec == 5)
		this->load_3_lbl->show();
	
	if (sec == 6) 
		this->load_4_lbl->show();

	if (sec == 8) 
		this->load_5_lbl->show();
	
	if (sec == 9) 
	{
		timer->stop();
		while(timer->isActive ())
		{
			short x = 0;
		}
		delete timer;
		timer = NULL;
		emit isClosed();
		this->done(0);
		return;
	}	
	sec++;
}
