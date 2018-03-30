//
// Created by inenovski on 2/27/18.
//

/*
 * ~~~~~~~~~~~~~~~~~~~~ TradingObject class ~~~~~~~~~~~~~~~~~~~~
 *
 * This class will hold trading information about the ticker that it represents.
 * Information like the ticker name, the current capital invested in this ticker,
 * total transactions done, daily PNL, and more..
 * */

#pragma once

#include <string>
#include <vector>

#include "DateTime.h"


class TradingObject {

public:
    TradingObject()=default;
    explicit TradingObject(std::string a_tickerName);
    void buy(int amount);
    void sell(int amount);

private:
    std::string  m_tickerName;
    double       m_capitalInObject;
    int          m_totalTransactions; // total number of buy/sell for this trading object
    int          m_numShares;

    std::vector<DateTime>  m_tradingDates;
    std::vector<double>    m_dailyPNL;
};
