//
// Created by inenovski on 2/18/18.
//

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
#include <fstream>

#include "ConfigParser.h"
#include "Configurations.h"
#include "Database.h"
#include "DateTime.h"
#include "TickerData.h"
#include "TradingObject.h"
#include "MessageLog.h"
#include "Utilities.h"


class Simulator {

public:
    Simulator()=default;
    Simulator(int argc, char** argv);
    void prepareModel();
    void handleTrading(double a_signal, double& a_TickerPrice);
    void recordStatistics();
    void run_all_configs();
    void run_with_different_parameter_specified_on_command_line();

    virtual double calculateSignal(double a_currentAdjClose) = 0;
    virtual void   runSimulation() = 0;

protected:
    //variables
    int                              m_argc;
    char**                           m_argv;
    Database*                        m_db;
    std::unique_ptr<ConfigParser>    m_parser;
    std::unique_ptr<Configurations>  m_configs;
    std::unique_ptr<MessageLog>      m_msgLog;
    std::vector<TradingObject>       m_tradingContainer;

    double    m_availableCap;
    double    m_maxCapPerStock;
    double    m_capInCurrentStock;
    double    m_entrySig;
    double    m_exitSig;
    int       m_exitDaysInPosition;
    int       m_currentDaysInPosition;
    int       m_currentPositionsHeld;
    int       m_positionsPerTradeBuy;
    int       m_positionsPerTradeSell;
    int       m_maxPositionsPerInstrument;
    int       m_signalInvertionsPerInstrument;
    bool      m_positiveSign;

    int       m_dailyReport;
    int       m_monthlyReport;
    int       m_transactionsReport;

    std::ofstream m_out;

    // functions
    void invertSignals();
    void positiveSignTrading(double& a_signal, double& a_TickerPrice);
    void negativeSignTrading(double& a_signal, double& a_TickerPrice);
};
