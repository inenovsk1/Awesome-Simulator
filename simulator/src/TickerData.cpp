#pragma warning(disable : 4996)
#include "TickerData.h"


TickerData::TickerData() {

}


TickerData::~TickerData() {

}


void TickerData::parseFile(std::string fileName) {
	std::ifstream f(fileName);
	std::string columnNames;
	std::getline(f, columnNames);

	std::string line;
	bool first_iteration = true;
	int index = 0;

	while (!f.eof()) {
		line.clear();
		std::getline(f, line);
		char* lineData = (char*)line.c_str();
		std::vector<std::string> data;

		char* singleData;
		singleData = strtok(lineData, ",");
		
		while (singleData) {
			data.push_back(std::string(singleData));
			singleData = strtok(NULL, ",");
		}

		// make sure the vector that holds prices for that date is created first
		if (first_iteration) {
			//Note !! This should be data.size() - 1 but since I am adding an additional field on the fly
			//I go loop 1 more time to create another vector
			for (int i = 0; i < data.size(); ++i) {
				m_priceData.push_back({});
			}
		}

		int fieldID_index = 0;

		for (int i = 0; i < data.size(); ++i) {
			if (i - 1 == DATE_DATA) {
				m_date.push_back(Date(data.at(i)));
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
				double adj_open = (m_priceData[FieldID_OPEN].at(index - 1) * m_priceData[FieldID_ADJ_CLOSE].at(index - 1)) /
								  m_priceData[FieldID_CLOSE].at(index - 1);
				m_priceData.at(fieldID_index++).push_back(adj_open);
				m_priceData.at(fieldID_index++).push_back(std::stod(data.at(i)));
				continue;
			}

			m_priceData.at(fieldID_index++).push_back(std::stod(data.at(i)));
		}

		index++;
	}
}


std::string TickerData::head() {
	std::ostringstream out;
	out << "Date       Open        High        Low       Close      Adj. Close        Volume" << std::endl;

	for (int i = 0; i < 5; ++i) {
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


std::vector<double> TickerData::operator[](Date date) {
	Date earliest = m_date.at(0);
	Date latest = m_date.back();

	if (date < earliest || date > latest) {
		return {};
	}

	int index = 0;
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


std::vector<double> TickerData::operator[](std::string date) {
	Date current(date);
	Date earliest = m_date.at(0);
	Date latest = m_date.back();

	if (current < earliest || current > latest) {
		return {};
	}

	int index = 0;
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

std::vector<double>& TickerData::operator[](Field_ID id) {
	return m_priceData[id];
}
