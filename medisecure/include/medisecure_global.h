#ifndef MEDISECURE_GLOBAL_H
#define MEDISECURE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(MEDISECURE_LIBRARY)
#  define MEDISECURESHARED_EXPORT Q_DECL_EXPORT
#else
#  define MEDISECURESHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // MEDISECURE_GLOBAL_H