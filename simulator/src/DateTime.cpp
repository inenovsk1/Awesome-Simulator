#include "DateTime.h"


/*
NAME
    DateTime::DateTime

SYNOPSIS
    DateTime::DateTime(int a_year, int a_month, int a_day)

    a_year   -> Year to be used for the date
    a_month  -> Month to be used for the date
    a_day    -> Day to be used for the date

DESCRIPTION
    Constructs a DateTime object from 3 integers

RETURNS
    Nothing

AUTHOR
    Ivaylo Nenovski

DATE
    February 27, 2017
*/
DateTime::DateTime(int a_year, int a_month, int a_day) {
	m_year = a_year;
	m_month = a_month;
	m_day = a_day;
}


/*
NAME
    DateTime::DateTime

SYNOPSIS
    DateTime::DateTime(std::string a_strDate)

    a_strDate   -> String representation of date to be used. It must be
    			   in YYYY-MM-DD format!

DESCRIPTION
    Constructs a DateTime object from string

RETURNS
    Nothing

AUTHOR
    Ivaylo Nenovski

DATE
    February 27, 2017
*/
DateTime::DateTime(std::string a_strDate) {
	auto date = (char*)a_strDate.c_str();
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


/*
NAME
    DateTime::toString

SYNOPSIS
    std::string DateTime::toString() const

DESCRIPTION
    Gives a string representation of a DateTime object in YYYY-MM-DD format

RETURNS
    The string representation of the current DateTime object

AUTHOR
    Ivaylo Nenovski

DATE
    February 27, 2017
*/
std::string DateTime::toString() const {
	std::ostringstream str_date;
	str_date << m_year << "-" << m_month << "-" << m_day;
	return str_date.str();
}


/*
 * Overloaded less than, greater than, equal and stream extraction operators
 * for better and easier usability of the class
*/
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


/*
NAME
    DateTime::incrementDate

SYNOPSIS
    void DateTime::incrementDate();

DESCRIPTION
    Increments the date to the next date in the Gregorian calendar. This function
    examines and handles all the edge cases when incrementing a date.
    Tested and works!

RETURNS
    Nothing

AUTHOR
    Ivaylo Nenovski

DATE
    February 27, 2017
*/
void DateTime::incrementDate() {
    if(m_day == 31 && m_month == 12) {
        m_year++;
        m_day = 1;
        m_month = 1;
    }
    else if(m_day == 31 && (m_month % 2 == 1 || m_month == 8)) {
        m_day = 1;
        m_month++;
    }
    else if(m_day == 30 && (m_month % 2 == 0 && !m_month == 8)) {
        m_day = 1;
        m_month++;
    }
    else if(m_day == 28 && m_month == 2) {
        m_day = 1;
        m_month++;
    }
    else {
        m_day++;
    }
}


/*
NAME
    DateTime::operator++

SYNOPSIS
    DateTime & DateTime::operator++();

DESCRIPTION
    Overloading the prefix ++ operator! This function calls the incrementDate function
    internally, however, it has extra code to check whether the date to which it is
    incremented is a weekend day or not. i.e. if the current date happens to be a
    Friday, whenever we call date++, the new date will jump 2 days ahead to a Monday.
    This is done with the purpose that there is no stock data during weekends, therefore one does
    not really need to access those dates in the first place.

RETURNS
    The new incremented date

AUTHOR
    Ivaylo Nenovski

DATE
    February 27, 2017
*/
DateTime & DateTime::operator++() {
    this->incrementDate();

	int numOfYearsSince1900 = this->m_year - 1900;

	// structure representing time
    tm currentTime;

    currentTime.tm_year = numOfYearsSince1900;
    currentTime.tm_mon = this->m_month - 1; // since tm_mon range is 0-11
    currentTime.tm_mday = this->m_day;
    currentTime.tm_isdst = -1;
	currentTime.tm_hour = 10;
	currentTime.tm_min = 0;
	currentTime.tm_sec = 0;

	mktime(&currentTime);

	if(currentTime.tm_wday == 0) {
        this->incrementDate();
    }

    if(currentTime.tm_wday == 6) {
        this->incrementDate();
        this->incrementDate();
    }

    return *this;
}


/*
NAME
    DateTime::operator++

SYNOPSIS
    DateTime DateTime::operator++(int dummyParameter);

DESCRIPTION
    Overloading the prefix ++ operator! This function calls prefix operator, however,
    it returns the old value, as postfix operators usually do!

RETURNS
    The old date value

AUTHOR
    Ivaylo Nenovski

DATE
    February 27, 2017
*/
DateTime DateTime::operator++(int dummyParameter) {
    DateTime oldTime(*this);
    operator++(); // pre-increment
    return oldTime;
}
