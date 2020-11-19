#include "ui_MapScale.h"
#include <QtGui>

class MapScaleClass : public QWidget, public Ui::ScaleMenu
{
	Q_OBJECT
public:
	MapScaleClass(QWidget* parent);
	~MapScaleClass();
private:
	short k;
	long kk;
private slots:
	void _slot1k();
	void _slot10k();
	void _slot25k();
	void _slot50k();

	void _slot100k();
	void _slot200k();
	void _slot500k();
	void _slot1kk();

	void _slot4kk();
	void _slot5kk();
	void _slot8kk();
	void _slot40kk();

	void _slotButtonClickSound();
signals:
	void signScale(double scale);
};