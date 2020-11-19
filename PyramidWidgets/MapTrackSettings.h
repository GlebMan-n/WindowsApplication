#include <QtGui>
#include "ui_MapTrackSettings.h"

class MapTrackSettings : public QDialog, public Ui::MapTrackSettings
{
	Q_OBJECT
public:
	MapTrackSettings(QWidget* parent);
	~MapTrackSettings();
private:
	QWidget* _parent;
	bool _changingType;
	//настройки
	short _format;
	short _color;
	double _width;
	short _discretType;
	short _discret;
	short _autoBreak;
private:
	bool _loadSettings();
	bool _saveSettings();
	void _init();
private slots:
	void _slotChangeFormat(int index);
	void _slotChangeColor(int index);
	void _slotChangeWidth(int index);
	void _slotChangeDiscr(int index);
	void _slotChangeDiscrType(int index);
	void _slotChangeAutobreak(int index);
	void _slotBreakTrack();
	void _slotApply();
signals:
	void signBreakTrack(bool clear);
	void signUpdateSettings();

};