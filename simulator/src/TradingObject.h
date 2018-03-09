//
// Created by inenovski on 2/27/18.
//

#pragma once

#include <string>


class TradingObject {

public:
    TradingObject()=default;
    explicit TradingObject(std::string a_tickerName);

private:
    std::string m_tickerName;
    double m_capitalInObject;
    int m_totalTransactions; // total number of buy/sell for this trading object

};
