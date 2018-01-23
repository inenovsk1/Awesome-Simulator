/*
 * ~~~~~~~~~~~~~~~~~~~~ Configurations class ~~~~~~~~~~~~~~~~~~~~
 *
 * This class holds the parameters specified in the Configurations.ini file and allows
 * for the parameters to be programatically changed and runtime.
 * */

#pragma once

#include <iostream>
#include <string>
#include <map>
#include <unordered_map>

class Configurations {

public:
	Configurations() = default;
	explicit Configurations(std::map<std::string, std::unordered_map<std::string, std::string>>&& parsed_configs);
	~Configurations() = default;
	void setConfigs(std::map<std::string, std::unordered_map<std::string, std::string>> parsed_configs);
	void modifyParam(std::string header, std::string key, std::string new_val);
    std::map<std::string, std::unordered_map<std::string, std::string>> getConfigurations() const;
	std::string accessParameter(std::string header, std::string key);
    friend std::ostream& operator<< (std::ostream& a_out, const Configurations& a_configs);

private:
	std::map<std::string, std::unordered_map<std::string, std::string>> m_configs;
};

