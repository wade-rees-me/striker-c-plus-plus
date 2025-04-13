#include "arguments.hpp"
#include "constants.hpp"
#include "parameters.hpp"
#include "rules.hpp"
#include "simulator.hpp"
#include "strategy.hpp"
#include <cstdlib>
#include <ctime>
#include <future>
#include <iostream>
#include <string>

//
int main(int argc, char *argv[]) {
    Arguments arguments(argc, argv);
    Parameters parameters(&arguments);
    Rules rules(arguments.getDecks());
    Strategy strategy(arguments.getDecks(), arguments.getStrategy(), arguments.getNumberOfDecks());
    Simulator *simulators[32];
    Report finalReport;
    std::vector<std::future<Simulator *>> futures;

    std::cout.imbue(std::locale("en_US.UTF-8"));
    std::cout.setf(std::ios::unitbuf);

    std::cout << "Start: " << STRIKER_WHO_AM_I << std::endl;
    std::cout << "  -- arguments -------------------------------------------------------------------" << std::endl;
    parameters.print();
    rules.print();
    std::cout << "  --------------------------------------------------------------------------------" << std::endl;

    // Launch simulations asynchronously
    finalReport.init(&parameters, (int64_t)std::time(nullptr));
    for (int i = 0; i < arguments.getNumberOfThreads(); ++i) {
        simulators[i] = new Simulator(&parameters, &rules, &strategy);
        futures.push_back(std::async(std::launch::async, &Simulator::simulatorRunOnce, simulators[i]));
    }
    for (auto &fut : futures) { // Merge results
        finalReport.merge(fut.get()->getReport());
    }

    finalReport.finish((int64_t)std::time(nullptr));
    std::cout << "  -- results ---------------------------------------------------------------------" << std::endl;
    finalReport.print();
    std::cout << "  --------------------------------------------------------------------------------" << std::endl;
    std::cout << "  -- insert ----------------------------------------------------------------------" << std::endl;
    finalReport.insert();
    std::cout << "  --------------------------------------------------------------------------------" << std::endl;

    return 0;
}
