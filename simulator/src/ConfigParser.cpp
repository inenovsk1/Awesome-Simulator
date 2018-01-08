#include "ConfigParser.h"


ConfigParser::ConfigParser(std::string file_name) {
	m_file = std::move(file_name);
	HEADER_REGEX.assign(HEADER_STR);
	PARAMETER_REGEX.assign(PARAMETER_STR);
	COMMENT_REGEX.assign(COMMENT_STR);
}

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

void ConfigParser::extractHeader(std::string line) {
	std::string headerName;

	for (char c : line) {
		if (c == '[' || c == ']') {
			continue;
		}

		headerName.push_back(c);
	}

	m_currentHeader = headerName;
}

void ConfigParser::storeValueInTable(std::string line) {
	auto pos = line.find('=');
	std::string key = line.substr(0, pos);
	std::string val = line.substr(pos + 1);
	m_configurations[m_currentHeader][key] = val;
}

ConfigParser::MatchResult ConfigParser::identifyLine(std::string line) {

	if (std::regex_match(line, HEADER_REGEX)) {
		return MatchResult::Header;
	}

	if (std::regex_match(line, PARAMETER_REGEX)) {
		return MatchResult::ParameterValuePair;
	}

	if (std::regex_match(line, COMMENT_REGEX)) {
		return MatchResult::Comment;
	}
}

std::map<std::string, std::unordered_map<std::string, std::string>> ConfigParser::getConfigs() {
	return m_configurations;
}


std::map<std::string, std::unordered_map<std::string, std::string>> ConfigParser::moveConfigs() {
	return std::move(m_configurations);
}
