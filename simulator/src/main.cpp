#include <iostream>
#include "TickerData.h"
#include "Database.h"
#include "ConfigParser.h"
#include "Configurations.h"
#include "Utilities.h"

using namespace std;

int main(int argc, char** argv) {

	Utils::determineCommandLineOptions(argc, argv);

	ConfigParser parser(argv[1]);
	parser.parseConfigurations();
	Configurations configs(parser.moveConfigs());

	Database & db = Database::getDatabaseInstance(configs.accessParameter("universe", "Data_Directory"),
                                                  configs.accessParameter("universe", "Universe_File"));
	cout << "Done loading DB!" << endl;

	std::vector<double> price_point = db["AAPL"]["2012-10-05"];
	std::vector<double>& apple_open_historical = db["AAPL"][TickerData::FieldID_OPEN];

    cout << "Open price for apple at index 583 is " << apple_open_historical.at(583) << endl;
	cout << "High price for Apple at 2012-10-05 was " << price_point.at(TickerData::FieldID_HIGH) << endl;
	cout << "Low price for Apple at 2012-10-05 was " << price_point.at(TickerData::FieldID_LOW) << endl;

	cout << "\nConfigurations used:" << endl;
    cout << configs << endl;

	return 0;
}