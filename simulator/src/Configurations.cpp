#include "Configurations.h"


/*
NAME
    Configurations::Configurations

SYNOPSIS
    Configurations::Configurations(std::map<std::string, std::unordered_map<std::string, std::string>> parsed_configs);

    parsed_configs  -> The data structure used when parsing the configuration file.
    				   It is copied in the Configurations class

DESCRIPTION
    Constructor for the Configurations class

RETURNS
    Nothing

AUTHOR
    Ivaylo Nenovski

DATE
    January 23, 2018
*/
Configurations::Configurations(std::map<std::string, std::unordered_map<std::string, std::string>> parsed_configs) {
	m_configs = parsed_configs;
}


/*
NAME
    Configurations::modifyParam

SYNOPSIS
    void Configurations::modifyParam(std::string header, std::string key, std::string new_val)

    header    -> Header section in the configuration file
    key       -> Name of parameter to be changed
    new_val   -> The new value to which the parameter will be modified

DESCRIPTION
    Gives the ability to change parameters programmatically at runtime without
    the need of re-running to simulator again

RETURNS
    Nothing

AUTHOR
    Ivaylo Nenovski

DATE
    January 25, 2018
*/
void Configurations::modifyParam(std::string header, std::string key, std::string new_val) {
	m_configs[header][key] = new_val;
}


/*
NAME
    Configurations::getInternalDataConfigs

SYNOPSIS
    std::map<std::string, std::unordered_map<std::string, std::string>> Configurations::getInternalDataConfigs() const

DESCRIPTION
    Get a copy of the current parameters

RETURNS
    Nothing

AUTHOR
    Ivaylo Nenovski

DATE
    January 23, 2018
*/
std::map<std::string, std::unordered_map<std::string, std::string>> Configurations::getInternalDataConfigs() const {
    return m_configs;
}


/*
NAME
    Configurations::accessParameter

SYNOPSIS
    std::string Configurations::accessParameter(std::string header, std::string key)

    header   -> Header section in the configuration file
    key      -> Name of parameter to be accessed

DESCRIPTION
    Access a parameter specified by header and key

RETURNS
    Nothing

AUTHOR
    Ivaylo Nenovski

DATE
    January 25, 2018
*/
std::string Configurations::accessParameter(std::string header, std::string key) {
	return m_configs[header][key];
}


/*
NAME
    Configurations::operator<<

SYNOPSIS
    std::ostream& operator<< (std::ostream& a_out, const Configurations& a_configs);

    a_out      -> The stream that this function is going to write to
    a_configs  -> The configurations object that is going to get written

DESCRIPTION
    Writes the contents of the configurations object to a stream. It can be a file, or stdout, or
    any other stream that was passed. Useful for outputting the used configurations in the report
    files of the simulation

RETURNS
    The written stream

AUTHOR
    Ivaylo Nenovski

DATE
    January 23, 2018
*/
std::ostream& operator<< (std::ostream& a_out, const Configurations& a_configs) {
	for (auto headerSection : a_configs.getInternalDataConfigs()) {
        a_out << "SECTION " << headerSection.first << ": " << std::endl;

        for (auto parameterPair : headerSection.second) {
			a_out << parameterPair.first << " = " << parameterPair.second << std::endl;
		}

        a_out << std::endl;
	}

    return a_out;
}
