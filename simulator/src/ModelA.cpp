//
// Created by inenovski on 2/27/18.
//

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
    return -((a_currentAdjClose - m_20DayMovingAvg) / m_20DayMovingAvg) * 100;
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


void ModelA::handleTrading(double signal) {

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

    // iterate over all tickers in the database
    for(auto tickerName = m_db->begin(); tickerName != m_db->end(); ++tickerName) {
        // get the ticker historical data
        TickerData& currentTickerData = (*m_db)[*tickerName];

        // create a trading object with the ticker name
        m_tradingContainer.emplace_back(TradingObject(*tickerName));

        //skip trading the first 20 days until there is enough info for calculating moving avg
        short twentyDays = 0;

        //reset current amount invested in stock
        m_capInCurrentStock = 0;

        //iterate over existing dates
        for(auto date = currentTickerData.begin(); date != currentTickerData.end(); ++date) {
            // get today's prices
            std::vector<double> todayPrices = currentTickerData[*date];

            // collect the first 20 days for calculating moving avg
            if(twentyDays < MOVING_AVG_DAY_RANGE) {
                m_twentyDaysPrices[twentyDays] = todayPrices[TickerData::FieldID_ADJ_CLOSE];
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
                continue;
            }

            double currentAdjClose = todayPrices[TickerData::FieldID_ADJ_CLOSE];

            //calculate signal
            double signal = calculateSignal(currentAdjClose);
            std::cout << "Calculating ticker for " << *tickerName << " at " << *date << ": " << signal << std::endl;

            // update the new moving average
            calculate20DayMovingAvg(currentAdjClose);

            // determine whether to go long or short depending on the signal and buy/sell
            handleTrading(signal);
        }
    }

    // record statistics for the simulation
    recordStatistics();
}

