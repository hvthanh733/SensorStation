#ifndef BUSINESSHANDLER_H
#define BUSINESSHANDLER_H

#include <iostream>
#include <thread>
#include "ConfigReader.h"
#include "Saver.h"
#include "UDPUnicast.h"

void callError(std::string error);
void throwError(const std::string &message);
void cerrError(const std::string &message);

class BusinessHandler{
public:
    BusinessHandler(const std::string& configfile);
    void run();  
    void heartbeatSending();
    void recevfromSensor();

private:
    ConfigReader config;
    bool checkConfig();
    std::vector<uint8_t> heartbeatData = {0x00, 0x01};
    bool running;
 
};



#endif