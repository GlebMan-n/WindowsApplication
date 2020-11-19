#include <windows.h>
#include "GeneralMenuClass.h"
#include <PyramidM\MSVC_2008\CalcFunctions.h>

GeneralMenuForm::GeneralMenuForm(QWidget *parent,double version)
{
	this->setParent(parent);
	setupUi(this);
	this->setGeometry(parent->geometry());
	_setVers(version);

	connect(this->begin_mission_but,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(this->search_but,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(this->sat_but,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(this->sync_but,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(this->gen_sett_but,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(this->gps_sett_but,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(this->connect_sett_but,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(this->map_but,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));
	connect(this->exit_but,SIGNAL(clicked()),SLOT(_slotButtonClickSound()));

	connect(this->begin_mission_but,SIGNAL(clicked()),SIGNAL(signBeginMission()));
	connect(this->search_but,SIGNAL(clicked()),SIGNAL(signSearch()));
	connect(this->sat_but,SIGNAL(clicked()),SIGNAL(signSattelites()));
	connect(this->sync_but,SIGNAL(clicked()),SIGNAL(signSync()));
	connect(this->gen_sett_but,SIGNAL(clicked()),SIGNAL(signGenSettings()));
	connect(this->gps_sett_but,SIGNAL(clicked()),SIGNAL(signGpsSettings()));
	connect(this->connect_sett_but,SIGNAL(clicked()),SIGNAL(signConnectionSettings()));
	connect(this->map_but,SIGNAL(clicked()),SIGNAL(signOmens()));
	connect(this->exit_but,SIGNAL(clicked()),SIGNAL(signExit()));



	_initTime();
}

void GeneralMenuForm::_setVers(double version)
{
	versionBut->setText(tr("       ")+QString::number(version));
}

GeneralMenuForm::~GeneralMenuForm()
{

}

void GeneralMenuForm::_slotTime(QString day, QString sdate, QString stime)
{
	this->gen_wnd_day_lbl->setText(day);
	this->gen_wnd_date_lbl->setText(sdate);
	this->gen_wnd_time_lbl->setText(stime);
}

void GeneralMenuForm::_initTime()
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

void GeneralMenuForm::_slotButtonClickSound()
{
	CalcFunctions::soundPressedBut(5);
}