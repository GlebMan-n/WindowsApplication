#include "stdafx.h"
#include "gis\GrUnits\TknMapNonScale.h"

//структуры использующие GisBase

struct marsh
	{
		marsh()
		{
			rasst_ob = 0;
			rasst_point = 0;
			rasst_target = 0;
			skor_sred = 0;
			skor_tek = 0;
			angle = 0;
			time_mis = 0;
			time_of_stop = 0;
			skor_max = 0;
			skor_old = 0;
		}
		//расстояние
		double rasst_ob;
		double rasst_point;
		double rasst_target;
		//скорость
		double skor_old;
		double skor_sred; //средняя скорость
		double skor_tek;
		double skor_max;
		//угол
		double angle; //угол
		//время
		//SYSTEMTIME time_nach; //время начальное
		int time_mis; // время прошло
		int time_of_stop; // время стоянки
	};

	struct STR_POINT_ROUTE
	{
		STR_POINT_ROUTE()
		{
			fB			= 0;
			fL			= 0;

			iHour		= 0;
			iMinute		= 0;
			iSecond		= 0;
//			CTime time(2000, 1, 1, 0, 0, 0);
//			iTimePoint	= time.GetTime();
		}
		double fB;
		double fL;
		short iHour;
		short iMinute;
		short iSecond;
//		__int64 iTimePoint;
	};

	//структура данных GPS
	struct GpsData
	{
		GpsData()
		{
			int speed = 0;
			int avspeed = 0;
			int maxspeed = 0;
			int sat = 0;
			int height = 0;
			int batt = 0;
			int timePso = 0;
			double true_cource = 0;
			QString lat = "";
			QString lon = "";
			QString timeSun = "";
			QString timeSett = "";
			QString timeRise = "";
			QString timeUtc = "";
		}
		int speed;
		int avspeed;
		int maxspeed;
		int sat;
		int height;
		int batt;
		int timePso;
		double true_cource;
		QString lat;
		QString lon;
		QString timeSun;
		QString timeSett;
		QString timeRise;
		QString timeUtc;
	};

	struct MARK 
	{
		MARK()
		{
			m_myMark = NULL;
			type = "";
			date = "";
			time = "";
			info  = "";
			lon = 0;
			lat = 0;
			height = 0;
		}
		TknBarrier *m_myMark;
		QString type;
		QString date;
		QString time;
		QString info;
		double lon;
		double lat;
		double height;
	};

	struct STR_KALKA_1 
	{
		STR_KALKA_1()
		{
			iByte1			= 255; // 1
			iCountByte		= 0; //4
			strName[0]		= '\0'; //50
			strReserve[0]		= '\0';//92
			fKoefToken		= 1;//8
			iByte2			= 0;//1
	//		m_pByte			= NULL;
			//итого 156 байт один блок
		}
		unsigned char iByte1;//байт проверки на корректность файла
		long iCountByte;
		char strName[50];

		char strReserve[92];

		double fKoefToken;

		unsigned char iByte2;//байт проверки на корректность файла
	//	unsigned char* m_pByte;
	};

	struct STR_ZAG_KALKA
	{
		STR_ZAG_KALKA()
		{
			iByte1			= 245;
			iVersion		= 1;
			iIsForPrint		= 0;

			iTypeSyst		= 0;
			iEllpsType		= 0; //5+3
			
			fBparam			= 0;//16
			
			fLparam			= 0;
			fBparam2		= 0;
			fLparam2		= 0;//40
			
			strReserve[0]	= '\0';//40 + 18 = 58 (округляется до большего кратного 8) = 64
			
			iByte2			= 10; //58 + 1 = 59 округлено до 64 
			
		}
		unsigned char iByte1;//байт проверки на корректность файла
		unsigned char iVersion;

		unsigned char iIsForPrint;//0-not print;1-for print;

		unsigned char iTypeSyst;
		unsigned char iEllpsType;
		
		double fBparam;
		double fLparam;
		double fBparam2;
		double fLparam2;

		
		char strReserve[18];
		
		unsigned char iByte2; //байт проверки на корректность файла
	
	};

		struct MAR_STRUCT 
	{
		MAR_STRUCT()
		{
			id = 0;//номер точки
			checked = false;//триггер посещения точки
			lon = 0;//долгота
			lat = 0;//широта
		}
		int id;//номер точки
		bool checked;//триггер посещения точки
		double lon;//долгота
		double lat;//широта
	};

		struct OBJIN //структура объектов на входе
	{
		OBJIN()
		{
			name = "";
			lon = 0;
			lat = 0;
		}
		QString name;
		double lon;
		double lat;
	};

	struct OBJOUT //структура объектов на выходе
	{
		OBJOUT()
		{
			name = "";
			lon = 0;
			lat = 0;
			rasst = 0;
			time = "000000";

		}
		QString name;
		double lon;
		double lat;
		int rasst;
		QString time;

	};

	struct PHOTOLIST
	{
		PHOTOLIST()
		{
			numb_photo = 0;//номер картинки
			name_group = "";//имя группы
			name_mis = "";//имя миссии
			lat = 0;//широта
			lon = 0;//долгота
			height = 0;//высота
			datetime = "";//дата время
			path = "";//путь к фото
			znak_foto = NULL; //знак фото

		}
		int numb_photo;//номер картинки
		QString name_group;//имя группы
		QString name_mis;//имя миссии
		double lat;//широта
		double lon;//долгота
		int height;//высота
		QString datetime;//дата время
		QString path;//путь к фото
		TknBarrier *znak_foto; //знак фото
	};

		struct GROUP
	{
		GROUP()
		{
			number = 0;
			name = "";
			mis = "";
			lat = 0;
			lon = 0;
			hei = 0;
			dist = 0;
			tom = 0;
			tos = 0;
			avs = 0;
			m_myGroup = NULL;
		}
		int number;//номер сообщения
		QString name;//название устройства
		QString mis;//миссия
		double lat;//широта 
		double lon;//долгота
		double hei;//высота
		SYSTEMTIME tymedate;//дата время
		double dist;//расстояние пройденное группой
		int tom;//время миссии группы
		int tos;//время простоя группы
		double avs;//средняя скорость группы
		TknBarrier *m_myGroup; //знак группы
	};

	struct GPS
	{
	GPS()
	{
		qport = "COM3"; // \\\\.\\ 
		qbsize = "8";
		qbrate = "9600";
		qparity = "0";
		qstopbits = "0";
	}
	QString qport;
	QString qbrate;
	QString qbsize;
	QString qparity;
	QString qstopbits;
	};

	struct SETTINGS
	{
		SETTINGS()
		{
			path_to_track = QCoreApplication::applicationDirPath() + "/data/tracks/" ;//Путь к трекам (пройденным)
			path_to_photo = QCoreApplication::applicationDirPath() + "/data/photo/" ;//Путь к фото
			path_to_video = QCoreApplication::applicationDirPath() + "/data/video/" ;//Путь к фото
			path_to_app = QCoreApplication::applicationDirPath();//Путь к приложению
			path_to_route = QCoreApplication::applicationDirPath() + "/data/route/";//Путь к маршрутам
			path_to_grid = QCoreApplication::applicationDirPath() + "/data/grid/";//Путь к палеткам
			path_to_images = QCoreApplication::applicationDirPath() + "/data/images/" ;//Путь к графическим файлам (спец форматы карты)
			path_to_signs = QCoreApplication::applicationDirPath() + "/data/signs/";
			path_to_marks = QCoreApplication::applicationDirPath() + "/data/marks/";
			pathToMRoute = QCoreApplication::applicationDirPath() + "/data/mroute/";			
			name_of_pc = "ural-tablet-kz";//Имя ПК

			url_to_kcpsIN = "10.1.0.167";	
			//"http://10.1.0.167/svc/TabletDataService.svc";//Урл для подключения к БД ВПУ

			url_to_kcpsOUT = "87.229.137.165";	
			//"http://87.229.137.165/svc/TabletDataService.svc";//Урл для подключения к БД КЦПС
			
			url_to_vpuIN = "10.1.0.167";
			url_to_vpuOUT = "87.229.137.165";

			url_to_shtabIN = "10.1.0.167";
			url_to_shtabOUT = "87.229.137.165";

			url_to_gkcpsIN = "10.1.0.167";
			url_to_gkcpsOUT = "87.229.137.165";

			id_device = "tablet1";//Идентификатор устройства
			version = 1.7;
			time_to_send_data = 30;//Частота отправки данных в КЦПС секунды
			time_zone = 0;
			lastLat = 0.975537;//Последняя записанная с GPS широта
			lastLon = 0.65533;//Последняя записанная с GPS долгота
			scale = 500000;
			defSrv = 0;//сервер по умолчанию 0 - кцпс, 1 - ВПУ 
			trackFormat = 0; // формат треков 0 -tknRoute, 1 - tknSimpleLine
			pathToPso = QCoreApplication::applicationDirPath ()+"\\tmp\\psoBackup\\";
			nameOfCurrentPso = "NULL";
			showTrackLayer = true;
			showRouteLayer = true;
			showMarksPosLayer = true;
			showCurrentPosLayer = true;
			showMRouteLayer = true;
			showGridLayer = true;
			showMegafonLayer = true;
			showBeelineLayer = true;
			showMtsLayer = true;
			showTele2Layer = true;
		}
		QString name_of_pc;//Имя ПК
		QString path_to_track;//Путь к трекам (пройденным)
		QString path_to_photo;//Путь к фото
		QString path_to_video;//Путь к видеозаписям
		QString path_to_app;//Путь к приложению
		QString path_to_route;//Путь к маршрутам
		QString path_to_grid;//Путь к палеткам
		QString path_to_images;//Путь к графическим файлам (спец форматы карты)
		QString path_to_signs;//Путь к приметам
		QString path_to_marks;//Путь к отметкам для карты

		QString url_to_vpuIN;//Урл для подключения к БД ВПУ
		QString url_to_vpuOUT;

		QString url_to_kcpsIN;//Урл для подключения к БД КЦПС
		QString url_to_kcpsOUT;

		QString url_to_shtabIN;
		QString url_to_shtabOUT;

		QString url_to_gkcpsIN;
		QString url_to_gkcpsOUT;

		QString id_device;//Идентификатор устройства
		QString pathToMRoute;
		QString nameOfCurrentPso;
		QString pathToPso;
		short time_to_send_data;//Частота отправки данных в КЦПС
		double lastLat;//Последняя записанная с GPS широта
		double lastLon;//Последняя записанная с GPS долгота
		double scale;
		double version;
		short defSrv;
		short trackFormat;
		int time_zone;//Часовой пояс
		GPS gps_settings;//настройки порта GPS 
		
		bool showTrackLayer;
		bool showRouteLayer;
		bool showMarksPosLayer;
		bool showCurrentPosLayer;
		bool showMRouteLayer;
		bool showGridLayer;
		bool showMegafonLayer;
		bool showBeelineLayer;
		bool showMtsLayer;
		bool showTele2Layer;


	};

	struct POINTS_TO_ARRIVE
	{
		POINTS_TO_ARRIVE()
		{
			lat = 0;
			lon = 0;
			checked = false;
			dist = 0;
		}
		double lat;
		double lon;
		double dist;
		bool checked;

	};
