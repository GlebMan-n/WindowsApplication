#include "stdafx.h"
#include <QtGui>
#include <QtDebug>
#include "PiramidM.h"
#include <gis_tool_qt/GisInterfaceQT/QToolBarMap.h>
#include <gis_tool_qt/GisInterfaceQT/QToolBarScale.h>
#include <gis_tool_qt/GisInterfaceQT/QToolBarRegion.h>
#include <gis_tool_qt/GisInterfaceQT/QToolBarRoseWind.h>
#include <gis_tool_qt/GisInterfaceQT/MapStatusBar.h>
#include <gis_tool_qt/GisInterfaceQT/PropertyDlg.h>
#include <gis_tool_qt/GisInterfaceQT/MapBaseWidget.h>
#include <gis_tool_qt/GisInterfaceQT/GisMapQtInterface.h>
#include <gis_tool_qt/GisInterfaceQT/QSettingsUtf8.h>
#include <gis_tool_qt/GisWidgets/GisAccessDlg.h>
#include <gis/GrUnits/TknMapNonScale.h>
#include <gis/GrUnits/TknTerminator.h>
#include "gis/GisDataInterface/ControlSelector.h"
#include <pyramidmediator.h>
#include <PyramidM\MSVC_2008\ExchangeDataClass.h>
#include <PyramidM\MSVC_2008\TknPaletka.h>
#include <PyramidM\MSVC_2008\TknRoute.h>

PiramidM::PiramidM(
#ifndef QT_NO_SPLASHSCREEN
	QSplashScreen *splash, 
#endif
	QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	setIconSize(QSize(16,16));
	setWindowIcon(QIcon(":/PiramidM/Pyramid.ico"));
	QSettings::Format format = QSettings::IniFormat;

	m_factory = new QtFactory();

	if(format == QSettings::NativeFormat)
		m_settings = new QSettingsUtf8();
	else if(format == QSettings::IniFormat)
	{
		QString nameIni = QCoreApplication::applicationName();
		if(nameIni.isEmpty())	nameIni = tr("PyramidSettings");
		QString pathIni = QCoreApplication::applicationDirPath();
		pathIni = pathIni + QDir::separator() + nameIni + QString(".ini");  
		m_settings = new QSettingsUtf8(pathIni, QSettings::IniFormat);
	}

	m_mapWidget = NULL;
	mediator = NULL;
	m_gisTool = NULL;
	m_splashScreen = NULL;
	gridLayer = NULL;
#ifndef QT_NO_SPLASHSCREEN
	m_splashScreen = splash;
#endif

	m_timer = new QTimer(this);
	connect(m_timer, SIGNAL( timeout ()), 
		this, SLOT (init ()));
	m_timer->start (0);
	

	////test
	//
	//m_timer1 = new QTimer(this);
	//connect(m_timer1, SIGNAL( timeout ()), 
	//	this, SLOT (_slotRedrawMap ()));
	//m_timer1->start (10);
}

PiramidM::~PiramidM()
{
	//if(gridLayer)
	//{
	//	gridLayer->deleteAll();
	//	delete gridLayer;
	//	gridLayer = NULL;
	//}

	if(m_settings)
	{
		m_mapWidget->saveSettings(m_settings);
			delete m_settings;
	}
	delete m_factory;

	xmlCleanupCharEncodingHandlers();
	xmlCleanupParser();


}
void PiramidM::_slotInitMapWidget()
{
	bool ret = true;
	do
	{
		ret = m_mapWidget->initGisAccess();
		if(!ret)
		{
			int ret = QMessageBox::warning(this, tr("Ошибка !!!"),
				tr("Не найдены настройки на картографическую базу данных.\n"
				"Выйти из приложения?"),
				QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Ok);
			if(ret == QMessageBox::Ok)
			{	
				QCoreApplication::exit();
				return ;
			}

			GisAccessDlg wgt(this);
			wgt.exec();
		}
	} while (!ret);

	#if !defined(_WIN32_WCE) && defined(WIN32)
	if(!m_mapWidget->initTopoFormat(UNN))
	{
		QMessageBox::warning (this, "GIS-Base", 
		tr("Ошибка инициализации топофона UNN"));
		return ;
	}
#else
	if(!m_mapWidget->initTopoFormat(UNM))
	{
		QMessageBox::warning (this, "GIS-Base", 
		tr("Ошибка инициализации топофона UNM"));
		return false;
	}
#endif

	if(!m_mapWidget->initInterfaceManager())
	{
		QMessageBox::warning (this, "GIS-Base", 
			tr("Ошибка инициализации менеджера топофона"));
		return ;
	}
	setCentralWidget(m_mapWidget);
	//m_mapWidget->getGisView()->setScaleRange(1000, 40000000);
	//m_mapWidget->setK_ScaleMin(0.01);
	
	m_mapWidget->getGisView()->generalOn();
	m_mapWidget->lockControlSelector()->fixedMap(false);
	m_mapWidget->unlockControlSelector();
	m_mapWidget->getMemBuffer()->SetSmoothingMode(SmoothingMode::SmoothingModeAntiAlias);
	if(m_gisTool)
	{
		delete m_gisTool;
		m_gisTool = NULL;
	}
	m_gisTool = new GisViewWidget::GisMoveInterface(m_mapWidget);
	m_mapWidget->addGisTool(m_gisTool);
}
bool PiramidM::initMapWidget ()
{
	if(m_mapWidget)
	{
		delete m_mapWidget;
		m_mapWidget = NULL;
	}
	m_mapWidget = new MapBaseWidget(this);
	m_mapWidget->setFactory(m_factory);
	
	bool ret = true;
	do
	{
		ret = m_mapWidget->initGisAccess();
		if(!ret)
		{
			if(m_splashScreen)
			{
				m_splashScreen->finish(this);
			}

			int ret = QMessageBox::warning(this, tr("Ошибка !!!"),
				tr("Не найдены настройки на картографическую базу данных.\n"
				"Выйти из приложения?"),
				QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Ok);
			if(ret == QMessageBox::Ok)
			{	
				QCoreApplication::exit();
				return false;
			}

			GisAccessDlg wgt(this);
			wgt.exec();
		}
	} while (!ret);

#if !defined(_WIN32_WCE) && defined(WIN32)
	if(!m_mapWidget->initTopoFormat(UNN))
	{
		#ifndef QT_NO_SPLASHSCREEN
		if(m_splashScreen)
		{
			m_splashScreen->finish(this);
		}
		#endif

		QMessageBox::warning (this, "GIS-Base", 
		tr("Ошибка инициализации топофона UNN"));
		return false;
	}
#else
	if(!m_mapWidget->initTopoFormat(UNM))
	{
		QMessageBox::warning (this, "GIS-Base", 
		tr("Ошибка инициализации топофона UNM"));
		return false;
	}
#endif

	if(!m_mapWidget->initInterfaceManager())
	{

		#ifndef QT_NO_SPLASHSCREEN
		if(m_splashScreen)
		{
			m_splashScreen->finish(this);
		}
		#endif

		QMessageBox::warning (this, "GIS-Base", 
			tr("Ошибка инициализации менеджера топофона"));
		return false;
	}
	#ifndef QT_NO_SPLASHSCREEN
	if(m_splashScreen)
	{
		m_splashScreen->finish(this);
	}
	#endif
	setCentralWidget(m_mapWidget);
	m_mapWidget->getGisView()->setScaleRange(1000, 40000000);
	m_mapWidget->setK_ScaleMin(0.01);
	m_mapWidget->getGisView()->generalOn();
	m_mapWidget->lockControlSelector()->fixedMap(false);
	m_mapWidget->unlockControlSelector();
	m_mapWidget->getMemBuffer()->SetSmoothingMode(SmoothingMode::SmoothingModeAntiAlias);

	if(m_gisTool)
	{
		delete m_gisTool;
		m_gisTool = NULL;
	}

	m_gisTool = new GisViewWidget::GisMoveInterface(m_mapWidget);
	m_mapWidget->addGisTool(m_gisTool);


	return true;
}

bool PiramidM::connectGisBase()
{
	PropertyDlg dlg(this);
	if(dlg.exec() == QDialog::Rejected)
		return false;

	return true;
}

void PiramidM::init()
{
	if(m_timer)
	{
		m_timer->stop();
		while(m_timer->isActive())
		{}
		delete m_timer;
		m_timer = NULL;

	}

#ifndef QT_NO_SPLASHSCREEN
	if(m_splashScreen)
	{
		m_splashScreen->showMessage (tr("Инициализация приложения..."),
			Qt::AlignHCenter | Qt::AlignBottom);
	}
#endif
	
	if(!initMapWidget ())
	{
		#ifndef QT_NO_SPLASHSCREEN
		if(m_splashScreen)
		{
			m_splashScreen->finish(this);
		}
		#endif

		QMessageBox::critical (this, tr("Внимание!"),
			tr("Ошибка инициализации карты!\nПриложение будет закрыто!"));

		close();
		return;
	}

#ifndef QT_NO_SPLASHSCREEN
	if(m_splashScreen)
	{
		m_splashScreen->showMessage (tr("Завершение инициализации..."),
			Qt::AlignHCenter | Qt::AlignBottom);
	}
#endif
 
	m_mapWidget->loadSettings(m_settings);

#ifndef QT_NO_SPLASHSCREEN
	if(m_splashScreen)
		m_splashScreen->finish(this);
#endif
	//this->show();
	//m_mapWidget->getGisView()->setScale(8000000);
	//drawRoute();
	//return;

	if(mediator)
	{
		mediator->deleteLater();
		mediator = NULL;
	}
	mediator = new PyramidMediator(this,m_mapWidget);
	connect(m_mapWidget,SIGNAL(signUpdateRegion()),mediator,SLOT(_slotUpdateScale()));
	connect(mediator,SIGNAL(signInitMapWidget()),this,SLOT(_slotInitMapWidget()));
	connect(this,SIGNAL(signOk()),mediator,SIGNAL(signOk()));
	connect(this,SIGNAL(signMenu()),mediator,SIGNAL(signMenu()));
	connect(this,SIGNAL(signF1()),mediator,SIGNAL(signF1()));
	connect(this,SIGNAL(signF2()),mediator,SIGNAL(signF2()));
	connect(this,SIGNAL(signF3()),mediator,SIGNAL(signF3()));
	connect(this,SIGNAL(signKeyUp()),mediator,SIGNAL(signKeyUp()));
	connect(this,SIGNAL(signKeyDown()),mediator,SIGNAL(signKeyDown()));
	connect(this,SIGNAL(signKeyLeft()),mediator,SIGNAL(signKeyLeft()));
	connect(this,SIGNAL(signKeyRight()),mediator,SIGNAL(signKeyRight()));
	mediator->start();
	mediator->setFactory(m_factory);
	
	if(!m_mapWidget->isSmooth())
		m_mapWidget->setSmooth(true);
	

	//if(m_mapWidget->getGisView()->isGeneralOn())
	//	m_mapWidget->getGisView()->generalOff();

	m_mapWidget->getGisView()->generalOn();
	
}

MapBaseWidget* PiramidM::getMap()
{
	return m_mapWidget;
}
//
void PiramidM::_slotRedrawMap()
{
	if(test)
	{
		test = !test;
		double _testLat = 0.975481896882307;
		double _testLon = 0.655307802151887;
		PosBL posBL = {_testLat, _testLon};
		m_mapWidget->move2Point(posBL);
		//m_mapWidget->getGisView()->move2point(posBL);
		m_mapWidget->redrawDynamic();
		m_mapWidget->redrawFull();
	}
	else
	{
		test = !test;
		double _testLat = 0.955481896882307;
		double _testLon = 0.685307802151887;
		PosBL posBL = {_testLat, _testLon};
		m_mapWidget->move2Point(posBL);
		m_mapWidget->redrawFull();
		m_mapWidget->redrawDynamic();
	}
}
bool PiramidM::event(QEvent *event)
{
	if (event->type() == QEvent::KeyRelease) 
	{		
		//отпущена клавиша
		//получаем событие нажатие кнопки
		QKeyEvent *keyEvent = (QKeyEvent *)event;
		 switch(keyEvent->key()) 
		 {

			 ///////////
			 case Qt::Key_9:

				 if(keyEvent->modifiers() & Qt::CTRL && Qt::SHIFT && Qt::Key_Menu ) 
			   {
					//было нажато сочетание Ctrl + S

				  emit signF1();
			   }
			 break;
///////////////
				case Qt::Key_3:

				//f2 dev key
			 	if(keyEvent->modifiers() & Qt::CTRL && Qt::SHIFT && Qt::Key_Menu ) 
			   {
					//было нажато сочетание Ctrl + S

				  emit signF2();
			   }
				break;
		////////////
			 case Qt::Key_5:
			//f3 dev key

			 	if(keyEvent->modifiers() & Qt::CTRL && Qt::SHIFT && Qt::Key_Menu ) 
			   {
					//было нажато сочетание Ctrl + S


				  emit signF3();
			   }
				break;
			///////////////

			 case Qt::Key_8:
			//f3 dev key

			 	if(keyEvent->modifiers() & Qt::CTRL && Qt::SHIFT && Qt::Key_Menu ) 
			   {
					//было нажато сочетание Ctrl + S

				  emit signMenu();
			   }
				break;
		///////////////
		 case Qt::Key_Up:

			 emit signKeyUp();
		break;
		/////////////
		 case Qt::Key_Down:

			 emit signKeyDown();
		break;	
		//////////
		 case Qt::Key_Left:

			 emit signKeyLeft();
		break;
		////////////
		 case Qt::Key_Right:

			 emit signKeyRight();
		break;
		////////////
		 case Qt::Key_Return:

					//было нажато сочетание Ctrl + S

			  emit signOk();

		break;
		//////////////
              default:
				  QWidget::keyPressEvent(keyEvent); //иначе передаем событие дальше

        }
	}
	return QWidget::event(event); 
}
//
//void PiramidM::keyPressEvent(QKeyEvent *event) 
//{
//	////////////////////
//         switch(event->key()) 
//		 {
//
//			 ///////////
//			 case Qt::Key_5:
//
//				 if(event->modifiers() & Qt::CTRL && Qt::SHIFT && Qt::Key_Menu ) 
//			   {
//					//было нажато сочетание Ctrl + S
//
//				  emit signF1();
//			   }
//			 break;
/////////////////
//				case Qt::Key_3:
//
//				//f2 dev key
//			 	if(event->modifiers() & Qt::CTRL && Qt::SHIFT && Qt::Key_Menu ) 
//			   {
//					//было нажато сочетание Ctrl + S
//
//				  emit signF2();
//			   }
//				break;
//		////////////
//			 case Qt::Key_9:
//			//f3 dev key
//
//			 	if(event->modifiers() & Qt::CTRL && Qt::SHIFT && Qt::Key_Menu ) 
//			   {
//					//было нажато сочетание Ctrl + S
//
//
//				  emit signF3();
//			   }
//				break;
//			///////////////
//
//			 case Qt::Key_4:
//			//f3 dev key
//
//			 	if(event->modifiers() & Qt::CTRL && Qt::SHIFT && Qt::Key_Menu ) 
//			   {
//					//было нажато сочетание Ctrl + S
//
//				  emit signMenu();
//			   }
//				break;
//		///////////////
//		 case Qt::Key_Up:
//
//			 emit signKeyUp();
//		break;
//		/////////////
//		 case Qt::Key_Down:
//
//			 emit signKeyDown();
//		break;	
//		//////////
//		 case Qt::Key_Left:
//
//			 emit signKeyLeft();
//		break;
//		////////////
//		 case Qt::Key_Right:
//
//			 emit signKeyRight();
//		break;
//		////////////
//		 case Qt::Key_Return:
//
//					//было нажато сочетание Ctrl + S
//
//			  emit signOk();
//
//		break;
//		//////////////
//              default:
//				  QWidget::keyPressEvent(event); //иначе передаем событие дальше
//        }
//}

//

void PiramidM::drawPal()
{

	bool in = InitLayerForKalka(&gridLayer);
	if(!in)
		return;

	QString path = tr("C:\\temp\\pso.pal");
	QString name;
	QFile file(path);
	if(!file.open(QIODevice::ReadOnly))
	{
		qDebug() << tr("файла нет");
		return ;
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

		if(f<=0) return ;

		std::istrstream pFile((const char*)m_pBYTE, size);

		TknPaletka* tknPaletka;
		tknPaletka = NULL;
		tknPaletka = new TknPaletka();


		bool bRet =  tknPaletka->loadFromBlob(pFile);

		if(! bRet)	
		{
			qDebug() << tr("ошибка загрузки");
			delete [] m_pBYTE;
			return;
		}
		gridLayer->addObject(tknPaletka);


		//delete tknPaletka;
		//tknPaletka = NULL;
		delete [] m_pBYTE;
	}
	fclose(stream2);
}


void PiramidM::drawRoute()
{

	bool in = InitLayerForKalka(&gridLayer);
	if(!in)
		return;

	QString path = tr("C:\\temp\\pso.route");
	QString name;
	QFile file(path);
	if(!file.open(QIODevice::ReadOnly))
	{
		qDebug() << tr("файла нет");
		return ;
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

		if(f<=0) return ;

		std::istrstream pFile((const char*)m_pBYTE, size);

		TknRoute* tknRoute;
		tknRoute = NULL;
		tknRoute = new TknRoute();


		bool bRet =  tknRoute->loadFromBlob(pFile);

		if(! bRet)	
		{
			qDebug() << tr("ошибка загрузки");
			delete [] m_pBYTE;
			return;
		}
		gridLayer->addObject(tknRoute);
		delete [] m_pBYTE;
	}
	fclose(stream2);
}

bool PiramidM::InitLayerForKalka(StoreVectorData<FastVectorData>** layer)
{
	PosSystem *system = pos_system_new(possys_geodetic);
	*layer = new StoreVectorData<FastVectorData>(system);
	CctError* error = m_mapWidget->getGisView()->addGeoData(*layer, true);
	if(! cct_error_success( error ) )
	{
		cct_error_free(error);
		delete *layer;
		*layer = NULL;
		return false;
	}
	pos_system_free(system);
	return true;
}

void PiramidM::drawEllips()
{

}

void PiramidM::drawText()
{

}

void PiramidM::drawLine()
{

}

bool PiramidM::routeToFile(TknRoute* tknRoute,QString path)
{
	std::ostrstream pFile;

	bool bSave=tknRoute->saveToBlob(pFile);//сохраняем знак в поток
	
	if(!bSave)  
		return false;

	long iLen=pFile.pcount(); //длинна знака
	if(iLen<1) 
		return false;
	

	BYTE* m_pChar =(BYTE*)pFile.str(); //записываем в байты
	FILE* stream = NULL;
	std::string path_to_file( path.toLocal8Bit().constData(), path.toLocal8Bit().length());
	stream = fopen(path_to_file.c_str() , "wb" );
	long f = fwrite(m_pChar, sizeof(char),  iLen, stream); 
	fclose(stream);
	return true;
}