#ifndef AMSimulation_SuperstripStitcher_h_
#define AMSimulation_SuperstripStitcher_h_

#include <random>
#include <vector>
#include <map>

namespace slhcl1tt {

class SuperstripStitcher {
  public:
    // Constructors
    SuperstripStitcher(unsigned nLayers, unsigned nFakers);

    // Destructor
    ~SuperstripStitcher() {}


    // Operators
    // Return a vector of indices given a vector of module ids
    // Input module ids are assumed to be sorted
    std::vector<unsigned> stitch(const std::vector<unsigned>& layers);
    std::vector<unsigned> stitch_layermap(const std::vector<unsigned>& layers);

    // Debug
    void print();


  private:
    // Member data
    const unsigned nLayers_;
    const unsigned nFakers_;

    // Random number engine (C++11)
    //std::default_random_engine randEngine_;
    std::mt19937 randEngine_;
    std::uniform_int_distribution<unsigned> randNumber_;

    // Make a list of combinations to choose the indices
    std::vector<unsigned> seven_choose_six_;
    std::vector<unsigned> eight_choose_six_;
    std::vector<unsigned> eight_choose_seven_;

    // Make a map to merge layers in barrel and in endcap
    std::map<unsigned, unsigned> layermap_;
};

}  // namespace slhcl1tt

#endif
