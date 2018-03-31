//
// Created by inenovski on 2/18/18.
//

#include "Simulator.h"


Simulator::Simulator(int argc, char** argv) : m_argc(argc), m_argv(argv) {
    m_out.open("TestingResults.txt");
    m_positiveSign = true;
}


/*
NAME
    Simulator::prepareModel

SYNOPSIS
    void Simulator::prepareModel()

DESCRIPTION
    Prepare the model for trading - Initialize the database and read the
    configuration files. Initialize any configuration parameters needed
    in this function as well.

RETURNS
    Nothing

AUTHOR
    Ivaylo Nenovski

DATE
    March 3, 2018
*/
void Simulator::prepareModel() {
    m_parser = std::make_unique<ConfigParser>(m_argv[1]);
    m_parser->parseConfigurations();

    m_configs = std::make_unique<Configurations>(m_parser->getConfigs());
    m_msgLog  = std::make_unique<MessageLog>();

    m_db = &(Database::getDatabaseInstance(m_configs->accessParameter("universe", "data_directory"),
                                           m_configs->accessParameter("universe", "universe_file")));

    //trading parameters
    m_availableCap          = std::stod(m_configs->accessParameter("portfolioconstraints", "available_capital"));
    m_maxCapPerStock        = std::stod(m_configs->accessParameter("portfolioconstraints", "max_cap_per_stock"));
    m_entrySig              = std::stod(m_configs->accessParameter("entry", "entry_signal_threshold"));
    m_exitSig               = std::stod(m_configs->accessParameter("exit", "exit_signal_threshold"));
    m_exitDaysInPosition    = std::stoi(m_configs->accessParameter("exit", "days_in_position"));
    m_positionsPerTradeBuy  = std::stoi(m_configs->accessParameter("trading", "positions_per_trade_buy"));
    m_positionsPerTradeSell = std::stoi(m_configs->accessParameter("trading", "positions_per_trade_sell"));
    m_maxPositionsPerInstrument = std::stoi(m_configs->accessParameter("portfolioconstraints",
                                                                       "max_positions_per_instrument"));

    //statistics parameters
    m_dailyReport        = std::stoi(m_configs->accessParameter("reports", "daily"));
    m_monthlyReport      = std::stoi(m_configs->accessParameter("reports", "monthly"));
    m_transactionsReport = std::stoi(m_configs->accessParameter("reports", "transaction"));
}


/*
NAME
    Simulator::invertSignals

SYNOPSIS
    void Simulator::invertSignals()

DESCRIPTION
    To invert the signs of the entry and exit parameters whenever
    the trading signal changes signs. Also, reset the current days
    in position to 0. Invertion is curcial for trading, because this
    is how one knows when to buy and when to sell

RETURNS
    Nothing

AUTHOR
    Ivaylo Nenovski

DATE
    March 28, 2018
*/
void Simulator::invertSignals() {
    m_signalInvertionsPerInstrument++;
    m_currentDaysInPosition = 0;
    m_positiveSign = !m_positiveSign;
    m_entrySig = -m_entrySig;
    m_exitSig  = -m_exitSig;
}


/*
NAME
    Simulator::positiveSignTrading

SYNOPSIS
    void Simulator::positiveSignTrading(double& a_signal, double& a_TickerPrice)

    a_signal        -> The calculated signal for the current date, current stock
    a_TickerPrice   -> The price for the corresponding ticker at the current trading
                       date

DESCRIPTION
    Whenever the entry and exit parameters are positive we want
    to sell!! Handle selling my taking into consideration all of
    the given parameters and if possible sell instruments

RETURNS
    Nothing

AUTHOR
    Ivaylo Nenovski

DATE
    March 28, 2018
*/
void Simulator::positiveSignTrading(double& a_signal, double& a_TickerPrice) {
    // code to handle selling

    if(a_signal >= m_exitSig) {
        //halt trading, hold on positions, and wait for price to go up
        m_out << "Signal too high! Halt!" << std::endl;
        return;
    }

    if(a_signal >= m_entrySig) { // && a_signal <= m_exitSig
        int  numInstrumentsSell = m_currentPositionsHeld > m_positionsPerTradeSell ?
                                  m_positionsPerTradeSell : m_currentPositionsHeld;
        bool daysInPositionOver = m_currentDaysInPosition > m_exitDaysInPosition;

        if(daysInPositionOver) {
            m_out << "Max days in position exceeded! Halt position and don't trade anymore!" << "\n";
            return;
        }

        m_currentPositionsHeld -= numInstrumentsSell;
        m_availableCap += numInstrumentsSell * a_TickerPrice;
        m_capInCurrentStock -= numInstrumentsSell * a_TickerPrice;
        m_currentDaysInPosition++;

        m_out << numInstrumentsSell << " stocks were sold today at a price of " << a_TickerPrice << "\n";
    }
}


/*
NAME
    Simulator::negativeSignTrading

SYNOPSIS
    void Simulator::negativeSignTrading(double& a_signal, double& a_TickerPrice)

    a_signal        -> The calculated signal for the current date, current stock
    a_TickerPrice   -> The price for the corresponding ticker at the current trading
                       date

DESCRIPTION
    Whenever the entry and exit parameters are negative we want
    to buy!! Handle buying my taking into consideration all of
    the given parameters and if possible buy instruments

RETURNS
    Nothing

AUTHOR
    Ivaylo Nenovski

DATE
    March 28, 2018
*/
void Simulator::negativeSignTrading(double& a_signal, double& a_TickerPrice) {
    // code to handle buying

    if(a_signal <= m_exitSig) {
        //halt trading, hold on positions, and wait for price to go up
        m_out << "Signal too low! Halt!" << "\n";
        return;
    }

    if(a_signal <= m_entrySig) { // && a_signal >= m_exitSig
        double potentialAvailableCapAfterBuy = m_availableCap - m_positionsPerTradeBuy * a_TickerPrice;
        double CapInStockAfterBuy            = m_capInCurrentStock + m_positionsPerTradeBuy * a_TickerPrice;
        bool   daysInPositionOver            = m_currentDaysInPosition > m_exitDaysInPosition;
        bool   actionPerformed               = false;

        if(daysInPositionOver || m_currentPositionsHeld > m_maxPositionsPerInstrument) {
            m_out << "Days in position exceeded or too many instruments held! " <<
                     "Halt position and don't trade anymore!\n";
            return;
        }

        if(potentialAvailableCapAfterBuy > 0 && CapInStockAfterBuy < m_maxCapPerStock) {
            m_currentPositionsHeld += m_positionsPerTradeBuy;
            m_availableCap -= m_positionsPerTradeBuy * a_TickerPrice;
            m_capInCurrentStock += m_positionsPerTradeBuy * a_TickerPrice;
            m_currentDaysInPosition++;
            actionPerformed = true;
        }

        if(actionPerformed) {
            m_out << m_positionsPerTradeBuy << " stocks were bought today at a price of " << a_TickerPrice << "\n";
        }
        else {
            m_out << "No trading was performed today!\n";
        }
    }
}


/*
NAME
    Simulator::handleTrading

SYNOPSIS
    void Simulator::handleTrading(double a_signal, int& a_daysInPosition)

    a_signal        -> The calculated signal for the current date, current stock
    a_TickerPrice   -> The price for the corresponding ticker at the current trading
                       date

DESCRIPTION
    Main function to handle trading. It will make sure that the
    signal hasn't changed signs, but if it has then it will call invertSign.
    After that it uses positive or negative tradding depending on the
    inverted signal

RETURNS
    Nothing

AUTHOR
    Ivaylo Nenovski

DATE
    March 28, 2018
*/
void Simulator::handleTrading(double a_signal, double& a_TickerPrice) {

    if((m_positiveSign && a_signal < 0) || (!m_positiveSign && a_signal > 0)) {
        invertSignals();
    }

    if(m_positiveSign) {
        positiveSignTrading(a_signal, a_TickerPrice);
    }
    else {
        negativeSignTrading(a_signal, a_TickerPrice);
    }

}


/*
NAME
    Simulator::recordStatistics

SYNOPSIS
    void Simulator::recordStatistics()

DESCRIPTION
    Fill this when function is done..

RETURNS
    Nothing

AUTHOR
    Ivaylo Nenovski

DATE
    March 28, 2018
*/
void Simulator::recordStatistics() {
    m_out << "\n\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
    m_out << "Configurations used for current simulation:\n";
    m_out << *m_configs << "\n";
}
