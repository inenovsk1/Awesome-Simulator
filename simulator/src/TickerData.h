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
#include "DateTime.h"

class TickerData {

public:
	TickerData() = default;
	~TickerData() = default;
	void parseFile(std::string fileName);
	std::string head();

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
		FieldID_SPLITS
	};

	std::vector<double> operator[](DateTime date);
	std::vector<double> operator[](std::string date);
	std::vector<double> & operator[](Field_ID id);

private:
	std::vector<DateTime> m_date;
	std::vector<std::vector<double>> m_priceData;

	static const int DATE_DATA = -1;
	static const int ADJ_OPEN_DATA = 6;
	/* Order in which the field ids are set in the m_priceData structure
	m_open m_high m_low m_close m_adj_close m_adj_open m_volume m_dividends m_splits */
};

/*
use IBM as reference ticker to calculate how many dates
*/


/*
given date, what index it is?? - still need to implement
*/

