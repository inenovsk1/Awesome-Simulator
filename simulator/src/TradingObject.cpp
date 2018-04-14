//
// Created by inenovski on 2/27/18.
//

#include "TradingObject.h"

// Constructor for TradingObject class
TradingObject::TradingObject(std::string a_tickerName) {
    m_tickerName = a_tickerName;
    m_capitalInObject = 0;
    m_totalTransactions = 0;
    m_numShares = 0;
}


/*
NAME
    TradingObject::addCapital

SYNOPSIS
    void TradingObject::addCapital(double a_amount)

    a_amount   -> Amount to be added to the current capital invested in
                  this trading object

DESCRIPTION
    Adds capital to trading object

RETURNS
    Nothing

AUTHOR
    Ivaylo Nenovski

DATE
    April 3, 2018
*/
void TradingObject::addCapital(double a_amount) {
    m_capitalInObject += a_amount;
    m_dailyCapital.push_back(a_amount);
}


/*
NAME
    TradingObject::removeCapital

SYNOPSIS
    void TradingObject::removeCapital(double a_amount)

    a_amount   -> Amount to be removed to the current capital invested in
                  this trading object

DESCRIPTION
    Removes capital to trading object

RETURNS
    Nothing

AUTHOR
    Ivaylo Nenovski

DATE
    April 3, 2018
*/
void TradingObject::removeCapital(double a_amount) {
    m_capitalInObject -= a_amount;
    m_dailyCapital.push_back(-a_amount);
}


/*
NAME
    TradingObject::addTransaction

SYNOPSIS
    void TradingObject::addTransaction(int a_amount)

    a_amount   -> Amount of new transactions that occurred and need to be
                  added to the trading object's total transactions

DESCRIPTION
    Updates total transaction count and keeps a record of every amount of transactions
    that ever occurred. A transaction can be either going long or short.

RETURNS
    Nothing

AUTHOR
    Ivaylo Nenovski

DATE
    April 3, 2018
*/
void TradingObject::addTransaction(int a_amount) {
    m_totalTransactions += a_amount;
    m_dailyTransactions.push_back(a_amount);
}


/*
NAME
    TradingObject::addShares

SYNOPSIS
    void TradingObject::addShares(int a_amount)

    a_amount   -> Amount of new shares bought when going long

DESCRIPTION
    Adds amount of shares to the trading object when going long

RETURNS
    Nothing

AUTHOR
    Ivaylo Nenovski

DATE
    April 3, 2018
*/
void TradingObject::addShares(int a_amount) {
    m_numShares += a_amount;
    m_dailyShares.push_back(a_amount);
}


/*
NAME
    TradingObject::removeShares

SYNOPSIS
    void TradingObject::removeShares(int a_amount)

    a_amount   -> Amount of shares sold when going short

DESCRIPTION
    Removes amount of shares from the trading object when going short

RETURNS
    Nothing

AUTHOR
    Ivaylo Nenovski

DATE
    April 3, 2018
*/
void TradingObject::removeShares(int a_amount) {
    m_numShares -= a_amount;
    m_dailyShares.push_back(-a_amount);
}


/*
NAME
    TradingObject::addSignal

SYNOPSIS
    void TradingObject::addSignal(double a_signal)

    a_signal   -> Signal to be recorded for the current date

DESCRIPTION
    The trading object keeps a history of every calculated signal for
    every trading date. This function adds the signal for the current date
    to the history vector of all signals

RETURNS
    Nothing

AUTHOR
    Ivaylo Nenovski

DATE
    April 3, 2018
*/
void TradingObject::addSignal(double a_signal) {
    m_signals.push_back(a_signal);
}


/*
NAME
    TradingObject::addDate

SYNOPSIS
    void TradingObject::addDate(DateTime a_date)

    a_date   -> Date to be added to the history of trading dates

DESCRIPTION
    Keeps history of all trading dates for current trading object

RETURNS
    Nothing

AUTHOR
    Ivaylo Nenovski

DATE
    April 3, 2018
*/
void TradingObject::addDate(DateTime a_date) {
    m_tradingDates.push_back(a_date);
}


/*
NAME
    TradingObject::getName

SYNOPSIS
    std::string TradingObject::getName()

DESCRIPTION
    Returns ticker that the current trading object represents

RETURNS
    Nothing

AUTHOR
    Ivaylo Nenovski

DATE
    April 3, 2018
*/
std::string TradingObject::getName() {
    return m_tickerName;
}


/*
NAME
    TradingObject::getDates

SYNOPSIS
    std::vector<DateTime> TradingObject::getDates()

DESCRIPTION
    Returns history vector for all trading dates for current ticker

RETURNS
    Same as description!

AUTHOR
    Ivaylo Nenovski

DATE
    April 3, 2018
*/
std::vector<DateTime> TradingObject::getDates() {
    return m_tradingDates;
}


/*
NAME
    TradingObject::getSignals

SYNOPSIS
    std::vector<double> TradingObject::getSignals()

DESCRIPTION
    Returns history vector for all signals for current ticker

RETURNS
    Same as description!

AUTHOR
    Ivaylo Nenovski

DATE
    April 3, 2018
*/
std::vector<double> TradingObject::getSignals() {
    return m_signals;
}


/*
NAME
    TradingObject::getDailyCapital

SYNOPSIS
    std::vector<double> TradingObject::getDailyCapital()

DESCRIPTION
    Returns history vector for all daily capital changes for
    current ticker

RETURNS
    Same as description!

AUTHOR
    Ivaylo Nenovski

DATE
    April 3, 2018
*/
std::vector<double> TradingObject::getDailyCapital() {
    return m_dailyCapital;
}


/*
NAME
    TradingObject::getDailyTransactions

SYNOPSIS
    std::vector<int> TradingObject::getDailyTransactions()

DESCRIPTION
    Returns history vector for all transactions for each day for
    current ticker

RETURNS
    Same as description!

AUTHOR
    Ivaylo Nenovski

DATE
    April 3, 2018
*/
std::vector<int> TradingObject::getDailyTransactions() {
    return m_dailyTransactions;
}


/*
NAME
    TradingObject::getDailyShares

SYNOPSIS
    std::vector<int> TradingObject::getDailyShares()

DESCRIPTION
    Returns history vector for number of shares held for each day for
    current ticker

RETURNS
    Same as description!

AUTHOR
    Ivaylo Nenovski

DATE
    April 3, 2018
*/
std::vector<int> TradingObject::getDailyShares() {
    return m_dailyShares;
}
