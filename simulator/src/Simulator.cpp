//
// Created by inenovski on 2/18/18.
//

#include "Simulator.h"


Simulator::Simulator(int argc, char** argv) : m_argc(argc), m_argv(argv) {
    m_out.open("TradingSummary.txt");
    m_transactionStatistics.open("TransactionReport.txt");
    m_dailyStatistics.open("DailyReport.txt");
    m_positiveSign = true;

    m_yesterdayCapInStock = 0;
    m_yesterdayAvailableCap = m_availableCap;
    m_capInvested = 0;
    m_capReturned = 0;
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
    a_tradingObject -> The trading object for the current ticker that's being traded

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
void Simulator::positiveSignTrading(double& a_signal, double& a_TickerPrice, TradingObject& a_tradingObject) {
    // code to handle selling

    bool actionPerformed = false;

    if(a_signal >= m_exitSig) {
        //halt trading, hold on positions, and wait for price to go up
        a_tradingObject.removeShares(0);
        a_tradingObject.addCapital(0);
        a_tradingObject.addTransaction(0);

        return;
    }

    if(a_signal >= m_entrySig) {
        int  numInstrumentsSell = m_sharesHeld > m_positionsPerTradeSell ?
                                  m_positionsPerTradeSell : m_sharesHeld;

        bool daysInPositionOver = m_currentDaysInPosition > m_exitDaysInPosition;

        if(daysInPositionOver) {
            a_tradingObject.removeShares(0);
            a_tradingObject.addCapital(0);
            a_tradingObject.addTransaction(0);

            return;
        }

        m_sharesHeld -= numInstrumentsSell;
        m_availableCap += numInstrumentsSell * a_TickerPrice;
        m_capInStock -= numInstrumentsSell * a_TickerPrice;
        m_currentDaysInPosition++;

        a_tradingObject.removeShares(numInstrumentsSell);
        a_tradingObject.addCapital(numInstrumentsSell * a_TickerPrice);
        a_tradingObject.addTransaction(numInstrumentsSell);
        m_capReturned += numInstrumentsSell * a_TickerPrice;

        // daily risk of return
        //a_tradingObject.addDailyRateOfReturn((a_TickerPrice - m_priceAtWhichWentLong) / m_priceAtWhichWentLong * 100);
        actionPerformed = true;
    }

    // in case signal is too big and did not fit in the entry/exit signal bracket
    if(actionPerformed) {
        return;
    }
    else {
        a_tradingObject.removeShares(0);
        a_tradingObject.addCapital(0);
        a_tradingObject.addTransaction(0);
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
    a_tradingObject -> The trading object for the current ticker that's being traded

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
void Simulator::negativeSignTrading(double& a_signal, double& a_TickerPrice, TradingObject& a_tradingObject) {
    // code to handle buying

    bool actionPerformed = false;

    if(a_signal <= m_exitSig) {
        //halt trading, hold on positions, and wait for price to go up

        a_tradingObject.addShares(0);
        a_tradingObject.removeCapital(0);
        a_tradingObject.addTransaction(0);

        return;
    }

    if(a_signal <= m_entrySig) {
        double potentialAvailableCapAfterBuy = m_availableCap - m_positionsPerTradeBuy * a_TickerPrice;
        double CapInStockAfterBuy            = m_capInStock + m_positionsPerTradeBuy * a_TickerPrice;
        bool   daysInPositionOver            = m_currentDaysInPosition > m_exitDaysInPosition;

        if(daysInPositionOver || m_sharesHeld > m_maxPositionsPerInstrument) {

            a_tradingObject.addShares(0);
            a_tradingObject.removeCapital(0);
            a_tradingObject.addTransaction(0);

            return;
        }

        if(potentialAvailableCapAfterBuy > 0 && CapInStockAfterBuy < m_maxCapPerStock) {
            m_sharesHeld += m_positionsPerTradeBuy;
            m_availableCap -= m_positionsPerTradeBuy * a_TickerPrice;
            m_capInStock += m_positionsPerTradeBuy * a_TickerPrice;
            m_currentDaysInPosition++;

            a_tradingObject.addShares(m_positionsPerTradeBuy);
            a_tradingObject.removeCapital(m_positionsPerTradeBuy * a_TickerPrice);
            a_tradingObject.addTransaction(m_positionsPerTradeBuy);
            m_capInvested += m_positionsPerTradeBuy * a_TickerPrice;

            //m_priceAtWhichWentLong = a_TickerPrice;
            actionPerformed = true;
        }
    }

    // in case signal was too low to enter the entry/exit signal bracket
    if(actionPerformed) {
        return;
    }
    else {
        a_tradingObject.addShares(0);
        a_tradingObject.removeCapital(0);
        a_tradingObject.addTransaction(0);
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
    a_tradingObject -> The trading object for the current ticker that's being traded

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
void Simulator::handleTrading(double a_signal, double& a_TickerPrice, TradingObject& a_tradingObject) {

    if ((m_positiveSign && a_signal < 0) || (!m_positiveSign && a_signal > 0)) {
        invertSignals();
    }

    if (m_positiveSign) {
        positiveSignTrading(a_signal, a_TickerPrice, a_tradingObject);
    }
    else {
        negativeSignTrading(a_signal, a_TickerPrice, a_tradingObject);
    }

    a_tradingObject.addDailyPNL(m_capInStock - m_yesterdayCapInStock);
    m_yesterdayCapInStock = m_capInStock;

    a_tradingObject.addCumulativePNL(m_availableCap - m_yesterdayAvailableCap);
    m_yesterdayAvailableCap = m_availableCap;

    a_tradingObject.addTotalMarketValue(m_sharesHeld * a_TickerPrice);
    a_tradingObject.addNetMarketValue(m_capInvested / m_capReturned);
    a_tradingObject.addImbalance(a_tradingObject.getNetMarketValue().back() / a_tradingObject.getTotalMarketValue().back());

}


/*
NAME
    Simulator::dailyReport

SYNOPSIS
    void Simulator::dailyReport()

DESCRIPTION
    Outputs daily report for the simulator:

    * total num of positions (in my case these are the shares owned)
    * dollar PNL for current stock = value today - value yesterday
    * cumulative dollar PNL = value today - value yesterday
    * total market value = num shares * price
    * net market value = how much currently win/lose
    * Imbalance = Net/Total
    * Sharpe ratio*

RETURNS
    Nothing

AUTHOR
    Ivaylo Nenovski

DATE
    April 14, 2018
*/
void Simulator::dailyReport() {
    DateTime today = Utils::today();

    m_dailyStatistics << "Transaction report for simulation ran at: " << today << "\n\n\n";

    for (auto trObject = m_tradingContainer.begin(); trObject != m_tradingContainer.end(); ++trObject) {
        m_dailyStatistics << "Daily Statistics for ticker    -> " << trObject->getName() << "\n";
        m_dailyStatistics << "Date            Signal          Shares          PNL          Cum. PNL          Total Market Value          Net Market Value          Imbalance\n";

        for (unsigned int i = 0; i < trObject->getDates().size(); ++i) {
            m_dailyStatistics << std::right << trObject->getDates().at(i) <<
                                 std::setw(10) << std::right << std::setprecision(5) << trObject->getSignals().at(i) <<
                                 std::setw(17) << std::right << std::setprecision(5) << trObject->getDailyShares().at(i) <<
                                 std::setw(14) << std::right << std::setprecision(5) << trObject->getDailyPNL().at(i) <<
                                 std::setw(15) << std::right << std::setprecision(5) << trObject->getCumulativePNL().at(i) <<
                                 std::setw(23) << std::right << std::setprecision(5) << trObject->getTotalMarketValue().at(i) <<
                                 std::setw(27) << std::right << std::setprecision(5) << trObject->getNetMarketValue().at(i) <<
                                 std::setw(24) << std::right << std::setprecision(5) << trObject->getImbalance().at(i) << "\n\n";
        }
    }

    m_dailyStatistics << "\n\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
    m_dailyStatistics << "Configurations used for current simulation:\n";
    m_dailyStatistics << *m_configs << "\n";
}


/*
NAME
    Simulator::transactionReport

SYNOPSIS
    void Simulator::transactionReport()

DESCRIPTION
    Outputs report about every transaction that occurred during the simulation!

RETURNS
    Nothing

AUTHOR
    Ivaylo Nenovski

DATE
    April 2, 2018
*/
void Simulator::transactionReport() {
    DateTime today = Utils::today();

    m_transactionStatistics << "Transaction report for simulation ran at: " << today << "\n\n\n";

    for (auto trObject = m_tradingContainer.begin(); trObject != m_tradingContainer.end(); ++trObject) {
        m_transactionStatistics << "Statistics for ticker    -> " << trObject->getName() << "\n";
        m_transactionStatistics << "Date             Signal             Shares             Transactions             Capital In Object\n";

        for (unsigned int i = 0; i < trObject->getDates().size(); ++i) {
            m_transactionStatistics << std::setw(2) << std::right << trObject->getDates().at(i) <<
                              std::setw(13) << std::right << std::setprecision(5) << trObject->getSignals().at(i) <<
                              std::setw(17) << std::right << std::setprecision(5) << trObject->getDailyShares().at(i) <<
                              std::setw(24) << std::right << std::setprecision(5) << trObject->getDailyTransactions().at(i) <<
                              std::setw(25) << std::right << std::setprecision(5) << trObject->getDailyCapital().at(i) << "\n\n";
        }
    }

    m_transactionStatistics << "\n\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
    m_transactionStatistics << "Configurations used for current simulation:\n";
    m_transactionStatistics << *m_configs << "\n";
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

    if(m_dailyReport == 1) {
        dailyReport();
    }

    if (m_transactionsReport == 1) {
        transactionReport();
    }
}
