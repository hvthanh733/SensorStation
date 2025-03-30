#include <iostream>
#include "ConfigReader.h"
#include "BusinessHandler.h"

// Constructor
ConfigReader::ConfigReader(const std::string &filePath) {
    parseConfig(filePath);
}

// Helper function to trim whitespace from a string
std::string ConfigReader::trim(const std::string &str) {
    //    (space)
    // \t (tab)
    // \n (endl)
    // \r (return start of line)-
    size_t first = str.find_first_not_of(" \t\n\r");
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, last - first + 1);
}

void ConfigReader::parseConfig(const std::string &filePath) {
    // Open file .ini
    std::ifstream file(filePath);
    if (!file) {
        throwError("[Unable to open file: " +filePath+ "]");
    }

    std::string line, currentSection;
    while(std::getline(file, line)){
        line = trim(line);
        // Skip comments and empty lines
        if (line.empty() || line[0] == ';' || line[0] == '#') continue;

        if(line[0] == '[' && line.back() == ']'){
            currentSection = line.substr(1, line.size() - 2);
            currentSection = trim(currentSection);
            
        }
        // Handle key-value pairs
        else if (!currentSection.empty()) {
            size_t equalPos = line.find('=');
            if (equalPos == std::string::npos) continue;
            // get Key of Section
            std::string key = trim(line.substr(0, equalPos));

            // get Value of Key
            std::string value = trim(line.substr(equalPos + 1));

            configData[currentSection][key] = value;
        }
    }
}



// Function get string value 
std::string ConfigReader::getValue(const std::string &section, const std::string &key) const {
    auto sectionIt = configData.find(section);
    if (sectionIt == configData.end()) {
        // Write Error into log
        throwError("[Section not found: " + section + "]");
    }

    auto keyIt = sectionIt->second.find(key);
    if (keyIt == sectionIt->second.end()) {
        // Write Error into log
        throwError("[Key of "+section+" not found]");
    }

    return keyIt->second;
}

// Function get an integer value
int ConfigReader::getIntValue(const std::string &section, const std::string &key) const {
    try {
        auto sectionIt = configData.find(section);
    
        std::string valueStr = getValue(section, key);  
        
        std::regex pattern("^[0-9]+$");
        if (!std::regex_match(valueStr, pattern)) {
            throwError("[Value contains non-numeric or have special characters: " + valueStr + "]");
        }

        int value = std::stoi(valueStr);

        return value;
    } catch (const std::invalid_argument &e) {
        // Write Error into log
        throwError("[Invalid integer value for key: " + section + " - " + key +"]");
    } catch (const std::out_of_range &e) {
        // Write Error into log
        throwError("[Integer value out of range for key: " + section + " - " + key +"]");
    }
    return 0;
}
