//
// Created by inenovski on 2/18/18.
//

#include "Simulator.h"


Simulator::Simulator(int argc, char** argv) : m_argc(argc), m_argv(argv) {}


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

    m_configs = std::make_unique<Configurations>(std::move(m_parser->getConfigs()));
    m_msgLog = std::make_unique<MessageLog>();

    m_db = &(Database::getDatabaseInstance(m_configs->accessParameter("universe", "data_directory"),
                                           m_configs->accessParameter("universe", "universe_file")));

    m_availableCap   = std::stod(m_configs->accessParameter("portfolioconstraints", "available_capital"));
    m_maxCapPerStock = std::stod(m_configs->accessParameter("portfolioconstraints", "max_cap_per_stock"));
    m_entrySig       = std::stod(m_configs->accessParameter("entry", "entry_signal_threshold"));
    m_exitSig        = std::stod(m_configs->accessParameter("exit", "exit_signal_threshold"));
}


void Simulator::recordStatistics() {

}
