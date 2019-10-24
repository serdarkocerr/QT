#ifndef SINIFCORE_GLOBAL_H
#define SINIFCORE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(SINIFCORE_LIBRARY)
#  define SINIFCORESHARED_EXPORT Q_DECL_EXPORT
#else
#  define SINIFCORESHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // SINIFCORE_GLOBAL_H
