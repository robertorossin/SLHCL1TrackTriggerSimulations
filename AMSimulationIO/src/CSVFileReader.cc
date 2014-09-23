#include "SLHCL1TrackTriggerSimulations/AMSimulationIO/interface/CSVFileReader.h"

using namespace slhcl1tt;

#include <fstream>
#include <sstream>


// _____________________________________________________________________________
CSVFileReader::CSVFileReader(int verbose)
: verbose_(verbose) {}

CSVFileReader::~CSVFileReader() {}


int CSVFileReader::getTriggerTowerMap(TString src,
                                      std::map<unsigned, std::vector<unsigned> >& ttmap) {
    if (!src.EndsWith(".csv")) {
        std::cout << Error() << "Input filename must be .csv" << std::endl;
        return 1;
    }

    //if (verbose_)  std::cout << Info() << "Opening " << src << std::endl;
    std::vector<unsigned> values;

    unsigned i = 0;
    std::string line, issline;

    std::ifstream ifs(src.Data());
    while (std::getline(ifs, line)) {
        std::istringstream iss(line);

        while (std::getline(iss, issline, ',')) {  // split by commas
            if (i == 0)  continue;  // skip the first line
            values.push_back(std::stoi(issline));
        }
        ++i;
    }

    if (values.empty()) {
        std::cout << Error() << "Failed to read any trigger tower" << std::endl;
        return 1;
    }

    ttmap.clear();
    unsigned ttId = 0;
    for (unsigned i=0; i<values.size(); ++i) {
        if (i == 0)  continue;  // skip the first index
        if (values.at(i-1) <= 6 && values.at(i) <= 8) {  // eta_idx, phi_idx
            ttId = (values.at(i-1)-1) * 8 + (values.at(i)-1);  // mapped to 0-47
            ttmap.insert(std::make_pair(ttId, std::vector<unsigned>()));
        } else if (values.at(i) > 10000) {
            ttmap.at(ttId).push_back(values.at(i));
        }
    }
    return 0;
}

int CSVFileReader::getModuleCoordinateMap(TString src,
                                          std::map<unsigned, ModuleCoordinate>& coordmap) {
    if (!src.EndsWith(".csv")) {
        std::cout << Error() << "Input filename must be .csv" << std::endl;
        return 1;
    }

    //if (verbose_)  std::cout << Info() << "Opening " << src << std::endl;
    std::vector<unsigned> values;
    std::vector<float> values_f;

    unsigned i = 0;
    std::string line, issline;

    std::ifstream ifs(src.Data());
    while (std::getline(ifs, line)) {
        std::istringstream iss(line);

        unsigned j = 0;
        while (std::getline(iss, issline, ',')) {  // split by commas
            if (i == 0)  continue;  // skip the first line

            if (j%6 == 0)
                values.push_back(std::stoi(issline));
            else
                values_f.push_back(std::stof(issline));
            ++j;
        }
        ++i;
    }

    if (values.empty()) {
        std::cout << Error() << "Failed to read any module coordinate" << std::endl;
        return 1;
    }

    if (values.size()*5 != values_f.size()) {
        std::cout << Error() << "Failed to parse all module coordinates" << std::endl;
        return 1;
    }

    coordmap.clear();
    unsigned modId;
    ModuleCoordinate modCd;
    for (unsigned i=0; i<values.size(); ++i) {
        modId = values.at(i);
        modCd.rho = values_f.at(i*5 + 1);
        modCd.eta = values_f.at(i*5 + 2);
        modCd.phi = values_f.at(i*5 + 3);
        coordmap.insert(std::make_pair(modId, modCd));
    }

    return 0;
}

