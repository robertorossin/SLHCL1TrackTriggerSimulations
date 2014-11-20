#include "TFile.h"
#include "TTree.h"
#include "TROOT.h"
#include <stdint.h>
#include <vector>
#include <iostream>

typedef uint16_t count_type;
typedef uint32_t id_type;


#ifdef __CINT__
#pragma link C++ class vector<vector<float> >+;
#pragma link C++ class vector<vector<int> >+;
#pragma link C++ class vector<vector<id_type> >+;
#endif


int readRoads()
{
    gROOT->ProcessLine("#include <vector>");

    // Enter your file name
    TFile *tfile = new TFile("/uscms_data/d2/jiafu/L1TrackTrigger/CRAB_amsim/roads_numEvent100.1.root");
    assert(tfile != NULL);

    // Enter your TTree name
    TTree *ttree = (TTree *) tfile->Get("ntupler/tree");
    assert(ttree != NULL);
    TString prefix = "AMTTRoads_";
    TString suffix = "";

    // Enter the number of events to read
    //unsigned nevents = 10;
    unsigned nevents = ttree->GetEntries();

    // Setup vectors for the branches of interest
    std::vector<float> *                    vb_r                 = 0;
    std::vector<float> *                    vb_phi               = 0;
    std::vector<float> *                    vb_z                 = 0;
    //
    std::vector<id_type> *                  vr_bankIndex         = 0;
    std::vector<id_type> *                  vr_triggerTowerId    = 0;
    std::vector<count_type> *               vr_nSuperstrips      = 0;
    std::vector<std::vector<id_type> > *    vr_stubSuperstripIds = 0;
    std::vector<std::vector<unsigned> > *   vr_stubRefs          = 0;

    // Setup the TTree branches to those vectors
    ttree->SetBranchAddress("TTStubs_r"        , &(vb_r));
    ttree->SetBranchAddress("TTStubs_phi"      , &(vb_phi));
    ttree->SetBranchAddress("TTStubs_z"        , &(vb_z));
    //
    ttree->SetBranchAddress(prefix + "bankIndex"         + suffix, &(vr_bankIndex));
    ttree->SetBranchAddress(prefix + "triggerTowerId"    + suffix, &(vr_triggerTowerId));
    ttree->SetBranchAddress(prefix + "nSuperstrips"      + suffix, &(vr_nSuperstrips));
    ttree->SetBranchAddress(prefix + "stubSuperstripIds" + suffix, &(vr_stubSuperstripIds));
    ttree->SetBranchAddress(prefix + "stubRefs"          + suffix, &(vr_stubRefs));


     // Loop over events
    for (unsigned ievt=0; ievt<nevents; ++ievt) {
        // Get a new event
        ttree->GetEntry(ievt);

        // Number of roads
        const unsigned nroads = vr_bankIndex->size();
        std::cout << "Event " << ievt << " has " << nroads << " roads." << std::endl;

        // Loop over the roads
        for (unsigned i=0, j=0; i<nroads; ++i) {
            // Number of stubs
            const unsigned nsuperstrips = vr_nSuperstrips->at(i);
            const unsigned nstubs = vr_stubRefs->at(i).size();

            std::cout << "  - Road " << i << " has " << nstubs << " stubs in " << nsuperstrips << " superstrips." << std::endl;

            // Group by superstrip id
            std::vector<std::vector<unsigned> > stubs_by_ssId;
            unsigned ref=0;
            id_type ssId=0, old_ssId=0;

            for (j=0; j<nstubs; ++j) {
                ref = vr_stubRefs->at(i).at(j);
                ssId = vr_stubSuperstripIds->at(i).at(j);
                std::cout << "      - Stub " << j << " ssId: " << ssId << " r,phi,z: " << vb_r->at(ref) << "," << vb_phi->at(ref) << "," << vb_z->at(ref) << std::endl;

                if (j == 0 || ssId != old_ssId) {
                    stubs_by_ssId.push_back(std::vector<unsigned>());
                }
                stubs_by_ssId.back().push_back(j);
                old_ssId = ssId;
            }

            //for (unsigned j=0; j<stubs_by_ssId.size(); ++j) {
            //    std::cout << "      - Superstrip " << j << " has " << stubs_by_ssId.at(j).size() << " stubs." << std::endl;
            //}

            // Make combinations
            std::vector<unsigned> indices;
            int ii, ij;
            for (ii=0; ii<int(nsuperstrips); ++ii)
                indices.push_back(0);  // init indices to zeroes

            j = 0;
            while (true) {
                std::cout << "      - Stub Comb " << j << " consists of: ";
                for (ii=0; ii<int(nsuperstrips); ++ii)
                    std::cout << stubs_by_ssId.at(ii).at(indices[ii]) << " ";
                std::cout << std::endl;

                for (ii=nsuperstrips-1; ii>=0; --ii) {
                    if (indices[ii] != stubs_by_ssId.at(ii).size() - 1)
                        break;  // take the last index that has not reached the end
                }
                if (ii == -1)  break;

                indices[ii] += 1;  // increment that index
                for (ij = ii+1; ij<int(nsuperstrips); ++ij) {
                    indices[ij] = 0;  // set indices behind that index to zeroes
                }
                ++j;
            }
        }

        std::cout << std::endl;
    }

    // Close the file
    tfile->Close();

    return 0;
}
