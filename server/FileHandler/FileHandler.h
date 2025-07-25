#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include "functions.h"
#include "../Logger/logger.h"

// Реализовать логику .csv файла и настроить вывод

class FileHandler{
public:
    FileHandler(std::string path = "../../Data/CDR_journal.csv");
    std::string readLine();
    void writeLine(std::string data);
    void setPath(std::string path);
    void resetOffset();
    ~FileHandler();
private:
    std::string path;
    std::fstream file;
};