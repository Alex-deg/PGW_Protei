#include "FileHandler.h"

FileHandler::FileHandler(std::string path){
    //path = checkPath(); // ? Может быть сделать bash скрипт для создания журнала
    this->path = path;
    file.open(path, std::ios::in | std::ios::out);
    if(file.is_open())
        std::cout << "success" << std::endl;
    else std::cout << "sosal" << std::endl;
}

std::string FileHandler::readLine()
{
    std::string result = "";
    std::getline(file, result);
    if(file.eof()){
        std::cout << "You have reached the end of the file" << std::endl;
    }
    return result;
}

FileHandler::~FileHandler(){
    file.close();
}

void FileHandler::writeLine(std::string data){
    std::cout << "writing in file..." << std::endl;
    std::cout << "data : " << data << std::endl;
    file.seekp(0, std::ios::end);
    file << '\n' + data;
}

void FileHandler::resetOffset(){
    file.clear();
    file.seekg(0, std::ios::beg);
    file.seekp(0, std::ios::beg);
}