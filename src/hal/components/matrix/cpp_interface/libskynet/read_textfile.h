#ifndef READ_TEXTFILE_H
#define READ_TEXTFILE_H

#include "external_var.h"
#include "iostream"
#include "fstream"
#include "vector"

class read_textfile
{
public:
    read_textfile();
    std::vector<std::string> read(std::string filename);
};

#endif // READ_TEXTFILE_H
