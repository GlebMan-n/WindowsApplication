#include "ui_MapSettings.h"
#include <QtGui>

class MapSettingsClass : public QWidget, public Ui::MapSettings
{
	Q_OBJECT
public:
	MapSettingsClass(QWidget* parent);
	~MapSettingsClass();
private slots:
	void _slotButtonClickSound();
signals:
	void signTrackSettings();
	void signSearchGps();
	void signSpeedFormat();
	void signDeviceManager();
	void signBrightness();

};