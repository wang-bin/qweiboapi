TEMPLATE = lib
TARGET = QWeiboAPI

QT += core gui network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG   += console
CONFIG   -= app_bundle

CONFIG *= qweiboapi-buildlib

#var with '_' can not pass to pri?
STATICLINK = 0
PROJECTROOT = $$PWD/..
!include(libQWeiboAPI.pri): error("could not find libQWeiboAPI.pri")
preparePaths($$OUT_PWD/../out)


RESOURCES +=

win32 {
    RC_FILE = $${PROJECTROOT}/res/QWeiboAPI.rc
#no depends for rc file by default, even if rc includes a header. Makefile target use '/' as default, so not works iwth win cmd
    rc.target = $$clean_path($$RC_FILE) #rc obj depends on clean path target
    rc.depends = $$PWD/QWeiboAPI/version.h
#why use multiple rule failed? i.e. add a rule without command
    isEmpty(QMAKE_SH) {
        rc.commands = @copy /B $$system_path($$RC_FILE)+,, #change file time
    } else {
        rc.commands = @touch $$RC_FILE #change file time
    }
    QMAKE_EXTRA_TARGETS += rc
}
OTHER_FILES += $$RC_FILE

TRANSLATIONS = $${PROJECTROOT}/i18n/QWeiboAPI_zh_CN.ts

SOURCES *= \
    qput.cpp \
    qweiboapi_global.cpp \
    weibo.cpp

SDK_HEADERS *= \
    QWeiboAPI/dptr.h \
    QWeiboAPI/qweiboapi_global.h \
    QWeiboAPI/weibo.h \
    QWeiboAPI/version.h


HEADERS *= \
    $$SDK_HEADERS \
    QWeiboAPI/qput.h


SDK_INCLUDE_FOLDER = QWeiboAPI
include($$PROJECTROOT/deploy.pri)
