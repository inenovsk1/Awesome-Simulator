/*
 * ~~~~~~~~~~~~~~~~~~~~ DateTime class ~~~~~~~~~~~~~~~~~~~~
 *
 * This class will be responsible for operations with dates, such as
 * comparing dates, parsing dates from strings, subtracting dates,
 * adding dates, etc..
 * */

#pragma once

#include <cstring>
#include <sstream>
#include <string>

class DateTime {

public:
	DateTime() = default;
	DateTime(int a_year, int a_month, int a_day);
	DateTime(std::string str_date);
	~DateTime() = default;
	std::string toString() const;

    friend bool operator<(const DateTime & lhs, const DateTime & rhs);
    friend bool operator>(const DateTime & lhs, const DateTime & rhs);
    friend bool operator==(const DateTime & lhs, const DateTime & rhs);
    friend std::ostream & operator<<(std::ostream & out, const DateTime & date);

private:
	int m_year;
	int m_month;
	int m_day;
};

