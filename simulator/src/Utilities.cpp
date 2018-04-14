#include "Utilities.h"


/*
NAME
    Utils::toLowerCase

SYNOPSIS
    std::string Utils::toLowerCase(std::string& a_word)

    a_word   -> String to be converted into all lower case

DESCRIPTION
    Converts a string into all lower case characters

RETURNS
    A new string with all lower case characters

AUTHOR
    Ivaylo Nenovski

DATE
    February 16, 2018
*/
std::string Utils::toLowerCase(std::string& a_word) {
    std::string lowerCaseWord;

    for (char c : a_word) {
        lowerCaseWord.push_back(tolower(c));
    }

    return lowerCaseWord;
}


/*
NAME
    Utils::isArgumentConfigurationFile

SYNOPSIS
    bool Utils::isArgumentConfigurationFile(std::string a_argument)

    a_argument   -> The string to be checked for correct format

DESCRIPTION
    Makes sure that the passed argument is in fact a configuration file
    by checking if it ends in the "ini" extension

RETURNS
    True if format is correct and false otherwise

AUTHOR
    Ivaylo Nenovski

DATE
    February 16, 2018
*/
bool Utils::isArgumentConfigurationFile(std::string a_argument) {
    auto pos = a_argument.find(".ini");

    if (pos == std::string::npos) {
        return false;
    }

    return true;
}


/*
NAME
    Utils::handleCommandLineArguments

SYNOPSIS
    void Utils::handleCommandLineArguments(std::string a_argument)

    a_argument   -> Given argument to the simulator

DESCRIPTION
    Since different command line options are expected, this function will check
    what was passed and call the correct functionality based on the argument.
    Right now only --help is supported.. Add more in the future

RETURNS
    Nothing

AUTHOR
    Ivaylo Nenovski

DATE
    February 16, 2018
*/
void Utils::handleCommandLineArguments(std::string a_argument) {
    if (a_argument == "--help") {
        showUsage();
        exit(0);
    }
}


/*
NAME
    Utils::showUsage

SYNOPSIS
    void Utils::showUsage()

DESCRIPTION
    Display detailed information about the simulator usage. I tried to mimic
    UNIX/Linux programs which always display more usage info when prompted with
    the --help flag

RETURNS
    Nothing

AUTHOR
    Ivaylo Nenovski

DATE
    February 16, 2018
*/
void Utils::showUsage() {
    std::cout << "Financial Simulator by Ivaylo Nenovski.\nTo run executable, type \"simulator\". "
            "Here are some of the command line options:\n\n";
    std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
    std::cout << "--help        to display help of how to run the simulation\n\n";
    std::cout << "Configurations.ini       whenever run with a .ini file it means that one is running\n"
            "the tool with an already written configuration file and testing with some set of parameters.\n"
            "This will run the simulator in normal mode.\n\n";
}


/*
NAME
    Utils::determineCommandLineOptions

SYNOPSIS
    void Utils::determineCommandLineOptions(int argc, char** argv)

    argc   -> Number of command line arguments passed
    argv   -> Array of command line arguments passed

DESCRIPTION
    Handle cases in which different amount command line arguments are passed.
    This will be extremely helpful for multi run when a lot of command line
    arguments are provided

RETURNS
    Nothing

AUTHOR
    Ivaylo Nenovski

DATE
    February 16, 2018
*/
void Utils::determineCommandLineOptions(int argc, char** argv) {
    switch(argc) {
        case 0:
            // impossible to fall in this case since c++ executable itself is a command argument => argc >= 1 always!!
            break;

        case 1:
            std::cerr << "Usage: \"simulator Configurations.ini\" or use --help parameter for more info.." << std::endl;
            exit(1);

        case 2: {  // enclosing scope b/c creating new variables in case
            std::string argumentVal(argv[1]);

            if (isArgumentConfigurationFile(argumentVal)) {
                // break and continue the simulation as the argument is an ini file
                break;
            }
            else {
                std::string argument = argv[1];
                handleCommandLineArguments(argument);
            }
        }
            break;

        default:
            std::cerr << "Error occurred while running the simulator. For more usage info type --help\n";
            exit(1);
    }
}


std::string Utils::today() {
    time_t     now = time(0);
    tm         tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

    return buf;
}
