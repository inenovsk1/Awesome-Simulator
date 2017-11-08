#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include "Date.h"

class TickerData {

public:
	TickerData();
	~TickerData();
	void parseFile(std::string fileName);
	std::string head();

	std::vector<double> operator[](Date date);
	std::vector<double> operator[](std::string date);

	enum Field_ID {
		FieldID_OPEN,
		FieldID_HIGH,
		FieldID_LOW,
		FieldID_CLOSE,
		FieldID_ADJ_CLOSE,
		FieldID_VOLUME,
		FieldID_DIVIDENDS,
		FieldID_SPLITS
	};

private:
	std::vector<Date> m_date;
	std::vector<double> m_open;
	std::vector<double> m_high;
	std::vector<double> m_low;
	std::vector<double> m_close;
	std::vector<double> m_adj_close;
	std::vector<double> m_volume;
	std::vector<double> m_dividends;
	std::vector<double> m_splits;
};

/*
use IBM as reference ticker to calculate how many dates

questions:
1) Sould I make the member of TickerData pointers, so that I can return references with []
   instead of making copies?
2) Overload the [] operator to return data for a whole field ?
3) When giving a data that does not exist should I return empty vector or vector with all values -999 or throw an exception?
*/

