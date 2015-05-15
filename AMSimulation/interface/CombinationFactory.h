#ifndef AMSimulation_CombinationFactory_h_
#define AMSimulation_CombinationFactory_h_

#include <vector>

namespace slhcl1tt {

class CombinationFactory {
  public:
    // Constructor
    CombinationFactory() : verbose_(true) {}

    // Destructor
    ~CombinationFactory() {}

    // Enum
    enum Flag { BAD=999999999 };

    // Arrange combinations
    // groups[i][j] is the j-th element in the i-th group
    // combinations[i][j] is the j-th element in the i-th combination
    template<typename T>
    std::vector<std::vector<T> > combine(const std::vector<std::vector<T> >& groups, const unsigned maxN) {
        std::vector<T> combination;
        std::vector<std::vector<T> > combinations;

        const int ngroups = groups.size();
        std::vector<unsigned> indices(ngroups, 0);  // init to zeroes

        int i=0, j=0;
        while (true) {
            combination.clear();
            for (i=0; i<ngroups; ++i) {
                if (groups.at(i).size())
                    combination.push_back(groups.at(i).at(indices.at(i)));
                else  // empty group
                    combination.push_back(CombinationFactory::BAD);
            }
            combinations.push_back(combination);

            for (i=ngroups-1; i>=0; --i)
                if (groups.at(i).size())
                    if (indices.at(i) != groups.at(i).size() - 1)
                        break;  // take the last index that has not reached the end
            if (i == -1)  break;

            indices[i] += 1;  // increment that index
            for (j=i+1; j<ngroups; ++j)
                indices[j] = 0;  // set indices behind that index to zeroes
        }

        if (combinations.size() > maxN)
            combinations.resize(maxN);

        return combinations;
    }

    // Debug
    void print();


  private:
    // Member data
    int verbose_;
};

}

#endif
