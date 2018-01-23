#include "ConfigParser.h"


// constructor
ConfigParser::ConfigParser(std::string file_name) {
	m_file = std::move(file_name);
	HEADER_REGEX.assign(HEADER_STR);
	PARAMETER_REGEX.assign(PARAMETER_STR);
	COMMENT_REGEX.assign(COMMENT_STR);
}


/*
NAME
    ConfigParser::parseConfigurations

SYNOPSIS
    void ConfigParser::parseConfigurations();

DESCRIPTION
    Parses the file line by line, and according to the type of instruction that is
    detected on each line, it uses a different mechanism (function) to process it.

RETURNS
    Nothing

AUTHOR
    Ivaylo Nenovski

DATE
    November 15, 2017
*/
void ConfigParser::parseConfigurations() {
	std::ifstream in(m_file);

	if (!in) {
		std::cerr << "Failed to open file/file does not exist! (ConfigParser)" << std::endl;
		exit(1);
	}

	while (!in.eof()) {
		std::string line;
		std::getline(in, line);

		MatchResult lineType = identifyLine(line);

		switch (lineType) {
		case Header:
			extractHeader(line);
			break;

		case ParameterValuePair:
			storeValueInTable(line);
			break;

		case Comment:
			continue;
			
		default:
			continue;
		}
	}
}


/*
NAME
    ConfigParser::extractHeader

SYNOPSIS
    void ConfigParser::extractHeader(std::string line);

    line   -> Contains the current line that is being parsed at the moment

DESCRIPTION
    Extracting the name of a section from a line in the configuration file that represents
    the start of a new section, and setting up the current header to the newly extracted one.
    For example for [Universe], the extracted value will be "universe".

RETURNS
    Nothing

AUTHOR
    Ivaylo Nenovski

DATE
    November 15, 2017
*/
void ConfigParser::extractHeader(std::string a_line) {
	std::string headerName;

	for (char c : a_line) {
		if (c == '[' || c == ']') {
			continue;
		}

		headerName.push_back(c);
	}

	m_currentHeader = Utils::toLowerCase(headerName);
}


/*
NAME
    ConfigParser::storeValueInTable

SYNOPSIS
    void ConfigParser::storeValueInTable(std::string line);

    line   -> A line of type ParameterValuePair

DESCRIPTION
    Parses the line into a key and the value using the '=' sign as the delimiter.
    After the line is parsed it is stored in the configurations data structure.

RETURNS
    Nothing

AUTHOR
    Ivaylo Nenovski

DATE
    November 15, 2017
*/
void ConfigParser::storeValueInTable(std::string a_line) {
	auto pos = a_line.find('=');
	std::string key = a_line.substr(0, pos);
	std::string val = a_line.substr(pos + 1);
	m_configurations[m_currentHeader][key] = val;
}


/*
NAME
    ConfigParser::identifyLine

SYNOPSIS
    MatchResult ConfigParser::identifyLine(std::string line);

    line   -> The line to be identified for type of parameter

DESCRIPTION
    Identifies what each line in the configuration file represents by using regular
    expressions to match all possible outcomes.

RETURNS
    This function returns a value from the MatchResult structure.

AUTHOR
    Ivaylo Nenovski

DATE
    November 15, 2017
*/
ConfigParser::MatchResult ConfigParser::identifyLine(std::string a_line) {

	if (std::regex_match(a_line, HEADER_REGEX)) {
		return MatchResult::Header;
	}

	if (std::regex_match(a_line, PARAMETER_REGEX)) {
		return MatchResult::ParameterValuePair;
	}

	if (std::regex_match(a_line, COMMENT_REGEX)) {
		return MatchResult::Comment;
	}
}


std::map<std::string, std::unordered_map<std::string, std::string>> ConfigParser::getConfigs() {
	return m_configurations;
}


std::map<std::string, std::unordered_map<std::string, std::string>> ConfigParser::moveConfigs() {
	return std::move(m_configurations);
}
