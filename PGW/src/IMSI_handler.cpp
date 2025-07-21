#include "IMSI_handler.h"

std::string IMSI::getIMSI()
{
    return imsi;
}

bool IMSI::setIMSI(std::string imsi)
{
    if(checkIMSI(imsi)){
        this->imsi = imsi;
        return true;
    }
    else std::cout << "your IMSI isn't corrent" << std::endl;
    return false;

}

bool IMSI::checkIMSI(std::string imsi)
{
    if(imsi.size() != 15)
        return false;
    for(auto &x : imsi){
        if (!(x > 47 && x < 58))
            return false;
    }
    return true;
}
