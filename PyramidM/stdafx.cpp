#include "stdafx.h"

#ifdef DEBUG_MEMORY_LEAK
//#include <vld/vld.h>
#endif

#ifdef _DEBUG
	#pragma comment(lib, "GisBase_d.lib")
	#pragma comment(lib, "GisBaseCore_d.lib")
	#pragma comment(lib, "GisDataInterface_d.lib")
	#pragma comment(lib, "GisInterfaceQT_d.lib")
	#pragma comment(lib, "GrUnits_d.lib")
	#pragma comment(lib, "QtExtendUi_d.lib")
	#pragma comment(lib, "GisWidgets_d.lib")
	#pragma comment(lib, "QtImgDev_d.lib")
	#pragma comment(lib, "GdiExtImgDev_d.lib")
  #pragma comment(lib, "libxml2_d.lib")
#else
	#pragma comment(lib, "GisBase.lib")
	#pragma comment(lib, "GisBaseCore.lib")
	#pragma comment(lib, "GisDataInterface.lib")
	#pragma comment(lib, "GisInterfaceQT.lib")
	#pragma comment(lib, "GrUnits.lib")
	#pragma comment(lib, "QtExtendUi.lib")
	#pragma comment(lib, "GisWidgets.lib")
	#pragma comment(lib, "QtImgDev.lib")
	#pragma comment(lib, "GdiExtImgDev.lib")
	#pragma comment(lib, "libxml2.lib")
#endif