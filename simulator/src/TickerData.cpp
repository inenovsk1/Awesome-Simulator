#include "TickerData.h"


/*
NAME
    TickerData::parseFile

SYNOPSIS
    void TickerData::parseFile(std::string fileName);

    fileName    -> The name of a csv file containing data for the current ticker.

DESCRIPTION
    Opens and reads a csv file containing data for the ticker that the file
    was named after. So, if the fileName is AAPL.csv then the ticker being parsed
    will be AAPL. While parsing, the C function strtok is used, because it
    performs much faster than its C++ istringstream counterpart.

RETURNS
    Nothing

AUTHOR
    Ivaylo Nenovski

DATE
    November 18, 2017
*/
void TickerData::parseFile(std::string fileName) {
	std::ifstream f(fileName);
	std::string columnNames;
	std::getline(f, columnNames);

	std::string line;
	bool first_iteration = true;

	while (!f.eof()) {
		line.clear();
		std::getline(f, line);
		auto lineData = (char*)line.c_str();
		std::vector<std::string> data;

        // setup and use strtok for parsing
		char* singlePrice;
        const char *delim = ",";
        char **nextToken;
		singlePrice = strtok_r(lineData, delim, nextToken);
		
		while (singlePrice) {
			data.emplace_back(std::string(singlePrice));
			singlePrice = strtok_r(nullptr, delim, nextToken);
		}

		// make sure the vector that holds prices for that date is created first
		if (first_iteration) {
			//Note !! This should be data.size() - 1 but since I am adding an additional field on the fly
			//I loop 1 more time to create another vector
			for (int i = 0; i < data.size(); ++i)
				m_priceData.push_back({});
		}

        // keep track of fields - open, high, low, close, etc..
		unsigned long fieldID_index = 0;

		for (unsigned long i = 0; i < data.size(); ++i) {
			if (i - 1 == DATE_DATA) {
				m_date.emplace_back(DateTime(data.at(i)));
				continue;
			}

			if (i == ADJ_OPEN_DATA && first_iteration) {
				double adj_open = -999;
				m_priceData.at(fieldID_index++).push_back(adj_open);
				first_iteration = false;
				m_priceData.at(fieldID_index++).push_back(std::stod(data.at(i)));
				continue;
			}

			if (i == ADJ_OPEN_DATA) {
				double adj_open = (m_priceData[FieldID_OPEN].back() * m_priceData[FieldID_ADJ_CLOSE].back()) /
								   m_priceData[FieldID_CLOSE].back();

				m_priceData.at(fieldID_index++).push_back(adj_open);
				m_priceData.at(fieldID_index++).push_back(std::stod(data.at(i)));
				continue;
			}

			m_priceData.at(fieldID_index++).push_back(std::stod(data.at(i)));
		}
	}
}


/*
NAME
    TickerData::head

SYNOPSIS
    std::string TickerData::head();

DESCRIPTION
    Displays the first 5 rows of data for the ticker.

RETURNS
    A string containing the first 5 rows of data for this ticker.

AUTHOR
    Ivaylo Nenovski

DATE
    November 18, 2017
*/
std::string TickerData::head() {
	std::ostringstream out;
	out << "DateTime       Open        High        Low       Close      Adj. Close        Volume" << std::endl;

	for (unsigned long i = 0; i < 5; ++i) {
		out << m_date.at(i).toString() << "   ";
		out << m_priceData[FieldID_OPEN].at(i) << "    ";
		out << m_priceData[FieldID_HIGH].at(i) << "    ";
		out << m_priceData[FieldID_LOW].at(i) << "    ";
		out << m_priceData[FieldID_CLOSE].at(i) << "      ";
		out << m_priceData[FieldID_ADJ_CLOSE].at(i) << "          ";
		out << m_priceData[FieldID_VOLUME].at(i);
		out << std::endl;
	}

	return out.str();
}


/*
NAME
    TickerData::operator[]

SYNOPSIS
    std::vector<double> TickerData::operator[](DateTime date);

    date   -> a DateTime object representing a given date

DESCRIPTION
    Get the all the prices of this ticker for the specified date.

RETURNS
    If data for this date exists, returns a vector of prices for that date,
    otherwise it returns an empty vector.

AUTHOR
    Ivaylo Nenovski

DATE
    November 18, 2017
*/
std::vector<double> TickerData::operator[](DateTime date) {
	DateTime earliest = m_date.at(0);
	DateTime latest = m_date.back();

	if (date < earliest || date > latest) {
		return {};
	}

	unsigned long index = 0;

	for (auto it = m_date.begin(); it != m_date.end(); ++it) {
		if (*it == date) {
			break;
		}

		index++;
	}

	std::vector<double> ret;
	ret.push_back(m_priceData[FieldID_OPEN].at(index));
	ret.push_back(m_priceData[FieldID_HIGH].at(index));
	ret.push_back(m_priceData[FieldID_LOW].at(index));
	ret.push_back(m_priceData[FieldID_CLOSE].at(index));
	ret.push_back(m_priceData[FieldID_ADJ_CLOSE].at(index));
	ret.push_back(m_priceData[FieldID_ADJ_OPEN].at(index));
	ret.push_back(m_priceData[FieldID_VOLUME].at(index));
	ret.push_back(m_priceData[FieldID_DIVIDENDS].at(index));
	ret.push_back(m_priceData[FieldID_SPLITS].at(index));

	return ret;
}


/*
NAME
    TickerData::operator[]

SYNOPSIS
    std::vector<double> TickerData::operator[](std::string date);

    date   -> a date given in string format. The format of this parameter
              should be "mm-dd-yyyy".

DESCRIPTION
    Get the all the prices of this ticker for the specified date.

RETURNS
    If data for this date exists, returns a vector of prices for that date,
    otherwise it returns an empty vector.

AUTHOR
    Ivaylo Nenovski

DATE
    November 18, 2017
*/
std::vector<double> TickerData::operator[](std::string date) {
	DateTime current(date);
	DateTime earliest = m_date.at(0);
	DateTime latest = m_date.back();

	if (current < earliest || current > latest) {
		return {};
	}

	unsigned long index = 0;
	for (auto it = m_date.begin(); it != m_date.end(); ++it) {
		if (*it == current) {
			break;
		}

		index++;
	}

	std::vector<double> ret;
	ret.push_back(m_priceData[FieldID_OPEN].at(index));
	ret.push_back(m_priceData[FieldID_HIGH].at(index));
	ret.push_back(m_priceData[FieldID_LOW].at(index));
	ret.push_back(m_priceData[FieldID_CLOSE].at(index));
	ret.push_back(m_priceData[FieldID_ADJ_CLOSE].at(index));
	ret.push_back(m_priceData[FieldID_ADJ_OPEN].at(index));
	ret.push_back(m_priceData[FieldID_VOLUME].at(index));
	ret.push_back(m_priceData[FieldID_DIVIDENDS].at(index));
	ret.push_back(m_priceData[FieldID_SPLITS].at(index));

	return ret;
}


/*
NAME
    TickerData::operator[]

SYNOPSIS
    std::vector<double> & TickerData::operator[](Field_ID id);

    id     -> the Field id for which all available prices will be returned.

DESCRIPTION
    Get all the prices of this ticker for the specified field id.

RETURNS
    A reference to a vector that contains all prices for the given Field id.

AUTHOR
    Ivaylo Nenovski

DATE
    November 18, 2017
*/
std::vector<double> & TickerData::operator[](Field_ID id) {
	return m_priceData[id];
}
