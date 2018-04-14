#include <iostream>
#include <chrono>

#include "ModelA.h"

using namespace std;

int main(int argc, char** argv) {

    Utils::determineCommandLineOptions(argc, argv);

    auto start = std::chrono::high_resolution_clock::now();

    ModelA strategyOne(argc, argv);
    strategyOne.runSimulation();

    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(end - start);

    cout << "Simulation run took " << elapsed.count() << " seconds to complete!" << endl;

	return 0;
}
