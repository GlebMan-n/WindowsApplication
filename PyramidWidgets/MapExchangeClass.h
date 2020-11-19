#include "ui_MapExchangeMenu.h"
#include <QtGui>

class MapExchangeClass : public QWidget, public Ui::ExchangeMenu
{
	Q_OBJECT
public:
	MapExchangeClass(QWidget* parent);
	~MapExchangeClass();
private:
	bool _loadSettings();
private:
	short _toKcps;
private slots:
	//загрузка палетки

	//загрузка маршрута
	
	//меню отправки фото
	//меню отправки треков
	//меню отправки видео
	//прием статуса
	void _slotStatusRoute(QString status);
	void _slotStatusGrid(QString status);
	void _slotLoadRoute();
	void _slotLoadGrid();
	void _slotButtonClickSound();
	void _slotSendMarks();
	void _slotSetSendStatusMarks(bool result);
signals:
	void signLoadRoute(short source);
	void signLoadGrid(short source);

	void signDrawRoute(QString path);
	void signDrawGrid(QString path);

	void signSendTrack();
	void signSendPhoto();
	void signSendVideo();
	void signSendMarks(short source);



};