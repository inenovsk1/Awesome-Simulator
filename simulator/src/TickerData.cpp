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

		for (int i = 0; i < data.size(); ++i) {
			switch(i) {
			case 0:
				m_date.push_back(Date(data.at(i)));
				break;

			case 1:
				m_open.push_back(std::stod(data.at(i)));
				break;

			case 2:
				m_high.push_back(std::stod(data.at(i)));
				break;

			case 3:
				m_low.push_back(std::stod(data.at(i)));
				break;

			case 4:
				m_close.push_back(std::stod(data.at(i)));
				break;

			case 5:
				m_adj_close.push_back(std::stod(data.at(i)));
				break;

			case 6:
				m_volume.push_back(std::stod(data.at(i)));
				break;

			case 7:
				m_dividends.push_back(std::stod(data.at(i)));
				break;

			case 8:
				m_splits.push_back(std::stod(data.at(i)));
			}
		}
	}
}


std::string TickerData::head() {
	std::ostringstream out;
	out << "Date       Open        High        Low       Close      Adj. Close        Volume" << std::endl;

	for (int i = 0; i < 5; ++i) {
		out << m_date.at(i).toString() << "   ";
		out << m_open.at(i) << "    ";
		out << m_high.at(i) << "    ";
		out << m_low.at(i) << "    ";
		out << m_close.at(i) << "      ";
		out << m_adj_close.at(i) << "          ";
		out << m_volume.at(i);
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
	ret.push_back(m_open.at(index));
	ret.push_back(m_high.at(index));
	ret.push_back(m_low.at(index));
	ret.push_back(m_close.at(index));
	ret.push_back(m_adj_close.at(index));
	ret.push_back(m_volume.at(index));
	ret.push_back(m_dividends.at(index));
	ret.push_back(m_splits.at(index));

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
	ret.push_back(m_open.at(index));
	ret.push_back(m_high.at(index));
	ret.push_back(m_low.at(index));
	ret.push_back(m_close.at(index));
	ret.push_back(m_adj_close.at(index));
	ret.push_back(m_volume.at(index));
	ret.push_back(m_dividends.at(index));
	ret.push_back(m_splits.at(index));

	return ret;
}
