#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstring>
#include "Date.h"

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

	std::vector<double> operator[](Date date);
	std::vector<double> operator[](std::string date);
	std::vector<double> & operator[](Field_ID id);

private:
	std::vector<Date> m_date;
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

