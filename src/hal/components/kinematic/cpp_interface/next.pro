QT -= gui

TEMPLATE = lib
DEFINES += NEXT_LIBRARY

CONFIG += c++20
TARGET = kinematic

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    next.cpp

HEADERS += \
    next_global.h \
    next.h

INCLUDEPATH +=  /usr/include/ \
                /usr/local/include/kdl/ \
                /usr/include/eigen3/ \
                /usr/local/lib/ \

LIBS += -L/usr/local/lib/ \

LIBS += -lorocos-kdl

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
