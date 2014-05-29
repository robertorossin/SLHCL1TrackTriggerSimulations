#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/StubCleaner.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/PatternGenerator.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/PatternMatcher.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/TrackFitter.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/NTupleMaker.h"

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


// _____________________________________________________________________________
int main(int argc, char **argv) {

    // Declare a group of options that will be allowed only on command line
    namespace po = boost::program_options;
    int verbose;
    po::options_description generic("Generic options");
    generic.add_options()
        ("version"             , "Print version")
        ("help,h"              , "Produce help message")
        ("cleanStubs,C"        , "Clean stubs and pick one unique stub per layer")
        ("generateBank,G"      , "Generate associative memory pattern bank")
        ("mergeBank,M"         , "Merge associative memory pattern bank")
        ("analyzeBank,A"       , "Analyze associative memory pattern bank")
        ("patternRecognition,R", "Run associative memory pattern recognition")
        ("trackFitting,F"      , "Perform track fitting")
        ("write,W"             , "Write full ntuple")
        ("verbosity,v"         , po::value<int>(&verbose)->default_value(1), "Verbosity level (-1 = very quiet; 0 = quiet, 1 = verbose, 2+ = debug)")
        ("no-color"            , "Turn off colored text")
        ("timing"              , "Show timing information")
        ;

    // Declare a group of options that will be allowed both on command line
    // and in config file
    std::string input, output, layout, bankfile, roadfile, trackfile;
    int maxEvents, maxPatterns, maxRoads, maxHits, maxTracks;
    bool nofilter;
    PatternBankOption bankOption;
    TrackFitterOption fitOption;
    po::options_description config("Configuration");
    config.add_options()
        ("input,i"      , po::value<std::string>(&input)->required(), "Specify input files")
        ("output,o"     , po::value<std::string>(&output)->required(), "Specify output file")
        ("maxEvents,n"  , po::value<int>(&maxEvents)->default_value(-1), "Specfiy max number of events")
        ("maxPatterns"  , po::value<int>(&maxPatterns)->default_value(-1), "Specfiy max number of patterns")
        ("maxRoads"     , po::value<int>(&maxRoads)->default_value(-1), "Specfiy max number of roads per event")
        ("maxHits"      , po::value<int>(&maxHits)->default_value(-1), "Specfiy max number of hits per road")
        ("maxTracks"    , po::value<int>(&maxTracks)->default_value(-1), "Specfiy max number of tracks per event")

        // Only for stub filtering
        ("no-filter"    , po::bool_switch(&nofilter)->default_value(false), "Do not apply filtering")

        // Only for bank generation
        ("layout,L"     , po::value<std::string>(&layout), "Specify trigger tower layout file")

        // Only for pattern matching
        ("bank,B"       , po::value<std::string>(&bankfile), "Specify pattern bank file")

        // Only for writing full ntuple
        ("roads"        , po::value<std::string>(&roadfile), "Specify file containing the roads")
        ("tracks"       , po::value<std::string>(&trackfile), "Specify file containing the tracks")

        // Specifically for a pattern bank
        ("bank_minPt"              , po::value<float>(&bankOption.minPt)->default_value(   2.0), "Specify min pt")
        ("bank_maxPt"              , po::value<float>(&bankOption.maxPt)->default_value(9999.0), "Specify max pt")
        ("bank_minEta"             , po::value<float>(&bankOption.minEta)->default_value(-2.2), "Specify min eta (signed)")
        ("bank_maxEta"             , po::value<float>(&bankOption.maxEta)->default_value( 2.2), "Specify max eta (signed)")
        ("bank_minPhi"             , po::value<float>(&bankOption.minPhi)->default_value(-M_PI), "Specify min phi (from -pi to pi)")
        ("bank_maxPhi"             , po::value<float>(&bankOption.maxPhi)->default_value( M_PI), "Specify max phi (from -pi to pi)")
        ("bank_nLayers"            , po::value<int>(&bankOption.nLayers)->default_value(6), "Specify # of layers")
        ("bank_nSubLadders"        , po::value<int>(&bankOption.nSubLadders)->default_value(32), "Specify # of subladders (a.k.a. segments)")
        ("bank_nSubModules"        , po::value<int>(&bankOption.nSubModules)->default_value(512), "Specify # of submodules (a.k.a. superstrips)")
        ("bank_nMisses"            , po::value<int>(&bankOption.nMisses)->default_value(0), "Specify # of allowed misses")
        ("bank_nFakeSuperstrips"   , po::value<int>(&bankOption.nFakeSuperstrips)->default_value(0), "Specify # of fake superstrips")
        ("bank_nDCBits"            , po::value<int>(&bankOption.nDCBits)->default_value(0), "Specify # of DC bits")
        ("bank_requireTriggerTower", po::value<bool>(&bankOption.requireTriggerTower)->default_value(false), "Apply trigger tower requirement")
        ("bank_activeLayers"       , po::value<std::vector<int> >(&bankOption.activeLayers), "Specify active layers (NOT IMPLEMENTED)")
        ("bank_inactiveLayers"     , po::value<std::vector<int> >(&bankOption.inactiveLayers), "Specify inactive layers (NOT IMPLEMENTED)")
        ("bank_triggerTowers"      , po::value<std::vector<int> >(&bankOption.triggerTowers), "Specify trigger towers (NOT IMPLEMENTED)")

        // Specifically for a track fitter
        ("fit_pqType"              , po::value<int>(&fitOption.pqType)->default_value(0), "Specify choice of variables for p,q")
        ("fit_pbins"               , po::value<int>(&fitOption.pbins)->default_value(100), "Specify # of bins for p")
        ("fit_qbins"               , po::value<int>(&fitOption.qbins)->default_value(100), "Specify # of bins for q")
        ("fit_pmin"                , po::value<float>(&fitOption.pmin)->default_value(-1), "Specify min value for p")
        ("fit_qmin"                , po::value<float>(&fitOption.qmin)->default_value(-1), "Specify min value for q")
        ("fit_pmax"                , po::value<float>(&fitOption.pmax)->default_value(1), "Specify max value for p")
        ("fit_qmax"                , po::value<float>(&fitOption.qmax)->default_value(1), "Specify max value for q")
        ("fit_sigma"               , po::value<float>(&fitOption.sigma)->default_value(3), "Specify resolution for the distance parameter")
        ("fit_minWeight"           , po::value<float>(&fitOption.minWeight)->default_value(0.1), "Specify minimum weight to create a track")
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

    // Options found in both PatternBankOption and TrackFitterOption
    fitOption.nLayers = bankOption.nLayers;

    // At least one option needs to be called
    if (!(vm.count("cleanStubs")         ||
          vm.count("generateBank")       ||
          vm.count("mergeBank")          ||
          vm.count("analyzeBank")        ||
          vm.count("patternRecognition") ||
          vm.count("trackFitting")       ||
          vm.count("write")              ) ) {
        std::cout << "Must select one of '-C', '-G', '-M', '-A', '-R', '-F' or 'W'" << std::endl;
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
    bankOption.nSubLadders      = getPowerOfTwo(bankOption.nSubLadders);
    bankOption.nSubModules      = getPowerOfTwo(bankOption.nSubModules);
    bankOption.nLayers          = std::min(std::max(3, bankOption.nLayers), 8);
    bankOption.nSubLadders      = std::min(std::max(1, bankOption.nSubLadders), 32);
    bankOption.nSubModules      = std::min(std::max(1, bankOption.nSubModules), 512);
    bankOption.nMisses          = std::min(std::max(0, bankOption.nMisses), 3);
    bankOption.nFakeSuperstrips = std::min(std::max(0, bankOption.nFakeSuperstrips), 3);
    bankOption.nDCBits          = std::min(std::max(0, bankOption.nDCBits), 4);

    // Protection against conflict in nSubModules vs. nDCBits
    if (bankOption.nSubModules == 1 && bankOption.nDCBits > 0) {
        std::cout << Warning() << "Requested # of submodules: 1, # of DC bits: " << bankOption.nDCBits << " --> " << 0 << std::endl;
        bankOption.nDCBits = 0;
    }
    if (bankOption.nSubModules == 2 && bankOption.nDCBits > 1) {
        std::cout << Warning() << "Requested # of submodules: 2, # of DC bits: " << bankOption.nDCBits << " --> " << 1 << std::endl;
        bankOption.nDCBits = 1;
    }
    if (bankOption.nSubModules == 4 && bankOption.nDCBits > 2) {
        std::cout << Warning() << "Requested # of submodules: 4, # of DC bits: " << bankOption.nDCBits << " --> " << 2 << std::endl;
        bankOption.nDCBits = 2;
    }


    // _________________________________________________________________________
    // Calling main functions
    if (vm.count("cleanStubs")) {
        std::cout << Color("magenta") << "Parsed pattern bank options:" << EndColor() << std::endl;
        std::cout << bankOption << std::endl;
        std::cout << Color("magenta") << "Start stub cleaning..." << EndColor() << std::endl;

        StubCleaner cleaner(bankOption);
        cleaner.setFilter(!nofilter);
        cleaner.setNEvents(maxEvents);
        cleaner.setVerbosity(verbose);
        int exitcode = cleaner.run(output, input);
        if (exitcode) {
            std::cerr << "An error occurred during stub cleaning. Exiting." << std::endl;
            return exitcode;
        }

        std::cout << "Stub cleaning " << Color("lgreenb") << "DONE" << EndColor() << "." << std::endl;


    } else if (vm.count("generateBank")) {
        std::cout << Color("magenta") << "Parsed pattern bank options:" << EndColor() << std::endl;
        std::cout << bankOption << std::endl;
        std::cout << Color("magenta") << "Start pattern bank generation..." << EndColor() << std::endl;

        PatternGenerator generator(bankOption);
        generator.setNEvents(maxEvents);
        generator.setMaxPatterns(maxPatterns);
        generator.setVerbosity(verbose);
        int exitcode = generator.run(output, input, layout);
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
        std::cout << Color("magenta") << "Parsed pattern bank options:" << EndColor() << std::endl;
        std::cout << bankOption << std::endl;
        std::cout << Color("magenta") << "Start pattern recognition..." << EndColor() << std::endl;

        PatternMatcher matcher(bankOption);
        matcher.setNEvents(maxEvents);
        matcher.setMaxPatterns(maxPatterns);
        matcher.setMaxRoads(maxRoads);
        matcher.setMaxHits(maxHits);
        matcher.setVerbosity(verbose);
        int exitcode = matcher.run(output, input, bankfile);
        if (exitcode) {
            std::cerr << "An error occurred during pattern recognition. Exiting." << std::endl;
            return exitcode;
        }

        std::cout << "Pattern recognition " << Color("lgreenb") << "DONE" << EndColor() << "." << std::endl;

    } else if (vm.count("trackFitting")) {
        std::cout << Color("magenta") << "Parsed track fitting options:" << EndColor() << std::endl;
        std::cout << fitOption << std::endl;
        std::cout << Color("magenta") << "Start track fitting..." << EndColor() << std::endl;

        TrackFitter fitter(fitOption);
        fitter.setNEvents(maxEvents);
        fitter.setMaxTracks(maxTracks);
        fitter.setVerbosity(verbose);
        int exitcode = fitter.run(output, input);
        if (exitcode) {
            std::cerr << "An error occurred during track fitting. Exiting." << std::endl;
            return exitcode;
        }

        std::cout << "Track fitting " << Color("lgreenb") << "DONE" << EndColor() << "." << std::endl;

    } else if (vm.count("write")) {
        std::cout << Color("magenta") << "Start writing full ntuple..." << EndColor() << std::endl;

        NTupleMaker writer;
        writer.setNEvents(maxEvents);
        writer.setVerbosity(verbose);
        int exitcode = writer.run(output, input, roadfile, trackfile);
        if (exitcode) {
            std::cerr << "An error occurred during writing full ntuple. Exiting." << std::endl;
            return exitcode;
        }

        std::cout << "Writing full ntuple " << Color("lgreenb") << "DONE" << EndColor() << "." << std::endl;
    }

    return EXIT_SUCCESS;
}
