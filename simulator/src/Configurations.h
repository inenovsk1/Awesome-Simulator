/*
 * ~~~~~~~~~~~~~~~~~~~~ Configurations class ~~~~~~~~~~~~~~~~~~~~
 *
 * This class holds the parameters specified in the Configurations.ini file and allows
 * for the parameters to be programmatically changed at runtime.
 * */

#pragma once

#include <iostream>
#include <string>
#include <map>
#include <unordered_map>

class Configurations {

public:
	Configurations() = default;
	explicit Configurations(std::map<std::string, std::unordered_map<std::string, std::string>> parsed_configs);
	~Configurations() = default;
	void modifyParam(std::string header, std::string key, std::string new_val);
    std::string accessParameter(std::string header, std::string key);
    std::map<std::string, std::unordered_map<std::string, std::string>> getInternalDataConfigs() const;
    friend std::ostream& operator<< (std::ostream& a_out, const Configurations& a_configs);

private:
	std::map<std::string, std::unordered_map<std::string, std::string>> m_configs;
};

