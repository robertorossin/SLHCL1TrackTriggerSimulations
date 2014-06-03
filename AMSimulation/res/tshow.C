#include "TFile.h"
#include "TTree.h"
#include "TString.h"

#include <bitset>
#include <iostream>
#include <vector>

static const unsigned MAX_NLAYERS = 8;

void tshow(const char *input,
           unsigned int i = 0,
           unsigned int n = 20,
           const char *format = "i")
{
    TFile* tfile = TFile::Open(input);
    if (!tfile) {
        std::cout << "Cannot open file: " << input << std::endl;
        return;
    }
    TTree* ttree = (TTree*) tfile->Get("patternBank");
    if (!ttree) {
        std::cout << "Cannot get patternBank tree from file: " << input << std::endl;
        return;
    }

    TString fformat(format);
    if (!(fformat == "i" || fformat == "x" || fformat == "b" || fformat == "s")) {
        std::cout << "Unrecognized format option: " << format << std::endl;
        return;
    }

    std::vector<unsigned>       * superstripIds = 0;
    std::vector<unsigned short> * superstripBits = 0;
    ttree->SetBranchAddress("superstripIds", &superstripIds);
    ttree->SetBranchAddress("superstripBits", &superstripBits);

    unsigned nentries = ttree->GetEntries();
    for (unsigned ii=0; ii<n; ++ii) {
        if ((i + ii) >= nentries)  break;
        ttree->GetEntry(i + ii);

        std::cout << Form("Pattern %4u  ", i + ii);
        if (fformat == "i") {
            for (unsigned l=0; l<MAX_NLAYERS; ++l) {
                std::cout << Form("%10u", superstripIds->at(l)) << "  ";
            }
        } else if (fformat == "x") {
            for (unsigned l=0; l<MAX_NLAYERS; ++l) {
                std::cout << Form("%#x", superstripIds->at(l)) << "  ";
            }
        } else if (fformat == "b") {
            for (unsigned l=0; l<MAX_NLAYERS; ++l) {
                std::cout << (bitset<32>) superstripIds->at(l) << "  ";
            }
        } else if (fformat == "s") {
            for (unsigned l=0; l<MAX_NLAYERS; ++l) {
                unsigned ssId = superstripIds->at(l);
                unsigned moduleId = (ssId >> 14) & 0x3FFFF;
                unsigned col      = (ssId >> 9 ) & 0x1F;
                unsigned row      = (ssId >> 0 ) & 0x1FF;

                std::cout << Form("(%6u,%2u,%4u)", moduleId, col, row) << "  ";
            }
        }
        std::cout << std::endl;
    }

    return;
}


/*
Example usage
-------------
root -l -b -q tshow.C+\(\"patternBank.root\",0,10,\"b\"\)
*/

