#include <iostream>
#include "UDPUnicast.h"
#include "BusinessHandler.h"

UDPUnicast::UDPUnicast()
    :port(port) {
    socketFd = socket(AF_INET, SOCK_DGRAM, 0);
    if (socketFd < 0) {
        throwError("Socket creation failed: " + std::string(strerror(errno)));
    }
}


UDPUnicast::~UDPUnicast() {
    stop();
}


void UDPUnicast::setOnError(const std::function<void(const std::string &)> &callback) {
    onError = callback;
}

void UDPUnicast::stop() {
    // if (!running) return;
    // running = false;
    if (socketFd >= 0) {
        close(socketFd);
        socketFd = -1;
    }
}

void UDPUnicast::sendTo(const std::string &ip, int port, const std::vector<uint8_t> &data) {
    if (socketFd < 0) {
        throwError("Socket is not initialized: " + std::string(strerror(errno)));
    }

    struct sockaddr_in sendAddr{};
    memset(&sendAddr, 0, sizeof(sendAddr));
    sendAddr.sin_family = AF_INET;
    sendAddr.sin_port = htons(port);

    if (inet_pton(AF_INET, ip.c_str(), &sendAddr.sin_addr) <= 0) {
        throwError("Invalid IP address: " + ip);
    }

    ssize_t sentLen = sendto(socketFd, data.data(), data.size(), 0, (struct sockaddr *)&sendAddr, sizeof(sendAddr));
    
    if (sentLen < 0) {
        std::string errorMsg = "[Failed to send data to " + ip + ":" + std::to_string(port) + 
                               " - " + strerror(errno) + "]";
        cerrError(errorMsg);
        return;  
    }

    std::cout << "Sent " << sentLen << " bytes to " << ip << ":" << port << std::endl;
}


// Fuction call 1 time to received Data
void UDPUnicast::receiveData(std::vector<uint8_t>& data){
    if (socketFd < 0) {
        throw std::runtime_error("Socket is not initialized");
    }

    char buffer[1024];
    struct  sockaddr_in senderAddr{};
    socklen_t senderAddrLen = sizeof(senderAddr);
    ssize_t recvData = recvfrom(socketFd, buffer, sizeof(buffer),0,
                        (struct sockaddr *)& senderAddr, &senderAddrLen);
    
    std::cout << "UDPUnicast Received Data: ";
    for(int i = 0; i < recvData; i++){
        std::cout<< std::hex << std::setw(2) << std::setfill('0') << (int)(unsigned char)buffer[i] << " ";
    }
    std::cout<<std::endl;

    if(recvData < 0){
        // Write Error into log
        cerrError("[Receiving data failed]");
    }
    data.assign(buffer, buffer + recvData);
    // Optional: Log sender information
    char senderIP[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &senderAddr.sin_addr, senderIP, sizeof(senderIP));
    std::cout << "Received " << std::dec << recvData << " bytes from " << senderIP
              << ":" << ntohs(senderAddr.sin_port) << std::endl;
}

// Fuction bind to Port
void UDPUnicast::bindToPort(int port) {
    if (socketFd < 0) {
        // Write Error into log
        throwError("[Socket is not initialized]");
    }

    struct sockaddr_in receivedAddr{};
    memset(&receivedAddr, 0, sizeof(receivedAddr));
    receivedAddr.sin_family = AF_INET;
    receivedAddr.sin_port = htons(port);
    receivedAddr.sin_addr.s_addr = INADDR_ANY;

    // Allow address reuse
    int optval = 1;
    if (setsockopt(socketFd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0) {
        // Write Error into log
        throwError("[Reuse socket failed]");
    }
    if (bind(socketFd, (struct sockaddr *)&receivedAddr, sizeof(receivedAddr)) < 0) {
        // Write Error into log
        throwError("[Socket binding failed]");
    } else {
        std::cout << "Bind successful on port " << port << std::endl;
    }
}



void UDPUnicast::setOnDataReceived(const std::function<void(const std::vector<uint8_t> &)> &callback) {
    onDataReceived = callback;
}

void UDPUnicast::listenfromUnicast(){
    processIncomingDatagrams();
}
void UDPUnicast::processIncomingDatagrams() {
    if (socketFd < 0) {
        cerrError("Socket is not initialized, cannot receive data!");
        return;
    }

    char buffer[4096];
    while (true) {
        sockaddr_in senderAddr{};
        socklen_t addrLen = sizeof(senderAddr);

        ssize_t recvLen = recvfrom(socketFd, buffer, sizeof(buffer), 0, 
                                   (struct sockaddr *)&senderAddr, &addrLen);

        if (recvLen > 0) {
            std::vector<uint8_t> data(buffer, buffer + recvLen);
            if (onDataReceived) {
                onDataReceived(data);
            }
        } else if (recvLen < 0) {
            perror("Receive failed");
            if (onError) onError("Receive failed");
        }
    }
}
