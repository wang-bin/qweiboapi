/******************************************************************************
    Weibo: login, logout and upload api
    Copyright (C) 2012-2014 Wang Bin <wbsecg1@gmail.com>

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


#include "QWeiboAPI/weibo.h"

#include <QtCore/QDir>
#include <QtCore/QFile>
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
#include <QtCore/QUrlQuery>
#endif //QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/QImage>
#include <QtDebug>
#include "qput.h"
#include "QWeiboAPI/requestparameter.h"

namespace QWeiboAPI {

Weibo::Weibo(QObject *parent)
    :QObject(parent)
{
    mPut = new QPut(this);
    //connect(mPut, SIGNAL(ok(QString)), this, SIGNAL(ok()));
    connect(mPut, SIGNAL(fail(QString)), this, SIGNAL(error(QString)));
    connect(mPut, SIGNAL(fail(QString)), this, SLOT(dumpError(QString)));
    connect(mPut, SIGNAL(ok(QString)), this, SIGNAL(ok(QString)));
    connect(mPut, SIGNAL(ok(QString)), this, SLOT(dumpOk(QString)));
    connect(this, SIGNAL(loginOk()), SLOT(processNextRequest()), Qt::DirectConnection);
}

Weibo::~Weibo()
{
    if (!mRequests.isEmpty()) {
        qDeleteAll(mRequests);
        mRequests.clear();
    }
}

void Weibo::setUSer(const QString &user)
{
    mUser = user;
}

void Weibo::setPassword(const QString &passwd)
{
    mPasswd = passwd;
}

void Weibo::setAccessToken(const QByteArray &token)
{
    mAccessToken = token;
}

QByteArray Weibo::accessToken() const
{
    return mAccessToken;
}

void Weibo::createRequest(Request *request)
{
    mRequests.append(request);
    //TODO: better way to check login(from error code)
    if (mAccessToken.isEmpty()) {
        qDebug("Not login.");
        login();
    } else {
        processNextRequest();
    }
}

void Weibo::login()
{
    if (mUser.isEmpty() || mPasswd.isEmpty()) {
        qWarning("user name and password can't be empty");
        return;
    }
    mPut->reset();
    QUrl url(kOAuthUrl);
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
    QUrlQuery urlqurey;
    urlqurey.addQueryItem("client_id", sAppKey);
    urlqurey.addQueryItem("client_secret", sAppSecret);
    urlqurey.addQueryItem("grant_type", "password");
    urlqurey.addQueryItem("username", mUser);
    urlqurey.addQueryItem("password", mPasswd);
    url.setQuery(urlqurey);
#else
    url.addQueryItem("client_id", sAppKey);
    url.addQueryItem("client_secret", sAppSecret);
    url.addQueryItem("grant_type", "password");
    url.addQueryItem("username", mUser);
    url.addQueryItem("password", mPasswd);
#endif //QT_VERSION_CHECK(5, 0, 0)
    connect(mPut, SIGNAL(ok(QString)), SLOT(parseOAuth2ReplyData(QString)));
    connect(mPut, SIGNAL(fail(QString)), SIGNAL(loginFail()));
    mPut->setUrl(url);
    qDebug("begin login...");
    mPut->post();
}

void Weibo::logout()
{

}
//仅支持JPEG、GIF、PNG格式，图片大小小于5M
void Weibo::updateStatusWithPicture(const QString &status, const QString &fileName)
{
    mStatus = status;
    mFile = fileName;
    if (mAccessToken.isEmpty()) {
        qDebug("Not login.");
        connect(this, SIGNAL(loginOk()), SLOT(sendStatusWithPicture()));
        login();
        return;
    }
    sendStatusWithPicture();
}

void Weibo::processNextRequest()
{
    if (mRequests.isEmpty())
        return;
    Request *request = mRequests.takeFirst();
    (*request)
            ("access_token", mAccessToken)
            ("uid", mUid)
            ;
    mPut->reset();
    mPut->setUrl(request->url());
    if (request->type() == Request::Get) {
        mPut->get();
    } else if (request->type() == Request::Post){
        mPut->post();
    }
    delete request;
}

void Weibo::parseOAuth2ReplyData(const QString &data)
{
    static bool in = false;
    if (in)
        return;
    in = true;
    //{"access_token":"2.00xxxxD","remind_in":"4652955","expires_in":4652955,"uid":"12344"}
    QByteArray d(data.toUtf8());
    int i = d.indexOf("access_token");
    int p0 = d.indexOf(":", i) + 2;
    int p1 = d.indexOf("\"", p0);
    mAccessToken = d.mid(p0, p1 - p0);
    i = d.indexOf("uid");
    p0 = d.indexOf(":", i) + 2;
    p1 = d.indexOf("\"", p0);
    mUid = d.mid(p0, p1 - p0);
    qDebug("token=%s, uid=%s", mAccessToken.constData(), mUid.constData());

    disconnect(this, SLOT(parseOAuth2ReplyData(QString)));
    disconnect(this, SIGNAL(loginFail()));
    emit loginOk();
}

void Weibo::sendStatusWithPicture()
{
    qDebug("update weibo with picture");
    QString path(mFile);
    //TODO: gif
    if (!path.endsWith("jpg", Qt::CaseInsensitive)
            && !path.endsWith("jpeg", Qt::CaseInsensitive)
            && !path.endsWith("png", Qt::CaseInsensitive)
            && !path.endsWith("gif", Qt::CaseInsensitive)) {
        QImage image(path);
        path = QDir::tempPath() + "/weibotemp" + ".jpg";
        if (!image.save(path)) {
            qWarning("convert image failed! %s", qPrintable(path));
            return;
        }
    }
    QFile f(path);
    if (!f.open(QIODevice::ReadOnly)) {
        qDebug("open error: %s", qPrintable(f.errorString()));
        return;
    }
    QByteArray data = f.readAll();
    f.close();

    connect(mPut, SIGNAL(ok(QString)), this, SIGNAL(sendOk()));

    mPut->reset();
    QUrl url(kApiHost + "statuses/upload.json");
    mPut->setUrl(url);
    mPut->addTextPart("access_token", mAccessToken);
    mPut->addTextPart("status", QUrl::toPercentEncoding(mStatus));
    mPut->addDataPart("image/jpg", "pic", data, path);
    mPut->upload();
}

void Weibo::dumpError(const QString &error)
{
    qDebug() << ">>>>>>>" << __FUNCTION__ << error  << "<<<<<<<";
}

void Weibo::dumpOk(const QString &data)
{
    qDebug() << ">>>>>>>" << __FUNCTION__ << data << "<<<<<<<";
}

} //namespace QWeiboAPI
