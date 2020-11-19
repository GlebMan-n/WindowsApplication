#include "ui_GenWizzard.h"
#include <QtGui>
#include <PyramidM\MSVC_2008\WorkStructures.h>
#include <QtCore>

class FileExplorerClass;

class GeneralWizardClass : public QDialog, public Ui::WizzardDlg
{
	Q_OBJECT
public:
	GeneralWizardClass(QWidget *parent = 0);
	~GeneralWizardClass();
	//��� ��������� �������
	bool loadSettings();
	bool isExec;
	void _init();
private:
	//���������
	bool _loadPathSettings();
	
	bool _loadSettings();
	void _saveSettings();
	bool _deleteSettingsFile();

	void _lockRouteButtons();
	void _lockPaletkaButtons();
	void _lockNaviButtons();
	void _lockDisctButtons();
	void _udpatePal();
	void _updateRoute();

	
private:
	//��������� (���������)
	QWidget* _parent;
	wizardSettings _settings;
	QString _palName;
	QString _pathToPal;
	QString _pathToRoute;
	QString _urlToVpu;
	QString _urlToKcps;
	double defLat;
	double defLon;
	FileExplorerClass* fileExplorerPal;
	FileExplorerClass* fileExplorerRoute;
	short _toKcps;
private slots:

	//�������� 1
	void _slotLoadPaletka();
	void _slotNextPaletka();
	void _slotPervPaletka();
	void _slotDrawSelectedPaletka();
	void _slotDrawLastPaletka();
	void _slotDontUsePaletka();
	void _slotNextPage1();

	//�������� 2
	void _slotLoadRoute();
	void _slotNextRoute();
	void _slotPervRoute();
	void _slotDrawSelectedRoute();
	void _slotDrawLastRoute();
	void _slotDontUseRoute();
	void _slotBackPage2();
	void _slotNextPage2();

	//�������� 3
	void _slotNavMarsh();
	void _slotNavCoord();
	void _slotNavPoint();
	void _slotDiscr30();
	void _slotDiscr60();
	void _slotDiscr180();
	void _slotBackPage3();
	void _slotSave();

	void _slotCancel();
	void _slotFinish();
	
	//�� ���� ���� �������� ���� � ����������� �������
	void _slotPaletka(QString fullPath);
	void _slotStatPaletka(bool status);

	void _slotRoute(QString fullPath);
	void _slotStatRoute(bool status);

	void _slotMapPoint(double lat, double lon);

	void _slotPalServChange(int toKcps);
	void _slotRouteServChange(int toKcps);

	void _slotButtonClickSound();

signals:
	//������ �� �������� ����� �������
	void signLoadPaletka(short source);
	void signLoadRoute(short source);
	void signMapPoint();
	void signLoadWizSet();
	void singUpdateSettings();
	void signLoadSaveSettings();
	void signDeleteThis();
};