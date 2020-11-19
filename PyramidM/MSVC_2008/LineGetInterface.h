#include "stdafx.h"
#include "gis\GrUnits\TknMapScale.h"


class TknSimpleLines;
class LineGetInterface: public QObject, public GisInputInterface 
{
	Q_OBJECT

public:
	/** конструктор */
	LineGetInterface(GisView *pt);
	/** деструктор */
	virtual ~LineGetInterface();

	Pos2D mouse1;
	Pos2D mouse2;

	struct coordinates
	{
		coordinates()
		{
			lat1 = 0;
			lon1 = 0;
			lat2 = 0;
			lon2 = 0;
		}
		double lat1;
		double lon1;
		double lat2;
		double lon2;
	};

	coordinates COORDINATES;
protected:
	/** обработчик события нажатия клавиш манипулятора мышь */
	virtual RetTypeRedraw mousePressEvent(const Mouse& mouse, ImageDevice *dev);
	virtual RetTypeRedraw mouseMoveEvent (const Mouse& mouse, ImageDevice *dev);
	virtual RetTypeRedraw detach();

protected:
	VectorData	  *m_pLassoLayer;
	TknSimpleLines *m_pTkn;
	TknSimpleLines *m_pTkn1;
	TknSimpleLines *m_pTkn2;
	bool isPressOnce;
signals:
	void lineIs();
	void firstClick();
};