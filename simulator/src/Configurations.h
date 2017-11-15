#pragma once

#include <string>
#include <unordered_map>
#include <map>

class Configurations {

public:
	Configurations();
	Configurations(std::map<std::string, std::unordered_map<std::string, std::string> >&& parsed_configs);
	~Configurations();
	void setConfigs(std::map<std::string, std::unordered_map<std::string, std::string> > parsed_configs);
	void modifyParam(std::string header, std::string key, std::string new_val);
	std::string param(std::string header, std::string key);

private:
	std::map<std::string, std::unordered_map<std::string, std::string> > m_configs;
};

