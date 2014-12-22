#ifndef AMSimulation_Picky_h_
#define AMSimulation_Picky_h_

#include <vector>

namespace slhcl1tt {

class Picky {
  public:
    // Constructors
    Picky();

    // Destructor
    ~Picky() {}

    // Operators
    void setTrack(float pt, float eta, float phi, float vz, int charge);  // unused
    bool applyCuts(unsigned lay16, const float& absdphi, const float& absdz, const float& absdr);
    unsigned findInterior(unsigned lay16, const float& z, const float& r);

    // Debug
    void print();

  private:
    // Member data
    float simPt_;
    float simEta_;
    float simPhi_;
    float simVZ_;
    int   simCharge_;

    std::vector<float>  barrel_r_coords_;
    std::vector<float>  barrel_phi_cuts_;
    std::vector<float>  barrel_z_cuts_;

    std::vector<float>  endcap_z_coords_;
    std::vector<float>  endcap_phi_cuts_;
    std::vector<float>  endcap_r_cuts_;
};

}

#endif
