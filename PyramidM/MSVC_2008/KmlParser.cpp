#include "stdafx.h"
#include <QtGui>
#include "PyramidM\MSVC_2008\KmlParser.h"
#include <PyramidM\MSVC_2008\CalcFunctions.h>
#include <gis\GrUnits\TknMapScale.h>

//шаг отсчета в градусах
short degr_global = 15;
double near_distance_global = 2;
bool square_on = false;

KmlClass::KmlClass(StoreVectorData<FastVectorData>* layer, GdiGis::Color color, QObject* parent, short numbOfThread)
{
	_stopped = true;

	if(parent)
		this->setParent(parent);	
	_layer = layer;

	m_pTypePoly =  new TypePolygonObj();
	GdiGis::Color colorLine =  GdiGis::Color(100,0,0,0);
	m_pTypePoly->setLineType(TypeLinesObj::SOLID_LINE, colorLine, 0.5);
	m_pTypePoly->m_colorFill = color;
	m_pTypePoly->m_fill_type = TypePolygonObj::GLASS_FILL;

	_numbOfThread = numbOfThread;
}

KmlClass::~KmlClass()
{
	this->deleteLater();
}

void KmlClass::run()
{
	_stopped = false;
	
	while(!_stopped)
	{
		//загрузка xml
		try
		{
			_loadPolyFromKmlFile(_path);
		}
		catch(...)
		{
			CalcFunctions::logError("_loadPolyFromKmlFile "+ _path);
			break;
		}
		
		break;
	}

	emit signFinish(_numbOfThread);
}
void KmlClass::setPathToDirKml(QString path)
{
		_path = path;
}

void KmlClass::_loadPolyFromKmlFile(QString path)
{
	emit signStatus(tr("загрузка файла..."),_numbOfThread);
	QDomDocument doc("kmlFile");
	QFile file(path);

	if (!file.open(QIODevice::ReadOnly))
	{
		emit signStatus(tr("не могу найти файл"),_numbOfThread);
		return;
	}

	emit signStatus(tr("загружаю содержимое файла"),_numbOfThread);
	if (!doc.setContent(&file)) 
	{
		file.close();
		emit signStatus(tr("не могу открыть файл"),_numbOfThread);
		return;
	}

	file.close();
	emit signStatus(tr("содержимое файла загружено"),_numbOfThread);
	QString tmp;
	tmp = "";

	//весь файл уровень <kml>
	QDomElement docElem = doc.documentElement();
	tmp += docElem.tagName();
	//получаем корренной элемент уровень <document>
	QDomNode document = docElem.firstChild();
	QDomElement docElem1 = document.toElement();
	tmp += docElem1.tagName();
	//первая запись уровня <document> 
	QDomNode nextDocNode = document.firstChild();
	QDomElement docElem2 = nextDocNode.toElement();
	tmp += docElem2.tagName();
	
	if(docElem1.tagName() == "Folder")
	{
		emit signStatus(tr("многодокументный xml") ,_numbOfThread);
		QDomNode nextFolderNode = document.firstChild();
		while(!nextFolderNode.isNull()) 
		{
			QDomElement nextFolderElem = nextFolderNode.toElement();
			QString tmp;
			tmp = nextFolderElem.tagName();

			if(nextFolderElem.tagName() == "name")
			{									
			}

			if(nextFolderElem.tagName() == "visibility")
			{							
			}

			if(nextFolderElem.tagName() == "open")
			{		
			}

			if(nextFolderElem.tagName() == "Document")
			{
				//разбираем документ
				_docParser(nextFolderElem.firstChild());
			}

			nextFolderNode = nextFolderNode.nextSibling();
		}
	}
	else
	{
		emit signStatus(tr("однодокументный xml"),_numbOfThread);
		_docParser(nextDocNode);
	}
}


void KmlClass::_docParser(QDomNode nextDocNode)
{
	
	emit signStatus(tr("начинаю разбор файла"),_numbOfThread);
	
	while(!nextDocNode.isNull()) 
	{
		//получаем элемент из записи
		QDomElement nextDocElem = nextDocNode.toElement();
		//элемент не пустой
		if(!nextDocElem.isNull()) 
		{

			//ПОЛУЧАЕМ СТИЛИ
			if(nextDocElem.tagName() == "Style")
			{
				
				KmlStyle kmlStyle;
				kmlStyle.id = nextDocElem.attribute("id");
				kmlStyle.threahold_value = nextDocElem.attribute("threshold_value").toDouble();
				QDomNode nextStyleNode = nextDocElem.firstChild();

					
				while(!nextStyleNode.isNull()) 
				{
					QDomElement nextStyleElem = nextStyleNode.toElement();
	
					if(nextStyleElem.tagName() == "LineStyle")
					{
						QDomNode nextLineStyleNode = nextStyleElem.firstChild();
						while(!nextLineStyleNode.isNull()) 
						{
							QDomElement nextLineStyleElem = nextLineStyleNode.toElement();
														
							if(nextLineStyleElem.tagName() == "color")
							{
								QString tmp = nextLineStyleElem.text();
								kmlStyle.lineStyle.visible = tmp.left(2).toInt();
								kmlStyle.lineStyle.color = tmp.right(6);									
							}

							if(nextLineStyleElem.tagName() == "width")
							{
								kmlStyle.lineStyle.width = nextLineStyleElem.text().toInt();								
							}

							nextLineStyleNode = nextLineStyleNode.nextSibling();
						}
					}

					if(nextStyleElem.tagName() == "PolyStyle")
					{
						
						QDomNode nextPolyStyleNode = nextStyleElem.firstChild();
						while(!nextPolyStyleNode.isNull()) 
						{
							QDomElement nextPolyStyleElem = nextPolyStyleNode.toElement();
												
							if(nextPolyStyleElem.tagName() == "color")
							{
								QString tmp = nextPolyStyleElem.text();
								kmlStyle.polygonStyle.visible = tmp.left(2).toInt();
								kmlStyle.polygonStyle.color = tmp.right(6);									
							}

							nextPolyStyleNode = nextPolyStyleNode.nextSibling();
						}
					}

					//следующая запись
					nextStyleNode = nextStyleNode.nextSibling();

				}
				//сохраняем стиль в вектор
				//std::vector <KmlStyle> vectorKmlStyles;
				//vectorKmlStyles.push_back(kmlStyle);
			}
			////////////////СТИЛИ ПОЛУЧИЛИ
			//////////////////////////////
					
					
			//ПОЛУЧАЕМ ОТМЕТКИ (ПОЛИГОНЫ)
			
			if(nextDocElem.tagName() == "Placemark")
			{
				
				QDomNode nextPlacemarkNode = nextDocElem.firstChild();

				//получим имя отметки
				QString placemarkName = nextDocElem.attribute("name");
				//получим имя стиля
				QString styleName = nextDocElem.attribute("styleUrl");
				
				
				//по имени стиля можно определить стиль полигона
				
				while(!nextPlacemarkNode.isNull()) 
				{
					QDomElement nextPlacemarkElem = nextPlacemarkNode.toElement();

					//получаем полигон
					if(nextPlacemarkElem.tagName() == "Polygon")
					{
						QDomNode nexPolygontNode = nextPlacemarkElem.firstChild();
						QString extrude;
						QString tessellate;
						while(!nexPolygontNode.isNull()) 
						{
							QDomElement nextPolygonElem = nexPolygontNode.toElement();


							if(nextPolygonElem.tagName() == "extrude")
								extrude = nextPolygonElem.text();					
							
							if(nextPolygonElem.tagName() == "tessellate")
								tessellate = nextPolygonElem.text();	

							if(nextPolygonElem.tagName() == "outerBoundaryIs")
							{
								QDomNode nextOuterBoundaryIsNode = nextPolygonElem.firstChild();
								while(!nextOuterBoundaryIsNode.isNull()) 
								{
									QDomElement nextOuterBoundaryIsElem = nextOuterBoundaryIsNode.toElement();
									
									if(nextOuterBoundaryIsElem.tagName() == "LinearRing")
									{
										QDomNode nextLinearRingNode = nextOuterBoundaryIsElem.firstChild();
										while(!nextLinearRingNode.isNull()) 
										{
											QDomElement nextLinearRingElem = nextLinearRingNode.toElement();
											if(nextLinearRingElem.tagName() == "coordinates")
											{
												//координаты получены! нарисовать полигон по координатам
												//и дело сделано (стиль можно получить в styleName)
												QString coordinates;
												coordinates = nextLinearRingElem.text();
												_doWorkWithCoordinates(coordinates);
	
											}
											if(_stopped)
												return;
											nextLinearRingNode = nextLinearRingNode.nextSibling();
										}
										
									}
									if(_stopped)
										return;
									nextOuterBoundaryIsNode = nextOuterBoundaryIsNode.nextSibling();
								}
							}
							if(_stopped)
								return;
							nexPolygontNode = nexPolygontNode.nextSibling();
						}
					}
					//следующая запись
					if(_stopped)
						return;
					nextPlacemarkNode = nextPlacemarkNode.nextSibling();
				}
			}
			//ПОЛУЧИЛИ ПОЛИГОНЫ
			/////////////////////////////
		}
		if(_stopped)
			break;
		//вызываем следующий элемент уровня <document>
		nextDocNode = nextDocNode.nextSibling();

	}
	return;
}

void KmlClass::_doWorkWithCoordinates(QString coordinates)
{
	//cоздаем полигон
	TknPolygon* m_pTknPoly;
	m_pTknPoly = new TknPolygon(m_pTypePoly);
	_layer->addObject(m_pTknPoly);

	QTextStream stream(&coordinates);
	 QString line;
	 do 
	 {

		line = stream.readLine();
		Pos pos;
		pos.lat = line.section(',', 1, 1).toDouble();
		pos.lon = line.section(',', 0, 0).toDouble();

		if(_isPosGood(pos))
		{
			m_pTknPoly->add(pos.lat*CCT_PI_180,pos.lon*CCT_PI_180);
		}

	 } 
	 while (!line.isNull());
}

bool KmlClass::_isPosGood(Pos pos)
{
	return (pos.lat > 0 && pos.lat < 90 && pos.lon > 0 && pos.lon < 180);
}

void  KmlClass::clear()
{
	_layer->deleteAll();
}

void KmlClass::stop()
{
	_stopped = true;
}

bool KmlClass::isStopped()
{
	return _stopped;
}

void KmlClass::setNumOfThread(short numbOfThread)
{
	_numbOfThread = numbOfThread;
}


LoadKmlFilesClass::LoadKmlFilesClass(StoreVectorData<FastVectorData>* layer,
									 GdiGis::Color color, 
									 QString dirPath,
									 QObject* parent)
{
	_numOfCore = 0;
	_numOfCore =  QThread::idealThreadCount () - 1;
	if(_numOfCore < 2)
		_numOfCore = 1;
	
	_layer = layer;

	_dirPath = dirPath;
	QDir dir(_dirPath);
	fileList = dir.entryInfoList(QStringList() << "*.kml" , QDir::Files, QDir::Size);
	
	_color = color;
	_loadSettings();
}

LoadKmlFilesClass::~LoadKmlFilesClass()
{
	_saveSettings();
	for(int i = 0; i < klmClasses.size(); i++)
	{
		klmClasses.at(i)->stop();
		while(!klmClasses.at(i)->isFinished())
		{
			MyThread::msleep(5);
		}
		klmClasses.at(i)->~KmlClass();
	}
}

void LoadKmlFilesClass::_saveSettings()
{

}

bool LoadKmlFilesClass::_loadSettings()
{
	return true;
}

void LoadKmlFilesClass::_slotBegin()
{
	_saveSettings();
	//заполняем массив потоков
	short numb = 0;
	if(_numOfCore < fileList.size())
		numb = _numOfCore;
	else
		numb = fileList.size();

	if(numb == 0)
	{ 
		//emit signWorkCompleate();
		return;
	}

	if(klmClasses.size() > 0)
		klmClasses.erase(klmClasses.begin(),klmClasses.end());

	for(int i = 0; i < numb; i++)
	{
		//
		KmlClass* _kmlClass;
		_kmlClass = new KmlClass(_layer,_color,this,i);	
		connect(_kmlClass,SIGNAL(signFinish(short)),SLOT(_initKmlClass(short)));
		connect(_kmlClass,SIGNAL(signStatus(QString,short)),SLOT(_slotSetStatus(QString,short)));
		klmClasses.push_back(_kmlClass);
		//
		_initKmlClass(i);
	}
}

void LoadKmlFilesClass::_initKmlClass(short numbOfThread)
{
	try	
	{
		if(fileList.empty())
		{			
			emit signWorkCompleate();
			return;
		}

		klmClasses.at(numbOfThread)->setPathToDirKml(fileList.first().absoluteFilePath ());
		fileList.removeFirst();
		klmClasses.at(numbOfThread)->start();
	}
	catch(...)
	{
		CalcFunctions::logError("_initKmlClass");
	}
}