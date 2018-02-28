//
// Created by inenovski on 2/27/18.
//

#include "LongShort.h"

LongShort::LongShort(int argc, char** argv) : Simulator(argc, argv) {
    // empty constructor
}


void LongShort::prepareModel() {
    m_parser = std::make_unique<ConfigParser>(m_argv[1]);
    m_parser->parseConfigurations();

    m_configs = std::make_unique<Configurations>(std::move(m_parser->getConfigs()));

    // reinterpret cast does not represent any CPU instructions, it simply tells the compiler
    // to treat the sequence of bits in (expressions) as if it is <new-type> !!!
    m_db = reinterpret_cast<Database*>(Database::getDatabaseInstance(m_configs->accessParameter("universe", "data_directory"),
                                                                     m_configs->accessParameter("universe", "universe_file")));
}


double LongShort::calculateTicker() {

}


void LongShort::runSimulation() {

}

