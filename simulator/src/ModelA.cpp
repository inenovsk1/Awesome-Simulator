#include "ModelA.h"


ModelA::ModelA(int argc, char** argv) : Simulator(argc, argv) {
    m_twentyDaysPrices = new double[MOVING_AVG_DAY_RANGE];
    m_currentDaysInPosition = 0;
}


ModelA::~ModelA() {
    delete[] m_twentyDaysPrices;
}


/*
NAME
    ModelA::calculateSignal

SYNOPSIS
    double ModelA::calculateSignal(double a_currentAdjClose)

    a_currentAdjClose  -> Today's adjusted close price for current ticker

DESCRIPTION
    Compute the signal according to the 20 day moving average

RETURNS
    A signal for the ticker

AUTHOR
    Ivaylo Nenovski

DATE
    March 14, 2018
*/
double ModelA::calculateSignal(double a_currentAdjClose) {
    return ((a_currentAdjClose - m_20DayMovingAvg) / m_20DayMovingAvg) * 100; // removed the negative sign upfront (-)
}


/*
NAME
    ModelA::calculate20DayMovingAvg

SYNOPSIS
    void ModelA::calculate20DayMovingAvg(double a_todayAdjClose)

    a_todayAdjClose  -> Today's adjusted close price for current ticker

DESCRIPTION
    Shift the prices in the m_twentyDaysPrices array with 1 day backwards,
    after which add the today adjusted close price at the beginning.
    After the shift is complete, calculate the new moving average!

RETURNS
    Nothing

AUTHOR
    Ivaylo Nenovski

DATE
    March 14, 2018
*/
void ModelA::calculate20DayMovingAvg(double a_todayAdjClose) {
    //shift all prices with one day backwards
    for(int i = 0; i < MOVING_AVG_DAY_RANGE - 1; ++i) {
        m_twentyDaysPrices[i] = m_twentyDaysPrices[i + 1];
    }

    //add newest price at the end of array of last 20 days prices
    m_twentyDaysPrices[MOVING_AVG_DAY_RANGE - 1] = a_todayAdjClose;

    //calculate the new moving average
    double sum = 0;

    for(int i = 0; i < MOVING_AVG_DAY_RANGE; ++i) {
        sum += m_twentyDaysPrices[i];
    }

    m_20DayMovingAvg = sum / MOVING_AVG_DAY_RANGE;
}


/*
NAME
    ModelA::runSimulation

SYNOPSIS
    void ModelA::runSimulation()

DESCRIPTION
    This is the main function of the model. It calls all other necessary
    functions to perform a simulation. Initially it calls prepareModel to prepare
    the model. After that it iterates over all existing tickers in the database
    and for each date that there is price data, it calculates a signal, based
    on which it calls handleTrading(). handleTrading, on the other hand,
    make trading decisions according to the configuration parameters passed, and
    records everything in the TradingObject object for each ticker. At the end
    the recordStatistics function is called, which outputs every trade, statistic,
    calculation to a file

RETURNS
    Nothing

AUTHOR
    Ivaylo Nenovski

DATE
    March 14, 2018
*/
void ModelA::runSimulation() {
    // prepare the model for a simulation run
    prepareModel();

    double initialCap = m_availableCap;
    m_totalCapInvested = 0;
    m_totalCapReturned = 0;
    m_totalSharesHeld = 0;

    // iterate over all tickers in the database
    for(auto tickerName = m_db->begin(); tickerName != m_db->end(); ++tickerName) {
        //setup initial variables for each ticker
        m_currentDaysInPosition = 0;
        m_capInStock = 0;
        m_signalInvertionsPerInstrument = 0;
        m_stockCapInvested = 0;
        m_stockCapReturned = 0;
        m_stockSharesHeld = 0;

        std::cout << "Currently trading ticker -> " << *tickerName << "\n";

        double totalCapitalInCirculationForTicker = m_availableCap;
        m_out << "\nStats for ticker -> " << *tickerName << ":\n";
        m_out << "Available capital -> " << m_availableCap << "\n";

        // get the ticker historical data
        TickerData& currentTickerData = (*m_db)[*tickerName];

        // create a trading object with the ticker name
        TradingObject currentTradingObject(*tickerName);

        //skip trading the first 20 days until there is enough info for calculating moving avg
        short twentyDays = 0;

        //reset current amount invested in stock
        m_capInStock = 0;

        //iterate over existing dates
        for(auto date = currentTickerData.begin(); date != currentTickerData.end(); ++date) {
            currentTradingObject.addDate(*date);

            // get today's prices
            std::vector<double> todayPrices = currentTickerData[*date];

            // collect the first 20 days for calculating moving avg
            if(twentyDays < MOVING_AVG_DAY_RANGE) {
                m_twentyDaysPrices[twentyDays] = todayPrices[TickerData::FieldID_ADJ_CLOSE];
                currentTradingObject.addSignal(0);
                currentTradingObject.addShares(0);
                currentTradingObject.addCapital(0);
                currentTradingObject.addTransaction(0);
                currentTradingObject.addDailyPNL(0);
                currentTradingObject.updateCumulativePNL(0);
                currentTradingObject.addStockTotalMarketValue(0);
                currentTradingObject.addStockNetMarketValue(0);
                currentTradingObject.addStockImbalance(0);
                twentyDays++;
                continue;
            }

            //calculate the INITIAL moving average
            if(twentyDays == MOVING_AVG_DAY_RANGE) {
                twentyDays++; // increment so it does not fall in the other if statements that involve this variable

                double sum = 0;

                for(int i = 0; i < MOVING_AVG_DAY_RANGE; ++i) {
                    sum += m_twentyDaysPrices[i];
                }

                m_20DayMovingAvg = sum / MOVING_AVG_DAY_RANGE;
            }

            // defend against dates for which historical data does not exist e.g. July 4th
            if(todayPrices.empty()) {
                currentTradingObject.addSignal(0);
                currentTradingObject.addShares(0);
                currentTradingObject.addCapital(0);
                currentTradingObject.addTransaction(0);
                currentTradingObject.addDailyPNL(0);
                currentTradingObject.updateCumulativePNL(0);
                currentTradingObject.addStockTotalMarketValue(0);
                currentTradingObject.addStockNetMarketValue(0);
                currentTradingObject.addStockImbalance(0);
                continue;
            }

            double currentAdjClose = todayPrices[TickerData::FieldID_ADJ_CLOSE];

            //calculate signal
            double signal = calculateSignal(currentAdjClose);
            currentTradingObject.addSignal(signal);
            //std::cout << "Calculating signal for ticker " << *tickerName << " at " << *date << ": " << signal << std::endl;

            // update the new moving average
            calculate20DayMovingAvg(currentAdjClose);

            // use quadruple root of the product of open, low, high, close prices to approximate the trading price
            double tradingPrice = std::pow(todayPrices[TickerData::FieldID_OPEN] * todayPrices[TickerData::FieldID_HIGH] *
                                           todayPrices[TickerData::FieldID_LOW] * todayPrices[TickerData::FieldID_CLOSE], 1.0/4);

            // determine whether to go long or short depending on the signal and buy/sell
            handleTrading(signal, tradingPrice, currentTradingObject);
        }

        m_tradingContainer.push_back(currentTradingObject);

        totalCapitalInCirculationForTicker -= m_availableCap;
        m_out << "Available Capital after trading -> " << m_availableCap << "\n";

        if(totalCapitalInCirculationForTicker > 0) {
            m_out << "Capital lost from stock -> " << totalCapitalInCirculationForTicker << "\n";
        }
        else {
            m_out << "Capital won from stock -> " << std::abs(totalCapitalInCirculationForTicker) << "\n";
        }

        m_out << "# Invertions occurred: " << m_signalInvertionsPerInstrument << "\n";
    }

    std::cout << "\nTotal earnings -> " << (m_availableCap - initialCap) << std::endl;
    m_out << "\nTotal earnings -> " << (m_availableCap - initialCap) << "\n";

    // record statistics for the simulation
    recordStatistics();
}

