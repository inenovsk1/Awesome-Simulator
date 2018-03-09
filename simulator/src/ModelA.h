//
// Created by inenovski on 2/27/18.
//

#pragma once


#include <vector>
#include "Simulator.h"

class ModelA : Simulator {

public:
    ModelA(int argc, char** argv);
    double calculateTicker() override;
    void runSimulation() override;

private:
    std::vector<TradingObject> m_tradingContainer;
    double m_20DayMovingAvg;
};

