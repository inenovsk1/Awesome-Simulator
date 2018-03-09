//
// Created by inenovski on 2/18/18.
//

/*
 * ~~~~~~~~~~~~~~~~~~~~ Simulator class ~~~~~~~~~~~~~~~~~~~~
 *
 * Container for the entire simulator. Every other class will either be contained
 * in this one or derived from it.
 * */

#pragma once

#include <memory>

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
    virtual double calculateTicker() = 0;
    virtual void runSimulation() = 0;

    void run_all_configs();
    void run_with_different_parameter_specified_on_command_line();

protected:
    int                              m_argc;
    char**                           m_argv;
    Database*                        m_db;
    std::unique_ptr<ConfigParser>    m_parser;
    std::unique_ptr<Configurations>  m_configs;
    std::unique_ptr<MessageLog>      m_msgLog;
};
