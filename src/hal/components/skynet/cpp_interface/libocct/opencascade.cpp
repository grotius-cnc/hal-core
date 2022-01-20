#include "opencascade.h"

#include <OSD.hxx>
#include <AIS_Trihedron.hxx>
#include <AIS_ViewCube.hxx>
#include <AIS_Selection.hxx>
#include <AIS_ColoredShape.hxx>
#include <AIS_ColoredDrawer.hxx>
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

Opencascade::Opencascade(QWidget *parent) : QGLWidget(parent)
{
    setBackgroundRole( QPalette::NoRole );
    setMouseTracking( true );
}

bool Opencascade::Readstepfile(const std::string& theStepName, bool end_of_assembly)
{
    // std::cout<<"Start of visit readstepfilefunction"<<std::endl;
    //! Empty temponairy bucketvec, the visit funtion will fill up the bucket for one stepfile.
    Ais_ShapeVec.clear();

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

    m_view->FitAll();

    // std::cout<<"End of visit readstepfile function"<<std::endl;
    //! Time to put the Ais_ShapeVec data into the Assembly.

    //! The stepfile including sub shapes.
    STEPFILE f;
    f.Ais_ShapeVec.resize(Ais_ShapeVec.size());
    f.Ais_ShapeVec=Ais_ShapeVec;
    Stepfilevec.push_back(f);

    //! When multiple stepfiles are loaded trigger the end of assembly input.
    if(end_of_assembly){
        std::cout<<"End of assembly "<<std::endl;

        ASSEBMLY a;
        a.stepfiles.resize(Stepfilevec.size());
        a.stepfiles=Stepfilevec;
        AssemblyVec.push_back(a);
        Stepfilevec.clear();

        //! Add previewing the last added assembly of the : AssemblyVec.
        for(unsigned int i=0; i<AssemblyVec.back().stepfiles.size(); i++){
            for(unsigned int j=0; j<AssemblyVec.back().stepfiles.at(i).Ais_ShapeVec.size(); j++){
                m_context->Display(AssemblyVec.back().stepfiles.at(i).Ais_ShapeVec.at(j),Standard_False);
            }
        }
    }

    //! Attach a 3d tcp origin to the machine
    tcpx=draw_primitives().draw_3d_line({0,0,0},{100,0,0});
    tcpx=draw_primitives().colorize(tcpx,Quantity_NOC_RED,0);

    m_context->Display(tcpx,false);

    tcpy=draw_primitives().draw_3d_line({0,0,0},{0,100,0});
    tcpy=draw_primitives().colorize(tcpy,Quantity_NOC_GREEN,0);
    m_context->Display(tcpy,false);

    tcpz=draw_primitives().draw_3d_line({0,0,0},{0,0,100});
    tcpz=draw_primitives().colorize(tcpz,Quantity_NOC_BLUE,0);
    m_context->Display(tcpz,false);

    return 1;
}

void Opencascade::Visit(const TDF_Label& theLabel)
{
    //theLabel.EntryDump(std::cout);
    Quantity_Color aColor;
    TopoDS_Shape aShape;
    Handle(TDataStd_Name) aName;
    if (theLabel.FindAttribute(TDataStd_Name::GetID(), aName)){
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
            Ais_ShapeVec.push_back(new AIS_Shape(aShape));
            //if(aColor.Name()!=Quantity_NOC_YELLOW && aColor.Name()!=Quantity_NOC_RED){
            Ais_ShapeVec.back()->SetColor(aColor);
            //}
            Ais_ShapeVec.back()->SetDisplayMode(AIS_Shaded);
            Ais_ShapeVec.back()->Attributes()->SetFaceBoundaryDraw(true);
            Ais_ShapeVec.back()->Attributes()->SetFaceBoundaryAspect(
                        new Prs3d_LineAspect(Quantity_NOC_BLACK, Aspect_TOL_SOLID, 1.));
            Ais_ShapeVec.back()->Attributes()->SetIsoOnTriangulation(true);
            //! Temporairy stepfile preview:
            m_context->Display(Ais_ShapeVec.back(),Standard_False);
        }
    }
    /*! Repeat the visit function for each childmember. */
    for (TDF_ChildIterator c(theLabel); c.More(); c.Next())
    {
        Visit(c.Value());
    }
    //std::cout<<"end of visit b"<<std::endl;
}

void Opencascade::Move_assembly_joints(MACHINE *ptr){

    //! Create a transformation matrix for every assembly. Also create a offset matrix.
    gp_Trsf MyTrsfResult[AssemblyVec.size()],trsf1[AssemblyVec.size()];
    gp_Trsf Trsf_HAL;

    //! Iterate trough the projects:
    for(unsigned int i=0; i<AssemblyVec.size(); i++){

        //! Apply user defined hal input parameters for rotation and translation.
        gp_Trsf Trsf;
        gp_Quaternion aQuat;
        aQuat.SetEulerAngles (gp_YawPitchRoll, ptr[i].machine_rotation.z, ptr[i].machine_rotation.y, ptr[i].machine_rotation.x);
        Trsf.SetRotation(aQuat);
        Trsf_HAL.SetTranslation({0,0,0},{ptr[i].machine_offset.x,ptr[i].machine_offset.y,ptr[i].machine_offset.z});
        Trsf_HAL.Multiply(Trsf);
        MyTrsfResult[i].Multiply(Trsf_HAL);

        //! Machine base offset and rotation.
        double x=0;
        double y=0;
        double z=0;
        for(unsigned int j=0; j<AssemblyVec.at(i).stepfiles.size()-1; j++){

            //! Add new axis endpoint to the kinematic chain value.
            //! This are hal input parameters that defines the kinematic model xyz offsets.
            x+=ptr[i].joint[j].axis_endpoint.x;
            y+=ptr[i].joint[j].axis_endpoint.y;
            z+=ptr[i].joint[j].axis_endpoint.z;

            //! Current joint, axis value, units: mm - radians.
            double value=ptr[i].joint[j].curpos;

            gp_Trsf MyTrsf;
            if(ptr[i].joint[j].rotationtype==0){
                // rotx
                MyTrsf.SetRotation(gp_Ax1(gp_Pnt(x,y,z), gp_Dir(1,0,0)), value);
            }
            if(ptr[i].joint[j].rotationtype==1){
                // roty
                MyTrsf.SetRotation(gp_Ax1(gp_Pnt(x,y,z), gp_Dir(0,1,0)), value);
            }
            if(ptr[i].joint[j].rotationtype==2){
                // rotz
                MyTrsf.SetRotation(gp_Ax1(gp_Pnt(x,y,z), gp_Dir(0,0,1)), value);
            }
            if(ptr[i].joint[j].rotationtype==3){
                // transx
                MyTrsf.SetTranslation({0,0,0},{value,0,0});
            }
            if(ptr[i].joint[j].rotationtype==4){
                // transy
                MyTrsf.SetTranslation({0,0,0},{0,value,0});
            }
            if(ptr[i].joint[j].rotationtype==5){
                // transz
                MyTrsf.SetTranslation({0,0,0},{0,0,value});
            }
            if(ptr[i].joint[j].rotationtype==6){
                // rotaxis
                //! Have to look in kdl what this for type is.
            }
            if(ptr[i].joint[j].rotationtype==7){
                // transaxis
                //! Have to look in kdl what this for type is.
            }
            if(ptr[i].joint[j].rotationtype==8){
                // none
                //! Kdl type::none
            }

            //! Create new position in the matrix multiplication chain.
            //! This is matrix multiplication to get a new 3d situation.
            MyTrsfResult[i].Multiply(MyTrsf);
            AssemblyVec.at(i).stepfiles.at(j).MyTrsf=MyTrsfResult[i];
        }
    }

    //! Apply results
    for(unsigned int i=0; i<AssemblyVec.size(); i++){
        //! For the first stepfile wich represents the fixed machine base, apply the Trsf_HAL
        for(unsigned int j=0; j<2; j++){
            for(unsigned int k=0; k<AssemblyVec.at(i).stepfiles.at(j).Ais_ShapeVec.size(); k++){
                AssemblyVec.at(i).stepfiles.at(j).Ais_ShapeVec.at(k)->SetLocalTransformation(Trsf_HAL);
            }
        }

        for(unsigned int j=1; j<AssemblyVec.at(i).stepfiles.size(); j++){
            for(unsigned int k=0; k<AssemblyVec.at(i).stepfiles.at(j).Ais_ShapeVec.size(); k++){
                //! Apply the matrix of the previous joint or axis.
                AssemblyVec.at(i).stepfiles.at(j).Ais_ShapeVec.at(k)->SetLocalTransformation(AssemblyVec.at(i).stepfiles.at(j-1).MyTrsf);
            }
        }
    }

    //! Move the 3d tcp origin with the machine
    tcpx=draw_primitives().rotate_translate_3d_quaternion(tcpx,{e_ptr[0].curpos_cart_euler[0],
                                                                e_ptr[0].curpos_cart_euler[1],
                                                                e_ptr[0].curpos_cart_euler[2]},
                                                                e_ptr[0].curpos_cart_euler[5],
                                                                e_ptr[0].curpos_cart_euler[4],
                                                                e_ptr[0].curpos_cart_euler[3]);

    tcpy=draw_primitives().rotate_translate_3d_quaternion(tcpy,{e_ptr[0].curpos_cart_euler[0],
                                                                e_ptr[0].curpos_cart_euler[1],
                                                                e_ptr[0].curpos_cart_euler[2]},
                                                                e_ptr[0].curpos_cart_euler[5],
                                                                e_ptr[0].curpos_cart_euler[4],
                                                                e_ptr[0].curpos_cart_euler[3]);

    tcpz=draw_primitives().rotate_translate_3d_quaternion(tcpz,{e_ptr[0].curpos_cart_euler[0],
                                                                e_ptr[0].curpos_cart_euler[1],
                                                                e_ptr[0].curpos_cart_euler[2]},
                                                                e_ptr[0].curpos_cart_euler[5],
                                                                e_ptr[0].curpos_cart_euler[4],
                                                                e_ptr[0].curpos_cart_euler[3]);
     m_context->CurrentViewer()->Redraw();
}

void Opencascade::set_background_image(std::string file){
    // https://sourceforge.net/projects/freeimage/
    Standard_CString FileName(file.c_str());
    Aspect_FillMethod FillStyle(Aspect_FM_STRETCH);//Aspect_FM_STRETCH
    Standard_Boolean update(Standard_True);
    m_view->SetBackgroundImage(FileName, FillStyle, update) ;
}

void Opencascade::show_shape(Handle(AIS_Shape) shape){
    m_context->Display(shape,Standard_False);
}

void Opencascade::parse_gcode(GCODE gcode){

    std::vector<BLOCK> blockvec=gcode.blockvec;

    gp_Pnt p1{0,0,0},p2{0,0,0},pc{0,0,0};

    // Set gcode positions from hal user input.
    double x=gcode.offset.x;
    double y=gcode.offset.y;
    double z=gcode.offset.z;
    double euler_x=gcode.rotation.x;
    double euler_y=gcode.rotation.y;
    double euler_z=gcode.rotation.z;

    // Remove previous gcode
    erase_gcode();

    // Draw gcode
    for(unsigned int i=0; i<blockvec.size(); i++){

        p2={blockvec.at(i).X,blockvec.at(i).Y,blockvec.at(i).Z};
        double I=blockvec.at(i).I;
        double J=blockvec.at(i).J;
        double K=blockvec.at(i).K;
        pc.SetX(p1.X()+I);
        pc.SetY(p1.Y()+J);
        pc.SetZ(p1.Z()+K);

        if(i>0 && blockvec.at(i).blocktype==G0){
            Handle(AIS_Shape) Ais_shape=draw_primitives().draw_3d_line(p1,p2);
            Ais_shape=draw_primitives().colorize(Ais_shape,Quantity_NOC_BLUE,0);
            Ais_shape=draw_primitives().rotate_translate_3d_quaternion(Ais_shape,{x,y,z},euler_z,euler_y,euler_x);
            gcodevec.push_back(Ais_shape);

        }
        if(i>0 && blockvec.at(i).blocktype==G1){
            Handle(AIS_Shape) Ais_shape=draw_primitives().draw_3d_line(p1,p2);
            Ais_shape=draw_primitives().rotate_translate_3d_quaternion(Ais_shape,{x,y,z},euler_z,euler_y,euler_x);
            Ais_shape=draw_primitives().colorize(Ais_shape,Quantity_NOC_BLACK,0);
            gcodevec.push_back(Ais_shape);
        }
        if(i>0 && (blockvec.at(i).blocktype==G2)){

            // Arc section
            // X=xend, Y=yend. For arc given a G0 startposition and a XY endposition. http://linuxcnc.org/docs/html/gcode/g-code.html#gcode:g2-g3
            // I=offset xcenter-xstart, J=offset ycenter-ystart, G2=clockwise (cw), G3=counterclockwise (ccw)
            Handle(AIS_Shape) Ais_shape=draw_primitives().draw_cp_2d_arc(pc,p2,p1);
            Ais_shape=draw_primitives().rotate_translate_3d_quaternion(Ais_shape,{x,y,z},euler_z,euler_y,euler_x);
            Ais_shape=draw_primitives().colorize(Ais_shape,Quantity_NOC_BLACK,0);
            gcodevec.push_back(Ais_shape);
        }
        if(i>0 && (blockvec.at(i).blocktype==G3)){
            Handle(AIS_Shape) Ais_shape=draw_primitives().draw_cp_2d_arc(pc,p1,p2);
            Ais_shape=draw_primitives().rotate_translate_3d_quaternion(Ais_shape,{x,y,z},euler_z,euler_y,euler_x);
            Ais_shape=draw_primitives().colorize(Ais_shape,Quantity_NOC_BLACK,0);
            gcodevec.push_back(Ais_shape);
        }
        p1=p2;
    }

    //! Attach a 3d origin.
    Handle(AIS_Shape) Ais_shape=draw_primitives().draw_3d_line({0,0,0},{100,0,0});
    Ais_shape=draw_primitives().colorize(Ais_shape,Quantity_NOC_RED,0);
    Ais_shape=draw_primitives().rotate_translate_3d_quaternion(Ais_shape,{x,y,z},euler_z,euler_y,euler_x);
    gcodevec.push_back(Ais_shape);

    Ais_shape=draw_primitives().draw_3d_line({0,0,0},{0,100,0});
    Ais_shape=draw_primitives().colorize(Ais_shape,Quantity_NOC_GREEN,0);
    Ais_shape=draw_primitives().rotate_translate_3d_quaternion(Ais_shape,{x,y,z},euler_z,euler_y,euler_x);
    gcodevec.push_back(Ais_shape);

    Ais_shape=draw_primitives().draw_3d_line({0,0,0},{0,0,100});
    Ais_shape=draw_primitives().colorize(Ais_shape,Quantity_NOC_BLUE,0);
    Ais_shape=draw_primitives().rotate_translate_3d_quaternion(Ais_shape,{x,y,z},euler_z,euler_y,euler_x);
    gcodevec.push_back(Ais_shape);

    //! Draw the gcode.
    for(unsigned int i=0; i<gcodevec.size(); i++){
        m_context->Display(gcodevec.at(i),false);
    }
}

void Opencascade::erase_gcode(){
    for(unsigned int i=0; i<gcodevec.size(); i++){
        m_context->SetSelected(gcodevec.at(i),false);
        m_context->EraseSelected(false);
    }
    gcodevec.clear();
}

void Opencascade::move_gcode(POINT p, EULER e){
    for(unsigned int i=0; i<gcodevec.size(); i++){
        gcodevec.at(i)=draw_primitives().rotate_translate_3d_quaternion(gcodevec.at(i),{p.x,p.y,p.z},e.z,e.y,e.x);
        m_context->Redisplay(gcodevec.at(i),false);
    }
}

void Opencascade::get_selections(){ // Updated by jointpos function from mainwindow.

    for(m_context->InitSelected(); m_context->MoreSelected(); m_context->NextSelected()){

        const TopoDS_Shape& aSelShape = m_context->SelectedShape();
        std::cout<<"selected shape type:"<<aSelShape.ShapeType()<<std::endl;
        // Shapetype 6=line.

        //        for(unsigned int i=0; i<bucketvec.size(); i++){
        //            if(m_context->SelectedShape()==bucketvec.at(i).Ais_shape->Shape()){
        //                std::cout<<"match found at Ais_bucket i:"<<i<<std::endl;

        //                // Print some extra content:
        //                std::cout<<"primitivetype:"<<bucketvec.at(i).primitivetype<<std::endl;

        //                // Print euler content:
        //                for(unsigned int j=0; j<bucketvec.at(i).eulervec.size(); j++){
        //                    std::cout<<"index:"<<j<<"euler_z:"<<bucketvec.at(i).eulervec.at(j).Z()<<std::endl;
        //                    std::cout<<"index:"<<j<<"euler_y:"<<bucketvec.at(i).eulervec.at(j).Y()<<std::endl;
        //                    std::cout<<"index:"<<j<<"euler_x:"<<bucketvec.at(i).eulervec.at(j).X()<<std::endl;
        //                }
        //            }
        //        }

        TopExp_Explorer explorer;

        for(explorer.Init(aSelShape, TopAbs_EDGE); explorer.More(); explorer.Next()){

            const TopoDS_Edge& edge = TopoDS::Edge(explorer.Current());

            TopoDS_Vertex v1,v2;
            TopExp::Vertices(edge,v1,v2);
            gp_Pnt p1= BRep_Tool::Pnt(v1);
            gp_Pnt p2= BRep_Tool::Pnt(v2);

            std::cout<<"edge p1 x: "<<p1.X()<<" y:"<<p1.Y()<<" z:"<<p1.Z()<<std::endl;
            std::cout<<"edge p2 x: "<<p2.X()<<" y:"<<p2.Y()<<" z:"<<p2.Z()<<std::endl;

            // An attempt to get data from child items. Not working.
            //
            //            TopoDS_Iterator it(aSelShape);
            //            for(; it.More(); it.Next()){
            //                const TopoDS_Shape child=it.Value();
            //                std::cout<<"child shapetype:"<<child.ShapeType()<<std::endl;

            //                TopExp_Explorer expl;

            //                for(expl.Init(child, TopAbs_ShapeEnum(7)); expl.More(); expl.Next()){

            //                    /* Enum shape types.
            //                    TopAbs_COMPOUND,
            //                    TopAbs_COMPSOLID,
            //                    TopAbs_SOLID,
            //                    TopAbs_SHELL,
            //                    TopAbs_FACE,
            //                    TopAbs_WIRE,
            //                    TopAbs_EDGE,
            //                    TopAbs_VERTEX, ==> 7
            //                    TopAbs_SHAPE
            //                    */

            //                    std::cout<<"inside shape enum 7"<<std::endl;
            //                    const TopoDS_Vertex& vertex = TopoDS::Vertex(expl.Current());
            //                    gp_Pnt p= BRep_Tool::Pnt(vertex);
            //                    std::cout<<"    child vertex p x: "<<p.X()<<" y:"<<p.Y()<<" z:"<<p.Z()<<std::endl;

            //                }
            //            }

            std::cout<<"NEXT-SHAPE"<<std::endl;
        }

        // A example how to get points from a vertex.
        //
        //        for(explorer.Init(aSelShape, TopAbs_VERTEX); explorer.More(); explorer.Next()){
        //            const TopoDS_Vertex& vertex = TopoDS::Vertex(explorer.Current());
        //            gp_Pnt p1= BRep_Tool::Pnt(vertex);
        //            std::cout<<"vertex p x: "<<p1.X()<<" y:"<<p1.Y()<<" z:"<<p1.Z()<<std::endl;
        //        }
    }
}

void Opencascade::delete_selections(){

    //    std::cout<<"bucketvecsize before:"<<bucketvec.size()<<std::endl;
    //    int nr_to_remove=0;
    //    for(m_context->InitSelected(); m_context->MoreSelected(); m_context->NextSelected()){

    //        // Find the match of selected item in the Ais_databucket.
    //        // In this case one item can be removed each time.
    //        for(unsigned int i=0; i<bucketvec.size(); i++){

    //            if(m_context->SelectedShape()==bucketvec.at(i).Ais_shape->Shape()){
    //                //std::cout<<"match found, remove this item from Ais_bucket"<<std::endl;
    //                nr_to_remove=i;
    //            }
    //        }
    //        bucketvec.erase(bucketvec.begin()+nr_to_remove);
    //    }

    //    // Quick way to erase items.
    //    m_context->EraseSelected(false);

    //    // For check print content.

    //    for(unsigned int i=0; i<bucketvec.size(); i++){

    //        TopExp_Explorer explorer;
    //        for(explorer.Init(bucketvec.at(i).Ais_shape->Shape(), TopAbs_EDGE); explorer.More(); explorer.Next()){

    //            const TopoDS_Edge& edge = TopoDS::Edge(explorer.Current());

    //            TopoDS_Vertex v1,v2;
    //            TopExp::Vertices(edge,v1,v2);
    //            gp_Pnt p1= BRep_Tool::Pnt(v1);
    //            gp_Pnt p2= BRep_Tool::Pnt(v2);

    //            std::cout<<"content left edge p1 x: "<<p1.X()<<" y:"<<p1.Y()<<" z:"<<p1.Z()<<std::endl;
    //            std::cout<<"content left edge p2 x: "<<p2.X()<<" y:"<<p2.Y()<<" z:"<<p2.Z()<<std::endl;
    //        }
    //    }
    //    std::cout<<"bucketvecsize after:"<<bucketvec.size()<<std::endl;
}

void Opencascade::Redraw(){
    //m_context->CurrentViewer()->Redraw();
    m_view->Redraw();
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

        m_viewer->SetDefaultLights();
        m_viewer->SetLightOn();

        /// This is the interactivve 3d box
        //  View : top, bottom, side, 3d, etc.

        opencascade::handle<AIS_ViewCube> aisViewCube = new AIS_ViewCube;
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

        /// Set background homogenius, one color.
        //  m_view->SetBackgroundColor(Quantity_NOC_GRAY49);
        //  m_viewer->SetDefaultShadingModel(Graphic3d_TypeOfShadingModel::V3d_COLOR);

        /// Set background with gradient stylesheet
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

        Quantity_Color cola,colb;
        cola.SetValues(0.3,0.3,0.3,Quantity_TOC_RGB);
        colb.SetValues(0.6,0.6,0.6,Quantity_TOC_RGB);
        m_view->SetBgGradientColors(cola,colb,Aspect_GFM_DIAG2 , false);

        /// View as wireframe or shaded
        //m_context->SetDisplayMode(AIS_WireFrame, Standard_False);
        m_context->SetDisplayMode(AIS_Shaded, Standard_False);

        Handle(Prs3d_Drawer) t_hilight_style = m_context->HighlightStyle();
        t_hilight_style->SetMethod(Aspect_TOHM_COLOR);
        t_hilight_style->SetColor(Quantity_NOC_LIGHTYELLOW);
        t_hilight_style->SetDisplayMode(1);
        t_hilight_style->SetTransparency(0.2f);

        Handle(Prs3d_Drawer) t_select_style = m_context->SelectionStyle();
        t_select_style->SetMethod(Aspect_TOHM_COLOR);
        t_select_style->SetColor(Quantity_NOC_LIGHTSEAGREEN);
        t_select_style->SetDisplayMode(1);
        t_select_style->SetTransparency(0.4f);

        /// Show grid
        m_viewer->SetRectangularGridValues(0,0,1,1,0);
        m_viewer->SetRectangularGridGraphicValues(2.01,2.01,0);
        m_viewer->ActivateGrid(Aspect_GT_Rectangular,Aspect_GDM_Lines);

        /// Show triedron. This is the 3d axis cross at the lower left of the screen.
        m_view->TriedronDisplay(Aspect_TOTP_LEFT_LOWER, Quantity_NOC_GOLD, 0.08, V3d_ZBUFFER);

        /*
        /// Show triedron at a specific place on the screen
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

        m_view->MustBeResized();
    }

    set_background_image("/opt/hal-core/rtlib/background.jpg");
    Set_orthographic();
}

void Opencascade::Set_orthographic(){
    m_view->Camera()->SetProjectionType (Graphic3d_Camera::Projection_Orthographic);
    m_view->Update();
}

void Opencascade::Set_perspective(){
    m_view->Camera()->SetProjectionType (Graphic3d_Camera::Projection_Perspective);
    m_view->Redraw();
    //m_view->Update();
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
}

void Opencascade::wheelEvent(QWheelEvent *event)
{
    m_view->StartZoomAtPoint(event->position().x(),event->position().y());
    m_view->ZoomAtPoint(0, 0, event->angleDelta().y(), 0);
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










