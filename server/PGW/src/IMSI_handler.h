#pragma once

#include <string>
#include <iostream>
#include <vector>

class IMSI{
public:
    std::string getIMSI();
    bool setIMSI(std::string imsi);
private:
    bool checkIMSI(std::string imsi);
    std::string imsi;
};

class BlackList{
public:

private:
    std::vector<IMSI> black_list;
};