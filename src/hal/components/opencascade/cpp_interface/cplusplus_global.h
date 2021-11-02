#ifndef NEXT_GLOBAL_H
#define NEXT_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(NEXT_LIBRARY)
#  define NEXT_EXPORT Q_DECL_EXPORT
#else
#  define NEXT_EXPORT Q_DECL_IMPORT
#endif

#endif // NEXT_GLOBAL_H
