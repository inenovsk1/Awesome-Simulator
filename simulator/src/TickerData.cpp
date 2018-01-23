#include "TickerData.h"


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

		char* singleData;
		singleData = strtok(lineData, ",");
		
		while (singleData) {
			data.emplace_back(std::string(singleData));
			singleData = strtok(nullptr, ",");
		}

		// make sure the vector that holds prices for that date is created first
		if (first_iteration) {
			//Note !! This should be data.size() - 1 but since I am adding an additional field on the fly
			//I go loop 1 more time to create another vector
			for (int i = 0; i < data.size(); ++i)
				m_priceData.push_back({});
		}

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

std::vector<double> & TickerData::operator[](Field_ID id) {
	return m_priceData[id];
}
