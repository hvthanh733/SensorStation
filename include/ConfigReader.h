#ifndef CONFIGREADER_H
#define CONFIGREADER_H

#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <cstdint>
#include <stdexcept>
#include <regex>
#include <sstream>
#include "Saver.h"

class ConfigReader{
public:
    ConfigReader(const std::string &filePath);
    
    std::string getValue(const std::string &section, const std::string &key) const;
    
    // Check type Integer
    int getIntValue(const std::string &section, const std::string &key) const;
    std::string fileName = std::filesystem::path(__FILE__).filename().string();

private:
    std::map<std::string, std::map<std::string, std::string>> configData;
    std::string getFilename();
    void parseConfig(const std::string &filePath);
    std::string trim(const std::string &str);
    std::string error;
};

#endif