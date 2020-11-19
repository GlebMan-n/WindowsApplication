#include "ui_MapMessageMenu.h"
#include <QtGui>

class MapMessageClass: public QDialog, public Ui::MessageMenu
{
	Q_OBJECT
public:
	MapMessageClass(QWidget* parent);
	~MapMessageClass();
	void init(QString message);
private:
	QString otvetMessage;
	short _toKcps;
	bool _loadSettings();
	QString _parsMessage(QString message);
	
private slots:
	void _slotSendResponse();
	void _slotResponseSended(bool sended);
	void _slotChangeSource(int index);

	void _slotLoadGrid();
	void _slotLoadRoute();
	void _slotStatusRoute(QString status);
	void _slotStatusGrid(QString status);
signals:
	void signMessageIsRead(QString response,short toKcps);
	void signGetGrid(short source);
	void signGetRoute(short source);
	void signDrawRoute(QString status);
	void signDrawGrid(QString status);
};