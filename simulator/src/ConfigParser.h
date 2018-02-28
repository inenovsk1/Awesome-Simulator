/*
 * ~~~~~~~~~~~~~~~~~~~~ ConfigParser class ~~~~~~~~~~~~~~~~~~~~
 *
 * Parses the configuration file for the simulator, determines the type of parameter and records its value.
 * Type of parsing outcomes per line in the configuration file :
 *     HEADER    - Header of a section in the config file. This is not an actual parameter but rather helps
 *                 identify what the following parameters (until the next section) will be used for. Each header
 *                 is a key associated with a separate entry into parsing table (std::map).
 *     PARAMETER - This is a regular "key=value" type of parameter. It is being recorded into a hash table
 *                 that is stored under the particular HEADER section that it falls into.
 *     COMMENT   - This line is a comment and is skipped by the parser. Nothing is recorded if the line is a
 *                 comment.
 * The class uses regular expressions to make sure that each line is recognized correctly and thus the file is
 * parsed, and each parameter is recorded as expected.
 * */

#pragma once

#include <iostream>
#include <regex>
#include <string>
#include <map>
#include <unordered_map>
#include <fstream>
#include "Utilities.h"

class ConfigParser {

public:
	ConfigParser() = default;
	explicit ConfigParser(std::string file_name);
	~ConfigParser() = default;
	void parseConfigurations();
	std::map<std::string, std::unordered_map<std::string, std::string>> getConfigs();

	enum MatchResult {
		Header,
		ParameterValuePair,
		Comment
	};

private:
    // member variables
	std::string m_file;
	std::string m_currentHeader;
	std::map<std::string, std::unordered_map<std::string, std::string>> m_configurations;

    // regex patterns
	const std::string HEADER_STR = R"(^\[\w+\]$)";
	const std::string PARAMETER_STR = R"(^.+=.+$)";
	const std::string COMMENT_STR = R"(^#.*$)";

    // regex objects
	std::regex HEADER_REGEX;
	std::regex PARAMETER_REGEX;
	std::regex COMMENT_REGEX;

    // member functions
	MatchResult identifyLine(std::string a_line);
	void storeValueInTable(std::string a_line);
	void extractHeader(std::string a_line);
};
