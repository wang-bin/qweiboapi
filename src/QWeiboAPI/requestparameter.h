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

#ifndef QWEIBOAPI_REQUESTPARAMETER_H
#define QWEIBOAPI_REQUESTPARAMETER_H

#include "QWeiboAPI/qweiboapi_global.h"
#include <QtCore/QMap>
#include <QtCore/QVariant>
namespace QWeiboAPI {

static const QString kOAuthUrl = "https://api.weibo.com/oauth2/access_token";
static const QString kApiHost = "https://api.weibo.com/2/";
//appkey, appsecret are weico for iOS
static QString sAppKey = "82966982";
static QString sAppSecret = "72d4545a28a46a6f329c4f2b1e949e6a";

class Q_EXPORT Request
{
public:
    enum RequestType {
        Get, Post
    };
    Request();
    virtual ~Request() {}
    RequestType type() const;
    QString apiUrl() const;
    QUrl url() const; //apiUrl() + parameters
    Request& prepare();
    //only the existing (name, value) will be modified, otherwise do nothing
    Request& operator ()(const QString& name, const QVariant& value);
    void addImage(const QString& file);
    void addImage(const QByteArray& data, const QString& format);

    QMap<QString, QVariant> paramsters() const;
protected:
    virtual void initParameters() {qDebug("Request::initParameters() !!!");}

    bool mEditable; //true in ctor
    RequestType mType;
    QString mApiUrl;
    QMap<QString, QVariant> mParameters;
};

#define REQUEST_API_BEGIN(Class) \
    class Q_EXPORT Class : public Request \
    { \
    public: \
        Class(); \
    protected: \
        void initParameters() { \
            (*this)

#define REQUEST_API_END() \
    ; \
  } \
};


REQUEST_API_BEGIN(LoginRequest)
        ("client_id", "sAppKey")
        ("client_secret", "sAppSecret")
        ("grant_type", "password")
        ("username", "mUser")
        ("password", "mPasswd")
REQUEST_API_END()

//返回最新的200条公共微博，返回结果非完全实时
REQUEST_API_BEGIN(PublicTimelineRequest)
        ("source", "")// 	false 	string 	采用OAuth授权方式不需要此参数，其他授权方式为必填参数，数值为应用的AppKey。
        ("access_token", "")// 	false 	string 	采用OAuth授权方式为必填参数，其他授权方式不需要此参数，OAuth授权后获得。
        ("count", 20)// 	false 	int 	单页返回的记录条数，最大不超过200，默认为20。
REQUEST_API_END()

//获取当前登录用户及其所关注用户的最新微博
REQUEST_API_BEGIN(HomeTimelineRequest)
        ("source", "") //false 	string 	采用OAuth授权方式不需要此参数，其他授权方式为必填参数，数值为应用的AppKey。
        ("access_token", "")// 	false 	string 	采用OAuth授权方式为必填参数，其他授权方式不需要此参数，OAuth授权后获得。
        ("since_id", 0)// 	false 	int64 	若指定此参数，则返回ID比since_id大的微博（即比since_id时间晚的微博），默认为0。
        ("max_id", 0)// 	false 	int64 	若指定此参数，则返回ID小于或等于max_id的微博，默认为0。
        ("count", 20)// 	false 	int 	单页返回的记录条数，最大不超过100，默认为20。
        ("page", 1)// 	false 	int 	返回结果的页码，默认为1。
        ("base_app", 0)// 	false 	int 	是否只获取当前应用的数据。0为否（所有数据），1为是（仅当前应用），默认为0。
        ("feature", 0)// 	false 	int 	过滤类型ID，0：全部、1：原创、2：图片、3：视频、4：音乐，默认为0。
        ("trim_user", 0)//	false 	int 	返回值中user字段开关，0：返回完整user字段、1：user字段仅返回user_id，默认为0。
REQUEST_API_END()

typedef HomeTimelineRequest FriendsTimelineRequest;

//获取用户发布的微博
REQUEST_API_BEGIN(UserTimelineRequest)
        ("source", "") //false 	string 	采用OAuth授权方式不需要此参数，其他授权方式为必填参数，数值为应用的AppKey。
        ("access_token", "")// 	false 	string 	采用OAuth授权方式为必填参数，其他授权方式不需要此参数，OAuth授权后获得。
        ("uid", -1)//false 	int64 	需要查询的用户ID.
        ("screen_name", "")// 	false 	string 	需要查询的用户昵称。 )
        ("since_id", 0)// 	false 	int64 	若指定此参数，则返回ID比since_id大的微博（即比since_id时间晚的微博），默认为0。
        ("max_id", 0)// 	false 	int64 	若指定此参数，则返回ID小于或等于max_id的微博，默认为0。
        ("count", 20)// 	false 	int 	单页返回的记录条数，最大不超过100，默认为20。
        ("page", 1)// 	false 	int 	返回结果的页码，默认为1。
        ("base_app", 0)// 	false 	int 	是否只获取当前应用的数据。0为否（所有数据），1为是（仅当前应用），默认为0。
        ("feature", 0)// 	false 	int 	过滤类型ID，0：全部、1：原创、2：图片、3：视频、4：音乐，默认为0。
        ("trim_user", 0)//	false 	int 	返回值中user字段开关，0：返回完整user字段、1：user字段仅返回user_id，默认为0。
REQUEST_API_END()

//获取用户发布的微博的ID
REQUEST_API_BEGIN(UserTimelineIdsRequest)
        ("source", "") //false 	string 	采用OAuth授权方式不需要此参数，其他授权方式为必填参数，数值为应用的AppKey。
        ("access_token", "")// 	false 	string 	采用OAuth授权方式为必填参数，其他授权方式不需要此参数，OAuth授权后获得。
        ("uid", -1)//false 	int64 	需要查询的用户ID.
        ("screen_name", "")// 	false 	string 	需要查询的用户昵称。 )
        ("since_id", 0)// 	false 	int64 	若指定此参数，则返回ID比since_id大的微博（即比since_id时间晚的微博），默认为0。
        ("max_id", 0)// 	false 	int64 	若指定此参数，则返回ID小于或等于max_id的微博，默认为0。
        ("count", 20)// 	false 	int 	单页返回的记录条数，最大不超过100，默认为20。
        ("page", 1)// 	false 	int 	返回结果的页码，默认为1。
        ("base_app", 0)// 	false 	int 	是否只获取当前应用的数据。0为否（所有数据），1为是（仅当前应用），默认为0。
REQUEST_API_END()

} //namespace QWeiboAPI
#endif // QWEIBOAPI_REQUESTPARAMETER_H
