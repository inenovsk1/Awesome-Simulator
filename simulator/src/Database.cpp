#include "Database.h"

// static variable declaration
std::vector<std::string> Database::m_universe;
std::map<std::string, TickerData*> Database::m_db;
Database* Database::db = nullptr;


// Constructor
Database::Database(std::string & directory, std::string & universeFile) {
	getTickersFromUniverse(universeFile);
	loadData(directory);
}


// Destructor
Database::~Database() {
	delete db;
}


/*
NAME
    Database::getTickersFromUniverse

SYNOPSIS
    void Database::getTickersFromUniverse(std::string universeFile);

    universeFile   -> Path to a file that contains all tickers from the specified universe.

DESCRIPTION
    Extracts all ticker names from the specified universe file and populates the
    m_universe member variable, which holds the names of all tickers in the current
    universe.

RETURNS
    Nothing

AUTHOR
    Ivaylo Nenovski

DATE
    November 20, 2017
*/
void Database::getTickersFromUniverse(std::string universeFile) {
	std::ifstream file(universeFile);

	while (!file.eof()) {
		std::string ticker;
		getline(file, ticker);

		if (ticker.empty()) continue;

		m_universe.push_back(ticker);
	}
}


/*
NAME
    Database::loadData

SYNOPSIS
    void Database::loadData(std::string directory);

    directory   -> Path to the directory that contains the historical data for the
                   specified universe

DESCRIPTION
    Iterates over all tickers in our universe, and for each opens the corresponding
    .csv file, parses its contents and stores them inside a map. The map uses the
    ticker's name as a key and a TickerData object as the value.

RETURNS
    Nothing

AUTHOR
    Ivaylo Nenovski

DATE
    November 20, 2017
*/
void Database::loadData(std::string directory) {
	for (std::string ticker : m_universe) {
		TickerData* data = new TickerData();
		std::string path = directory + "/" + ticker + ".csv";
		data->parseFile(path);
		m_db[ticker] = data;
		std::cout << "Load data for ticker " << ticker << std::endl;
	}
}


/*
NAME
    Database::getDatabaseInstance

SYNOPSIS
    Database & Database::getDatabaseInstance(std::string directory, std::string universeFile);

    directory    -> Path to the directory that contains the historical data for the
                    specified universe.
    universeFile -> Path to a file that contains all tickers from the specified universe.

DESCRIPTION
    This functions is responsible for the singleton behavior of the class. If the pointer db
    is already pointing to a valid pointer, then simply return the object that it points to.
    If, however, db does not point to anything, then instantiate a new database object and
    set db to point to it.

RETURNS
    A reference to the database class.

AUTHOR
    Ivaylo Nenovski

DATE
    November 20, 2017
*/
Database & Database::getDatabaseInstance(std::string directory, std::string universeFile) {
	if (db == nullptr) {
		db = new Database(directory, universeFile);
		return *db;
	}

    std::cout << "Database already loaded! Returning existing instance.." << std::endl;
	return *db;
}


// Overloading the subscript operator to access a particular TickerData object from the
// database.
TickerData & Database::operator[](std::string ticker) {
	return *m_db[ticker];
}
