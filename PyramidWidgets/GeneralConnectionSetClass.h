
#include "ui_GenConnectSettings.h"
#include <QtGui>
#include <QCoreApplication>

class GeneralConnectionSetClass : public QDialog, public Ui::ConnectSettingsMenu
{
	Q_OBJECT
public:
	GeneralConnectionSetClass(QWidget *parent = 0);
	~GeneralConnectionSetClass();
private:
	bool _loadSettings();
	void _saveSettings();
	void _initTime();
	void _setSettings(QString vpuAdr, QString kcpsAdr, QString idDevice, QString nameOfPc ,short timeToSend, short defSrvV);
	void _setNameOfPc();


	QString _urlKcpsIN;
	QString _urlKcpsOUT;

	QString _urlVpuIN;
	QString _urlVpuOUT;

	QString _url_to_gkcpsIN;
	QString _url_to_gkcpsOUT;

	QString _url_to_shtabIN;
	QString _url_to_shtabOUT;
	
	QString _idDevice;
	QString _nameOfPc;
	short _defSrv;
	short _timeToSend;
	QWidget* _parent;

private slots:
	void _slotTime(QString day, QString sdate, QString stime);
	void _slotApply();
	void _slotCancel();
	void _slotBack();
	void _slotDefSrv(int serv);
	void _slotKeyboard();
	void _slotButtonClickSound();
	void _slotUrlDlg();
signals:
	void signUpdateSettings();
	void signDefSrv(short defSrv);
	void signDeleteMenu();
};