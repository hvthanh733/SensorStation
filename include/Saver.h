#ifndef SAVER_H
#define SAVER_H

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <regex>
#include <mutex>

class Saver{
public:
    Saver(const std::string& name, const std::string& nameFile, const std::string& data, std::string nameSensor = "");
    void writeError2File();
    void writeData2File();
private:
    std::string directoryPath;
    std::string namefile;
    std::string sensor;
    std::string data;
    std::string Pathfile;
    std::ofstream file;


};



#endif