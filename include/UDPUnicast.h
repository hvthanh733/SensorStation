#ifndef UDPUNICAST_H
#define UDPUNICAST_H

#include <iostream>
#include <vector>
#include <cstdint>
#include <stdexcept>
#include <cstring>       // memset
#include <sys/types.h>   // socket types
#include <sys/socket.h>  // socket(), setsockopt()
#include <netinet/in.h>  // sockaddr_in, INADDR_ANY, htons()
#include <arpa/inet.h>   // inet_addr()
#include <unistd.h>      // close()
#include <functional>
#include <string>
#include <iomanip>
#include "Saver.h"

class UDPUnicast{
public:
    UDPUnicast();
    ~UDPUnicast();
    void bindToPort(int port);
    void receiveData(std::vector<uint8_t>& data);
    void sendTo(const std::string &ip, int port, const std::vector<uint8_t> &data);
    void stop();
    void listenfromUnicast();
    void setOnDataReceived(const std::function<void(const std::vector<uint8_t> &)> &callback);
    void setOnError(const std::function<void(const std::string &)> &callback);
    std::string fileName = std::filesystem::path(__FILE__).filename().string();

private:
    void processIncomingDatagrams();
    int port;
    int socketFd;
    std::function<void(const std::vector<uint8_t> &)> onDataReceived;
    std::function<void(const std::string &)> onError;
};


#endif 