#include <iostream>
#include <regex>
#include <fstream>
#include "TickerData.h"
#include "Database.h"
#include "ConfigParser.h"
#include "Configurations.h"

using namespace std;

int main(int argc, char** argv) {

	if (argc != 2) {
		cout << "Usage: sim config_file.ini" << endl;
		system("PAUSE");
		exit(1);
	}

	ConfigParser parser(argv[1]);
	parser.parseConfigurations();
	Configurations configs(parser.moveConfigs());

	Database & db = Database::getDatabaseInstance(configs.param("Universe", "data_directory"), configs.param("Universe", "universe_file"));
	cout << "Done loading DB!" << endl;

	std::vector<double> price_point = db["AAPL"]["2012-10-05"];
	std::vector<double>& apple_open_historical = db["AAPL"][TickerData::FieldID_OPEN];

	cout << "High price for Apple at 2012-10-05 was " << price_point.at(TickerData::FieldID_HIGH) << endl;
	cout << "Low price for Apple at 2012-10-05 was " << price_point.at(TickerData::FieldID_LOW) << endl;

	system("PAUSE");
	return 0;
}