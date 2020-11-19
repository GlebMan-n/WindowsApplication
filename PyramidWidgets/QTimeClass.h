
#include "Windows.h"
#include <QtGui>
#include <QtCore>
class QTimeClass : public QThread
{
	Q_OBJECT
public:
	QTimeClass(QObject* parent);
	~QTimeClass();

public:
	void stop();
	bool isStopped();
protected:
	void run();
private:
	volatile bool stopped;
	//������������� ����� �������� ������� �� ��������� UTC � GPS
	void _setTime(QTime timeUtc);


	bool _loadSettings();
	void _saveSettings();
private:
	//������� ���� �� ���������
	short _timeZone;	
	bool _updateTime; 
	short _timeToUpdate;
private slots:
	void _slotGpsData(QTime time, QDate date, bool status, double lat, double lon, double speed, double angle,short sat, float height);
	//���� ��� ������� �������� ����� ���������
	void _slotUpdateSettings();
signals:
	void signTime(QString day, QString sdate, QString stime);
};