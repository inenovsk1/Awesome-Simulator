#pragma once

#include <sstream>
#include <string>

class Date {

public:
	Date();
	Date(int a_year, int a_month, int a_day);
	Date(std::string str_date);
	~Date();
	std::string toString();
	friend bool operator<(const Date& lhs, const Date& rhs);
	friend bool operator>(const Date& lhs, const Date& rhs);
	friend bool operator==(const Date& lhs, const Date& rhs);

private:
	int m_year;
	int m_month;
	int m_day;
};

