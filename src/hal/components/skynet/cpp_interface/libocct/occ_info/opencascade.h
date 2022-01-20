#ifndef OPENCASCADE_H
#define OPENCASCADE_H

#include <QtCore/qglobal.h>
#if defined(OPENCASCADE_LIBRARY)
#  define OPENCASCADESHARED_EXPORT Q_DECL_EXPORT
#else
#  define OPENCASCADESHARED_EXPORT Q_DECL_IMPORT
#endif

#include <QWidget>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QApplication>
#include <QGridLayout>
#include <QTextEdit>
#include <QLabel>
#include <QTimer>
#include <QPair>
#include <QVariantList>

#include <AIS_InteractiveContext.hxx>
#include <OpenGl_GraphicDriver.hxx>
#include <V3d_View.hxx>
#include <Aspect_Handle.hxx>
#include <Aspect_DisplayConnection.hxx>
#include <Graphic3d_GraphicDriver.hxx>
#include <AIS_TexturedShape.hxx>

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

#include <draw_primitives.h>
#include <libgcoderead/gcode_interface.h>

typedef QPair<QString,QString> MyType;   // typedef for your type
Q_DECLARE_METATYPE(MyType);                     // makes your type available to QMetaType system

typedef QString LABEL;
Q_DECLARE_METATYPE(LABEL);

class OPENCASCADESHARED_EXPORT Opencascade : public QGLWidget
{
    Q_OBJECT

    // Background color homogenius
    Q_PROPERTY(int property_background_color READ read_property_background_color WRITE write_property_background_color)
    Q_PROPERTY(QString property_background_picture READ read_property_background_picture WRITE write_property_background_picture)

    // Interactive 3d box
    Q_PROPERTY(int property_show_interactive_3d_box READ read_property_show_interactive_3d_box WRITE write_property_show_interactive_3d_box)

    // Show or hide triedron (3d axis cross)
    Q_PROPERTY(int property_hide_or_show_triedron READ read_property_hide_or_show_triedron WRITE write_property_hide_or_show_triedron)

    // Viewtypes + Viewports + Zoom
    Q_PROPERTY(int property_show_orthographic_or_perspective READ read_property_show_orthographic_or_perspective WRITE write_property_show_orthographic_or_perspective)
    Q_PROPERTY(int property_view_wireframe_or_shaded READ read_property_view_wireframe_or_shaded WRITE write_property_view_wireframe_or_shaded)
    Q_PROPERTY(int property_view_front READ read_property_view_front WRITE write_property_view_front)
    Q_PROPERTY(int property_view_back READ read_property_view_back WRITE write_property_view_back)
    Q_PROPERTY(int property_view_top READ read_property_view_top WRITE write_property_view_top)
    Q_PROPERTY(int property_view_bottom READ read_property_view_bottom WRITE write_property_view_bottom)
    Q_PROPERTY(int property_view_left READ read_property_view_left WRITE write_property_view_left)
    Q_PROPERTY(int property_view_right READ read_property_view_right WRITE write_property_view_right)
    Q_PROPERTY(int property_zoom_all READ read_property_zoom_all WRITE write_property_zoom_all)

    // Erase all
    Q_PROPERTY(int property_erase_all READ read_property_erase_all WRITE write_property_erase_all)

    // Dro testrun
    Q_PROPERTY(int property_dro_testrun READ read_property_dro_testrun WRITE write_property_dro_testrun)

    // Set labeltext
    Q_PROPERTY(LABEL property_label READ read_property_label WRITE write_property_label)


public:
    Opencascade(QWidget *parent=nullptr);
    bool isInitialized=0;
    QVariant is_initialized();

    struct DRO{
        QString id; // find by id to update text values.
        double global_pos_x, global_pos_y, global_pos_z;
        double offset_x, offset_y, offset_z;

        // Text:
        double textheight;
        unsigned int color;
        double text_transparancy=0.0;
        QString text=""; // Keep this empty
        QString font="Arial";
        int z_layer=-3; // Topmost.
        Handle (AIS_TextLabel) aLabel;

        // Background label:
        QString background_picture="";
        double background_size_x, background_size_y, background_size_z;
        double background_transparancy=0.0;
        AIS_TexturedShape *aTextureShape;
    };
    std::vector<DRO> drovec;

    // Gcodefile of type ngc, txt, etc.
    void load_gcode(QString filename);

    // Tool of type stepfile.
    Handle(XCAFDoc_ColorTool) aColorTool;
    Handle(XCAFDoc_ShapeTool) aShapeTool;
    bool Readstepfile(const std::string& theStepName);
    void Visit(const TDF_Label& theLabel);
    void load_tool_cone(QVariant data);
    void translate_tool_cone(QVariant data);
    void load_stepfile_tool_on(QString stepfilename);
    std::vector<Handle(AIS_Shape)> Ais_StepfileToolOnVec;
    void load_stepfile_tool_off(QString stepfilename);
    std::vector<Handle(AIS_Shape)> Ais_StepfileToolOffVec;
    void translate_stepfile_tool(QVariant data);

    // Test
    void test(QString);
    void test1(QVariant data);

    void start_timer();
    void update_screen();

    void show_shape(Handle(AIS_Shape) ashape);
    void set_shape_selected(Handle(AIS_Shape) ashape);
    void clear_selection();
    void Redraw();

    // Dro
    void dro_testrun();
    Handle(AIS_TextLabel) create_dro_label(DRO dro);
    AIS_TexturedShape* create_dro_background(DRO dro);

    // Dynamic dro setup
    void setup_dro(QVariant data);
    // Dynamic dro update
    void update_dro(QVariant data);

    // View
    void set_orthographic();
    void set_perspective();
    void set_view_front();
    void set_view_back();
    void set_view_left();
    void set_view_right();
    void set_view_top();
    void set_view_bottom();

    // Zoom
    void zoom_all();

    // Selection
    void get_selections();
    void delete_selections();

    // Erase
    void erase_all();

    // View as wireframe or shaded
    void view_wireframe();
    void view_shaded();

    // View triedron (3d axis cross)
    void show_3d_triedron();
    void hide_3d_triedron();

    // View 3d interactive viewbox
    void show_3d_interactive_box();
    void hide_3d_interactive_box();

    // **********************  Property section

    // show interactive box
    int property_var_3d_box_show;
    int read_property_show_interactive_3d_box();
    void write_property_show_interactive_3d_box(int status);

    // show orthographic or perspective
    int property_var_show_orthographic_or_perspective;
    int read_property_show_orthographic_or_perspective();
    void write_property_show_orthographic_or_perspective (int status);

    // set view front
    int property_var_view_front;
    int read_property_view_front();
    void write_property_view_front (int status);

    // set view back
    int property_var_view_back;
    int read_property_view_back();
    void write_property_view_back (int status);

    // set view top
    int property_var_view_top;
    int read_property_view_top();
    void write_property_view_top (int status);

    // set view bottom
    int property_var_view_bottom;
    int read_property_view_bottom();
    void write_property_view_bottom (int status);

    // set view left
    int property_var_view_left;
    int read_property_view_left();
    void write_property_view_left (int status);

    // set view right
    int property_var_view_right;
    int read_property_view_right();
    void write_property_view_right (int status);

    // zoom all
    int property_var_zoom_all;
    int read_property_zoom_all();
    void write_property_zoom_all (int status);

    // erase all
    int property_var_erase_all;
    int read_property_erase_all();
    void write_property_erase_all (int status);

    // View wireframe or shaded
    int property_var_view_wireframe_or_shaded;
    int read_property_view_wireframe_or_shaded();
    void write_property_view_wireframe_or_shaded(int status);

    // Background color
    int property_var_background_color;
    int read_property_background_color();
    void write_property_background_color(int color);

    // Background picture
    QString property_var_background_picture;
    QString read_property_background_picture();
    void write_property_background_picture(QString file);
    void set_background_image(std::string file);

    // Label text
    LABEL property_var_label;
    LABEL read_property_label();
    void write_property_label(LABEL contents);

    // Dro test preview
    int property_var_dro_testrun;
    int read_property_dro_testrun();
    void write_property_dro_testrun(int status);

    // Hide or view triedron (3d axis cross)
    int property_var_hide_or_show_triedron;
    int read_property_hide_or_show_triedron();
    void write_property_hide_or_show_triedron(int status);

public:
    Handle(AIS_InteractiveContext) m_context;
private:
    void m_initialize_context();
     Handle(AIS_InteractiveContext) m_context_dro;
    Handle(V3d_Viewer) m_viewer;
    Handle(V3d_View) m_view;
    Handle(Graphic3d_GraphicDriver) m_graphic_driver;
    Handle(AIS_InteractiveObject) m_aisViewCube;

    // Textlabel

    // Xyz axis sign.
    Handle(Geom_Axis2Placement) axis;
    Handle(AIS_Trihedron) aisTrihedron;
    std::vector<Handle(AIS_Trihedron)> aisTrihedrons;

    // Dro
    TopoDS_Shape t_topo_dro;
    Handle(AIS_Shape) t_ais_dro = new AIS_Shape(t_topo_dro);
    Handle (AIS_TextLabel) textlabel=new AIS_TextLabel;
    Quantity_NameOfColor EnumBackgroundColor;

    // Interactive 3d viewbox.
    opencascade::handle<AIS_ViewCube> aisViewCube = new AIS_ViewCube;

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
    QTimer *timer;
signals:

public slots:
};


#endif // OPENCASCADE_H


































