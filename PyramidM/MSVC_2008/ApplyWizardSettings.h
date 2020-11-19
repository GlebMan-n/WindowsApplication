#include "stdafx.h"
#include <QObject>



class ApplyWizzardSettings : public QObject
{
	Q_OBJECT
public:	
	ApplyWizzardSettings(QObject* parent);
	~ApplyWizzardSettings();
	bool loadSettings();

signals:
	void signDrawGrid(QString path);
	void signDrawRoute(QString path);
	void signNavi(double lat, double lon);
	void signDiscret(short discr);
};