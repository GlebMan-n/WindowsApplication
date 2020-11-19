#define CCT_PI_180 0.01745329251994329547
#define CCT_180_PI 57.2957795130823208769

//GPGGA ���������////////////////////////////
enum GPGGAobserv {NODATA, OBSPOL, OBSDEF }; 
enum GPGGAshirnapr {N,S};
enum GPGGAdolgnapr {E,W};

//$GPGGA, hhmmss.ss, 1111.11, a, yyyyy.yy, a, x, xx, x.x, xxx, M, x.x, M, x.x, xxxx*hh 
//������: $GPGGA,004241.47,5532.8492,N,03729.0987,E,1,04,2.0 ,-0015,M,,,,*31 
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

//����������� ����� �� ������ ����������� ��������������. 
QString time;
//�������������� ������ ��������������
double shir;
//�����/�� (N/S). 
QString snapr;
//�������������� ������� ��������������. 
double dolg;
//�����/������ (E/W). 
QString dnapr;
/*
��������� �������� GPS �������: 
0 = ����������� �������������� �� �������� ��� �� �����; 
1 = GPS ����� ������� ��������, �������� ����������� ��������������; 
2 = ���������������� GPS �����, �������� �������, �������� ����������� ��������������; 
*/
GPGGAobserv observ;
//���������� ������������ ��������� (00-12, ����� ���������� �� ����� �������). 
float satellite;
//������ ��������� �������� �������� ��������� (HDOP). 
float geomfact;
//������ ������� �������� ���/���� ������ ����. 
float visota;
//������� ��������� ������ ������������ �������, �����. (����������� ����)
//(����������� ����)
//����������� �������� - �������� ����� ������ ����������� WGS-84 � ������� ����(�������), �-� = ������� ���� ���� ����������. 
float geoidelips;
//������� ��������� ��������, �����. 
//(������������ ����)
//������� ���������������� ������ GPS - ����� � �������� � ������� ���������� SC104 ���� 1 ��� 9 ����������, ��������� ������, ���� ���������������� ����� �� ������������. 
float difpop;
///����������� �������, ���������� ���������������� ��������, ID, 0000-1023.
QString iddifst;
};
////////////////////////////////

//GPRMC ���������///////////////
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
//����� �������� �������������� UTC 
QString time;
//���������: � = ��������������, V = �������������� �������������� �������� 
GPRMCstatus status;
//�������������� ������ ��������������, 
double shir;
//�����/�� ������
QString snapr;
//�������������� ������� ��������������
double dolg;
//�����/������ �������
QString dnapr;
//�������� ��� ������������ (SOG) � ����� (���������� � ��)
double speed;
// �������� ����������� ����� � �������� 
double napr;
//����: dd/mm/yy 
QString data;
//��������� ��������� � �������� 
double magvar1;
//�����/������ (E/W)
QString magvar2;

};

//////////////////////////////////////
//��������� �����//////////////
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

