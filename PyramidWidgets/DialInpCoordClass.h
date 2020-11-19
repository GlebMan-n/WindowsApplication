
#include "ui_DialogInputCoord.h"
#include <QDialog>


class InputCoordinatesClass : public QDialog, public Ui::InputCoordDlg
{
	Q_OBJECT
public:
	InputCoordinatesClass(QWidget *parent,double lat = 0, double lon = 0);
	double Lat;
	double Lon;
	bool result;

private:
	void setPropertyes(double lat, double lon);
	bool b_east;
	bool b_noth;

private slots:
	void slotOk();
	void slotCancel();
	void slotCheckEast(int x);
	void slotCheckNoth(int x);

	void slotPlusLatDegr();
	void slotPlusLatMin();
	void slotPlusLatSec();

	void slotMinusLatDegr();
	void slotMinusLatMin();
	void slotMinusLatSec();

	void slotPlusLonDegr();
	void slotPlusLonMin();
	void slotPlusLonSec();

	void slotMinusLonDegr();
	void slotMinusLonMin();
	void slotMinusLonSec();
	void _slotButtonClickSound();
	
};