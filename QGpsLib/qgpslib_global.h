#ifndef QGPSLIB_GLOBAL_H
#define QGPSLIB_GLOBAL_H

#include <QtCore/qglobal.h>

#ifdef QGPSLIB_LIB
# define QGPSLIB_EXPORT Q_DECL_EXPORT
#else
# define QGPSLIB_EXPORT Q_DECL_IMPORT
#endif

#endif // QGPSLIB_GLOBAL_H
