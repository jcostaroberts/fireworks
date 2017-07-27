#include "boost/program_options.hpp"
#include <cstdlib>
#include "hanabi.hh"
#include <iostream>
#include <random>
#include <vector>

class Options {
public:
    size_t runs;
    size_t players;
    bool debug;
};

void
parse_options(int argc, const char *argv[], Options *options) {
    namespace po = boost::program_options;
    po::options_description desc("Options");
    desc.add_options()
        ("help,h", "Print help messages")
        ("players,p", po::value<size_t>(&(options->players))->required(),
         "Number of players (required)")
        ("runs,r", po::value<size_t>(&(options->runs))->default_value(1), "Number of runs")
        ("debug,d", po::bool_switch(&(options->debug)), "Include debug output");

    po::variables_map vm;
    try {
        po::store(parse_command_line(argc, argv, desc), vm);
        po::notify(vm);
    } catch (std::exception e) {
        std::cout << "Error parsing options." << std::endl;
        std::cout << desc << std::endl;
        exit(-1);
    }

    if (options->players < 2 || options->players > 5) {
        std::cout << "Number of players must be between 2 and 5." << std::endl;
        exit(-1);
    }
}

int
main(int argc, const char *argv[]) {
    Options options;
    parse_options(argc, argv, &options);

    Hanabi hanabi(options.players, options.debug);
    hanabi.deal();
    hanabi.play();
    std::cout << "SCORE: " << hanabi.score() << std::endl;
}
