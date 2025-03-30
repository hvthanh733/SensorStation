#include "Saver.h"



Saver::Saver(const std::string& name, const std::string& nameFile, const std::string& data, std::string nameSensor)
   : namefile(nameFile),sensor(nameSensor),data(data), directoryPath("logs") {

    std::filesystem::create_directories(directoryPath);

    if(name == "Error"){
        Pathfile = directoryPath + "/[System][SensorStation].csv";
    }else if(name == "Data"){
        Pathfile = directoryPath + "/[Data]["+sensor+"].csv";
    }
    
    file.open(Pathfile, std::ios::out | std::ios::binary | std::ios::app);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << Pathfile << std::endl;
    } else {
        std::cout << "Saving data to: " << Pathfile << std::endl;
    }
}

void Saver::writeError2File() {
    auto now = std::chrono::system_clock::now();
    auto nowTime = std::chrono::system_clock::to_time_t(now + std::chrono::hours(7));

    file << std::put_time(std::localtime(&nowTime), "[%d%m%Y_%H%M%S]")
    << "[Error in " << namefile <<"]"<<data <<std::endl;

    file.flush();
    file.close();
}
void Saver::writeData2File() {
    auto now = std::chrono::system_clock::now();
    auto nowTime = std::chrono::system_clock::to_time_t(now + std::chrono::hours(7));

    file << std::put_time(std::localtime(&nowTime), "[%d%m%Y_%H%M%S]")
         <<"["<<data<<"]"<<std::endl;
    file.flush();
    file.close();
}
