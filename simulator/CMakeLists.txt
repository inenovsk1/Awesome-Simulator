cmake_minimum_required(VERSION 3.9)
project(simulator)

set(CMAKE_CXX_STANDARD 14)

#set(CMAKE_BUILD_TYPE Release)  # RELEASE BUILD
set(CMAKE_BUILD_TYPE Debug)    # DEBUG BUILD

add_executable(simulator
        src/ConfigParser.cpp
        src/ConfigParser.h
        src/Configurations.cpp
        src/Configurations.h
        src/Database.cpp
        src/Database.h
        src/DateTime.cpp
        src/DateTime.h
        src/main.cpp
        src/TickerData.cpp
        src/TickerData.h
        src/Utilities.h
        src/Utilities.cpp)
