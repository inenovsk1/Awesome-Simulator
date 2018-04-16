/*
 * ~~~~~~~~~~~~~~~~~~~~ TickerData class ~~~~~~~~~~~~~~~~~~~~
 *
 * This class will hold historical data for a particular ticker.
 * It will also allow one to access price data by a date object, date string, or field id.
 *
 * The class defines an enumeration of field ids so that one can easily access the different
 * types of data. The field ids are:
 * 									FieldID_Open, FieldID_HIGH, FieldID_LOW, FieldID_CLOSE,
 * 									FieldID_ADJ_CLOSE, FieldID_ADJ_OPEN, FieldID_VOLUME,
 * 									FieldID_DIVIDENDS, FieldID_SPLITS.
 *
 * The data is stored as vector of vectors of doubles. The initial vector holds price data for
 * each field, therefore the initial vector holds no more than 8 vectors. Each of these vectors holds
 * data for the corresponding field in the order the the fields appear, so the first vector is
 * Open prices, the second vector is High prices, etc. Each of the 8 vectors will have as many entries
 * as there are dates that our data goes back to, therefore if our data goes back to Jan 1, 2000, there are
 * approximately 4200 trading days between Jan 1, 2000 and today (Feb 8, 2018), so each vector will
 * contain approximately 4200 prices.
 * */

#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstring>
#include <algorithm>
#include "DateTime.h"

class TickerData {

public:
	TickerData() = default;
    TickerData(std::string ticker);
	~TickerData() = default;
	std::string head();
    DateTime readFirstDateFromFile(std::string pathToFile);
    bool insufficientData(std::string pathToFile, DateTime earliestUniverseDate);

    void parseFile(std::string pathToCurrentTickerFile,
                   std::string pathToUniverseReferenceFile,
                   DateTime earliestUniverseDate);

    std::vector<DateTime> findMissingDates(DateTime earliestUniverseDate,
                                           DateTime earliestCurrentDate,
                                           std::string pathToUniverseReference);

    void handleMissingData(std::string pathToCurrentTickerFile,
                           std::string pathToUniverseReference,
                           DateTime earliestUniverseDate);

    DateTime getEarliestDate();
    std::vector<DateTime>::iterator begin();
    std::vector<DateTime>::iterator end();

	// this is a global enum! Might be better to use enum struct instead!
	enum Field_ID : uint16_t {
		FieldID_OPEN,
		FieldID_HIGH,
		FieldID_LOW,
		FieldID_CLOSE,
		FieldID_ADJ_CLOSE,
		FieldID_ADJ_OPEN,
		FieldID_VOLUME,
		FieldID_DIVIDENDS,
		FieldID_SPLITS,
        IDS_MAX = FieldID_SPLITS
	};

	std::vector<double> operator[](DateTime date);
	std::vector<double> operator[](std::string date);
	std::vector<double> &operator[](Field_ID id);

private:
	std::vector<DateTime> m_tickerDates;
	std::vector<std::vector<double>> m_priceData;
    std::string m_tickerName;

	static const int DATE_DATA_INDICATOR = 0;
	static const int ADJ_OPEN_DATA_INDICATOR = 6;
    static const int UNAVAILABLE_DATA = -999;
};
