#include "CDR.h"

CDR::CDR(){
    //path = checkPath(); // ? Может быть сделать bash скрипт для создания журнала
    file.open(path, std::ios::in | std::ios::out);
}

std::string CDR::readLine()
{
    std::string result = "";
    std::getline(file, result);
    if(file.eof()){
        std::cout << "You have reached the end of the file" << std::endl;
    }
    return result;
}

CDR::~CDR(){
    file.close();
}

void CDR::writeLine(std::string data){
    file.seekp(0, std::ios::end);
    file << '\n' + data;
}

void CDR::resetOffset(){
    file.clear();
    file.seekg(0, std::ios::beg);
    file.seekp(0, std::ios::beg);
}