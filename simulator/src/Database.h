#pragma once

#include <map>
#include <string>
#include <iostream>
#include "TickerData.h"

class Database {

public:
	Database(Database const&) = delete;
	Database operator=(Database const&) = delete;
	~Database();
	static Database & getDatabaseInstance(std::string directory, std::string universeFile);
	TickerData & operator[](std::string ticker);

private:
	// member functions
	Database(std::string & directory, std::string & universeFile);
	void loadData(std::string directory);
	void getTickersFromUniverse(std::string universeFile);
	
	// member variables
	static std::vector<std::string> m_universe;
	static std::map<std::string, TickerData*> m_db;
	static Database* db;
	//static std::shared_ptr<Database> db; ?
};

