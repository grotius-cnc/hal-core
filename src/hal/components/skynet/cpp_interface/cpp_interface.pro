QT += core gui opengl
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = lib
DEFINES += CPP_INTERFACE_LIBRARY

CONFIG += c++17
TARGET = cyberdyne

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    cpp_interface.cpp \
    libdialog/example.cpp \
    libdxfrw/drw_classes.cpp \
    libdxfrw/drw_entities.cpp \
    libdxfrw/drw_header.cpp \
    libdxfrw/drw_objects.cpp \
    libdxfrw/dx_iface.cpp \
    libdxfrw/intern/drw_dbg.cpp \
    libdxfrw/intern/drw_textcodec.cpp \
    libdxfrw/intern/dwgbuffer.cpp \
    libdxfrw/intern/dwgreader.cpp \
    libdxfrw/intern/dwgreader15.cpp \
    libdxfrw/intern/dwgreader18.cpp \
    libdxfrw/intern/dwgreader21.cpp \
    libdxfrw/intern/dwgreader24.cpp \
    libdxfrw/intern/dwgreader27.cpp \
    libdxfrw/intern/dwgutil.cpp \
    libdxfrw/intern/dxfreader.cpp \
    libdxfrw/intern/dxfwriter.cpp \
    libdxfrw/intern/rscodec.cpp \
    libdxfrw/libdwgr.cpp \
    libdxfrw/libdxfrw.cpp \
    libdxfrw/libdxfrw_functions.cpp \
    libgcode/gcode_interface.cpp \
    libgcode/gcode_program.cpp \
    libgcode/parser.cpp \
    libocct/draw_primitives.cpp \
    libocct/opencascade.cpp \
    libskynet/char_to_string.cpp \
    libskynet/directory.cpp \
    libspline/bezier_spline.cpp \
    libspline/cubic_spline.cpp \
    libspline/spline.cpp \
    mainwindow.cpp

HEADERS += \
    cpp_interface.h \
    cpp_interface_global.h \
    external_var.h \
    libdialog/portable-file-dialogs.h \
    libdxfrw/drw_base.h \
    libdxfrw/drw_classes.h \
    libdxfrw/drw_entities.h \
    libdxfrw/drw_header.h \
    libdxfrw/drw_interface.h \
    libdxfrw/drw_objects.h \
    libdxfrw/dx_data.h \
    libdxfrw/dx_iface.h \
    libdxfrw/intern/drw_cptable932.h \
    libdxfrw/intern/drw_cptable936.h \
    libdxfrw/intern/drw_cptable949.h \
    libdxfrw/intern/drw_cptable950.h \
    libdxfrw/intern/drw_cptables.h \
    libdxfrw/intern/drw_dbg.h \
    libdxfrw/intern/drw_textcodec.h \
    libdxfrw/intern/dwgbuffer.h \
    libdxfrw/intern/dwgreader.h \
    libdxfrw/intern/dwgreader15.h \
    libdxfrw/intern/dwgreader18.h \
    libdxfrw/intern/dwgreader21.h \
    libdxfrw/intern/dwgreader24.h \
    libdxfrw/intern/dwgreader27.h \
    libdxfrw/intern/dwgutil.h \
    libdxfrw/intern/dxfreader.h \
    libdxfrw/intern/dxfwriter.h \
    libdxfrw/intern/rscodec.h \
    libdxfrw/libdwgr.h \
    libdxfrw/libdxfrw.h \
    libdxfrw/libdxfrw_functions.h \
    libdxfrw/main_doc.h \
    libgcode/gcode_interface.h \
    libgcode/gcode_program.h \
    libgcode/parser.h \
    libocct/draw_primitives.h \
    libocct/opencascade.h \
    libskynet/char_to_string.h \
    libskynet/directory.h \
    libspline/bezier_spline.h \
    libspline/cubic_spline.h \
    libspline/spline.h \
    mainwindow.h

# Ruckig trajectory
INCLUDEPATH += /opt/hal-core/src/hal/components/skynet/cpp_interface/libruckig/include
LIBS += -L/opt/hal-core/src/hal/components/skynet/cpp_interface/libruckig/build -lruckig

# Kdl kinematics
INCLUDEPATH +=  /usr/include/ \
                /usr/local/include/kdl/ \
                /usr/include/eigen3/ \
                /usr/local/lib/

LIBS += -L/usr/local/lib/
LIBS += -lorocos-kdl

# Opencascade
LIBS += -L/usr/local/lib/

INCLUDEPATH +=  /usr/local/include/opencascade/ \
                /usr/include/ \
                /usr/local/lib/

LIBS += -lTKPrim
LIBS += -lTKernel -lTKMath -lTKTopAlgo -lTKService
LIBS += -lTKG2d -lTKG3d -lTKV3d -lTKOpenGl
LIBS += -lTKBRep -lTKXSBase -lTKGeomBase
LIBS += -lTKMeshVS -lTKXSDRAW
LIBS += -lTKLCAF -lTKXCAF -lTKCAF
LIBS += -lTKCDF -lTKBin -lTKBinL -lTKBinXCAF -lTKXml -lTKXmlL -lTKXmlXCAF
# -- IGES support
LIBS += -lTKIGES
# -- STEP support
LIBS += -lTKSTEP -lTKXDESTEP -lTKXDEIGES
# -- STL support
LIBS += -lTKSTL
# -- OBJ/glTF support

LIBS += -lTKRWMesh

#src/base/io_occ_base_mesh.cpp \
#src/base/io_occ_gltf.cpp \
#src/base/io_occ_obj.cpp

# -- VRML support
LIBS += -lTKVRML

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

FORMS += \
    mainwindow.ui

RESOURCES += \
    icons.qrc
