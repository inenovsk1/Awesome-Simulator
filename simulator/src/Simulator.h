/*
 * ~~~~~~~~~~~~~~~~~~~~ Simulator class ~~~~~~~~~~~~~~~~~~~~
 *
 * Container for the entire simulator. Every other class will either be contained
 * in this one or derived from it. This class will deal with all the intricacies
 * of trading so that if anyone wants to build a new strategy one can simply
 * derive from this class and overwrite its virtual functions!
 * */

#pragma once

#include <memory>
#include <iomanip>
#include <fstream>
#include <cmath>

#include "ConfigParser.h"
#include "Configurations.h"
#include "Database.h"
#include "DateTime.h"
#include "TickerData.h"
#include "TradingObject.h"
#include "Utilities.h"


class Simulator {

public:
    Simulator()=default;
    Simulator(int argc, char** argv);
    void prepareModel();
    void handleTrading(double a_signal, double& a_TickerPrice, TradingObject& a_tradingObject);
    void recordStatistics();
    void run_all_configs();
    void run_with_different_parameter_specified_on_command_line();

    virtual double calculateSignal(double a_currentAdjClose) = 0;
    virtual void   runSimulation() = 0;

protected:
    // member variables
    int                              m_argc;
    char**                           m_argv;
    Database*                        m_db;
    std::unique_ptr<ConfigParser>    m_parser;
    std::unique_ptr<Configurations>  m_configs;
    std::vector<TradingObject>       m_tradingContainer;

    double    m_availableCap;
    double    m_capInvested;
    double    m_capReturned;
    double    m_maxCapPerStock;
    double    m_capInStock;
    double    m_entrySig;
    double    m_exitSig;
    int       m_exitDaysInPosition;
    int       m_currentDaysInPosition;
    int       m_sharesHeld;
    int       m_positionsPerTradeBuy;
    int       m_positionsPerTradeSell;
    int       m_maxPositionsPerInstrument;
    int       m_signalInvertionsPerInstrument;
    bool      m_positiveSign;

    int       m_dailyReport;
    int       m_monthlyReport;
    int       m_transactionsReport;

    double    m_yesterdayCapInStock;
    double    m_yesterdayAvailableCap;

    std::ofstream m_out;
    std::ofstream m_transactionStatistics;
    std::ofstream m_dailyStatistics;

    // member functions
    void invertSignals();
    void positiveSignTrading(double& a_signal, double& a_TickerPrice, TradingObject& a_tradingObject);
    void negativeSignTrading(double& a_signal, double& a_TickerPrice, TradingObject& a_tradingObject);

    void dailyReport();
    void transactionReport();
};
