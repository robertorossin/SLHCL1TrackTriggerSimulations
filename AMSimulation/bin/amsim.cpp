#include "../interface/PatternGenerator.h"
#include "../interface/PatternMatcher.h"
#include "../interface/Helper.h"

#include "boost/program_options.hpp"
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <exception>
#include <memory>
#include <string>


int main(int argc, char **argv) {

    // Declare a group of options that will be allowed only on command line
    namespace po = boost::program_options;
    int verbose;
    po::options_description generic("Generic options");
    generic.add_options()
        ("version"       , "Print version")
        ("help,h"        , "Produce help message")
        ("verbose,v"     , po::value<int>(&verbose)->default_value(1), "Verbosity level (-1 = very quiet; 0 = quiet, 1 = verbose, 2+ = debug)")
        ("generateBank,G", "Generate associative memory pattern bank")
        ("mergeBank,M"   , "Merge associative memory pattern bank")
        ("analyzeBank,A" , "Analyze associative memory pattern bank")
        ("runMatching,R" , "Run associative memory pattern matching")
        ;

    // Declare a group of options that will be allowed both on command line
    // and in config file
    bool filter, timing;
    int maxEvents, maxPatterns;
    std::string input, output, layout, bank;
    po::options_description config("Configuration");
    config.add_options()
        ("input,i"      , po::value<std::string>(&input)->required(), "Specify input files")
        ("output,o"     , po::value<std::string>(&output)->required(), "Specify output file")
        ("filter,f"     , po::value<bool>(&filter)->default_value(true), "Apply filters")
        ("timing,t"     , po::value<bool>(&timing)->default_value(false), "Use timer")
        ("maxEvents,n"  , po::value<int>(&maxEvents)->default_value(-1), "Specfiy max number of events")
        ("maxPatterns,m", po::value<int>(&maxPatterns)->default_value(-1), "Specfiy max number of patterns")
        ("layout,L"     , po::value<std::string>(&layout), "Specify layout file")
        ("bank,B"       , po::value<std::string>(&bank), "Specify pattern bank file")
        ;

    // Hidden options, will be allowed both on command line and in config file,
    // but will not be shown to the user.
    po::options_description hidden("Hidden options");
    hidden.add_options()
        ("config-file", po::value<std::vector<std::string> >(), "Specify config file")
        ;

    // Further group the options
    po::options_description cmdline_options;
    cmdline_options.add(generic).add(config).add(hidden);

    po::options_description cfgfile_options;
    cfgfile_options.add(config).add(hidden);

    po::options_description visible("Allowed options");
    visible.add(generic).add(config);

    // Allow a positional option, which does not specify any name at all
    po::positional_options_description p;
    p.add("config-file", -1);

    po::variables_map vm;
    try {
        po::store(po::command_line_parser(argc, argv).options(cmdline_options).positional(p).run(), vm);
        ifstream ifs("config-file");
        po::store(po::parse_config_file(ifs, cfgfile_options), vm);
        po::notify(vm);
    } catch (const boost::program_options::error& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    // At least one option needs to be called
    bool stop = !(vm.count("generateBank")) &&
                !(vm.count("mergeBank")) &&
                !(vm.count("analyzeBank")) &&
                !(vm.count("runMatching"));

    if (vm.count("help") || stop) {
        std::cout << visible << std::endl;
        return 1;
    }

    if (vm.count("generateBank")) {
        std::cout << "Start generating pattern bank..." << std::endl;

        PatternGenerator generator;
        generator.setFilter(filter);
        generator.setTiming(timing);
        generator.setNEvents(maxEvents);
        generator.setNPatterns(maxPatterns);
        generator.setVerbosity(verbose);

        int exitcode = generator.run(input, output, layout);
        if (exitcode) {
            std::cerr << "An error occurred during pattern bank generation. Exiting." << std::endl;
            return exitcode;
        }

        std::cout << "Pattern bank generation " << Color("lgreenb") << "DONE" << EndColor() << "." << std::endl;

    } else if (vm.count("mergeBank")) {
        // NOT IMPLEMENTED

    } else if (vm.count("analyzeBank")) {
        // NOT IMPLEMENTED

    } else if (vm.count("runMatching")) {
        std::cout << "Start running pattern matching..." << std::endl;

        PatternMatcher matcher;
        matcher.setTiming(timing);
        matcher.setNEvents(maxEvents);
        matcher.setVerbosity(verbose);

        int exitcode = matcher.run(input, output, bank);
        if (exitcode) {
            std::cerr << "An error occurred during pattern matching. Exiting." << std::endl;
            return exitcode;
        }

        std::cout << "Pattern matching " << Color("lgreenb") << "DONE" << EndColor() << "." << std::endl;
    }

    return EXIT_SUCCESS;
}
