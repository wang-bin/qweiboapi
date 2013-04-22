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

#include "QWeiboAPI/qweiboapi_global.h"
#include <QtCore/QObject>
#include <QtCore/QRegExp>
#include "version.h"

unsigned QWeiboAPI_Version()
{
    return QWEIBOAPI_VERSION;
}

QString QWeiboAPI_Version_String()
{
    return QWEIBOAPI_VERSION_STR;
}

QString QWeiboAPI_Version_String_Long()
{
    return QWEIBOAPI_VERSION_STR_LONG;
}

namespace QWeiboAPI {

QString aboutQWeiboAPI()
{
    return aboutQWeiboAPI_HTML().remove(QRegExp("<[^>]*>"));
}

QString aboutQWeiboAPI_HTML()
{
    static QString about = "<h3>QWeiboAPI " QWEIBOAPI_VERSION_STR_LONG "</h3>\n"
                "<p>" + QObject::tr("Qt wrapper for sina weibo api.\n") + "</p>"
                "<p>" + QObject::tr("Distributed under the terms of LGPLv2.1 or later.\n") + "</p>"
                "<p>Copyright (C) 2013 Wang Bin (aka. Lucas Wang) <a href='mailto:wbsecg1@gmail.com'>wbsecg1@gmail.com</a></p>\n"
                "<p>" + QObject::tr("Shanghai University, Shanghai, China\n") + "</p>"
                "<br>"
                "<p><a href='https://github.com/wang-bin/qweiboapi'>https://github.com/wang-bin/qweiboapi</a></p>";

    return about;
}

}
