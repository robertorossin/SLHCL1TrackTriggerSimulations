#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/SuperstripArbiter.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/SuperstripHasher.h"
using namespace slhcl1tt;

#include <cppunit/extensions/HelperMacros.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>


class TestSuperstripOperations : public CppUnit::TestFixture  {

CPPUNIT_TEST_SUITE( TestSuperstripOperations );
CPPUNIT_TEST( testHashModule );
CPPUNIT_TEST_SUITE_END();

private:
    SuperstripArbiter *arbiter_;
    SuperstripHasher *hasher_;
    std::vector<unsigned> values_;

public:
    void setUp() {

        std::vector<int> subLadderVarSize = {8, 16, 32, 16, 16, 32};
        std::vector<int> subModuleVarSize = {256, 256, 256, 256, 512, 512};
        arbiter_ = new SuperstripArbiter(6, subLadderVarSize, subModuleVarSize);
        //arbiter_->print();

        id_type subLadderNBits = msb(32/arbiter_->minSubLadderSize());
        id_type subModuleNBits = msb(1024/arbiter_->minSubModuleSize());
        hasher_ = new SuperstripHasher(subLadderNBits, subModuleNBits);
        //hasher_->print();

        // Read module ids
        unsigned i = 0, j = 0;
        std::string line;
        std::ifstream ifs("src/SLHCL1TrackTriggerSimulations/AMSimulation/data/module_connections.csv");
        while (std::getline(ifs, line)) {
            std::istringstream iss(line);
            std::string issline;
            j = 0;
            while (std::getline(iss, issline, ',')) {  // split by commas
                if (i != 0) {
                    unsigned v = std::stoi(issline);
                    if (j == 4) {  // skip the first line, keep the fourth index
                        values_.push_back(v);
                        break;
                    }
                    ++j;
                }
            }
            ++i;
        }
    }

    void tearDown() {
        delete arbiter_;
        delete hasher_;
    }

    void testHashModule() {
        std::map<unsigned, unsigned>  hashModuleMap;
        for (auto& v: values_)
            hashModuleMap[hasher_->hashModule(v)] = v;
        CPPUNIT_ASSERT_EQUAL(values_.size(), hashModuleMap.size());

        //for (auto& it: hashModuleMap)
        //    printf("%8u --> %8u\n", it.second, it.first);

        CPPUNIT_ASSERT_EQUAL((key_type)    0, hasher_->hashModule( 50000));
        CPPUNIT_ASSERT_EQUAL((key_type) 1000, hasher_->hashModule( 50862));
        CPPUNIT_ASSERT_EQUAL((key_type) 2000, hasher_->hashModule( 60841));
        CPPUNIT_ASSERT_EQUAL((key_type) 3000, hasher_->hashModule( 72619));
        CPPUNIT_ASSERT_EQUAL((key_type) 4000, hasher_->hashModule( 70649));
        CPPUNIT_ASSERT_EQUAL((key_type) 6000, hasher_->hashModule( 90211));
        CPPUNIT_ASSERT_EQUAL((key_type) 8000, hasher_->hashModule(105615));
        CPPUNIT_ASSERT_EQUAL((key_type)10000, hasher_->hashModule(130012));
        CPPUNIT_ASSERT_EQUAL((key_type)12000, hasher_->hashModule(151452));
        CPPUNIT_ASSERT_EQUAL((key_type)14000, hasher_->hashModule(201412));
        CPPUNIT_ASSERT_EQUAL((key_type)15427, hasher_->hashModule(221479));
    }
};

CPPUNIT_TEST_SUITE_REGISTRATION( TestSuperstripOperations );
