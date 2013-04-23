/******************************************************************************
    QWeiboAPI:  Qt wrapper for sina weibo api
    Copyright (C) 2013 Wang Bin <wbsecg1@gmail.com>

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

#ifndef QWEIBOAPI_VERSION_H
#define QWEIBOAPI_VERSION_H

#define QWEIBOAPI_MAJOR 0    //((QWEIBOAPI_VERSION&0xff0000)>>16)
#define QWEIBOAPI_MINOR 0    //((QWEIBOAPI_VERSION&0xff00)>>8)
#define QWEIBOAPI_PATCH 0    //(QWEIBOAPI_VERSION&0xff)


#define QWEIBOAPI_VERSION_MAJOR(V) ((V & 0xff0000) >> 16)
#define QWEIBOAPI_VERSION_MINOR(V) ((V & 0xff00) >> 8)
#define QWEIBOAPI_VERSION_PATCH(V) (V & 0xff)

#define QWEIBOAPI_VERSION_CHK(major, minor, patch) \
    (((major&0xff)<<16) | ((minor&0xff)<<8) | (patch&0xff))

#define QWEIBOAPI_VERSION QWEIBOAPI_VERSION_CHK(QWEIBOAPI_MAJOR, QWEIBOAPI_MINOR, QWEIBOAPI_PATCH)

/*! Stringify \a x. */
#define _TOSTR(x)   #x
/*! Stringify \a x, perform macro expansion. */
#define TOSTR(x)  _TOSTR(x)


/* the following are compile time version */
/* C++11 requires a space between literal and identifier */
static const char* const qtav_version_string = TOSTR(QWEIBOAPI_MAJOR) "." TOSTR(QWEIBOAPI_MINOR) "." TOSTR(QWEIBOAPI_PATCH) "(" __DATE__ ", " __TIME__ ")";
#define QWEIBOAPI_VERSION_STR        TOSTR(QWEIBOAPI_MAJOR) "." TOSTR(QWEIBOAPI_MINOR) "." TOSTR(QWEIBOAPI_PATCH)
#define QWEIBOAPI_VERSION_STR_LONG    QWEIBOAPI_VERSION_STR "(" __DATE__ ", " __TIME__ ")"

#endif // QWEIBOAPI_VERSION_H
