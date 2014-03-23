/******************************************************************************
    QWeiboAPI:  Media play library based on Qt and FFmpeg
    Copyright (C) 2012-2013 Wang Bin <wbsecg1@gmail.com>

*   This file is part of QWeiboAPI

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
******************************************************************************/


#ifndef QWEIBOAPI_GLOBAL_H
#define QWEIBOAPI_GLOBAL_H

#include <qglobal.h>
#include <dptr.h>

#if defined(Q_DLL_LIBRARY)
#  undef QWEIBOAPI_EXPORT
#  define QWEIBOAPI_EXPORT Q_DECL_EXPORT
#else
#  undef QWEIBOAPI_EXPORT
#  define QWEIBOAPI_EXPORT //Q_DECL_IMPORT //only for vc?
#endif

/* runtime version. used to compare with compile time version */
QWEIBOAPI_EXPORT unsigned QWeiboAPI_Version();
QWEIBOAPI_EXPORT QString QWeiboAPI_Version_String();
QWEIBOAPI_EXPORT QString QWeiboAPI_Version_String_Long();

namespace QWeiboAPI {
QWEIBOAPI_EXPORT QString aboutQWeiboAPI();
QWEIBOAPI_EXPORT QString aboutQWeiboAPI_HTML();
}
//TODO: always inline


/* --gnu option of the RVCT compiler also defines __GNUC__ */
#if defined(Q_CC_GNU) && !defined(Q_CC_RVCT)
#define GCC_VERSION_AT_LEAST(major, minor, patch) \
    (__GNUC__ > major || (__GNUC__ == major && (__GNUC_MINOR__ > minor \
    || (__GNUC_MINOR__ == minor && __GNUC_PATCHLEVEL__ >= patch))))
#else
/* Define this for !GCC compilers, just so we can write things like GCC_VERSION_AT_LEAST(4, 1, 0). */
#define GCC_VERSION_AT_LEAST(major, minor, patch) 0
#endif

#endif // QWEIBOAPI_GLOBAL_H

