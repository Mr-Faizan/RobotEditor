#ifndef ROBOTLIB_GLOBAL_H
#define ROBOTLIB_GLOBAL_H

#if defined(_WIN32) || defined(_WIN64)
  #if defined(ROBOTLIB_LIBRARY)
    #define ROBOTLIB_EXPORT __declspec(dllexport)
  #else
    #define ROBOTLIB_EXPORT __declspec(dllimport)
  #endif
#else
  #define ROBOTLIB_EXPORT
#endif

/*
#include <QtCore/qglobal.h>

#if defined(ROBOTLIB_LIBRARY)
#define ROBOTLIB_EXPORT Q_DECL_EXPORT
#else
#define ROBOTLIB_EXPORT Q_DECL_IMPORT
#endif
*/

#endif // ROBOTLIB_GLOBAL_H
