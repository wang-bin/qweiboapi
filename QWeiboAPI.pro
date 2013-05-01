TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS = libqweiboapi examples #tests

libqweiboapi.file = src/libQWeiboAPI.pro
examples.depends += libqweiboapi
tests.depends += libqweiboapi

OTHER_FILES += README.md \
    scripts/*.sh

EssentialDepends =
OptionalDepends =

include(root.pri)

PACKAGE_VERSION = 0.0.0
PACKAGE_NAME= QWeiboAPI

include(pack.pri)
#packageSet(0.0.0, QWeiboAPI)
