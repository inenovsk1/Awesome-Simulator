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
    TradingObject::addDailyPNL

SYNOPSIS
    void TradingObject::addDailyPNL(double a_amount)

    a_amount   -> Adds PNL for current day

DESCRIPTION
    Keeps history of daily PNL for the current trading object

RETURNS
    Nothing

AUTHOR
    Ivaylo Nenovski

DATE
    April 14, 2018
*/
void TradingObject::addDailyPNL(double a_amount) {
    m_dailyPNL.push_back(a_amount);
}


/*
NAME
    TradingObject::addCumulativePNL

SYNOPSIS
    void TradingObject::addCumulativePNL(double a_amount)

    a_amount   -> Adds cumulative PNL for current day

DESCRIPTION
    Keeps history of cumulative PNL on the whole available capital

RETURNS
    Nothing

AUTHOR
    Ivaylo Nenovski

DATE
    April 14, 2018
*/
void TradingObject::addCumulativePNL(double a_amount) {
    m_cummulativePNL.push_back(a_amount);
}


/*
NAME
    TradingObject::addTotalMarketValue

SYNOPSIS
    void TradingObject::addTotalMarketValue(double a_amount)

    a_amount   -> Adds total market value for each day owned by current trading object

DESCRIPTION
    Keeps history of total market value for the trading object for each day

RETURNS
    Nothing

AUTHOR
    Ivaylo Nenovski

DATE
    April 14, 2018
*/
void TradingObject::addTotalMarketValue(double a_amount) {
    m_totalMarketValue.push_back(a_amount);
}


/*
NAME
    TradingObject::addNetMarketValue

SYNOPSIS
    void TradingObject::addNetMarketValue(double a_amount)

    a_amount   -> Adds net market value for each day owned by current trading object

DESCRIPTION
    Keeps history of net market value for the trading object for each day

RETURNS
    Nothing

AUTHOR
    Ivaylo Nenovski

DATE
    April 14, 2018
*/
void TradingObject::addNetMarketValue(double a_amount) {
    m_netMarketValue.push_back(a_amount);
}


/*
NAME
    TradingObject::addImbalance

SYNOPSIS
    void TradingObject::addImbalance(double a_amount)

    a_amount   -> Adds imbalance for each day owned by current trading object

DESCRIPTION
    Keeps history of imbalance for the trading object for each day

RETURNS
    Nothing

AUTHOR
    Ivaylo Nenovski

DATE
    April 14, 2018
*/
void TradingObject::addImbalance(double a_amount) {
    m_imbalance.push_back(a_amount);
}


/*
NAME
    TradingObject::getDailyPNL

SYNOPSIS
    std::vector<double> TradingObject::getDailyPNL()

DESCRIPTION
    Returns the history vector for the daily PNL for the trading object

RETURNS
    Same as Description!

AUTHOR
    Ivaylo Nenovski

DATE
    April 14, 2018
*/
std::vector<double> TradingObject::getDailyPNL() {
    return m_dailyPNL;
}


/*
NAME
    TradingObject::getCumulativePNL

SYNOPSIS
    std::vector<double> TradingObject::getCumulativePNL()

DESCRIPTION
    Returns the history vector for the cumulative PNL for the trading object

RETURNS
    Same as Description!

AUTHOR
    Ivaylo Nenovski

DATE
    April 14, 2018
*/
std::vector<double> TradingObject::getCumulativePNL() {
    return m_cummulativePNL;
}


/*
NAME
    TradingObject::getTotalMarketValue

SYNOPSIS
    std::vector<double> TradingObject::getTotalMarketValue()

DESCRIPTION
    Returns the history vector for the total market value for the trading object

RETURNS
    Same as Description!

AUTHOR
    Ivaylo Nenovski

DATE
    April 14, 2018
*/
std::vector<double> TradingObject::getTotalMarketValue() {
    return m_totalMarketValue;
}


/*
NAME
    TradingObject::getNetMarketValue

SYNOPSIS
    std::vector<double> TradingObject::getNetMarketValue()

DESCRIPTION
    Returns the history vector for the net market value for the trading object

RETURNS
    Same as Description!

AUTHOR
    Ivaylo Nenovski

DATE
    April 14, 2018
*/
std::vector<double> TradingObject::getNetMarketValue() {
    return m_netMarketValue;
}


/*
NAME
    TradingObject::getImbalance

SYNOPSIS
    std::vector<double> TradingObject::getImbalance()

DESCRIPTION
    Returns the history vector for the imbalance for the trading object

RETURNS
    Same as Description!

AUTHOR
    Ivaylo Nenovski

DATE
    April 14, 2018
*/
std::vector<double> TradingObject::getImbalance() {
    return m_imbalance;
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
    Same as Description!

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
    Same as Description!

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
    Same as Description!

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
    Same as Description!

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
    Same as Description!

AUTHOR
    Ivaylo Nenovski

DATE
    April 3, 2018
*/
std::vector<int> TradingObject::getDailyShares() {
    return m_dailyShares;
}
