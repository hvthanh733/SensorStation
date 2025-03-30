#include "BusinessHandler.h"

BusinessHandler::BusinessHandler(const std::string& configfile) 
    : running(false), config(configfile){}

void BusinessHandler::heartbeatSending() {
    try {
        // Read the server IP and port from the configuration
        std::string serverIp = config.getValue("Server", "ip");
        int serverPort = config.getIntValue("Server", "port");

        // Create a UDPUnicast instance for sending heartbeats
        UDPUnicast udpUnicastHeartbeat;

        std::cout << "Starting heartbeat to " << serverIp << ":" << serverPort << std::endl;

        // Continuous loop for sending heartbeats periodically
        while (true) {
            try {
                // Send the heartbeat to the configured server IP and port
                udpUnicastHeartbeat.sendTo(serverIp, serverPort, heartbeatData);

                // Log the successful heartbeat transmission
                std::cout << "Sending heartbeat to " << serverIp << ":" << serverPort << std::endl;

                // Wait for 200 milliseconds before sending the next heartbeat
                std::this_thread::sleep_for(std::chrono::milliseconds(200));
            } catch (const std::exception& e) {
                // Catch and log any errors during heartbeat sending
                cerrError(std::string("[Error sending heartbeat: ") + e.what() +"]");
            }
        }
    } catch (const std::exception& e) {
        // Catch and log any errors during initialization of heartbeats
        // Write Error into log
        cerrError(std::string("[Error in heartbeatSending: ") + e.what() +"]");
    }
}

// Function check the configuration
bool BusinessHandler::checkConfig(){
    // 1. Validate server configuration
    std::string serverIp = config.getValue("Server", "ip");
    int serverPort = config.getIntValue("Server", "port");
    if (serverIp.empty() || serverPort <= 0 || serverPort > 65535) {
        // Write Error into log
        throwError("[Invalid server IP or port in configuration.]");
    }

    // 2. Validate Sensor and Station ports
    std::string sensorIp  = config.getValue("Sensor", "ip");
    int sensorPort        = config.getIntValue("Sensor", "port");
    int stationPort    = config.getIntValue("Station", "port");
    if (sensorIp.empty() || sensorPort <= 0 || sensorPort > 65535|| stationPort <= 0 || stationPort > 65535){
        // Write Error into log
        throwError( "[Invalid Sensor IP, Sensor port, or Station port in configuration.]");
    }

    // If execution reaches here, all checks have passed
    return true;
}

// Function received data from Sensor
void BusinessHandler::recevfromSensor(){
    std::string fileName = std::filesystem::path(__FILE__).filename().string();

    int portStation = config.getIntValue("Station", "port");
    std::string nameSensor = config.getValue("Sensor", "name");


    std::string ipSever = config.getValue("Server", "ip");
    int portServer = config.getIntValue("Server","port");
    UDPUnicast udpUnicastreceivedFromSensor;

    udpUnicastreceivedFromSensor.bindToPort(portStation);
   
    udpUnicastreceivedFromSensor.setOnDataReceived([fileName,&udpUnicastreceivedFromSensor, &nameSensor, ipSever, portServer, this](const std::vector<uint8_t>& data) {
        try {
            // Log received data
            std::cout << "Received data: ";
            for (auto byte : data) {
                std::cout << std::hex << static_cast<int>(byte) << "";
            }
            std::cout << std::dec << std::endl;
            udpUnicastreceivedFromSensor.sendTo(ipSever,portServer,data);

            // Conver to string
            std::string dataStr(data.begin(), data.end());
            Saver saver("Data", fileName, dataStr, nameSensor);

            saver.writeData2File();
            std::cout << "Data sent to " << ipSever << ":" << portServer << std::endl;
        } catch (const std::exception& e) {
            // Write Error into log
            cerrError(std::string("[Error processing data: ") + e.what() + "]");
        }

    });
    udpUnicastreceivedFromSensor.setOnError([this](const std::string &err) {
        // Write Error into log
        cerrError("[Unicast Error: " + std::string(err) + "]");
    });
    while (true) {
        std::cout<<"Station listenning from Sensor......."<<std::endl;
        udpUnicastreceivedFromSensor.listenfromUnicast();
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        udpUnicastreceivedFromSensor.stop();
    }
}

// Function run BusinessHandler
void BusinessHandler::run(){
    // Verify value
    if(!checkConfig()){
        std::cerr << "Error in configuration file. Exiting..." << std::endl;
    }
    running = true;

    // Starting Thread
    std::thread heartbeatThread(&BusinessHandler::heartbeatSending, this);
    std::thread SensorSendThread(&BusinessHandler::recevfromSensor, this);

    if (running == false) {
        run();
    }
    //Stop Thread
    if (heartbeatThread.joinable()) heartbeatThread.join();
    if(SensorSendThread.joinable()) SensorSendThread.join();
}

// Function log Error
void callError(std::string error){
    // Get name of file .cpp include bug/error
    std::string fileName = std::filesystem::path(__FILE__).filename().string();

    Saver saver("Error", fileName, error,"");
    // Call function to write error to System file CSV.
    saver.writeError2File();
}

// Function call Throw Error
void throwError(const std::string &message){
    callError(message);
    throw std::runtime_error(message);
}

// Function call Throw Error
void cerrError(const std::string &message){
    callError(message);
    std::cerr<<message<<std::endl;
}


