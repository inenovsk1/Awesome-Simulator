#pragma once

#include <iostream>
#include <regex>
#include <string>
#include <map>
#include <unordered_map>
#include <fstream>

class ConfigParser {

public:
	ConfigParser(std::string file_name);
	~ConfigParser();
	void parseConfigurations();
	std::map<std::string, std::unordered_map<std::string, std::string> > getConfigs();

	enum MatchResult {
		Header,
		ParameterValuePair,
		Comment
	};

private:
	std::string m_file;
	std::string m_currentHeader;
	std::map<std::string, std::unordered_map<std::string, std::string> > m_configurations;

	const std::string HEADER_STR = R"(^\[\w+\]$)";
	const std::string PARAMETER_STR = R"(^.+=.+$)";
	const std::string COMMENT_STR = R"(^#.*$)";

	std::regex HEADER_REGEX;
	std::regex PARAMETER_REGEX;
	std::regex COMMENT_REGEX;

	MatchResult identifyLine(std::string line);
	void storeValueInTable(std::string line);
	void extractHeader(std::string line);
};


/*
how many times has the param been access / misaccessed - not done

config file values class to hold the values - done

programatically change the parameters so don't read the config file again - done

replace parameters used more than once to be used only the last value - done

myybe add options to config file to only load some field IDS, not all of them*/