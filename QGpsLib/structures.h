#define CCT_PI_180 0.01745329251994329547
#define CCT_180_PI 57.2957795130823208769

//GPGGA структура////////////////////////////
enum GPGGAobserv {NODATA, OBSPOL, OBSDEF }; 
enum GPGGAshirnapr {N,S};
enum GPGGAdolgnapr {E,W};

//$GPGGA, hhmmss.ss, 1111.11, a, yyyyy.yy, a, x, xx, x.x, xxx, M, x.x, M, x.x, xxxx*hh 
//пример: $GPGGA,004241.47,5532.8492,N,03729.0987,E,1,04,2.0 ,-0015,M,,,,*31 
struct GPGGAstruct
{
	GPGGAstruct()
	{
		time = "000000";
		shir = 0.0;
		snapr = N;
		dolg = 0.0;
		dnapr = E;
		observ = NODATA;
		satellite = 0;
		geomfact = 0;
		visota = 0;
		geoidelips = 0;
		difpop = 0;
		iddifst = "";
	}

//Гринвичское время на момент определения местоположения. 
QString time;
//Географическая широта местоположения
double shir;
//Север/Юг (N/S). 
QString snapr;
//Географическая долгота местоположения. 
double dolg;
//Запад/Восток (E/W). 
QString dnapr;
/*
Индикатор качества GPS сигнала: 
0 = Определение местоположения не возможно или не верно; 
1 = GPS режим обычной точности, возможно определение местоположения; 
2 = Дифференциальный GPS режим, точность обычная, возможно определение местоположения; 
*/
GPGGAobserv observ;
//Количество используемых спутников (00-12, может отличаться от числа видимых). 
float satellite;
//Фактор Ухудшения Точности Плановых Координат (HDOP). 
float geomfact;
//Высота антенны приёмника над/ниже уровня моря. 
float visota;
//Единица измерения высоты расположения антенны, метры. (отсутствует поле)
//(отсутствует поле)
//Геоидальное различие - различие между земным эллипсоидом WGS-84 и уровнем моря(геоидом), ”-” = уровень моря ниже эллипсоида. 
float geoidelips;
//Единица измерения различия, метры. 
//(отстутствует поле)
//Возраст Дифференциальных данных GPS - Время в секундах с момента последнего SC104 типа 1 или 9 обновления, заполнено нулями, если дифференциальный режим не используется. 
float difpop;
///Индификатор станции, передающей дифференциальные поправки, ID, 0000-1023.
QString iddifst;
};
////////////////////////////////

//GPRMC структура///////////////
enum GPRMCstatus {A,V};

struct GPRMCstruct
{
	GPRMCstruct()
	{
		time = "000000";
		status = A;
		shir = 0.0;
		snapr = N;
		dolg = 0.0;
		dnapr = E;
		speed = 0;
		napr = 0;
		data = "000000";
		magvar1 = 0;
		magvar2 = "";
	}
//Время фиксации местоположения UTC 
QString time;
//Состояние: А = действительный, V = предупреждение навигационного приёмника 
GPRMCstatus status;
//Географическая широта местоположения, 
double shir;
//Север/Юг широты
QString snapr;
//Географическая долгота местоположения
double dolg;
//Запад/Восток долготы
QString dnapr;
//Скорость над поверхностью (SOG) в узлах (переведено в км)
double speed;
// Истинное направление курса в градусах 
double napr;
//Дата: dd/mm/yy 
QString data;
//Магнитное склонение в градусах 
double magvar1;
//Запад/Восток (E/W)
QString magvar2;

};

//////////////////////////////////////
//настройки порта//////////////
struct portset
{
portset()
{
	qport = "\\\\.\\COM3";
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
////////////////////////////////

