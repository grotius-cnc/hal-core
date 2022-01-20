#include "read_textfile.h"
#include "vector"

read_textfile::read_textfile()
{

}

std::vector<std::string> read_textfile::read(std::string filename){
    std::vector<std::string> strvec;
    std::fstream newfile;
    std::string str;
    newfile.open(filename, std::ios::in);
    if(newfile.is_open()){
        while(getline(newfile,str)){
            if(str=="/*"){
                break;
            }
            strvec.push_back(str);
        }
        newfile.close();
    }
    return strvec;
}

