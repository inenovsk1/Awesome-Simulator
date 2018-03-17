#include <iostream>

#include "ModelA.h"

using namespace std;

int main(int argc, char** argv) {

    Utils::determineCommandLineOptions(argc, argv);

    ModelA strategyOne(argc, argv);

    strategyOne.runSimulation();

	return 0;
}
