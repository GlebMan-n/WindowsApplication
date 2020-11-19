#include "stdafx.h"
#include <QThread>
#include "qreadwritelock.h"
#include "qfile.h"
#include "qtextstream.h"
#include "QString"
#include "Windows.h"

#include <QReadWriteLock>

class SearchGpsClass : public QThread
{
	Q_OBJECT
public:
	SearchGpsClass();
	~SearchGpsClass();
private:
	short isGpsCom(QString comPort, int brate = 9600);
protected:
	void run();
signals:
	void signListOfGpsPorts(QStringList listOfPorts);
};
