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
#include <vector>
#include <cmath>

namespace Utils {
    std::string toLowerCase(std::string& a_word);
    bool isArgumentConfigurationFile(std::string a_argument);
    void handleCommandLineArguments(std::string a_argument);
    void determineCommandLineOptions(int argc, char** argv);
    void showUsage();
    std::string today();
    double standardDeviation(std::vector<double> a_numbers);
    double average(std::vector<double> a_numbers);
}

#endif //SIMULATOR_UTILITIES_H
