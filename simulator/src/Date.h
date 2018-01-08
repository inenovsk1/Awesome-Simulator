#pragma once

#include <cstring>
#include <sstream>
#include <string>

class Date {

public:
	Date() = default;
	Date(int a_year, int a_month, int a_day);
	Date(std::string str_date);
	~Date() = default;
	std::string toString() const;

    friend bool operator<(const Date& lhs, const Date& rhs);
    friend bool operator>(const Date& lhs, const Date& rhs);
    friend bool operator==(const Date& lhs, const Date& rhs);
    friend std::ostream & operator<<(std::ostream & out, const Date & date);

private:
	int m_year;
	int m_month;
	int m_day;
};

