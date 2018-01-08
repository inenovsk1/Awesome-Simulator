#include "Database.h"

std::vector<std::string> Database::m_universe;
std::map<std::string, TickerData*> Database::m_db;
Database* Database::db = nullptr;


Database::Database(std::string & directory, std::string & universeFile) {
	getTickersFromUniverse(universeFile);
	loadData(directory);
}


Database::~Database() {
	delete db;
}


void Database::getTickersFromUniverse(std::string universeFile) {
	std::ifstream file(universeFile);

	while (!file.eof()) {
		std::string ticker;
		getline(file, ticker);

		if (ticker.empty()) continue;

		m_universe.push_back(ticker);
	}
}


void Database::loadData(std::string directory) {
	for (std::string ticker : m_universe) {
		TickerData* data = new TickerData();
		std::string path = directory + "/" + ticker + ".csv";
		data->parseFile(path);
		m_db[ticker] = data;
		std::cout << "Load data for ticker " << ticker << std::endl;
	}
}


Database & Database::getDatabaseInstance(std::string directory, std::string universeFile) {
	if (db == nullptr) {
		db = new Database(directory, universeFile);
		return *db;
	}

	return *db;
}


TickerData & Database::operator[](std::string ticker) {
	return *m_db[ticker];
}
