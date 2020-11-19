#include "stdafx.h"
#include "gis\GrUnits\TknMapScale.h"
//����� ������ ����� �� �����

class TknSimpleLines;
class PointOnMapClass: public QObject, public GisInputInterface 
{
	Q_OBJECT

public:
	/** ����������� */
	PointOnMapClass(GisView *pt);
	/** ���������� */
	virtual ~PointOnMapClass();

	
	Pos2D mouse2;

	double Lat;
	double Lon;


protected:
	/** ���������� ������� ������� ������ ������������ ���� */
	virtual RetTypeRedraw mousePressEvent(const Mouse& mouse, ImageDevice *dev);
	virtual RetTypeRedraw detach();

protected:
	VectorData	  *m_pLassoLayer;
	TknSimpleLines *m_pTkn;
private:
	private slots:
		void _slotClearLassoLayer();
signals:
	void clickIs();
	void signSendCoord(double lat, double lon);
	void signAzimuth(double azimuth);
};