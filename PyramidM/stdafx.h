#if !defined(AFX_STDAFX_H_INCLUDED_PiramidM)
#define AFX_STDAFX_H_INCLUDED_PiramidM

#include <QtGui>

#if defined(_WIN32_WCE) || defined(WIN32)
#include <windows.h>
#include <objbase.h>
#include <initguid.h>
#endif

#include <gis/GisBase/cppgis/GisView.h>
#include <gis/GisBase/cppgis/TopoData.h>
#include <gis/GisDataInterface/GisAccess.h>

#if !defined(_WIN32_WCE) && defined(WIN32)
#include <gis/GisBase/GdiImgDev/GdiNative.h>
using namespace GdiNative;
#endif

#include <gis/GisBase/QtImgDev/QtNative.h>
using namespace QtNative;

#if defined(_WIN32_WCE) || defined(WIN32)
#include <gis/GisBase/GdiExtImgDev/GdiExtNative.h>
using namespace GdiExtNative;
#endif

#endif
