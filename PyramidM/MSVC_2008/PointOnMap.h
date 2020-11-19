#include "stdafx.h"
#include "gis\GrUnits\TknMapScale.h"
//класс выбора точки на карте

class TknSimpleLines;
class PointOnMapClass: public QObject, public GisInputInterface 
{
	Q_OBJECT

public:
	/** конструктор */
	PointOnMapClass(GisView *pt);
	/** деструктор */
	virtual ~PointOnMapClass();

	
	Pos2D mouse2;

	double Lat;
	double Lon;


protected:
	/** обработчик события нажатия клавиш манипулятора мышь */
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