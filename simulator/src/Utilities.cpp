#include "Utilities.h"

std::string Utils::toLowerCase(std::string& a_word) {
    std::string lowerCaseWord;

    for (char c : a_word) {
        lowerCaseWord.push_back(tolower(c));
    }

    return lowerCaseWord;
}


bool Utils::isArgumentConfigurationFile(std::string a_argument) {
    auto pos = a_argument.find(".ini");

    if (pos == std::string::npos) {
        return false;
    }

    return true;
}


void Utils::handleCommandLineArguments(std::string a_argument) {
    if (a_argument == "--help") {
        showUsage();
        exit(0);
    }
}


void Utils::showUsage() {
    std::cout << "Financial Simulator by Ivaylo Nenovski.\nTo run executable, type \"simulator\". "
            "Here are some of the command line options:\n\n";
    std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
    std::cout << "--help        to display help of how to run the simulation\n\n";
    std::cout << "Configurations.ini       whenever run with a .ini file it means that one is running\n"
            "the tool with an already written configuration file and testing with some set of parameters.\n"
            "This will run the simulator in normal mode.\n\n";
}


void Utils::determineCommandLineOptions(int argc, char** argv) {
    switch(argc) {
        case 0:
            // impossible in c++ since executable itself is a command argument => argc >= 1 always!!
            break;

        case 1:
            std::cerr << "Usage: \"sim Configurations.ini\" or use --help parameter for more info.." << std::endl;
            exit(1);

        case 2: {  // enclosing scope b/c creating new variables in case
            std::string argumentVal(argv[1]);

            if (isArgumentConfigurationFile(argumentVal)) {
                break;
            }
            else {
                std::string argument = argv[1];
                handleCommandLineArguments(argument);
            }
        }
            break;

        default:
            std::cerr << "Error occur while running the simulator. For more usage info type --help\n";
            exit(1);
    }
}
