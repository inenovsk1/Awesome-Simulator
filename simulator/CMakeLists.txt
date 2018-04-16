cmake_minimum_required(VERSION 3.9)
project(simulator)

set(CMAKE_CXX_STANDARD 14)

#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

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
        src/Utilities.cpp
        src/Simulator.cpp
        src/Simulator.h
        src/TradingObject.cpp
        src/TradingObject.h
        src/ModelA.cpp
        src/ModelA.h)
