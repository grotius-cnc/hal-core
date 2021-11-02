QT -= gui

TEMPLATE = lib
DEFINES += CPP_INTERFACE_LIBRARY
# Name of the library:
TARGET = trajectory_6DoFs

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    cpp_interface.cpp \
    libgcoderead/gcode_program.cpp \
    libgcoderead/parser.cpp \
    libruckig/brake.cpp \
    libruckig/position-step1.cpp \
    libruckig/position-step2.cpp \
    libruckig/velocity-step1.cpp \
    libruckig/velocity-step2.cpp \
    libskynet/char_to_string.cpp \
    libskynet/directory.cpp \

HEADERS += \
    cpp_interface_global.h \
    cpp_interface.h \
    libgcoderead/gcode_program.h \
    libgcoderead/parser.h \
    libruckig/block.hpp \
    libruckig/brake.hpp \
    libruckig/input_parameter.hpp \
    libruckig/output_parameter.hpp \
    libruckig/position.hpp \
    libruckig/profile.hpp \
    libruckig/roots.hpp \
    libruckig/ruckig.hpp \
    libruckig/trajectory.hpp \
    libruckig/velocity.hpp \
    libskynet/char_to_string.h \
    libskynet/directory.h


INCLUDEPATH+= /libruckig \
#LIBS+= -L/home/user/Desktop/Linux-Embedded/linux-hal/halcompile-trajectory-ruckig/cpp_interface/ruckig/build -lruckig

INCLUDEPATH+=

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

# this copies the configuration files etc to the build direcory. So user has only to edit the source directory.
# copydata.commands = $(COPY_DIR) $$PWD/* $$OUT_PWD
# first.depends = $(first) copydata
# export(first.depends)
# export(copydata.commands)
# QMAKE_EXTRA_TARGETS += first copydata
