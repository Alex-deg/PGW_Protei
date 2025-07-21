#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include "functions.h"

// Реализовать логику .csv файла и настроить вывод

class CDR{
public:
    CDR();
    std::string readLine();
    void writeLine(std::string data);
    void resetOffset();
    ~CDR();
private:
    std::string path = "CDR_journal.csv";
    std::fstream file;
};