//
// Created by inenovski on 2/27/18.
//

#pragma once


#include "Simulator.h"

class LongShort : Simulator {

public:
    LongShort(int argc, char** argv);
    void prepareModel() override;
    double calculateTicker() override;
    void runSimulation() override;

private:

};


