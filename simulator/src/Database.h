/*
 * ~~~~~~~~~~~~~~~~~~~~ Database class ~~~~~~~~~~~~~~~~~~~~
 *
 * This class will hold all the available historical data inside of an in-memory database.
 * This class will use the singleton pattern, so that no copies or multiple instances
 * can be created.
 * */

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
	static Database& getDatabaseInstance(std::string dataDirectory, std::string universeFile);
	TickerData & operator[](std::string ticker);
    DateTime getEarliestDate();
	static std::vector<std::string>::iterator begin();
    static std::vector<std::string>::iterator end();

private:
	// member functions
	Database(std::string & directory, std::string & universeFile, DateTime earliestDate);
	void loadData(std::string directory, DateTime earliestUniverseDate);
	void getTickersFromUniverse(std::string universeFile);
    static DateTime calculateEarliestUniverseDate(std::string dataDirectory);
	
	// member variables
	static std::vector<std::string> m_universe;
	static std::map<std::string, TickerData*> m_db;
	static Database* db;
    static DateTime m_earliestDate;
};
