//
// Created by inenovski on 2/25/18.
//

#pragma once

#include <iostream>
#include <fstream>

class MessageLog {

public:
    MessageLog() = default;

private:

};

/*
 * Make it look like cout, specify file + associate severity level.
 * How significant the error is?
 * The other output is part of simulator class - simply keep data structures
 * that keep track of everything in the simulation and write reports at the end
 * -> daily statistics, trades that occurred, etc.. */