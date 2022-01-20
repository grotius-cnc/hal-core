#ifndef OPENCASCADE_H
#define OPENCASCADE_H

#include <QWidget>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QApplication>

#include <AIS_InteractiveContext.hxx>
#include <OpenGl_GraphicDriver.hxx>
#include <V3d_View.hxx>
#include <Aspect_Handle.hxx>
#include <Aspect_DisplayConnection.hxx>
#include <Graphic3d_GraphicDriver.hxx>

#include <QGLWidget>
#ifdef _WIN32
#include <WNT_Window.hxx>
#else
#undef None
#include <Xw_Window.hxx>
#endif

#include <BRepPrimAPI_MakeSphere.hxx>
#include <BRepPrimAPI_MakeCone.hxx>
#include <BRepPrimAPI_MakeTorus.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <AIS_Shape.hxx>
#include <TDF_Label.hxx>

//show xyz axis
#include <Geom_Axis2Placement.hxx>
#include <AIS_Trihedron.hxx>

#include <libocct/draw_primitives.h>

#include <external_var.h>

//! Make conversion's easy:
#define toRadians M_PI/180.0
#define toDegrees (180.0/M_PI)

#define gp_Euler gp_Pnt // Used as universal toolset.

extern gp_Trsf level0x1x2x3x4x5x6;

class Opencascade: public QGLWidget
{
    Q_OBJECT
public:
    explicit Opencascade(QWidget *parent = nullptr);

    //strore the data from the stepfile.
    std::vector<TopoDS_Shape> Shape;
    std::vector<Handle(AIS_Shape)> ais_Shape;
    std::vector<Quantity_Color> colv;

    Handle(XCAFDoc_ColorTool) aColorTool;
    Handle(XCAFDoc_ShapeTool) aShapeTool;

    gp_Trsf level0x1x2x3x4x5x6;

    Handle(AIS_Shape) tcpx,tcpy,tcpz;

    std::vector<Handle(AIS_Shape)> Ais_ShapeVec;
    struct STEPFILE{
        std::vector<Handle(AIS_Shape)> Ais_ShapeVec;
        gp_Trsf MyTrsf={};
        gp_Trsf MyTrsfResult={};
    };
    std::vector<STEPFILE> Stepfilevec;

    struct ASSEBMLY{
        std::vector<STEPFILE> stepfiles;
        gp_Trsf MyTrsfDummy={};
    };
    std::vector<ASSEBMLY> AssemblyVec;
    std::vector<Handle(AIS_Shape)> previewbucketvec;

    //! Every machine has a shapevector. Multiple machines having multiple shapevectors.
    std::vector<Handle(AIS_Shape)> gcodevec;
    void parse_gcode(GCODE gcode);
    void move_gcode(POINT p, EULER e);
    void erase_gcode();

    //! Load stepfiles.
    bool Readstepfile(const std::string& theStepName, bool end_of_assembly);
    void Visit(const TDF_Label& theLabel);

    //! Move machine joints.
    void Move_assembly_joints(MACHINE *ptr);

    void show_shape(Handle(AIS_Shape) shape);
    void Redraw();

    // View
    void Set_orthographic();
    void Set_perspective();
    void set_view_front();
    void set_view_back();
    void set_view_left();
    void set_view_right();
    void set_view_top();
    void set_view_bottom();

    // Selection
    void get_selections();
    void delete_selections();

    // Background
    void set_background_image(std::string file);

private:
    void m_initialize_context();
    Handle(AIS_InteractiveContext) m_context;
    Handle(V3d_Viewer) m_viewer;
    Handle(V3d_View) m_view;
    Handle(Graphic3d_GraphicDriver) m_graphic_driver;
    Handle(AIS_InteractiveObject) m_aisViewCube;

    // Xyz axis sign.
    Handle(Geom_Axis2Placement) axis;
    Handle(AIS_Trihedron) aisTrihedron;
    std::vector<Handle(AIS_Trihedron)> aisTrihedrons;

protected:
    void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

protected:
    enum CurrentAction3d
    {
        CurAction3d_Nothing,
        CurAction3d_DynamicPanning,
        CurAction3d_DynamicZooming,
        CurAction3d_DynamicRotation
    };

private:
    Standard_Integer m_x_max;
    Standard_Integer m_y_max;
    CurrentAction3d m_current_mode;
    //gp_Trsf current_tcp;

    Handle(AIS_Shape) aisBody_tcp_xaxis, aisBody_tcp_yaxis, aisBody_tcp_zaxis;

    // Create the euler lines
    double toollenght=105;
    double linelenght=25;
    double coneheight=25;
    double conetopdiameter=1;
    double conebottomdiameter=5;
    double textheight=25;

    TopoDS_Edge edge_linepreview;
    Handle(AIS_Shape) aisBody_linepreview;

signals:

public slots:
};


#endif // OPENCASCADE_H


