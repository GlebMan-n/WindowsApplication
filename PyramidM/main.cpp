#include "stdafx.h"

#include <QApplication>
#include <QTranslator>
#include <QTextCodec>
#include <QLocale>
#ifndef QT_NO_SPLASHSCREEN
	#include <QSplashScreen>
#endif

#include <float.h>


//#include <vld/vld.h>


#include <gis_tool_qt/QtExtendUi/QtSingleApplication.h>
#include "PiramidM.h"

#include <gis/GrUnits/StoreVectorData.h>
#include <gis/GrUnits/GisTable.h>
#include <gis/GrUnits/TokensClassif.h>

#if !defined(_WIN32_WCE) && defined(WIN32)

GdiExtNative::GdiExtInit  initExt;
GdiNative::GdiPlusInit init;

#endif

int main(int argc, char *argv[])
{
	//Q_INIT_RESOURCE(PiramidM);

#if !defined(_WIN32_WCE) && defined(WIN32)
	if(!init.Good()) // <gisbase> GDI+ не найдена
		return 1;
//	HRESULT rz = CoInitializeEx(0, COINIT_MULTITHREADED);
#endif

#ifndef _WIN32_WCE
	QtSingleApplication a(argc, argv);
	if (a.isRunning()) return 0;
	GisMonitor::init();
#else
	QApplication a(argc, argv);
#endif

	TknMapBase::makeFactory().addPlugin(new GrUnitsTokenPlugin());
	TypeTokenBase::makeFactory().addPlugin(new GrUnitsTypeTokenPlugin());

	GisTable::Cell::makeFactory().addPlugin(new GisTableCellPlugin());
	LabelPosition::makeFactory().addPlugin(new LabelPosPlugin());

	TypeLabelBase::makeFactory().addPlugin(new GisTablePlugin());
	TypeLabelBase::makeFactory().addPlugin(new ClassFormularPlugin());

	QCoreApplication::setOrganizationName("AKPS");
	QCoreApplication::setApplicationName("PyramidM");

#if defined(_WIN32_WCE) || defined(WIN32)
	QTextCodec::setCodecForTr(QTextCodec::codecForName("CP1251"));
#else
	QTextCodec::setCodecForTr(QTextCodec::codecForName("KOI8-R"));
	char *oldLoc = strdup(setlocale (LC_NUMERIC, NULL));
	setlocale (LC_NUMERIC, "C");
#endif

	QLocale::setDefault(QLocale(QLocale::Russian, QLocale::RussianFederation));

	QTranslator trans;
	trans.load("qt_ru.qm", ":/PiramidM/");
	a.installTranslator(&trans);

// 	int status;
// 	status=_controlfp(0,0);
// 	status &= ~EM_ZERODIVIDE;
// 	_controlfp(status,MCW_EM);

#ifndef _WIN32_WCE
	a.processEvents();
#endif

#if !defined(QT_NO_SPLASHSCREEN) && !defined(_DEBUG)
	QPixmap pixmap(":/PiramidM/logo.png");
	QSplashScreen splash(pixmap, Qt::SplashScreen | Qt::WindowStaysOnTopHint);
	splash.setMask(pixmap.mask());
#if defined(WIN32) && !defined(_DEBUG) 
	splash.show();
	PiramidM w(&splash);
#else
	PiramidM w;
#endif
	a.setActivationWindow(&w);
#else
	PiramidM w;
#endif

// #ifdef _WIN32_WCE
	//w.showMaximized();
// #else
// 	w.showNormal();
// #endif

#if defined(_WIN32_WCE) || defined(WIN32)
	return a.exec();
#else
	int rez = a.exec();
	setlocale (LC_NUMERIC, oldLoc);
	free(oldLoc);
	return rez;
#endif
}
