#include <iostream>
#include "TickerData.h"
#include "Database.h"

using namespace std;

int main(int argc, char** argv) {

	//TickerData aapl;
	//aapl.parseFile("E:\\home\\programming\\stock_data\\data\\S&P500\\AAPL.csv");
	//cout << aapl.head();
	//cout << "Done!" << endl;

	std::string data_directory = "E:\\home\\programming\\stock_data\\data\\S&P500";
	std::string universe_file = "E:\\home\\programming\\stock_data\\S&P500";

	Database & db = Database::getDatabaseInstance(data_directory, universe_file);
	std::vector<double> price_point = db["AAPL"]["2012-10-05"];
	cout << "High price for Apple at 2012-10-05 was " << price_point.at(TickerData::FieldID_HIGH) << endl;
	cout << "Low price for Apple at 2012-10-05 was " << price_point.at(TickerData::FieldID_LOW) << endl;

	system("PAUSE");
}