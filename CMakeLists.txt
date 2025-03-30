cmake_minimum_required(VERSION 3.10)

project(SensorStation)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)

# Show file cpp in src/
set(SOURCES
    src/main.cpp
    src/BusinessHandler.cpp
    src/Saver.cpp
    src/ConfigReader.cpp
    src/UDPUnicast.cpp
    # src/test2.cpp
)

# Create the executable
add_executable(SensorStation ${SOURCES})
