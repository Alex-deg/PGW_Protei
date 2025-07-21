#include "functions.h"

std::string checkPath()
{
    std::string correctPath;
    std::fstream file;
    file.exceptions(std::fstream::failbit | std::fstream::badbit);
    while(true){
        system("clear");
        std::cout << "Enter file name: ";
        std::cin >> correctPath;
        try{
            file.open(correctPath, std::ios::in | std::ios::out);
            file.close();
            return correctPath;
        }
        catch(const std::exception &e){
            std::cout << "File is not exist" << std::endl;
            std::cerr << e.what() << std::endl;
            system("read -p 'Press Enter to continue...' var");
        }
        
    }   
}

void cinFlush(){
    std::cin.clear();
    while(std::cin.get() != '\n');
}