QT -= gui

TEMPLATE = lib
DEFINES += CPP_INTERFACE_LIBRARY
# Name of the library:
TARGET = dofs

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    cpp_interface.cpp \
    libruckig/brake.cpp \
    libruckig/position-step1.cpp \
    libruckig/position-step2.cpp \
    libruckig/velocity-step1.cpp \
    libruckig/velocity-step2.cpp


HEADERS += \
    cpp_interface_global.h \
    cpp_interface.h \
    libruckig/block.hpp \
    libruckig/brake.hpp \
    libruckig/input_parameter.hpp \
    libruckig/output_parameter.hpp \
    libruckig/position.hpp \
    libruckig/profile.hpp \
    libruckig/roots.hpp \
    libruckig/ruckig.hpp \
    libruckig/trajectory.hpp \
    libruckig/velocity.hpp


#INCLUDEPATH+= /libruckig \
#LIBS+= -L/home/user/Desktop/Linux-Embedded/linux-hal/halcompile-trajectory-ruckig/cpp_interface/ruckig/build -lruckig

INCLUDEPATH+=

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    libruckig/ruckig-0.3.3.tar.gz \
    libruckig/setup.py
