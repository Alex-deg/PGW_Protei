#include "FileHandler.h"

FileHandler::FileHandler(std::string path){
    //path = checkPath(); // ? Может быть сделать bash скрипт для создания журнала
    this->path = path;
    file.open(path, std::ios::in | std::ios::out);
    if(file.is_open()){
        Logger::info("Файл с путем = " + path + " открыт успешно");
        std::cout << "success with " + path << std::endl;
    }
    else {
        Logger::warn("Файл не существует!");
        std::cout << ":( with " + path << std::endl;
    }
}

std::string FileHandler::readLine()
{
    std::string result = "";
    std::getline(file, result);
    if(file.eof()){
        Logger::warn("Достигнут конец файла!");
        std::cout << "You have reached the end of the file" << std::endl;
    }
    Logger::info("Чтение строки из файла!");
    return result;
}

FileHandler::~FileHandler(){
    file.close();
}

void FileHandler::writeLine(std::string data){
    file.seekp(0, std::ios::end);
    file << '\n' + data;
}

void FileHandler::setPath(std::string path){
    this->path = path;
}

void FileHandler::resetOffset(){
    file.clear();
    file.seekg(0, std::ios::beg);
    file.seekp(0, std::ios::beg);
}