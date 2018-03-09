//
// Created by inenovski on 2/27/18.
//

#include "ModelA.h"

ModelA::ModelA(int argc, char** argv) : Simulator(argc, argv) {
    // empty constructor
}


double ModelA::calculateTicker() {

}


void ModelA::runSimulation() {
    // prepare the model for a simulation run
    prepareModel();

    // iterate over all tickers in the database
    for(auto tickerName = m_db->begin(); tickerName != m_db->end(); ++tickerName) {
        // get the ticker historical data
        TickerData& currentTickerData = (*m_db)[*tickerName];

        // create a trading object with the ticker name
        m_tradingContainer.emplace_back(TradingObject(*tickerName));

        //iterate over existing dates
        for(auto date = currentTickerData.begin(); date != currentTickerData.end(); ++date) {
            // get today's prices
            std::vector<double> todayPrices = currentTickerData[*date];

            // defend against dates for which historical data does not exist e.g. July 4th
            if(todayPrices.empty()) {
                continue;
            }

            // calculate the new 20 days moving average

            //calculate signal

            //determine whether to go long or short depending on the signal

            //complete any transactions and record statistics
        }
    }
}

