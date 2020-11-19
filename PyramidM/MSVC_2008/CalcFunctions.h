#include <windows.h>
#include <c:\Program Files (x86)\Microsoft Visual Studio 9.0\VC\include\math.h>
#include <c:\Program Files (x86)\Microsoft Visual Studio 9.0\VC\include\strstream>
#include <QPainter>
#include <QProcess>
#include <QSound>
#include <QDir>
#include <QDate>
#include <QTime>
#include <QByteArray>
#include <windows.h>
#include <psapi.h>
#include <QString>
#define CCT_180_PI 57.2957795130823208769
#define CCT_PI_180 0.01745329251994329547
#define CCT_PI	   3.14159265358979323846


class CalcFunctions
{
public:

	//перевод координат из дес€тичных градусов в вид "√√∞ћћ`——" "
	static QString dDToDDMMSS(double degr)
	{
		double dd = degr * CCT_180_PI;
		QString result;
		short DD = (short)dd;
		short MM = (short)((dd - DD)*60);
		short SS = (((dd - DD)*60)-MM)*60;
		result = QString::number(DD)+ QString("∞")+QString::number(MM)+QString("`")+QString::number(SS)+QString("\" ");
		return result;
	}
	//им€ ѕ 
	static QString nameOfPc()
	{
		unsigned  long Size = 32 + 1;
		char *Buffer = new  char[Size];
		bool bRet = GetComputerNameA(Buffer, &Size);
		if (!bRet) 
			return QString("VPU2PC11");
		else
			return QString::fromUtf8(Buffer);
	}

	static QRect calcWindowPos(QRect pRect/*размер родител€*/, QRect mRect/*свой размер*/)
	{
		QRect result;
		result.setX(pRect.width()/2-mRect.width()/2);
		result.setY(pRect.height()/2-mRect.height()/2);
		result.setWidth(mRect.width());
		result.setHeight(mRect.height());
		return result;
	}

	
	//–асчет рассто€ни€ между двум€ координатами
	static double findRasst(double shirota, double shirota2, double dolgota, double dolgota2)
	{

		//разница между долготой1 и долготой 2
		double delta = findDelta(dolgota2*CCT_PI_180, dolgota*CCT_PI_180);

		//находим рассто€ние между точками по формуле гаверсинусов в  ћ

		return 2 * asin(sqrt(
					
			mySqr(sin(
				(shirota2 * CCT_PI_180 - shirota * CCT_PI_180) / 2
					)) 
			
			+

			cos(shirota * CCT_PI_180) 
			
			* 
			
			cos(shirota2 * CCT_PI_180)
			
			*
			
			mySqr(sin(
				delta / 2
			)) 
					
		)) * 6372.795;

	}

	static double findDelta(double a, double b)
	{
		if(a > b)
			a = a - b;
		else 
			a = b - a;
		return a;
	}

	static double mySqr(double a)
	{
		a = a * a;
		return a;
	}

//верно считает направление не по "питерски"
	static double calcTrueAngleRad(double shirota, double shirota2, double dolgota, double dolgota2)
	{
		double rad = 6372795;
		//получаем координаты точек в радианах
		double lat1 = shirota;
		double lat2 = shirota2;
		double long1 = dolgota;
		double long2 = dolgota2;
		//косинусы и синусы широт и разниц долгот
		double cl1 = cos(lat1);
		double cl2 = cos(lat2);
		double sl1 = sin(lat1);
		double sl2 = sin(lat2);
		double delta = long2 - long1;
		double cdelta = cos(delta);
		double sdelta = sin(delta);
		double x = (cl1*sl2) - (sl1*cl2*cdelta);
		double y = sdelta*cl2;
		double z = (atan(-y/x))*CCT_180_PI;//градусы = –адианы * CCT_180_PI; //радианы = градусы * CCT_PI_180;
		if (x < 0)  //вли€ет направление юг
		{
			z += 180;
		}
		//минус мен€ет запад и восток местами
		z = -(( fmod ((z + 180),360) - 180) * CCT_PI_180);///////////правлено дл€ корректировки
		double result = z - ((2*CCT_PI)* ( floor(z/(2*CCT_PI)) ));
		return result;
	}

	static double findRasstRadians(double lat, double lat2, double lon, double lon2)
	{

		//разница между долготой1 и долготой 2
		double delta = findDelta(lon2, lon);

		//находим рассто€ние между точками по формуле гаверсинусов в  ћ

		return 2 * asin(sqrt(
					
			mySqr(sin(
				(lat2  - lat) / 2
					)) 
			
			+

			cos(lat) 
			
			* 
			
			cos(lat2)
			
			*
			
			mySqr(sin(
				delta / 2
			)) 
					
		)) * 6372.795;

	}

	static double dDMM2D(QString str)
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
				//исходим из ее положени€
				//два символа слева - минуты, а три справа - доли минуты
				min = str[i-2]+ QString(str[i-1]) + str[i] + str[i+1] + QString(str[i+2]) + QString(str[i+3]);
				//4 и 3 слева от точки это градусы
				grad = str[i-4] + QString(str[i-3]);
				break; // работа цикла более не требуетс€
			}
		}
		//теперь переводим DD MM в DD.DDDDD ( DD.DDDDD = DD MM / 60 )
		x = grad.toDouble() + min.toDouble() / 60;
		return x;
	}


	static QPixmap rotatePixmap(int angleInt,char* pixmapPath)
	{
		//QString::fromUtf8(":/Pyramid/roza.png")
		QPainter painter;
		QPixmap pix_ = QPixmap(QString::fromUtf8(pixmapPath));
		QPixmap result = QPixmap(pix_.size());
		result.fill(QColor());
		result.setAlphaChannel(result);
		QPointF center(pix_.width() / qreal(2), pix_.height() / qreal(2));
		painter.begin(&result);
		painter.save();
		painter.setRenderHint(QPainter::Antialiasing);
		painter.setRenderHint(QPainter::SmoothPixmapTransform);
		painter.translate(center);
		painter.rotate(angleInt);
		painter.translate(-center);
		painter.fillRect(0,0,result.width(),result.height(),QColor(255, 0, 0, 0));
		painter.drawPixmap(QPointF(0, 0), pix_);
		painter.setBrush(Qt::NoBrush);
		painter.restore();
		painter.end();
		return result;
	}

	static void runKeyboard(QObject* parent)
	{
		//русска€
		LoadKeyboardLayoutA("00000419", KLF_ACTIVATE);
		//английска€
		//LoadKeyboardLayout("00000409", KLF_ACTIVATE);	
		QProcess *process = new QProcess(parent);
		QString wordPath = QCoreApplication::applicationDirPath () + "/add/osk.exe";
		process->start(wordPath);
	}

	
	static void soundPressedBut(int sig)
	{
		QSound photo(QCoreApplication::applicationDirPath () + "/sounds/CAMERA.WAV");
		QSound end_task(QCoreApplication::applicationDirPath () + "/sounds/message.wav");
		QSound marsh_lost(QCoreApplication::applicationDirPath () + "/sounds/message.wav");
		QSound marsh_end(QCoreApplication::applicationDirPath () + "/sounds/message.wav");
		QSound but_pressed(QCoreApplication::applicationDirPath () + "/sounds/but_press.wav");//
		
		QSound RouteEnd(QCoreApplication::applicationDirPath () + "/sounds/RouteEnd.wav");
		QSound RouteIn(QCoreApplication::applicationDirPath () + "/sounds/RouteIn.wav");
		QSound RouteOut(QCoreApplication::applicationDirPath () + "/sounds/RouteOut.wav");
		QSound RouteViaPoint(QCoreApplication::applicationDirPath () + "/sounds/RouteViaPoint.wav");
		QSound GPSConnected(QCoreApplication::applicationDirPath () + "/sounds/GPSConnected.wav");
		QSound GPSMissing(QCoreApplication::applicationDirPath () + "/sounds/GPSMissing.wav");
		QSound GPSPoor(QCoreApplication::applicationDirPath () + "/sounds/GPSPoor.wav");
		
		switch(sig)
		{
			case 0:
				photo.play();
			break;

			case 1:
				end_task.play();

			break;

			case 2:
				marsh_end.play();

			break;

			case 3:
				marsh_lost.play();

			break;

			case 4:
				marsh_end.play();

			break;

			case 5:
				but_pressed.play();
			break;
				
				
			case 6:
				RouteEnd.play();
				break;
			case 7:
				RouteIn.play();
				break;
			case 8:
				RouteOut.play();
				break;

			case 9:
				RouteViaPoint.play();
			break;

			case 10:
				GPSConnected.play();
			break;

			case 11:
				GPSMissing.play();
			break;

			case 12:
				GPSPoor.play();
			break;
		}
	}

	static void logError(const QString &error)
	{
		QString currDate = QDateTime::currentDateTime ().toString ();
		QString path = QCoreApplication::applicationDirPath();
		path +=	QDir::separator();
		path += QString("tmp");
		path +=	QDir::separator();
		path += QString("error.log");

		QFile file (path);
		if (!file.open(QIODevice::Append | QIODevice::Text))
			return;

		QString formatAdd = currDate + QString("\n")
			+ error + QString("\n");
		file.write(formatAdd.toLocal8Bit(), formatAdd.size());
		file.close ();

	}

	static int getProcessSize()
	{
	    PROCESS_MEMORY_COUNTERS pmc;
        GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
        return (pmc.WorkingSetSize/1048576); // в  Ѕ
	}

};