/*
 * ~~~~~~~~~~~~~~~~~~~~ Utilities ~~~~~~~~~~~~~~~~~~~~
 *
 * This header represents different utility stand-alone functions used throughout
 * the simulator. They are put in a separate namespace called Utils
 * */

#ifndef SIMULATOR_UTILITIES_H
#define SIMULATOR_UTILITIES_H

#include <string>
#include <iostream>

namespace Utils {
    std::string toLowerCase(std::string& a_word);
    bool isArgumentConfigurationFile(std::string a_argument);
    void handleCommandLineArguments(std::string a_argument);
    void determineCommandLineOptions(int argc, char** argv);
    void showUsage();
}

#endif //SIMULATOR_UTILITIES_H
