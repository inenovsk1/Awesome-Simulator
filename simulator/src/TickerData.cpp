#include "TickerData.h"


TickerData::TickerData(std::string ticker) {
    m_tickerName = ticker;
}


/*
NAME
    TickerData::parseFile

SYNOPSIS
    void TickerData::parseFile(std::string pathToCurrentTickerFile,
                           std::string pathToUniverseReferenceFile,
                           DateTime earliestUniverseDate);

    pathToCurrentTickerFile        -> Path to a csv file containing data for the current ticker.
    pathToUniverseReferenceFile    -> Path to a csv file for the reference ticker.
    earliestUniverseDate           -> Earliest date for which prices exist in the universe.

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
void TickerData::parseFile(std::string pathToCurrentTickerFile,
                           std::string pathToUniverseReferenceFile,
                           DateTime earliestUniverseDate) {
    bool internalVectorsCreated = false;

    if(insufficientData(pathToCurrentTickerFile, earliestUniverseDate)) {
        handleMissingData(pathToCurrentTickerFile, pathToUniverseReferenceFile, earliestUniverseDate);
        internalVectorsCreated = true;
    }


	std::ifstream f(pathToCurrentTickerFile);
	std::string columnNames;

    // removing the field names from the csv file
	std::getline(f, columnNames);

	std::string line;
	bool firstIteration = true;

	while (!f.eof()) {
		line.clear();
		std::getline(f, line);
		auto lineData = (char*)line.c_str();
		std::vector<std::string> data;

        // setup and use strtok_r for parsing
		char *singlePrice;
        const char *delimiter = ",";
        char **nextToken;
		singlePrice = strtok_r(lineData, delimiter, nextToken);
		
		while (singlePrice) {
			data.emplace_back(std::string(singlePrice));
			singlePrice = strtok_r(nullptr, delimiter, nextToken);
		}

		// make sure the vector that holds prices for each field ID are created first
		if (!internalVectorsCreated) {
			for (int i = 0; i < data.size(); ++i)
				m_priceData.push_back({});
		}

        // keep track of fields - open, high, low, close, etc..
		unsigned long fieldID_index = 0;

		for (unsigned long i = 0; i < data.size(); ++i) {
			if (i == DATE_DATA_INDICATOR) {
				m_tickerDates.emplace_back(DateTime(data.at(i)));
				continue;
			}

			if (i == ADJ_OPEN_DATA_INDICATOR && firstIteration) {
				double adj_open = UNAVAILABLE_DATA;
				m_priceData.at(fieldID_index++).push_back(adj_open);
				firstIteration = false;
				m_priceData.at(fieldID_index++).push_back(std::stod(data.at(i)));
				continue;
			}

			if (i == ADJ_OPEN_DATA_INDICATOR) {
				double adj_open = (m_priceData[FieldID_OPEN].back() * m_priceData[FieldID_ADJ_CLOSE].back()) /
								   m_priceData[FieldID_CLOSE].back();

                // push back adjusted open + volume so that loop continues linearly without any
                // modifications on the loop variable 'i'
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
    TickerData::getFirstDate

SYNOPSIS
    DateTime TickerData::readFirstDateFromFile(std::string dataDirectory);

    dataDirectory    -> Path to the directory that contains all data for the universe.

DESCRIPTION
    Get the first date for which there exists data for the current ticker.

RETURNS
    A DateTime object containing the earliest date for which we have historical data.

AUTHOR
    Ivaylo Nenovski

DATE
    February 13, 2018
*/
DateTime TickerData::readFirstDateFromFile(std::string pathToFile) {
    std::string line;
    std::ifstream in(pathToFile);

    // exhaust first line that contains field names
    std::getline(in, line);

    //extract earliest date
    line.clear();
    std::getline(in, line);
    auto firstCommaPosition = line.find(',');
    std::string date = line.substr(0, firstCommaPosition);

    in.close();

    // construct and return an anonymous DateTime object from the string
    return DateTime(date);
}


/*
NAME
    TickerData::insufficientData

SYNOPSIS
    bool TickerData::insufficientData(std::string pathToFile, DateTime earliestDate);

    pathToFile    -> Path to the file with historical data for current ticker.
    earliestDate  -> Earliest date for which historical data exists in the universe.

DESCRIPTION
    Decide whether the data for current ticker goes all the way back to the
    earliest date in the universe, or if it is insufficient and needs to be padded
    with missing data values.

RETURNS
    True if data is insufficient, false if everything is okay!

AUTHOR
    Ivaylo Nenovski

DATE
    February 13, 2018
*/
bool TickerData::insufficientData(std::string pathToFile, DateTime earliestUniverseDate) {
    DateTime currentTickerEarliestDate = readFirstDateFromFile(pathToFile);
    return earliestUniverseDate == currentTickerEarliestDate ? false : true;
}


/*
NAME
    TickerData::findMissingDates

SYNOPSIS
    std::vector<DateTime> TickerData::findMissingDates(DateTime earliestUniverseDate,
                                                   DateTime earliestTickerDate,
                                                   std::string pathToUniverseReference);

    earliestUniverseDate    -> Earliest date for which historical data exists in the universe.
    earliestTickerDate      -> Earliest date for which historical data exists for the current ticker.
    pathToUniverseReference -> Path to the data file used as a reference to calculate missing dates.

DESCRIPTION
    Use IBM as a reference ticker to calculate how far back prices for the universe go,
    and if there is data missing for some dates for the current ticker, then populate
    the priceData vector with UNAVAILABLE-DATA price to specify that no data exists for that date!

RETURNS
    A vector of DateTime objects representing all of the dates for which no historical price
    date exists!

AUTHOR
    Ivaylo Nenovski

DATE
    February 14, 2018
*/
std::vector<DateTime> TickerData::findMissingDates(DateTime earliestUniverseDate,
                                                   DateTime earliestTickerDate,
                                                   std::string pathToUniverseReference) {
    std::vector<DateTime> missingDates;
    std::ifstream inUniverseReference(pathToUniverseReference);
    std::string line;
    std::getline(inUniverseReference, line); // get initial line with field ids

    DateTime currentDate(earliestUniverseDate);

    while(!(currentDate == earliestTickerDate)) {
        line.clear();
        missingDates.push_back(currentDate);
        std::getline(inUniverseReference, line);
        auto pos = line.find(',');
        std::string date = line.substr(0, pos);
        currentDate = DateTime(date);
    }

    inUniverseReference.close();

    return missingDates;
}


/*
NAME
    TickerData::handleEmptyData

SYNOPSIS
    void TickerData::handleMissingData(std::string pathToCurrentTickerFile,
                                 std::string pathToUniverseReference,
                                 DateTime earliestUniverseDate);

    pathToCurrentTickerFile    -> Path to the file containing price data for the current ticker.
    pathToUniverseReference    -> Path to the data file used as a reference for the universe.
    earliestUniverseDate       -> Earliest date for which historical data exists for the universe.

DESCRIPTION
    Calculates which dates are missing from the current ticker's price data and populates
    its m_priceData vector with UNAVAILABLE_DATA prices (-999) for each of these dates and for
    each field ID.

RETURNS
    Nothing

AUTHOR
    Ivaylo Nenovski

DATE
    February 14, 2018
*/
void TickerData::handleMissingData(std::string pathToCurrentTickerFile,
                                   std::string pathToUniverseReference,
                                   DateTime earliestUniverseDate) {
    for (int i = 0; i < TickerData::IDS_MAX + 1; ++i) {
        // populate the outer vector with inner vectors for each field
        m_priceData.push_back({});
    }

    DateTime currentTickerEarliestDate = readFirstDateFromFile(pathToCurrentTickerFile);
    std::vector<DateTime> missingDates = findMissingDates(earliestUniverseDate,
                                                          currentTickerEarliestDate,
                                                          pathToUniverseReference);

    for (auto it = missingDates.begin(); it != missingDates.end(); ++it) {
        m_tickerDates.push_back(*it);

        for (unsigned long i = 0; i < TickerData::IDS_MAX + 1; ++i) {
            m_priceData.at(i).push_back(UNAVAILABLE_DATA);
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
		out << m_tickerDates.at(i).toString() << "   ";
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
	DateTime earliest = m_tickerDates.at(0);
	DateTime latest = m_tickerDates.back();

	if (date < earliest || date > latest) {
		return {};
	}

	unsigned long index = 0;

	for (auto it = m_tickerDates.begin(); it != m_tickerDates.end(); ++it) {
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
	DateTime earliest = m_tickerDates.at(0);
	DateTime latest = m_tickerDates.back();

	if (current < earliest || current > latest) {
        //msgLog writes data doesn't exist
		return {};
	}

    if(std::find(m_tickerDates.begin(), m_tickerDates.end(), current) == m_tickerDates.end()) {
        //msgLog writes data doesn't exist
        return {};
    }

	unsigned long index = 0;
	for (auto it = m_tickerDates.begin(); it != m_tickerDates.end(); ++it) {
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


/*
NAME
    TickerData::getEarliestDate

SYNOPSIS
    DateTime TickerData::getEarliestDate()

DESCRIPTION
    Get the earliest date with available data for this ticker

RETURNS
    Earliest date for this ticker

AUTHOR
    Ivaylo Nenovski

DATE
    March 9, 2017
*/
DateTime TickerData::getEarliestDate() {
    return m_tickerDates.front();
}


/*
NAME
    TickerData::begin

SYNOPSIS
    std::vector<DateTime>::iterator TickerData::begin()

DESCRIPTION
    Get the earliest date in existence for this ticker

RETURNS
    Earliest date for this ticker

AUTHOR
    Ivaylo Nenovski

DATE
    March 9, 2017
*/
std::vector<DateTime>::iterator TickerData::begin() {
    return m_tickerDates.begin();
}


/*
NAME
    TickerData::end

SYNOPSIS
    std::vector<DateTime>::iterator TickerData::end()

DESCRIPTION
    Get the latest date in existence for this ticker

RETURNS
    Latest date for this ticker

AUTHOR
    Ivaylo Nenovski

DATE
    March 9, 2017
*/
std::vector<DateTime>::iterator TickerData::end() {
    return m_tickerDates.end();
}
