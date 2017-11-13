#include "ConfigParser.h"


ConfigParser::ConfigParser() {
	
}


ConfigParser::~ConfigParser() {

}

void ConfigParser::parseConfigurations(std::string file) {
	// use std::regex_match for this function
	std::regex HEADER_REGEX(HEADER_STR);
	std::regex PARAMETER_REGEX(PARAMETER_STR);
	std::regex COMMENT_REGEX(COMMENT_STR);
}

ConfigParser::MatchResult ConfigParser::identifyLine(std::string line) {
	return Comment;
}

std::map<std::string, std::unordered_map<std::string, std::string> > ConfigParser::getConfigs() {
	return {};
}
