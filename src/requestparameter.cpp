/******************************************************************************
    Weibo: login, logout and upload api
    Copyright (C) 2012 Wang Bin <wbsecg1@gmail.com>

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


#include "QWeiboAPI/requestparameter.h"
#include <QtDebug>
#include <QtCore/QDir>
#include <QtCore/QFile>
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
#include <QtCore/QUrlQuery>
#endif //QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/QImage>
#include "qput.h"

namespace QWeiboAPI {

Request::Request():
    mEditable(false)
  , mType(Get)
{
}

Request::RequestType Request::type() const
{
    return mType;
}

QString Request::apiUrl() const
{
    if (!mApiUrl.isEmpty())
        return mApiUrl;
    return kApiHost + mApiPath + ".json";
}

QUrl Request::url() const
{
    QUrl url(apiUrl());
    if (!mParameters.isEmpty()) {
        QMap<QString, QVariant>::ConstIterator it = mParameters.constBegin();
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
        QUrlQuery urlqurey;
#endif //QT_VERSION_CHECK(5, 0, 0)
        for (; it != mParameters.constEnd(); ++it) {
            QString value = it.value().toString();
            if (value.isEmpty())
                continue;
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
            urlqurey.addQueryItem(it.key(), value);
#else
            url.addQueryItem(it.key(), value);
#endif //QT_VERSION_CHECK(5, 0, 0)
        }
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
        url.setQuery(urlqurey);
#endif //QT_VERSION_CHECK(5, 0, 0)
    }
    qDebug() << "request url with parameters: " << url.toString();
    return url;
}

Request& Request::prepare()
{
    if (mParameters.isEmpty()) {
        mEditable = true;
        initParameters();
        mEditable = false;
    }
    return *this;
}

QMap<QString, QVariant> Request::paramsters() const
{
    return mParameters;
}

Request& Request::operator ()(const QString& name, const QVariant& value)
{
    if (mEditable || mParameters.contains(name)) {
        qDebug() << name << "==>" << value;
        mParameters[name] = value;
    } else {
        qWarning() << "Can not set the parameter: " << name;
    }
    return *this;
}



LoginRequest::LoginRequest():
    Request()
{
    mType = Post;
    mApiUrl = kOAuthUrl;
}

} //namespace QWeiboAPI
