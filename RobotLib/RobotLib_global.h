#ifndef ROBOTLIB_GLOBAL_H
#define ROBOTLIB_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(ROBOTLIB_LIBRARY)
#define ROBOTLIB_EXPORT Q_DECL_EXPORT
#else
#define ROBOTLIB_EXPORT Q_DECL_IMPORT
#endif

#endif // ROBOTLIB_GLOBAL_H
