#include "ui_GenRaports.h"
#include <QtGui>

class GenRaportsClass : public QDialog, public Ui::GeneralReportsMenu
{
	Q_OBJECT
public:
	GenRaportsClass(QWidget *parent);	
	~GenRaportsClass();
private:
	//�������� ���������
	void sendReport(QString report);
	void setMessage(QString message);
	bool _loadSettings();
	QString _devicePosition();
private:
	//���������� ��� ��������������� ���������
	QString message;
	double _lat;
	double _lon;
	QWidget* _parent;
	short _toKcps;
private slots:
	void _slotGpsData(QTime time, QDate date, bool status, double lat, double lon, double speed, double angle,short sat, float height);
	void _slotMapPoint(double lat, double lon);
	//����� ������
	//���� ��� ������ ������ ����� �������� ���������
	void _slotRaportResult(bool result);
	//���� ��� �������� ����
	void slotClose();
	//���� ��� ������� ���� ���������
	void slotErase();
	//����� ��� ������ ���������
	
	void slotGetNaprSled();
	void slotSendNaprSled();
	void slotTryToFly();
	void slotSafeToSit();
	void slotNeedMeh();
	void slotOllOk();
	void slotNothingFindHere();
	void slotNo();
	void slotVSInThisNapr();
	void slotYes();

	void slotWeAFindAllPeople();
	void slotNotAnders();
	void slotFindNotAllPeople();
	void slotOperationsEnded();
	void slotCantContinueReturn();
	void slotOnTwoGroup();
	void slotNeedLamp();
	void slotNeedFuel();
	void slotVSBrouken();
	void slotNeedKompas();

	void slotNeedWeapons();
	void slotNeedFood();
	void slotCantMove();
	void slotNeedDrugs();
	void slotNeedDoctor();

	//���� ��� ������ ���������
	void slotGetAzimuth(double azimuth);
	void slotSendMessage();

	void slotBlueCat();
	void slotRedCat();
	void slotYellowCat();
	void _slotAzimuth(double azimuth);
	void _slotButtonClickSound();
	void _slotKeyboard();
signals:
	void signGiveAzimuth();
	void signDeleteThis();
	void signMessage(QString message, short toKcps);
	void signMapPoint();
	void signDelNavi();

};