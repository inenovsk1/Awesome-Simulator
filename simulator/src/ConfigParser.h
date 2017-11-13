#pragma once

#include <regex>
#include <string>
#include <map>
#include <unordered_map>

class ConfigParser {

public:
	ConfigParser();
	~ConfigParser();
	void parseConfigurations(std::string file);
	std::map<std::string, std::unordered_map<std::string, std::string> > getConfigs();

	enum MatchResult {
		Header,
		ParameterValuePair,
		Comment
	};

private:
	std::string file;
	std::map<std::string, std::unordered_map<std::string, std::string> > m_configurations;

	const std::string HEADER_STR = R"(^[\w+]$)";
	const std::string PARAMETER_STR = "^\w+=\w+$";
	const std::string COMMENT_STR = "^#";

	MatchResult identifyLine(std::string line);
};


/*config file class to read from config file and record in a hash table
keyd on param name and have param values
//how many times has the param been access / misaccessed

config file values class to hold the values

??? multiple threads static config file

programatically change the parameters so don't read the config file again

replace parameters used more than once to be used only the last value

myybe add options to config file to only load some field IDS, not all of them*/