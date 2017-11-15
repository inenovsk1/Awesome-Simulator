#include "Configurations.h"



Configurations::Configurations() {

}


Configurations::Configurations(std::map<std::string, std::unordered_map<std::string, std::string> > parsed_configs) {
	m_configs = parsed_configs;
}


Configurations::~Configurations() {

}


void Configurations::setConfigs(std::map<std::string, std::unordered_map<std::string, std::string> > parsed_configs) {
	m_configs = parsed_configs;
}


void Configurations::modifyParam(std::string header, std::string key, std::string new_val) {
	m_configs[header][key] = new_val;
}


std::string Configurations::param(std::string header, std::string key) {
	return m_configs[header][key];
}
