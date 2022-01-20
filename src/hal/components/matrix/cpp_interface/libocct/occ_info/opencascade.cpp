#include "opencascade.h"

#include <OSD.hxx>
#include <AIS_Trihedron.hxx>
#include <AIS_ViewCube.hxx>
#include <AIS_Selection.hxx>
#include <AIS_ColoredShape.hxx>
#include <AIS_ColoredDrawer.hxx>
#include <AIS_TextLabel.hxx>
#include <AIS_TexturedShape.hxx>
#include <AIS_MediaPlayer.hxx>
#include <BRepLib.hxx>
#include <BRepOffsetAPI_ThruSections.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepBuilderAPI_Transform.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepPrimAPI_MakePrism.hxx>
#include <BRepFilletAPI_MakeFillet.hxx>
#include <BRepOffsetAPI_MakeThickSolid.hxx>
#include <BRepPrimAPI_MakeCylinder.hxx>
#include <BRepAlgoAPI_Fuse.hxx>
#include <BRepTools_ReShape.hxx>
#include <BRepTools.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>
#include <gp_Trsf.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Solid.hxx>
#include <TopExp.hxx>
#include <TopTools_IndexedMapOfShape.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS_Compound.hxx>
#include <TPrsStd_AISPresentation.hxx>
#include <STEPControl_Writer.hxx>
#include <STEPControl_Reader.hxx>
#include <STEPCAFControl_Reader.hxx>
#include <STEPCAFControl_Controller.hxx>
#include <Geom_Line.hxx>
#include <Geom_Circle.hxx>
#include <Geom_TrimmedCurve.hxx>
#include <Geom_CartesianPoint.hxx>
#include <GC_MakeArcOfCircle.hxx>
#include <GC_MakeArcOfEllipse.hxx>
#include <GC_MakeCircle.hxx>
#include <GC_MakeEllipse.hxx>
#include <GC_MakeSegment.hxx>
#include <gce_MakeRotation.hxx>
#include <TopExp.hxx>
#include <TopoDS_Wire.hxx>
#include <TopoDS.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS_Edge.hxx>
#include <TDocStd_Document.hxx>
#include <TDocStd_Application.hxx>
#include <TDF_Label.hxx>
#include <TDF_AttributeIterator.hxx>
#include <TDF_ChildIterator.hxx>
#include <TDF_LabelSequence.hxx>
#include <TDataStd_Name.hxx>
#include <TDataStd_TreeNode.hxx>
#include <TDataStd_UAttribute.hxx>
#include <TNaming_NamedShape.hxx>
#include <TopTools.hxx>
#include <Geom_Plane.hxx>
#include <Geom_CylindricalSurface.hxx>
#include <Geom2d_Ellipse.hxx>
#include <Geom2d_TrimmedCurve.hxx>
#include "Geom_Axis2Placement.hxx"
#include <GCE2d_MakeSegment.hxx>
#include <GC_MakeArcOfCircle.hxx>
#include <XCAFDoc_Area.hxx>
#include <XCAFDoc_Centroid.hxx>
#include <XCAFDoc_Datum.hxx>
#include <XCAFDoc_Dimension.hxx>
#include <XCAFDoc_Location.hxx>
#include <XCAFDoc_Volume.hxx>
#include <XCAFDoc_DocumentTool.hxx>
#include <XCAFDoc_ColorTool.hxx>
#include <XCAFDoc_ShapeTool.hxx>
#include <XCAFApp_Application.hxx>
#include <XCAFDoc_ColorTool.hxx>
#include <XCAFDoc_ShapeTool.hxx>
#include <XCAFDoc_DocumentTool.hxx>
#include <Quantity_Color.hxx>
#include <Quantity_ColorRGBA.hxx>

#include "XCAFPrs_DocumentExplorer.hxx"
#include <TDataStd_Name.hxx>
#include <XCAFDoc_AssemblyItemId.hxx>
#include <XCAFDoc_AssemblyItemRef.hxx>

#include <BRepTools.hxx>
#include <Font_BRepFont.hxx>
#include <Font_BRepTextBuilder.hxx>
#include <Bnd_Box.hxx>

#include "gp_Elips.hxx"
#include <NCollection_Mat4.hxx>
#include <gp_Quaternion.hxx>

//! Make conversion's easy:
#define toRadians M_PI/180.0
#define toDegrees (180.0/M_PI)

Handle(AIS_Shape) Ais_ToolCone;

Handle(AIS_Shape) Ais_ShapeLoader;
std::vector<Handle(AIS_Shape)> Ais_ShapeLoaderVec;

Opencascade::Opencascade(QWidget *parent) : QGLWidget(parent)
{
    setBackgroundRole( QPalette::NoRole );
    setMouseTracking( true );

    //label = new QLabel(parent);
    //label->setMinimumWidth(200);
    //label->setMinimumHeight(100);
    //label->setAttribute(Qt::WA_TranslucentBackground);
    //label->setStyleSheet("background:transparent;");
    //label->setWindowFlags(Qt::FramelessWindowHint);

    // If you enable the timer, python gives "segmentation fault".
    // QTimer *timer = new QTimer(this);
    // connect(timer, &QTimer::timeout, this, &Opencascade::update_screen);
    // timer->start(200);
}

QVariant Opencascade::is_initialized(){
    if(isInitialized){
        return 1;
    } else return 0;
}

void Opencascade::start_timer(){
    if (!m_context.IsNull())
    {
        QTimer *timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, &Opencascade::update_screen);
        timer->start(500);
        m_view->Redraw();
    }
}

void Opencascade::update_screen(){
    m_view->Redraw();
}

// Loads and draws gcode.
void Opencascade::load_gcode(QString filename){
    erase_all(); // Cleanup the opencascade screen. Dro's will stay because the are saved in m_context_dro.
    std::vector<Handle(AIS_Shape)> aGcodeShapeVec = gcode_interface().load_gcode_file(filename.toStdString());
    for(unsigned int i=0; i<aGcodeShapeVec.size(); i++){
        m_context->Display(aGcodeShapeVec.at(i),Standard_False);
    }
}

bool Opencascade::Readstepfile(const std::string& theStepName)
{
    STEPCAFControl_Controller::Init();

    Handle(TDocStd_Document) aDoc;
    Handle(XCAFApp_Application) anApp = XCAFApp_Application::GetApplication();
    anApp->NewDocument("MDTV-XCAF", aDoc);

    STEPCAFControl_Reader aStepReader;
    aStepReader.SetColorMode(true);
    aStepReader.SetNameMode(true);
    aStepReader.SetLayerMode(true);
    aStepReader.SetPropsMode(true);

    aStepReader.ReadFile (theStepName.c_str());
    aStepReader.Transfer (aDoc);

    TDF_Label aRootLabel = aDoc->Main();

    aShapeTool = XCAFDoc_DocumentTool::ShapeTool(aRootLabel);
    aColorTool = XCAFDoc_DocumentTool::ColorTool(aRootLabel);

    Visit(aRootLabel);

    // Test, show content
    for(unsigned int i=0; i<Ais_ShapeLoaderVec.size(); i++){
        m_context->Display(Ais_ShapeLoaderVec.at(i),Standard_False);
    }

    m_view->FitAll();

    return 1;
}

void Opencascade::Visit(const TDF_Label& theLabel)
{
    //theLabel.EntryDump(std::cout);
    Quantity_Color aColor;
    TopoDS_Shape aShape;

    Handle(TDataStd_Name) aName;
    if (theLabel.FindAttribute(TDataStd_Name::GetID(), aName))
    {
        //std::cout << "  Name: " << aName->Get() << std::endl;
    }

    bool skip=0;
    if (aShapeTool->IsShape(theLabel))
    {
        //TopoDS_Shape aShape;
        //if a shape can be made from the current label, look for the color of that label:
        if(aShapeTool->GetShape(theLabel, aShape)){
            //std::cout<<"made a shape"<<std::endl;

            if(aColorTool->GetColor(aShape,XCAFDoc_ColorSurf,aColor)){
                skip=0;
            } else { skip=1;}

        }

        if(skip==0){
            Ais_ShapeLoaderVec.push_back(new AIS_Shape(aShape));

            if(aColor.Name()!=Quantity_NOC_YELLOW && aColor.Name()!=Quantity_NOC_RED){
                Ais_ShapeLoaderVec.back()->SetColor(aColor);
            }
            Ais_ShapeLoaderVec.back()->SetDisplayMode(AIS_Shaded);
            Ais_ShapeLoaderVec.back()->Attributes()->SetFaceBoundaryDraw(true);
            Ais_ShapeLoaderVec.back()->Attributes()->SetFaceBoundaryAspect(
                        new Prs3d_LineAspect(Quantity_NOC_BLACK, Aspect_TOL_SOLID, 1.));
            Ais_ShapeLoaderVec.back()->Attributes()->SetIsoOnTriangulation(true);
        }
    }

    /*! Repeat the visit function for each childmember. */
    for (TDF_ChildIterator c(theLabel); c.More(); c.Next())
    {
        Visit(c.Value());
    }
}

void Opencascade::load_tool_cone(QVariant data){
    QStringList list=data.toStringList();
    double bottomdiameter = list.at(0).toDouble();
    double topdiameter = list.at(1).toDouble();
    double height = list.at(2).toDouble();
    Ais_ToolCone = draw_primitives().draw_3d_cone({0,0,0},bottomdiameter,topdiameter,height);
    Ais_ToolCone = draw_primitives().colorize(Ais_ToolCone,Quantity_NOC_GREEN,0);
    m_context->Display(Ais_ToolCone,Standard_False);
}

void Opencascade::translate_tool_cone(QVariant data){
    QStringList list=data.toStringList();
    double x = list.at(0).toDouble();
    double y = list.at(1).toDouble();
    double z = list.at(2).toDouble();
    Ais_ToolCone = draw_primitives().rotate_translate_3d_quaternion(Ais_ToolCone,{x,y,z},0,0,0);
    m_context->Redisplay(Ais_ToolCone,Standard_False);
    m_context->Display(Ais_ToolCone,Standard_False);
}

void Opencascade::load_stepfile_tool_on(QString stepfilename){
    // Load the stepfile into the buffer : Ais_ShapeLoaderVec
    Readstepfile(stepfilename.toStdString());
    Ais_StepfileToolOnVec=Ais_ShapeLoaderVec;
    Ais_ShapeLoaderVec.clear();
}

void Opencascade::load_stepfile_tool_off(QString stepfilename){
    // Load the stepfile into the buffer : Ais_ShapeLoaderVec
    Readstepfile(stepfilename.toStdString());
    Ais_StepfileToolOffVec=Ais_ShapeLoaderVec;
    Ais_ShapeLoaderVec.clear();
}

void Opencascade::translate_stepfile_tool(QVariant data){
    QStringList list=data.toStringList();
    double x = list.at(0).toDouble();
    double y = list.at(1).toDouble();
    double z = list.at(2).toDouble();
    double euler_x = list.at(3).toDouble();
    double euler_y = list.at(4).toDouble();
    double euler_z = list.at(5).toDouble();
    int tool_status = list.at(6).toInt();

    for(unsigned int i=0; i<Ais_StepfileToolOnVec.size(); i++){
        Ais_StepfileToolOnVec.at(i) = draw_primitives().rotate_translate_3d_quaternion(Ais_StepfileToolOnVec.at(i),{x,y,z},euler_z,euler_y,euler_x);
        if(tool_status==1){
            Ais_StepfileToolOnVec.at(i)->SetTransparency(0);
            m_context->Redisplay(Ais_StepfileToolOnVec.at(i),Standard_False);
            m_context->Display(Ais_StepfileToolOnVec.at(i),Standard_False);
        }
        if(tool_status==0){
            Ais_StepfileToolOnVec.at(i)->SetTransparency(1);
            m_context->Redisplay(Ais_StepfileToolOnVec.at(i),Standard_False);
            m_context->Display(Ais_StepfileToolOnVec.at(i),Standard_False);
        }
    }
    for(unsigned int i=0; i<Ais_StepfileToolOffVec.size(); i++){
        Ais_StepfileToolOffVec.at(i) = draw_primitives().rotate_translate_3d_quaternion(Ais_StepfileToolOffVec.at(i),{x,y,z},euler_z,euler_y,euler_x);
        if(tool_status==1){
            Ais_StepfileToolOffVec.at(i)->SetTransparency(1);
            m_context->Redisplay(Ais_StepfileToolOffVec.at(i),Standard_False);
            m_context->Display(Ais_StepfileToolOffVec.at(i),Standard_False);
        }
        if(tool_status==0){
            Ais_StepfileToolOffVec.at(i)->SetTransparency(0);
            m_context->Redisplay(Ais_StepfileToolOffVec.at(i),Standard_False);
            m_context->Display(Ais_StepfileToolOffVec.at(i),Standard_False);
        }
    }
}

// Property section.
LABEL Opencascade::read_property_label(){
    return property_var_label;
}
void Opencascade::write_property_label(LABEL contents){
    property_var_label=contents;
    std::cout<<"label function catched!"<<std::endl;

    if(m_context.IsNull()){
        std::cout<<"set label property when opencascade is loaded"<<std::endl;
    } else {

        std::cout<<"contents label text:"<<property_var_label.toStdString()<<std::endl;
    }

}

// Dro testrun
int Opencascade::read_property_dro_testrun(){
    return property_var_dro_testrun;
}
void Opencascade::write_property_dro_testrun(int status){
    property_var_dro_testrun=status;

    if(m_context.IsNull()){
        std::cout<<"set dro testrun property when opencascade is loaded"<<std::endl;
    } else {
        if(property_var_dro_testrun==0){
            // Do nothing.
        }
        if(property_var_dro_testrun==1){
            dro_testrun();
        }
    }
}

// Interactive 3d box
int Opencascade::read_property_show_interactive_3d_box(){
    return property_var_3d_box_show;
}
void Opencascade::write_property_show_interactive_3d_box(int status){
    property_var_3d_box_show=status;

    if(m_context.IsNull()){
        std::cout<<"set show interactive 3d box property when opencascade is loaded"<<std::endl;
    } else {
        if(property_var_3d_box_show==0){
            hide_3d_interactive_box();
        }
        if(property_var_3d_box_show==1){
            show_3d_interactive_box();
        }
    }
}

// Show orthographic or perspective
int Opencascade::read_property_show_orthographic_or_perspective(){
    return property_var_show_orthographic_or_perspective;
}
void Opencascade::write_property_show_orthographic_or_perspective (int status){
    property_var_show_orthographic_or_perspective=status;

    if(m_context.IsNull()){
        std::cout<<"set orthographic or perspective property when opencascade is loaded"<<std::endl;
    } else {
        if(property_var_show_orthographic_or_perspective==0){
            set_orthographic();
        }
        if(property_var_show_orthographic_or_perspective==1){
            set_perspective();
        }
    }
}

// Set view front
int Opencascade::read_property_view_front(){
    return property_var_view_front;
}
void Opencascade::write_property_view_front (int status){
    property_var_view_front=status;

    if(m_context.IsNull()){
        std::cout<<"set view front property when opencascade is loaded"<<std::endl;
    } else {
        if(property_var_view_front==1){
            set_view_front();
        }
    }
}

// Set view back
int Opencascade::read_property_view_back(){
    return property_var_view_back;
}
void Opencascade::write_property_view_back(int status){
    property_var_view_back=status;

    if(m_context.IsNull()){
        std::cout<<"set view back property when opencascade is loaded"<<std::endl;
    } else {
        if(property_var_view_back==1){
            set_view_back();
        }
    }
}

// Set view top
int Opencascade::read_property_view_top(){
    return property_var_view_top;
}
void Opencascade::write_property_view_top(int status){
    property_var_view_top=status;

    if(m_context.IsNull()){
        std::cout<<"set view top when property opencascade is loaded"<<std::endl;
    } else {
        if(property_var_view_top==1){
            set_view_top();
        }
    }
}

// Set view bottom
int Opencascade::read_property_view_bottom(){
    return property_var_view_bottom;
}
void Opencascade::write_property_view_bottom(int status){
    property_var_view_bottom=status;

    if(m_context.IsNull()){
        std::cout<<"set view bottom property when opencascade is loaded"<<std::endl;
    } else {
        if(property_var_view_bottom==1){
            set_view_bottom();
        }
    }
}

// Set view left
int Opencascade::read_property_view_left(){
    return property_var_view_left;
}
void Opencascade::write_property_view_left(int status){
    property_var_view_left=status;

    if(m_context.IsNull()){
        std::cout<<"set view left property when opencascade is loaded"<<std::endl;
    } else {
        if(property_var_view_left==1){
            set_view_left();
        }
    }
}

// Set view right
int Opencascade::read_property_view_right(){
    return property_var_view_right;
}
void Opencascade::write_property_view_right(int status){
    property_var_view_right=status;

    if(m_context.IsNull()){
        std::cout<<"set view right property when opencascade is loaded"<<std::endl;
    } else {
        if(property_var_view_right==1){
            set_view_right();
        }
    }
}

// Zoom all
int Opencascade::read_property_zoom_all(){
    return property_var_zoom_all;
}
void Opencascade::write_property_zoom_all (int status){
    property_var_zoom_all=status;

    if(m_context.IsNull()){
        std::cout<<"set zoom all property when opencascade is loaded"<<std::endl;
    } else {
        if(property_var_zoom_all==1){
            zoom_all();
        }
    }
}

int Opencascade::read_property_erase_all(){
    return property_var_erase_all;
}
void Opencascade::write_property_erase_all(int status){
    property_var_erase_all=status;

    if(m_context.IsNull()){
        std::cout<<"set erase all property when opencascade is loaded"<<std::endl;
    } else {
        if(property_var_erase_all==1){
            erase_all();
        }
    }
}

int Opencascade::read_property_view_wireframe_or_shaded(){
    return property_var_view_wireframe_or_shaded;
}
void Opencascade::write_property_view_wireframe_or_shaded(int status){
    property_var_view_wireframe_or_shaded=status;

    if(m_context.IsNull()){
        std::cout<<"set wireframe or shaded property when opencascade is loaded"<<std::endl;
    } else {
        if(property_var_view_wireframe_or_shaded==0){
            view_wireframe();
        }
        if(property_var_view_wireframe_or_shaded==1){
            view_shaded();
        }
    }
}

// Background color
int Opencascade::read_property_background_color(){
    return property_var_background_color;
}

void Opencascade::write_property_background_color(int color){
    property_var_background_color=color;
    m_view->SetBackgroundColor(Quantity_NameOfColor(property_var_background_color)); // Quantity_NameOfColor::Quantity_NOC_GREEN, has a index nr.
    m_viewer->SetDefaultShadingModel(Graphic3d_TypeOfShadingModel::V3d_COLOR);
}

// Todo, background 2 color gradient

// Background picture
QString Opencascade::read_property_background_picture(){
    return property_var_background_picture;
}
void Opencascade::write_property_background_picture(QString file){
    property_var_background_picture=file;

    if(m_context.IsNull()){
        std::cout<<"set background picture property when opencascade is loaded"<<std::endl;
    } else {
        if(file.size()>0){
            set_background_image(property_var_background_picture.toStdString());
            m_viewer->SetDefaultShadingModel(Graphic3d_TypeOfShadingModel::Graphic3d_TOSM_DEFAULT);
        }
    }
}
void Opencascade::set_background_image(std::string file){
    // https://sourceforge.net/projects/freeimage/
    Standard_CString FileName(file.c_str());
    Aspect_FillMethod FillStyle(Aspect_FM_STRETCH);//Aspect_FM_STRETCH
    Standard_Boolean update(Standard_True);
    m_view->SetBackgroundImage(FileName, FillStyle, update) ;
}

// Triedron (3d axis cross)
int Opencascade::read_property_hide_or_show_triedron(){
    return property_var_hide_or_show_triedron;
}
void Opencascade::write_property_hide_or_show_triedron(int status){
    property_var_hide_or_show_triedron=status;

    if(m_context.IsNull()){
        std::cout<<"set triedron property when opencascade is loaded"<<std::endl;
    } else {
        if(property_var_hide_or_show_triedron==0){
            hide_3d_triedron();
        }
        if(property_var_hide_or_show_triedron==1){
            show_3d_triedron();
        }
    }
}

void Opencascade::show_3d_triedron(){
    m_view->TriedronDisplay(Aspect_TOTP_LEFT_LOWER, Quantity_NOC_GOLD, 0.08, V3d_ZBUFFER);

    /* Left for info:
    // Show triedron at a specific place on the screen
    axis = new Geom_Axis2Placement(gp::XOY());
    aisTrihedron = new AIS_Trihedron(axis);
    aisTrihedron->SetDatumDisplayMode(Prs3d_DM_WireFrame);
    aisTrihedron->SetDrawArrows(true);
    aisTrihedron->Attributes()->DatumAspect()->LineAspect(Prs3d_DP_XAxis)->SetWidth(2.5);
    aisTrihedron->Attributes()->DatumAspect()->LineAspect(Prs3d_DP_YAxis)->SetWidth(2.5);
    aisTrihedron->Attributes()->DatumAspect()->LineAspect(Prs3d_DP_ZAxis)->SetWidth(2.5);
    aisTrihedron->SetDatumPartColor(Prs3d_DP_XAxis, Quantity_NOC_RED2);
    aisTrihedron->SetDatumPartColor(Prs3d_DP_YAxis, Quantity_NOC_GREEN2);
    aisTrihedron->SetDatumPartColor(Prs3d_DP_ZAxis, Quantity_NOC_BLUE2);
    aisTrihedron->SetLabel(Prs3d_DP_XAxis, "");
    aisTrihedron->SetLabel(Prs3d_DP_YAxis, "");
    aisTrihedron->SetLabel(Prs3d_DP_ZAxis, "");
    //aisTrihedron->SetTextColor(Quantity_NOC_GRAY40);
    aisTrihedron->SetSize(60);
    aisTrihedron->SetTransformPersistence(
                new Graphic3d_TransformPers(Graphic3d_TMF_ZoomPers, axis->Ax2().Location()));
    aisTrihedron->Attributes()->SetZLayer(Graphic3d_ZLayerId_Topmost);
    aisTrihedron->SetInfiniteState(true);
    m_context->Display(aisTrihedron,Standard_False);
    */
}

void Opencascade::hide_3d_triedron(){
    m_view->TriedronErase();
}

// Show interactivve 3d box
void Opencascade::show_3d_interactive_box(){
    // opencascade::handle<AIS_ViewCube> aisViewCube = new AIS_ViewCube;
    aisViewCube->SetBoxColor(Quantity_NOC_GRAY75);
    //aisViewCube->SetFixedAnimationLoop(false);
    aisViewCube->SetDrawAxes(false);
    aisViewCube->SetSize(55);
    aisViewCube->SetFontHeight(12);
    aisViewCube->SetTransformPersistence(
                new Graphic3d_TransformPers(
                    Graphic3d_TMF_TriedronPers,
                    Aspect_TOTP_LEFT_UPPER,
                    Graphic3d_Vec2i(85, 85)));
    m_context->Display(aisViewCube, false);
    //aisViewCube->Attributes()->DatumAspect()->LineAspect(Prs3d_DP_XAxis)->SetColor(Quantity_NOC_RED2);
    //const Handle_Prs3d_DatumAspect& datumAspect = aisViewCube->Attributes()->DatumAspect();
    //datumAspect->ShadingAspect(Prs3d_DP_XAxis)->SetColor(Quantity_NOC_RED2);
    //datumAspect->ShadingAspect(Prs3d_DP_YAxis)->SetColor(Quantity_NOC_GREEN2);
    //datumAspect->ShadingAspect(Prs3d_DP_ZAxis)->SetColor(Quantity_NOC_BLUE2);
    //m_aisViewCube = aisViewCube;
}

void Opencascade::hide_3d_interactive_box(){
    aisViewCube->Delete();
}


void Opencascade::show_shape(Handle(AIS_Shape) ashape){
    m_context->Display(ashape,Standard_False);
}


void Opencascade::set_shape_selected(Handle(AIS_Shape) ashape){
    m_context->AddOrRemoveSelected(ashape,1); // Select the contour, hightlight it.
}

void Opencascade::clear_selection(){
    m_context->ClearSelected(1);
}

void Opencascade::get_selections(){ // Updated by jointpos function from mainwindow.

}

void Opencascade::delete_selections(){

}

void Opencascade::Redraw(){
    //m_context->CurrentViewer()->Redraw();
    m_view->Redraw();
}

void Opencascade::view_wireframe(){
    m_context->SetDisplayMode(AIS_WireFrame, Standard_False);
}

void Opencascade::view_shaded(){
    m_context->SetDisplayMode(AIS_Shaded, Standard_False);
}


void Opencascade::m_initialize_context()
{
    if (m_context.IsNull())
    {

        Handle(Aspect_DisplayConnection) m_display_donnection = new Aspect_DisplayConnection();

        if (m_graphic_driver.IsNull())
        {
            m_graphic_driver = new OpenGl_GraphicDriver(m_display_donnection);
        }

        WId window_handle = (WId) winId();
#ifdef _WIN32
        Handle(WNT_Window) wind = new WNT_Window((Aspect_Handle) window_handle);
#else
        Handle(Xw_Window) wind = new Xw_Window(m_display_donnection, (Window) window_handle);
#endif
        m_viewer = new V3d_Viewer(m_graphic_driver);


        m_view = m_viewer->CreateView();
        m_view->SetWindow(wind);

        /*! Set the camera in perspective mode */
        m_view->Camera()->SetProjectionType (Graphic3d_Camera::Projection_Perspective);

        if (!wind->IsMapped())
        {
            wind->Map();
        }

        m_context = new AIS_InteractiveContext(m_viewer);
        m_context_dro = new AIS_InteractiveContext(m_viewer);

        m_viewer->SetDefaultLights();
        m_viewer->SetLightOn();

        // Set background with gradient stylesheet
        //  Gradient sheme's for SetBgGradientColors:
        //  Aspect_GFM_CORNER1
        //  Aspect_GFM_CORNER2
        //  Aspect_GFM_CORNER3
        //  Aspect_GFM_CORNER4
        //  Aspect_GFM_DIAG1
        //  Aspect_GFM_DIAG2
        //  Aspect_GFM_HOR
        //  Aspect_GFM_NONE
        //  Aspect_GFM_VER

        // To be reviewed :
        //Quantity_Color cola,colb;
        //cola.SetValues(property_var_background_color_left.red()/255, property_var_background_color_left.green()/255, property_var_background_color_left.blue()/255,Quantity_TOC_RGB);
        //colb.SetValues(property_var_background_color_right.red()/255, property_var_background_color_right.green()/255, property_var_background_color_right.blue()/255,Quantity_TOC_RGB);
        //m_view->SetBgGradientColors(cola,colb,Aspect_GFM_DIAG2 , false);

        //        Handle(Prs3d_Drawer) t_hilight_style = m_context->HighlightStyle();
        //        t_hilight_style->SetMethod(Aspect_TOHM_COLOR);
        //        t_hilight_style->SetColor(Quantity_NOC_LIGHTYELLOW);
        //        t_hilight_style->SetDisplayMode(1);
        //        t_hilight_style->SetTransparency(0.2f);

        //        Handle(Prs3d_Drawer) t_select_style = m_context->SelectionStyle();
        //        t_select_style->SetMethod(Aspect_TOHM_COLOR);
        //        t_select_style->SetColor(Quantity_NOC_LIGHTSEAGREEN);
        //        t_select_style->SetDisplayMode(1);
        //        t_select_style->SetTransparency(0.4f);

        // Highlightstyle // https://tracker.dev.opencascade.org/view.php?id=30523
        const Handle(Prs3d_Drawer) highlightStyle = new Prs3d_Drawer();
        highlightStyle->SetColor(Quantity_NOC_GREEN);
        highlightStyle->SetZLayer(m_context->HighlightStyle()->ZLayer());
        m_context->SetHighlightStyle(highlightStyle);

        /// Show grid
        m_viewer->SetRectangularGridValues(0,0,1,1,0);
        m_viewer->SetRectangularGridGraphicValues(2.01,2.01,0);
        m_viewer->ActivateGrid(Aspect_GT_Rectangular,Aspect_GDM_Lines);

        m_view->MustBeResized();
    }
    isInitialized=1;
}

void Opencascade::setup_dro(QVariant data){

    DRO dro;
    QStringList list=data.toStringList();

    // Initialize dro data.
    for(int i=0; i<list.size(); i++){

        if(list.at(i)=="Id:"){ // Find id to change Text value.
            dro.id=list.at(i+1);
        }
        if(list.at(i)=="Refpos:"){ // Opencascade has 4 refpositions. It's now : right-top
            dro.global_pos_x=list.at(i+1).toDouble();
            dro.global_pos_y=list.at(i+2).toDouble();
            dro.global_pos_z=list.at(i+3).toDouble();
        }
        if(list.at(i)=="Offset:"){ // Local refpos offset.
            dro.offset_x=list.at(i+1).toDouble();
            dro.offset_y=list.at(i+2).toDouble();
            dro.offset_z=list.at(i+3).toDouble();
        }
        if(list.at(i)=="Text:"){ // Value.
            dro.text=list.at(i+1);
        }
        if(list.at(i)=="Textheight:"){ // Value int.
            dro.textheight=list.at(i+1).toDouble();
        }
        if(list.at(i)=="Textcolor:"){ // Opencascade enum.
            dro.color=list.at(i+1).toInt();
        }
        if(list.at(i)=="Font:"){ // Text font like : "Arial"
            dro.font=list.at(i+1);
        }
        if(list.at(i)=="Texttransparancy:"){ // Value 0-1
            dro.text_transparancy=list.at(i+1).toDouble();
        }
        if(list.at(i)=="Backgroundpicture:"){ // Format .png - .jpg etc. Compatible with freeimage lib.
            dro.background_picture=list.at(i+1);
        }
        if(list.at(i)=="Backgroundsize:"){
            dro.background_size_x=list.at(i+1).toDouble();
            dro.background_size_y=list.at(i+2).toDouble();
            dro.background_size_z=list.at(i+3).toDouble();
        }
        if(list.at(i)=="Backgroundtransparancy:"){
            dro.background_transparancy=list.at(i+1).toDouble();
        }
        if(list.at(i)=="End"){ // End sign of datablock.
            drovec.push_back(dro);
        }
    }

    // Check.
    std::cout<<"drovec size:"<<drovec.size()<<std::endl;

    // Draw dro's.
    for(unsigned int i=0; i<drovec.size(); i++){
        // The dro text
        drovec.at(i).aLabel=create_dro_label(drovec.at(i));
        m_context_dro->Display(drovec.at(i).aLabel,Standard_False);
        // The dro background picture, perform only is a filename is given.
        if(drovec.at(i).background_picture.size()!=0){
            drovec.at(i).aTextureShape=create_dro_background(drovec.at(i));
            m_context_dro->Display(drovec.at(i).aTextureShape,Standard_False);
        }
    }
    // Do not highlight on mouse hover
    m_context_dro->Activate(TopAbs_FACE, Standard_False);
}

//********************* SUDO MAKE INSTALL !!
void Opencascade::update_dro(QVariant data){
    // first argument : the Id , Second argument : value
    QStringList list=data.toStringList();
    // Check.
    // std::cout<<"function triggered update list, data:"<<list.at(0).toStdString()<<" next:"<<list.at(1).toStdString()<<std::endl;
    // std::cout<<"function triggered update dro, data:"<<drovec.at(0).id.toStdString()<<" next:"<<drovec.at(0).text.toStdString()<<std::endl;

    for(int i=0; i<list.size(); i++){
            for(unsigned int j=0; j<drovec.size(); j++){
                if(drovec.at(j).id==list.at(i)){
                    // Check.
                    // std::cout<<"list match found:"<<drovec.at(j).id.toStdString()<<std::endl;
                    drovec.at(j).text=list.at(i+1);
                    m_context_dro->Erase(drovec.at(j).aLabel,Standard_False);
                    drovec.at(j).aLabel=create_dro_label(drovec.at(j));
                    m_context_dro->Display(drovec.at(j).aLabel,Standard_False);
                }
            }
    }
}

void Opencascade::dro_testrun(){
    // AIS_TexturedShape *aTextureShape=create_dro_background(250,100,150,150,0.9,"/home/user/Desktop/Qt-Graphics_Widget/libicon/metallic_texture.jpg");
    // m_context_dro->Display(aTextureShape,Standard_False);

    double relative_x=0;
    double relative_y=0;
    double relative_z=0;

    DRO dro;
    dro.global_pos_x=250;
    dro.global_pos_y=100;
    dro.text_transparancy=0.0;

    for(unsigned int i=0; i<9; i++){

        if(i==0){
            dro.text="X";
        }
        if(i==1){
            dro.text="Y";
        }
        if(i==2){
            dro.text="Z";
        }
        if(i==3){
            dro.text="A";
        }
        if(i==4){
            dro.text="B";
        }
        if(i==5){
            dro.text="C";
        }
        if(i==6){
            dro.text="U";
        }
        if(i==7){
            dro.text="V";
        }
        if(i==8){
            dro.text="W";
        }

        dro.textheight=26;
        dro.offset_x=relative_x+0;
        dro.offset_y=relative_y+16;
        dro.offset_z=+relative_z+0;
        dro.color=Quantity_NameOfColor::Quantity_NOC_GREEN;
        dro.z_layer=-3; // Topmost.
        Handle (AIS_TextLabel) aLabel=create_dro_label(dro);
        m_context_dro->Display(aLabel,Standard_False);

        dro.text=QString::number(rand()%2500,'f',3);
        dro.textheight=26;
        dro.offset_x=relative_x+80;
        dro.offset_y=relative_y+16;
        dro.offset_z=relative_z+0;
        aLabel=create_dro_label(dro);
        m_context_dro->Display(aLabel,Standard_False);

        dro.text="ABS";
        dro.textheight=14;
        dro.offset_x=relative_x+0;
        dro.offset_y=relative_y+0;
        dro.offset_z=relative_z+0;
        aLabel=create_dro_label(dro);
        m_context_dro->Display(aLabel,Standard_False);

        dro.text="0.000"; // Abs value
        dro.textheight=14;
        dro.offset_x=relative_x+35;
        dro.offset_y=relative_y+0;
        dro.offset_z=relative_z+0;
        aLabel=create_dro_label(dro);
        m_context_dro->Display(aLabel,Standard_False);

        dro.text="DTG";
        dro.textheight=14;
        dro.offset_x=relative_x+120;
        dro.offset_y=relative_y+0;
        dro.offset_z=relative_z+0;
        aLabel=create_dro_label(dro);
        m_context_dro->Display(aLabel,Standard_False);

        dro.text="0.000"; // Dtg value
        dro.textheight=14;
        dro.offset_x=relative_x+155;
        dro.offset_y=relative_y+0;
        dro.offset_z=relative_z+0;
        aLabel=create_dro_label(dro);
        m_context_dro->Display(aLabel,Standard_False);

        dro.text="L";
        dro.textheight=15;
        dro.offset_x=relative_x+-20;
        dro.offset_y=relative_y+0;
        dro.offset_z=relative_z+0;
        dro.color=Quantity_NameOfColor::Quantity_NOC_RED;
        aLabel=create_dro_label(dro);
        m_context_dro->Display(aLabel,Standard_False);

        dro.text="H";
        dro.textheight=15;
        dro.offset_x=relative_x+-20;
        dro.offset_y=relative_y+16;
        dro.offset_z=relative_z+0;
        dro.color=Quantity_NameOfColor::Quantity_NOC_ORANGE;
        aLabel=create_dro_label(dro);
        m_context_dro->Display(aLabel,Standard_False);

        dro.text="G54";
        dro.textheight=14;
        dro.offset_x=relative_x+23;
        dro.offset_y=relative_y+15;
        dro.offset_z=relative_z+0;
        dro.color=Quantity_NameOfColor::Quantity_NOC_GREEN;
        aLabel=create_dro_label(dro);
        m_context_dro->Display(aLabel,Standard_False);

        // Shift in y
        relative_y-=50;
    }
    // Do not highlight on mouse hover
    m_context_dro->Activate(TopAbs_FACE, Standard_False );

    // Test cone draw
    /*
    Handle(AIS_Shape) aShape = draw_primitives().draw_3d_cone({0,0,0},1,25,50);
    aShape->SetMaterial(Graphic3d_MaterialAspect(Graphic3d_NameOfMaterial::Graphic3d_NOM_CHROME));
    AIS_TexturedShape *aTextureShape=new AIS_TexturedShape(aShape->Shape());
    aTextureShape->SetTextureFileName("/home/user/Desktop/Qt-Graphics_Widget/libicon/metallic_texture.jpg");
    aTextureShape->SetTextureMapOn();
    aTextureShape->SetDisplayMode(3); // Diplay the picture.
    m_context->Display(aTextureShape,Standard_True);
    */
}

Handle (AIS_TextLabel) Opencascade::create_dro_label(DRO dro){
    Handle (AIS_TextLabel) aTextLabel=new AIS_TextLabel;
    aTextLabel->SetText(dro.text.toStdString().c_str());
    aTextLabel->SetFont(dro.font.toStdString().c_str());
    aTextLabel->SetZLayer(dro.z_layer); // Zlayer enum 0=default, -2=top, -3=topmost
    aTextLabel->SetColor(Quantity_NameOfColor(dro.color)); // Enum index nr.
    aTextLabel->SetHeight(dro.textheight);
    //aDroHomedTextLabel->SetDisplayType(Aspect_TypeOfDisplayText::Aspect_TODT_SUBTITLE); // Enable background color
    //aDroHomedTextLabel->SetColorSubTitle(Quantity_NOC_RED);
    aTextLabel->SetTransparency(dro.text_transparancy);

    aTextLabel->SetTransformPersistence(
                new Graphic3d_TransformPers(
                    Graphic3d_TMF_2d,
                    Aspect_TOTP_RIGHT_UPPER,
                    Graphic3d_Vec2i(dro.global_pos_x, dro.global_pos_y)));

    // Offset the text from the face leftlower corner.
    gp_Trsf trsf;
    trsf.SetTranslation({0,0,0},{dro.offset_x,dro.offset_y,dro.offset_z});
    aTextLabel->SetLocalTransformation(trsf);
    return aTextLabel;
}

AIS_TexturedShape* Opencascade::create_dro_background(DRO dro){

    BRepBuilderAPI_MakeFace aFace(gp_Pln(gp_Pnt(0,0,0),gp_Dir(0,0,1)),0,Standard_Real(dro.background_size_x),0,Standard_Real(dro.background_size_y));
    AIS_TexturedShape *aTextureShape=new AIS_TexturedShape(aFace.Shape());
    aTextureShape->SetTextureFileName(dro.background_picture.toStdString().c_str());
    aTextureShape->SetTransparency(dro.background_transparancy); // Value between 0 and 1.
    aTextureShape->SetTextureMapOn();
    aTextureShape->SetDisplayMode(3); // Diplay the picture.
    aTextureShape->SetZLayer(-2);
    aTextureShape->SetTransformPersistence(
                new Graphic3d_TransformPers(
                    Graphic3d_TMF_2d,
                    Aspect_TOTP_RIGHT_UPPER,
                    Graphic3d_Vec2i(dro.global_pos_x, dro.global_pos_y)));
    return aTextureShape;

    // m_context_dro->Display(aTextureShape,Standard_False);
    // Do not highlight on mouse hover //https://dev.opencascade.org/content/highlight-without-select-object
    // m_context_dro->Activate(TopAbs_FACE, Standard_False );
}

void Opencascade::paintEvent(QPaintEvent *)
{
    if (m_context.IsNull())
    {
        m_initialize_context();
    }
    m_view->Redraw();
}

void Opencascade::resizeEvent(QResizeEvent *)
{
    if( !m_view.IsNull() )
    {
        m_view->MustBeResized();
    }
}

void Opencascade::mousePressEvent(QMouseEvent *event)
{
    if((event->buttons()&Qt::LeftButton) && (event->buttons()&Qt::RightButton))
    {
        m_x_max=event->x();
        m_y_max=event->y();
    }
    else if(event->buttons()&Qt::LeftButton)
    {
        m_context->MoveTo(event->pos().x(),event->pos().y(),m_view,Standard_True);

        AIS_StatusOfPick t_pick_status = AIS_SOP_NothingSelected;
        if(qApp->keyboardModifiers()==Qt::ControlModifier)
        {
            t_pick_status = m_context->ShiftSelect(true);
        }
        else
        {
            t_pick_status = m_context->Select(true);
        }
    }
    else if(event->buttons()&Qt::MidButton)
    {
        m_x_max=event->x();
        m_y_max=event->y();
        m_view->StartRotation(event->x(),event->y());
    }

    get_selections();
}

void Opencascade::mouseReleaseEvent(QMouseEvent *event)
{
    m_context->MoveTo(event->pos().x(),event->pos().y(),m_view,Standard_True);
}

void Opencascade::mouseMoveEvent(QMouseEvent *event)
{
    if((event->buttons()&Qt::LeftButton) && (event->buttons()&Qt::RightButton))
    {
        m_view->Pan(event->pos().x()-m_x_max,m_y_max-event->pos().y());
        m_x_max=event->x();
        m_y_max=event->y();
    }
    else if(event->buttons()&Qt::MidButton)
    {
        if(qApp->keyboardModifiers()==Qt::ShiftModifier)
        {
            m_view->Pan(event->pos().x()-m_x_max,m_y_max-event->pos().y());
            m_x_max=event->x();
            m_y_max=event->y();
        }
        else
        {
            m_view->Rotation(event->x(),event->y());
        }
    }
    else
    {
        m_context->MoveTo(event->pos().x(),event->pos().y(),m_view,Standard_True);
    }
    //std::cout<<"event x: "<<event->pos().x()<<std::endl;
    //std::cout<<"event y: "<<event->pos().y()<<std::endl;

    //std::cout<<"pos toplevel x"<<this->topLevelWidget()->pos().x()<<std::endl;
    //std::cout<<"pos toplevel y"<<this->topLevelWidget()->pos().y()<<std::endl;
}

void Opencascade::wheelEvent(QWheelEvent *event)
{
    m_view->StartZoomAtPoint(event->position().x(),event->position().y());
    m_view->ZoomAtPoint(0, 0, event->angleDelta().y(), 0);
}

void Opencascade::set_orthographic(){
    m_view->Camera()->SetProjectionType (Graphic3d_Camera::Projection_Orthographic);
    m_view->Update();
}

void Opencascade::set_perspective(){
    m_view->Camera()->SetProjectionType (Graphic3d_Camera::Projection_Perspective);
    m_view->Redraw();
    //m_view->Update();
}

void Opencascade::set_view_front()
{
    m_view->SetProj( V3d_Yneg );
}

void Opencascade::set_view_back()
{
    m_view->SetProj( V3d_Ypos );
}

void Opencascade::set_view_top()
{
    m_view->SetProj( V3d_Zpos );
}

void Opencascade::set_view_bottom()
{
    m_view->SetProj( V3d_Zneg );
}

void Opencascade::set_view_left()
{
    m_view->SetProj( V3d_Xneg );
}
void Opencascade::set_view_right()
{
    m_view->SetProj( V3d_Xpos );
}

void Opencascade::erase_all()
{
    m_context->EraseAll(0);
}

void Opencascade::zoom_all()
{
    m_view->FitAll();
}

void Opencascade::test(QString test)
{
    std::cout<<"TEST string:"<<test.toStdString()<<std::endl;
}
void Opencascade::test1(QVariant data)
{
    QStringList l=data.toStringList();
    for(int i=0; i<l.size(); i++){
        std::cout<<"list at i:"<<i<<" value:"<<l.at(i).toStdString()<<std::endl;
    }
}








