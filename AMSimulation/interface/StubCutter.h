#ifndef AMSimulation_StubCutter_h_
#define AMSimulation_StubCutter_h_

#include <vector>


namespace slhcl1tt {

class StubCutter {
  public:
    // Constructor
    StubCutter();

    // Destructor
    ~StubCutter() {}

    // Functions
    // Apply cuts on delta_s, depending on layer (compressed to 0-15)
    bool applyCuts(unsigned moduleId, const float ds);

    // Debug
    void print();

  private:
    // Member data
    std::vector<float>                barrel_ds_cuts_;
    std::vector<std::vector<float> >  endcap_ds_cuts_;
};

}

#endif
