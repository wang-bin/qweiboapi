TEMPLATE = app
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = login
STATICLINK = 0
PROJECTROOT = $$PWD/../..
include($$PROJECTROOT/src/libQWeiboAPI.pri)
preparePaths($$OUT_PWD/../../out)

SOURCES += main.cpp
HEADERS += 


#include($$PROJECTROOT/deploy.pri)
