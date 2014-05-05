#include "../interface/PatternGenerator.h"
#include "../interface/PatternMatcher.h"

#include "boost/program_options.hpp"
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <exception>
#include <memory>
#include <string>


int getPowerOfTwo(int n) {
    if (n <= 1)
        return 1;
    int m = 1;
    while (n > 1) {
        n /= 2;
        m *= 2;
    }
    return m;
}


int main(int argc, char **argv) {

    // Declare a group of options that will be allowed only on command line
    namespace po = boost::program_options;
    int verbose;
    po::options_description generic("Generic options");
    generic.add_options()
        ("version"             , "Print version")
        ("help,h"              , "Produce help message")
        ("generateBank,G"      , "Generate associative memory pattern bank")
        ("mergeBank,M"         , "Merge associative memory pattern bank")
        ("analyzeBank,A"       , "Analyze associative memory pattern bank")
        ("patternRecognition,R", "Run associative memory pattern recognition")
        ("verbose,v"           , po::value<int>(&verbose)->default_value(1), "Verbosity level (-1 = very quiet; 0 = quiet, 1 = verbose, 2+ = debug)")
        ("no-color"            , "Turn off colored text")
        ("timing"              , "Show timing information")
        ;

    // Declare a group of options that will be allowed both on command line
    // and in config file
    std::string input, output, layout, bank;
    int maxEvents, maxPatterns;
    bool nofilter;
    PatternBankOption bankOption;
    po::options_description config("Configuration");
    config.add_options()
        ("input,i"      , po::value<std::string>(&input)->required(), "Specify input files")
        ("output,o"     , po::value<std::string>(&output)->required(), "Specify output file")
        ("maxEvents,n"  , po::value<int>(&maxEvents)->default_value(-1), "Specfiy max number of events")
        ("maxPatterns,m", po::value<int>(&maxPatterns)->default_value(-1), "Specfiy max number of patterns")

        // Only for bank generation
        ("layout,L"     , po::value<std::string>(&layout), "Specify layout file")
        ("bank,B"       , po::value<std::string>(&bank), "Specify pattern bank file")
        ("no-filter"    , po::bool_switch(&nofilter)->default_value(false), "Do not apply filtering")

        // Specifically for a pattern bank
        ("bank_minPt"         , po::value<float>(&bankOption.minPt)->default_value(   2.0), "Specify min pt")
        ("bank_maxPt"         , po::value<float>(&bankOption.maxPt)->default_value(9999.0), "Specify max pt")
        ("bank_minEta"        , po::value<float>(&bankOption.minEta)->default_value(-2.5), "Specify min eta (signed)")
        ("bank_maxEta"        , po::value<float>(&bankOption.maxEta)->default_value( 2.5), "Specify max eta (signed)")
        ("bank_minPhi"        , po::value<float>(&bankOption.minPhi)->default_value(-M_PI), "Specify min phi (from -pi to pi)")
        ("bank_maxPhi"        , po::value<float>(&bankOption.maxPhi)->default_value( M_PI), "Specify max phi (from -pi to pi)")
        ("bank_nLayers"       , po::value<int>(&bankOption.nLayers)->default_value(6), "Specify # of layers")
        ("bank_nSubLadders"   , po::value<int>(&bankOption.nSubLadders)->default_value(32), "Specify # of subladders (a.k.a. segments)")
        ("bank_nSubModules"   , po::value<int>(&bankOption.nSubModules)->default_value(1024), "Specify # of submodules (a.k.a. superstrips)")
        ("bank_nMisses"       , po::value<int>(&bankOption.nMisses)->default_value(0), "Specify # of allowed misses")
        ("bank_nFakeHits"     , po::value<int>(&bankOption.nFakeHits)->default_value(0), "Specify # of fake hits")
        ("bank_nDCBits"       , po::value<int>(&bankOption.nDCBits)->default_value(0), "Specify # of DC bits")
        ("bank_activeLayers"  , po::value<std::vector<int> >(&bankOption.activeLayers), "Specify active layers")
        ("bank_inactiveLayers", po::value<std::vector<int> >(&bankOption.inactiveLayers), "Specify inactive layers")
        ("bank_sectors"       , po::value<std::vector<int> >(&bankOption.sectors), "Specify sectors (a.k.a.) trigger towers")
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

    // Parse program options
    po::variables_map vm;
    try {
        po::store(po::command_line_parser(argc, argv).options(cmdline_options).positional(p).run(), vm);
        if (vm.count("help") || argc==1) {
            std::cout << visible << std::endl;
            return EXIT_SUCCESS;
        }
        ifstream ifs("config-file");
        po::store(po::parse_config_file(ifs, cfgfile_options), vm);

        po::notify(vm);
    } catch (const boost::program_options::error& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    // At least one option needs to be called
    if (!(vm.count("generateBank") ||
          vm.count("mergeBank")    ||
          vm.count("analyzeBank")  ||
          vm.count("patternRecognition") )) {
        std::cout << "Must select one of '-G', '-M', '-A' or '-R'" << std::endl;
        std::cout << visible << std::endl;
        return 1;
    }

    if (vm.count("no-color")) {
        slhcl1tt::NoColor();
    }

    if (vm.count("timing")) {
        slhcl1tt::ShowTiming();
    }

    // Restrict the interger ranges
    bankOption.nSubLadders = getPowerOfTwo(bankOption.nSubLadders);
    bankOption.nSubModules = getPowerOfTwo(bankOption.nSubModules);
    bankOption.nLayers     = std::min(std::max(1, bankOption.nLayers), 8);
    bankOption.nSubLadders = std::min(std::max(1, bankOption.nSubLadders), 32);
    bankOption.nSubModules = std::min(std::max(1, bankOption.nSubModules), 1024);
    bankOption.nMisses     = std::min(std::max(0, bankOption.nMisses), 3);
    bankOption.nFakeHits   = std::min(std::max(0, bankOption.nFakeHits), 3);
    bankOption.nDCBits     = std::min(std::max(0, bankOption.nDCBits), 10);

    std::cout << Color("magenta") << "Parsed pattern bank options:" << EndColor() << std::endl;
    std::cout << bankOption << std::endl;

    // Calling main functions
    if (vm.count("generateBank")) {
        std::cout << Color("magenta") << "Start generating pattern bank..." << EndColor() << std::endl;

        PatternGenerator generator(bankOption);
        generator.setFilter(!nofilter);
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
        std::cout << "NOT IMPLEMENTED" << std::endl;

    } else if (vm.count("analyzeBank")) {
        std::cout << "NOT IMPLEMENTED" << std::endl;

    } else if (vm.count("patternRecognition")) {
        std::cout << Color("magenta") << "Start running pattern recognition..." << EndColor() << std::endl;

        PatternMatcher matcher(bankOption);
        matcher.setNEvents(maxEvents);
        matcher.setVerbosity(verbose);
        int exitcode = matcher.run(input, output, bank);
        if (exitcode) {
            std::cerr << "An error occurred during pattern recognition. Exiting." << std::endl;
            return exitcode;
        }

        std::cout << "Pattern recognition " << Color("lgreenb") << "DONE" << EndColor() << "." << std::endl;
    }

    return EXIT_SUCCESS;
}
