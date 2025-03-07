/*
 * ~~~~~~~~~~~~~~~~~~~~ ModelA class ~~~~~~~~~~~~~~~~~~~~
 *
 * This class is the first strategy that the simulator will offer.
 * It is going to calculate a signal based on the 20 day moving average and
 * trade according to that signal.
 * */

#pragma once


#include <vector>
#include <cstdlib>
#include "Simulator.h"

class ModelA : Simulator {

public:
    ModelA(int argc, char** argv);
    ~ModelA();

    void runSimulation() override;


private:
    // member functions
    double calculateSignal(double a_currentAdjClose)   override;
    void calculate20DayMovingAvg(double a_todayAdjClose);

    // member variables
    double      m_20DayMovingAvg;
    double*     m_twentyDaysPrices;
    const int   MOVING_AVG_DAY_RANGE = 20;
};

