/*
 * ~~~~~~~~~~~~~~~~~~~~ TradingObject class ~~~~~~~~~~~~~~~~~~~~
 *
 * This class will hold trading information about the ticker that it represents.
 * Information like the ticker name, the current capital invested in this ticker,
 * total transactions done, daily PNL, and more. It will also keep history of number
 * of transactions done for each day, calculated signal for each trading date, etc..
 * */

#pragma once

#include <string>
#include <vector>

#include "DateTime.h"


class TradingObject {

public:
    TradingObject()=default;
    explicit TradingObject(std::string a_tickerName);

    void addCapital(double a_amount);
    void removeCapital(double a_amount);
    void addTransaction(int a_amount);
    void addShares(int a_amount);
    void removeShares(int a_amount);
    void addSignal(double a_signal);
    void addDate(DateTime a_date);
    void addDailyPNL(double a_amount);
    void updateCumulativePNL(double a_amount);
    void addStockTotalMarketValue(double a_amount);
    void addStockNetMarketValue(double a_amount);
    void addStockImbalance(double a_amount);

    std::string getName();
    std::vector<DateTime> getDates();
    std::vector<double> getSignals();
    std::vector<double> getDailyCapital();
    std::vector<double> getDailyPNL();
    std::vector<double> getCumulativePNL();
    std::vector<double> getStockTotalMarketValue();
    std::vector<double> getStockNetMarketValue();
    std::vector<double> getStockImbalance();
    std::vector<int> getDailyTransactions();
    std::vector<int> getDailyShares();

private:
    std::string  m_tickerName;
    double       m_capitalInObject;
    int          m_totalTransactions; // total number of going long/short for this trading object
    int          m_numShares;

    std::vector<DateTime>  m_tradingDates;
    std::vector<double>    m_signals;
    std::vector<double>    m_dailyPNL;
    std::vector<double>    m_cumulativePNL;  // this is ticker based cumulative PNL, there's also TOTAl PNL in class simulator
    std::vector<double>    m_dailyCapital;
    std::vector<double>    m_stockTotalMarketValue;
    std::vector<double>    m_stockNetMarketValue;
    std::vector<double>    m_stockImbalance;
    std::vector<int>       m_dailyTransactions;
    std::vector<int>       m_dailyShares;
};
