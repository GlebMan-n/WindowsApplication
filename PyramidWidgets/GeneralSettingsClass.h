
#include "ui_GenSettings.h"
#include <QtGui>
#include <QCoreApplication>

class GeneralSettingsClass : public QDialog, public Ui::GeneralSettingsMenu
{
	Q_OBJECT
public:
	GeneralSettingsClass(QWidget *parent = 0);
	~GeneralSettingsClass();
private:
	bool _loadSettings();
	void _saveSettings();
	void _initTime();
	void _setSettings(QString pathToPhoto, QString pathToTracks, short timeZone);

	//настройки со значениями по умолчанию
	QString _pathToPhoto;
	QString _pathToTracks;
	short _timeZone;
	short _trackFormat;

private slots:
	void _slotChangeTimeZone(QString timeZone);
	void _slotTime(QString day, QString sdate, QString stime);
	void _slotApply();
	void _slotCancel();
	void _slotBack();
	void _slotTrackFormatChanged(int trackFormat);
	void _slotKeyboard();
	void _slotButtonClickSound();
signals:
	void signGeoDataSettings();
	void singNewTimeZone();
};