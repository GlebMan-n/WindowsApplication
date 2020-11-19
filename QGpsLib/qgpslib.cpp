#include "qgpslib.h"

QGpsLib::QGpsLib(QString qport, QString qbrate, QString qbsize, QString qparity, QString qstopbits)
{
	gpsTr = NULL;
	gpsTr = new QGpsThread(qport,qbrate,qbsize,qparity,qstopbits);
	connect(gpsTr, SIGNAL(signSendCurrentGpsString(QString)),SLOT(slotGPS(QString)));
	connect(gpsTr, SIGNAL(signSendError(QString)),SLOT(slotGPSerror(QString)));
	QString tmp = qport.right(2);
	if(tmp[0] == 'M' || tmp[0] == 'm')
	{
		QString tmp2(tmp[1]);
		_com = tmp2.toInt();
	}
	else
	{
		_com = tmp.toInt();
	}
}

QGpsLib::QGpsLib()
{

	gpsTr = NULL;
	if(!gpsTr)
	gpsTr = new QGpsThread();
	connect(gpsTr, SIGNAL(signSendCurrentGpsString(QString)),SLOT(slotGPS(QString)));
	connect(gpsTr, SIGNAL(signSendError(QString)),SLOT(slotGPSerror(QString)));

}


QGpsLib::~QGpsLib()
{
	gpsTr->stop();
	while (!gpsTr->isFinished())
	{
		MyThread::msleep(50);
	}
	gpsTr->~QGpsThread();
	gpsTr = NULL;
}

void QGpsLib::slotGPS(QString string)
{
	bool check = checkSum(&string);
	emit signSendAll(string,check);
	GPGGAstruct gpgga;
	GPRMCstruct gprmc;

	if(string.startsWith("$GPGGA"))
	{
		
		gpgga = gPGGADataHandler(string);
		emit signSendGpgga(gpgga, check,_com);

	}

	if(string.startsWith("$GNGGA"))
	{
		
		gpgga = gPGGADataHandler(string);
		emit signSendGpgga(gpgga, check,_com );

	}

	if(string.startsWith("$GPRMC"))
	{
	
		gprmc = gPRMCDataHandler(string);
		emit signSendGprmc(gprmc, check ,_com);
		

	}
	
	if(string.startsWith("$GNRMC"))
	{
	
		gprmc = gPRMCDataHandler(string);
		emit signSendGprmc(gprmc, check,_com );
		

	}
	
	
	
}

void QGpsLib::slotGPSerror(QString string)
{
	emit signSendError(string);
}



void QGpsLib::readDataFromGps(bool read)
{
	if(!gpsTr)
		return;
	if (read)
	{
		gpsTr->start();
	}
	else
	{
		gpsTr->stop();
		while(gpsTr->isRunning())
		{
			MyThread::msleep(50);
		}
	}

}


GPGGAstruct QGpsLib::gPGGADataHandler(QString str)
{ 
	
	GPGGAstruct GPGGA;
	switch (short i =  str.section(',', 6, 6).toInt())
	{
		case 0 :
			GPGGA.observ = NODATA;
			break;
		case 1 :
			GPGGA.observ = OBSPOL;
			break;
		case 2 :
			GPGGA.observ = OBSDEF;
			break;
	}

	GPGGA.time = str.section(',', 1, 1);
	GPGGA.snapr = str.section(',', 3, 3);
	GPGGA.dnapr = str.section(',', 5, 5);
	GPGGA.satellite = str.section(',', 7, 7).toFloat();
	GPGGA.geomfact = str.section(',', 8, 8).toDouble();
	GPGGA.visota = str.section(',', 9, 9).toDouble();
	GPGGA.geoidelips = str.section(',', 11, 11).toDouble();
	GPGGA.difpop = str.section(',', 13, 13).toDouble();
	GPGGA.iddifst = str.section(',', 14, 14);

	if (GPGGA.snapr == "S" )
		GPGGA.shir = -1*dDMM2D(str.section(',', 2, 2));
	else
		GPGGA.shir = dDMM2D(str.section(',', 2, 2));

	if (GPGGA.dnapr == "W") 
		GPGGA.dolg = -1*dDMM2D(str.section(',', 4, 4));
	else 
		GPGGA.dolg = dDMM2D(str.section(',', 4, 4));
	return GPGGA;
}


GPRMCstruct QGpsLib::gPRMCDataHandler(QString str)
{ 
	GPRMCstruct GPRMC;
	if (str.section(',', 2, 2) == "A") GPRMC.status = A;
	else GPRMC.status = V;


	GPRMC.time = str.section(',', 1, 1);
	GPRMC.snapr = str.section(',', 4, 4);
	GPRMC.dnapr = str.section(',', 6, 6);
	

	if (GPRMC.snapr == "S" )
		GPRMC.shir = -1*dDMM2D(str.section(',', 3, 3));
	else
		GPRMC.shir = dDMM2D(str.section(',', 3, 3));

	if (GPRMC.dnapr == "S" )
		GPRMC.dolg = -1*dDMM2D(str.section(',', 5, 5));
	else 
		GPRMC.dolg = dDMM2D(str.section(',', 5, 5));

	GPRMC.speed = str.section(',', 7, 7).toDouble() * 1.852;
	GPRMC.napr = str.section(',', 8, 8).toDouble();
	GPRMC.data = str.section(',', 9, 9);
	GPRMC.magvar1 = str.section(',', 10, 10).toDouble();
	GPRMC.magvar2 = str.section(',', 11, 11);

	return GPRMC;
}

double QGpsLib::dDMM2D(QString str)
{

	// градусы минуты, в х будет результат
	QString grad,min;
	double x=0;

	//в переменной str долгота или широта в градусах, минутах
	for (int i = 0; i < str.size(); ++i)
	{
		//ищем точку
		if(str[i] == '.')
		{			
			//исходим из ее положения
			//два символа слева - минуты, а три справа - доли минуты
			min = str[i-2]+ QString(str[i-1]) + str[i] + str[i+1] + QString(str[i+2]) + QString(str[i+3]);
			//4 и 3 слева от точки это градусы
			grad = str[i-4] + QString(str[i-3]);
			break; // работа цикла более не требуется
		}
	}
	//теперь переводим DD MM в DD.DDDDD ( DD.DDDDD = DD MM / 60 )
	x = grad.toDouble() + min.toDouble() / 60;
	return x;

}

bool QGpsLib::isStopped()
{
	return gpsTr->isFinished();
}

bool QGpsLib::checkSum(QString *string)
{
	
	QByteArray qb = string->toUtf8();

	char *nmea_line = qb.data();
	int i;
	int XOR;
	int c;
	for (XOR = 0, i = 0; i < strlen(nmea_line); i++) 
	{
		c = (unsigned char)nmea_line[i];
		if (c == '*') break;
		if (c != '$') XOR ^= c;
	}
    QString hexadecimal;
	hexadecimal.setNum(XOR,16).toUpper();

	QString checkSumStr(nmea_line);
	checkSumStr = checkSumStr.right(4);

	bool ok;
	short sum1 = hexadecimal.toShort(&ok,16);
	short sum2 = checkSumStr.toShort(&ok,16);
	return sum1 == sum2;
}

