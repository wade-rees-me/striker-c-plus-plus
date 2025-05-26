#include "arguments.hpp"
#include "constants.hpp"
#include "parameters.hpp"
#include "rules.hpp"
#include "simulator.hpp"
#include "strategy.hpp"
#include "xlog/xlog.hpp"
#include <cstdlib>
#include <ctime>
#include <future>
#include <iostream>
#include <string>

//
int main(int argc, char *argv[]) {
    xlog::init(xlog::SYSLOG_ADDRESS, xlog::SYSLOG_PORT);
    Arguments arguments(argc, argv);
    Parameters parameters(&arguments);
    Rules rules(arguments.getDecks());
    Strategy strategy(arguments.getDecks(), arguments.getStrategy(), arguments.getNumberOfDecks());
    Simulator *simulators[32];
    Report finalReport;
    std::vector<std::future<Simulator *>> futures;

    std::cout.imbue(std::locale("en_US.UTF-8"));
    std::cout.setf(std::ios::unitbuf);

    time_t start = xlog::start("Simulation started, strategy=%s, decks=%s, hands=%d", arguments.getStrategy().c_str(),
                               arguments.getDecks().c_str(), arguments.getNumberOfHands());
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
    xlog::stop(start, "Simulation started, strategy=%s, decks=%s, hands=%d", arguments.getStrategy().c_str(),
               arguments.getDecks().c_str(), arguments.getNumberOfHands());

    xlog::close();
    return 0;
}
