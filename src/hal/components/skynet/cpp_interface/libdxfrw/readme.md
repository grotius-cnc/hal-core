If you never used this dxfrw lib. You are lucky.

A example to write a dxf file, this took me quite a while to figur out, the dxfrw has a cruel documentaton lack :

    #include <libdxfrw/libdxfrw.h>
    #include "examplewriter.h"

    dxfRW dxf("file.dxf");
    //! Subclassing. Not doing other stuff.
    ExampleWriter writer(dxf);
    
    //! Fill up the subclassed "writer" with data.
    DRW_Line line;
    line.basePoint.x = 10.20;
    line.basePoint.y = 20.10;
    line.secPoint.x = 30.25;
    line.secPoint.y = 30;
    writer.linevec.push_back(line);
    
    //! Here it will write all data at once to the dxf file.
    dxf.write(&writer, DRW::Version::AC1027, false);
    
A example to read a dxf file :    

  `~ /libdxfrw_functions `
