#include "DateTime.h"


DateTime::DateTime(int a_year, int a_month, int a_day) {
	m_year = a_year;
	m_month = a_month;
	m_day = a_day;
}


DateTime::DateTime(std::string str_date) {
	auto date = (char*)str_date.c_str();
	char* num;
	num = strtok(date, "-");

	int dates[3];
	int i = 0;

	while (num != nullptr) {
		dates[i++] = std::stoi(std::string(num));
		num = strtok(nullptr, "-");
	}

	m_year = dates[0];
	m_month = dates[1];
	m_day = dates[2];
}


std::string DateTime::toString() const {
	std::ostringstream str_date;
	str_date << m_year << "-" << m_month << "-" << m_day;
	return str_date.str();
}

bool operator<(const DateTime& lhs, const DateTime& rhs) {
	if (lhs.m_year < rhs.m_year) {
		return true;
	}
	else if (lhs.m_year == rhs.m_year && lhs.m_month < rhs.m_month) {
		return true;
	}
	else if (lhs.m_year == rhs.m_year && lhs.m_month == rhs.m_month && lhs.m_day < rhs.m_day) {
		return true;
	}

	return false;
}


bool operator>(const DateTime& lhs, const DateTime& rhs) {
	return rhs < lhs;
}


bool operator==(const DateTime& lhs, const DateTime& rhs) {
	if (lhs.m_year == rhs.m_year && lhs.m_month == rhs.m_month && lhs.m_day == rhs.m_day) {
		return true;
	}

	return false;
}

std::ostream & operator<<(std::ostream & out, const DateTime & date) {
	out << date.toString();
	return out;
}
