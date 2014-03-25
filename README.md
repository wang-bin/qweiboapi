QWeiboAPI
=========

Qt wrapper for sina weibo api.

It was first used in my anothor project [PhotoKit](https://github.com/wang-bin/PhotoKit) in 2012 to send a weibo with picture.

自带了一个登录并获取微博的例子，examples/login

编译

    qmake
    make


## Generate Weibo API From Web

还在为查找weibo的api烦恼吗？我写了个bash脚本来结束你的噩梦。脚本可以分析weibo api网站生成一个接口文件。虽然还不是很完美，至少方便了很多。

There is a **bash** script `web2api.sh` to generate api from sina's website. Currently the output format is the same as that used in QWeiboAPI/src/requestparameter.h. Custom output will be added(e.g. for other languages)

#### How To Use

You need **bash** environment and **curl**.

命令

    ./web2api.sh

will parse the api on the website 1 by 1 and store the result in weiboapi.h

还可以用make命令来同时处理多个api来提升速度

    ./web2api.sh -make
    make -j4

This will first create a makefile. Then we can use **make** to parse multiple apis parallelly. It's mnuch faster.

The result is also stored in weiboapi.h.

Copy the result you want in weiboapi.h to src/requestparameter.h.

生成的接口都带有注释方便查看

比如

    // 2/statuses/home_timeline: 获取当前登录用户及其所关注用户的最新微博 
    REQUEST_API_BEGIN(statuses_home_timeline, "2/statuses/home_timeline")
            ("source", "")  //采用OAuth授权方式不需要此参数，其他授权方式为必填参数，数值为应用的AppKey。
            ("access_token", "")  //采用OAuth授权方式为必填参数，其他授权方式不需要此参数，OAuth授权后获得。
            ("since_id", 0)  //若指定此参数，则返回ID比since_id大的微博（即比since_id时间晚的微博），默认为0。
            ("max_id", 0)  //若指定此参数，则返回ID小于或等于max_id的微博，默认为0。
            ("count", 20)  //单页返回的记录条数，最大不超过100，默认为20。
            ("page", 1)  //返回结果的页码，默认为1。
            ("base_app", 0)  //是否只获取当前应用的数据。0为否（所有数据），1为是（仅当前应用），默认为0。
            ("feature", 0)  //过滤类型ID，0：全部、1：原创、2：图片、3：视频、4：音乐，默认为0。
            ("trim_user", 0)  //返回值中user字段开关，0：返回完整user字段、1：user字段仅返回user_id，默认为0。
    REQUEST_API_END()



是获取最新微博的接口。把生成的api代码贴到src/requestparameter.h后面（那里我只贴了没几个，没有的话贴上去）然后你就可以这样使用(见examples/login)

    Weibo weibo;
    QObject::connect(&weibo, SIGNAL(ok(QString)), txt, SLOT(append(QString)));
    QObject::connect(&weibo, SIGNAL(loginFail()), &failbox, SLOT(exec()));
    weibo.setUSer(user);
    weibo.setPassword(passwd);
    //weibo.login();
    Request *request = new statuses_public_timeline();
    weibo.createRequest(request);

#### ISSUES

- 自动生成的接口参数默认值未做分析，现在数字默认为0，字符串为""。需要自己手动根据参数注释修改

#### TODO

- 自定义生成接口输出模板，以方便给其他语言使用

注：我未申请appkey，可以在src/requrestparamster.h中修改sAppKey，sAppSecret

欢迎大家来完善。

