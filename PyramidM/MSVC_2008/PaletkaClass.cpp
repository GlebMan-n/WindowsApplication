#include "stdafx.h"
#include <MSVC_2008\PaletkaClass.h>
#include <PyramidM\MSVC_2008\TknPaletka.h>
#include <gis_tool_qt\GisInterfaceQT\MapBaseWidget.h>
#include <QMainWindow>

PaletkaClass::PaletkaClass(QWidget *parent, MapBaseWidget* map, StoreVectorData<FastVectorData>** gridLayer)
{
	this->setParent(parent);
	_tknPaletka = NULL;
	_map = map;
	_gridLayer = *gridLayer;
}

PaletkaClass::~PaletkaClass()
{

}
//четвертый шаг
//отрисовать палетку на карте
bool PaletkaClass::drawPaletkaOnMap(TknPaletka* tknPal,MapBaseWidget *m_mapWidget, StoreVectorData<FastVectorData>* m_pLayer,bool clearLayer)
{
	if (!tknPal)
		return false;

	if(!m_pLayer) 
	{
		return false;
	}
		
	if(clearLayer)
		m_pLayer->deleteAll();

	m_pLayer->addObject(tknPal);
	m_mapWidget->redrawFull();
	return true;
}

//это второй шаг
bool PaletkaClass::drawPaletkaOnMap(QString path, MapBaseWidget *m_mapWidget,StoreVectorData<FastVectorData>** m_pLayer,bool clearLayer)
{
	//проверяет все ли в порядке с палеткой по указанному пути
	//загружает ее и сохраняет в TknPaletka* _tknPaletka; (объявлена как private)
	if(_palFromFile(path))
	{
		//если все в порядке рисует добавляет палетку в слой
		return drawPaletkaOnMap(_tknPaletka,m_mapWidget,*m_pLayer,clearLayer);
	}
	return false;
}

QString PaletkaClass::renamePal(QString pathDir)
{
	QString path = pathDir;
	QString name;
	QFile file(path);
	if(!file.open(QIODevice::ReadOnly))
	{
		qDebug() << tr("файла нет");
		return QString("null");
	}

	int size = file.size ();
	file.close();

	FILE* stream2 = NULL;
	QByteArray ar = path.toLocal8Bit();
	char* path_char=ar.data();

	if( (stream2 = fopen( path_char , "rb" )) != NULL )
	{
		char* m_pBYTE=new char[size];

		long f = fread(m_pBYTE, sizeof(char), size, stream2);

		if(f<=0) return false;

		std::istrstream pFile((const char*)m_pBYTE, size);

		TknPaletka* tknPaletka;
		tknPaletka = NULL;
		tknPaletka = new TknPaletka();


		bool bRet =  tknPaletka->loadFromBlob(pFile);

		if(! bRet)	
		{
			qDebug() << tr("ошибка загрузки");
			delete [] m_pBYTE;
			return QString("null");
		}
		name = tr(tknPaletka->strNameTkn);

		delete tknPaletka;
		tknPaletka = NULL;
		delete [] m_pBYTE;
	}
	fclose(stream2);
	

	QFile newFile(path);
	if(!newFile.open(QIODevice::ReadWrite))
	{
		qDebug() << tr("файла нет");
		return QString("null");
	}
	QString neTrackName;
	neTrackName = _pathToGrid + "\\" + name +".pal";
	QFile testFile(neTrackName);
	//убиваем файл если уже был с таким именем
	if(testFile.exists ())
	{
		testFile.remove ();
		testFile.close();
	}
	//переименуем временный файл в имя самой палетки
	newFile.rename(neTrackName);
	newFile.close();
	return neTrackName;
}
//3
//получить палетку по пути
bool PaletkaClass::_palFromFile(QString path)
{
	QFile file(path);
	if(!file.open(QIODevice::ReadOnly))
	{
		qDebug() << tr("файла нет");
		return false;
	}
	int size = file.size ();
	file.close();

	FILE* stream2 = NULL;
	QByteArray ar = path.toLocal8Bit();
	char* path_char=ar.data();

	if( (stream2 = fopen( path_char , "rb" )) != NULL )
	{
		char* m_pBYTE=new char[size];

		long f = fread(m_pBYTE, sizeof(char), size, stream2);

		if(f<=0) return false;

		std::istrstream pFile((const char*)m_pBYTE, size);


		delete _tknPaletka;
		_tknPaletka = NULL;
		_tknPaletka = new TknPaletka();

		bool bRet =  _tknPaletka->loadFromBlob(pFile);
		if(!bRet)	
		{
			qDebug() << tr("ошибка загрузки");
			delete [] m_pBYTE;
			return false;
		}
		delete [] m_pBYTE;
	}
	fclose(stream2);
	return true;
}

void PaletkaClass::_slotGridLoaded(bool loaded)
{
	if(loaded)
	{
		if(!_loadSettings())
			return;
		QString pathToGrid(renamePal(QCoreApplication::applicationDirPath ()+"/tmp/tmp.pal"));
		emit signPathToGrid(pathToGrid);
		

	}
	else
		emit signPathToGrid(tr("null"));
}

bool PaletkaClass::_loadSettings()
{
	QString path_to_ini = QCoreApplication::applicationDirPath () + "/settings_akps.ini";
	if (QFile::exists(path_to_ini) ) 
	{
		QSettings akps_settings (path_to_ini, QSettings::IniFormat);
		_pathToGrid = akps_settings.value("programm/path_to_grid").toString();		
		return true;
	}
	return false;
}
//Путь к палетке падает сюда это первый шаг

//предварительно слой подгатавливается так
/*	PosSystem *system = pos_system_new(possys_geodetic);
	*layer = new StoreVectorData<FastVectorData>(system);
	CctError* error = _map->getGisView()->addGeoData(*layer, false);
	if(! cct_error_success( error ) )
	{
		cct_error_free(error);
		delete *layer;
		*layer = NULL;
		return false;
	}
	pos_system_free(system);
	return true;*/
//и передается в конструктор этого класса
//этот класс и работает с этим слоем добавляя в него палетки
void PaletkaClass::_slotDrawGrid(QString path)
{
	if(_gridLayer)
		drawPaletkaOnMap(path,_map,&_gridLayer,true);
}

void PaletkaClass::drawGrid(QString path)
{
	if(_gridLayer)
		drawPaletkaOnMap(path,_map,&_gridLayer,true);
}