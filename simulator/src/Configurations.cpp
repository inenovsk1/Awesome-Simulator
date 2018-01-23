#include "Configurations.h"


Configurations::Configurations(std::map<std::string, std::unordered_map<std::string, std::string>>&& parsed_configs) {
	m_configs = parsed_configs;
}


void Configurations::setConfigs(std::map<std::string, std::unordered_map<std::string, std::string>> parsed_configs) {
	m_configs = std::move(parsed_configs);
}


void Configurations::modifyParam(std::string header, std::string key, std::string new_val) {
	m_configs[header][key] = new_val;
}


std::map<std::string, std::unordered_map<std::string, std::string>> Configurations::getConfigurations() const {
    return m_configs;
}


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
	for (auto headerSection : a_configs.getConfigurations()) {
        a_out << "SECTION " << headerSection.first << ": " << std::endl;

        //for (auto parameterPair : headerSection.second.begin()) {
        for (auto parameterPair = headerSection.second.begin(); parameterPair != headerSection.second.end(); ++ parameterPair) {
			a_out << parameterPair->first << " = " << parameterPair->second << std::endl;
		}

        a_out << std::endl;
	}

    return a_out;
}
