
#include "ui_GenGpsSettings.h"
#include <QtGui>


class SelectDialog;
class GeneralGpsSetClass : public QDialog, public Ui::GpsSettingsMenu
{
	Q_OBJECT
public:
	GeneralGpsSetClass(QWidget *parent = 0);
	~GeneralGpsSetClass();
	void init();
private:
	QWidget* _parent;
	QTimer* m_timer;
	bool firstRun;
	SelectDialog* _dlg;
	QStringList comPorts;
	short _counter;
	
private:
	bool _loadSettings();
	void _saveSettings();
	void _setGpsSettings(QString com, QString brate, QString bsize, QString par, QString stopb);
	void _initTime();
	void _searchGps();
private slots:
	
	void _slotTime(QString day, QString sdate, QString stime);
	//получаем статус GPS
	void _slotIsGps(bool gps);

	void _slotApply();
	void _slotTest();
	void _slotCancel();
	void _slotButtonClickSound();
	void _slotComList(QStringList comList, QStringList comTypesList);
	void _slotBadSign();
	void _slotSearchGps();
	void _slotSetCurrentData(QString string,bool check);
	void _slotSetCurrentDataErrors(QString string);

	void _slotNumbOfGps(short numb);
	
signals:
	//откатываем настройки GPS к сохраненным
	//текущие данные GPS
	void signCurrentData();
	//перезапуск GPS
	void signGpsRestart();
	void signBack();
	void signUpdateSettings();
	void signScanCom();
	void signGetNumbOfGps();


};