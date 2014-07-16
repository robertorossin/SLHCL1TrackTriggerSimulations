#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/SuperstripArbiter.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/SuperstripHasher.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/SuperstripStitcher.h"
using namespace slhcl1tt;

#include <cppunit/extensions/HelperMacros.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>

// Code taken from https://docs.python.org/2/library/itertools.html#itertools.combinations
static std::vector<std::vector<unsigned> > combinations(const std::vector<unsigned>& pool, int k) {
    unsigned n = pool.size();
    int i, j;
    std::vector<unsigned> indices;
    for (i=0; i<k; ++i)
        indices.push_back(i);

    std::vector<std::vector<unsigned> > ret;
    while (true) {
        std::vector<unsigned> ret_inner;
        for (i=0; i<k; ++i) {
            ret_inner.push_back(pool.at(indices[i]));
        }
        ret.push_back(ret_inner);

        //for (i=0; i<k; ++i) {
        //    std::cout << indices[i] << ",";
        //}
        //std::cout << std::endl;

        for (i=k-1; i>=0; --i) {
            if (indices[i] != i + n - k)
                break;
        }
        if (i == -1) break;

        indices[i] += 1;
        for (j=i+1; j<k; ++j) {
            indices[j] = indices[j-1] + 1;
        }
    }
    return ret;
}


class TestSuperstripOperations : public CppUnit::TestFixture  {

CPPUNIT_TEST_SUITE(TestSuperstripOperations);
CPPUNIT_TEST(testSubLadder);
CPPUNIT_TEST(testSubModule);
CPPUNIT_TEST(testHashModule);
CPPUNIT_TEST(testHash);
CPPUNIT_TEST(testStitch);
CPPUNIT_TEST_SUITE_END();

private:
    SuperstripArbiter *arbiter_;
    SuperstripHasher *hasher_;
    SuperstripStitcher *stitcher_;
    std::vector<unsigned> values_;

public:
    void setUp() {
        int nLayers = 6;
        int nFakeSuperstrips = 3;

        std::vector<unsigned> subLadderVarSize = {8, 16, 32, 16, 16, 32};
        std::vector<unsigned> subModuleVarSize = {256, 256, 256, 256, 512, 512};
        std::vector<unsigned> subLadderECVarSize = {8, 8, 8, 16, 16, 16, 16, 32, 32, 16, 16, 16, 16, 32, 32};
        std::vector<unsigned> subModuleECVarSize = {256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 512, 512, 512, 512};
        arbiter_ = new SuperstripArbiter(subLadderVarSize, subModuleVarSize, subLadderECVarSize, subModuleECVarSize);
        //arbiter_->print();

        //id_type subLadderNBits = msb(32/arbiter_->minSubLadderSize());
        //id_type subModuleNBits = msb(1024/arbiter_->minSubModuleSize());
        id_type subLadderNBits = 1;
        id_type subModuleNBits = 5;
        hasher_ = new SuperstripHasher(subLadderNBits, subModuleNBits);
        //hasher_->print();

        stitcher_ = new SuperstripStitcher(nLayers, nFakeSuperstrips);

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
        delete stitcher_;
    }

    void testSubLadder() {
        // half-strip
        CPPUNIT_ASSERT_EQUAL(3U, arbiter_->subladder( 50000, 31*2));
        CPPUNIT_ASSERT_EQUAL(1U, arbiter_->subladder( 60000, 31*2));
        CPPUNIT_ASSERT_EQUAL(0U, arbiter_->subladder( 70000, 31*2));
        CPPUNIT_ASSERT_EQUAL(1U, arbiter_->subladder( 80000,  1*2));
        CPPUNIT_ASSERT_EQUAL(1U, arbiter_->subladder( 90000,  1*2));
        CPPUNIT_ASSERT_EQUAL(0U, arbiter_->subladder(100000,  1*2));
        CPPUNIT_ASSERT_EQUAL(3U, arbiter_->subladder(110000, 31*2));
        CPPUNIT_ASSERT_EQUAL(3U, arbiter_->subladder(120100, 31*2));
        CPPUNIT_ASSERT_EQUAL(3U, arbiter_->subladder(130200, 31*2));
        CPPUNIT_ASSERT_EQUAL(1U, arbiter_->subladder(140300, 31*2));
        CPPUNIT_ASSERT_EQUAL(1U, arbiter_->subladder(150400, 31*2));
        CPPUNIT_ASSERT_EQUAL(1U, arbiter_->subladder(180500, 31*2));
        CPPUNIT_ASSERT_EQUAL(1U, arbiter_->subladder(190600, 31*2));
        CPPUNIT_ASSERT_EQUAL(0U, arbiter_->subladder(200700, 31*2));
        CPPUNIT_ASSERT_EQUAL(0U, arbiter_->subladder(210800, 31*2));
        CPPUNIT_ASSERT_EQUAL(1U, arbiter_->subladder(220900,  1*2));
        CPPUNIT_ASSERT_EQUAL(1U, arbiter_->subladder(111000,  1*2));
        CPPUNIT_ASSERT_EQUAL(1U, arbiter_->subladder(121100,  1*2));
        CPPUNIT_ASSERT_EQUAL(1U, arbiter_->subladder(131200,  1*2));
        CPPUNIT_ASSERT_EQUAL(0U, arbiter_->subladder(141300,  1*2));
        CPPUNIT_ASSERT_EQUAL(0U, arbiter_->subladder(151400,  1*2));

        // full-strip
        CPPUNIT_ASSERT_EQUAL(3U, arbiter_->subladder( 50000, 31, false));
        CPPUNIT_ASSERT_EQUAL(1U, arbiter_->subladder( 80000,  1, false));
        CPPUNIT_ASSERT_EQUAL(3U, arbiter_->subladder(110000, 31, false));
        CPPUNIT_ASSERT_EQUAL(1U, arbiter_->subladder(220900,  1, false));
    }

    void testSubModule() {
        // half-strip
        CPPUNIT_ASSERT_EQUAL(3U, arbiter_->submodule( 50000,  959*2));
        CPPUNIT_ASSERT_EQUAL(3U, arbiter_->submodule( 60000,  959*2));
        CPPUNIT_ASSERT_EQUAL(3U, arbiter_->submodule( 70000,  959*2));
        CPPUNIT_ASSERT_EQUAL(3U, arbiter_->submodule( 80000, 1015*2));
        CPPUNIT_ASSERT_EQUAL(1U, arbiter_->submodule( 90000, 1015*2));
        CPPUNIT_ASSERT_EQUAL(1U, arbiter_->submodule(100000, 1015*2));
        CPPUNIT_ASSERT_EQUAL(3U, arbiter_->submodule(110000,  959*2));
        CPPUNIT_ASSERT_EQUAL(3U, arbiter_->submodule(120100,  959*2));
        CPPUNIT_ASSERT_EQUAL(3U, arbiter_->submodule(130200,  959*2));
        CPPUNIT_ASSERT_EQUAL(3U, arbiter_->submodule(140300,  959*2));
        CPPUNIT_ASSERT_EQUAL(3U, arbiter_->submodule(150400,  959*2));
        CPPUNIT_ASSERT_EQUAL(3U, arbiter_->submodule(180500,  959*2));
        CPPUNIT_ASSERT_EQUAL(3U, arbiter_->submodule(190600,  959*2));
        CPPUNIT_ASSERT_EQUAL(3U, arbiter_->submodule(200700,  959*2));
        CPPUNIT_ASSERT_EQUAL(3U, arbiter_->submodule(210800,  959*2));
        CPPUNIT_ASSERT_EQUAL(3U, arbiter_->submodule(220900, 1015*2));
        CPPUNIT_ASSERT_EQUAL(3U, arbiter_->submodule(111000, 1015*2));
        CPPUNIT_ASSERT_EQUAL(1U, arbiter_->submodule(121100, 1015*2));
        CPPUNIT_ASSERT_EQUAL(1U, arbiter_->submodule(131200, 1015*2));
        CPPUNIT_ASSERT_EQUAL(1U, arbiter_->submodule(141300, 1015*2));
        CPPUNIT_ASSERT_EQUAL(1U, arbiter_->submodule(151400, 1015*2));

        // full-strip
        CPPUNIT_ASSERT_EQUAL(3U, arbiter_->submodule( 50000,  959, false));
        CPPUNIT_ASSERT_EQUAL(3U, arbiter_->submodule( 80000, 1015, false));
        CPPUNIT_ASSERT_EQUAL(3U, arbiter_->submodule(110000,  959, false));
        CPPUNIT_ASSERT_EQUAL(3U, arbiter_->submodule(220900, 1015, false));
    }

    void testHashModule() {
        std::map<unsigned, unsigned>  hashModuleMap;
        for (auto& v: values_)
            hashModuleMap[hasher_->hashModule(v)] = v;
        CPPUNIT_ASSERT_EQUAL(values_.size(), hashModuleMap.size());

        //for (auto& it: hashModuleMap)
        //    printf("%8u --> %8u\n", it.second, it.first);

        CPPUNIT_ASSERT_EQUAL(    0U, hasher_->hashModule( 50000));
        CPPUNIT_ASSERT_EQUAL( 1000U, hasher_->hashModule( 50862));
        CPPUNIT_ASSERT_EQUAL( 2000U, hasher_->hashModule( 60841));
        CPPUNIT_ASSERT_EQUAL( 3000U, hasher_->hashModule( 72619));
        CPPUNIT_ASSERT_EQUAL( 4000U, hasher_->hashModule( 70649));
        CPPUNIT_ASSERT_EQUAL( 6000U, hasher_->hashModule( 90211));
        CPPUNIT_ASSERT_EQUAL( 8000U, hasher_->hashModule(105615));
        CPPUNIT_ASSERT_EQUAL(10000U, hasher_->hashModule(130012));
        CPPUNIT_ASSERT_EQUAL(12000U, hasher_->hashModule(151452));
        CPPUNIT_ASSERT_EQUAL(14000U, hasher_->hashModule(201412));
        CPPUNIT_ASSERT_EQUAL(15427U, hasher_->hashModule(221479));
    }

    void testHash() {
        CPPUNIT_ASSERT_EQUAL( 32103U, hasher_->hash( 827900423));
        CPPUNIT_ASSERT_EQUAL(106512U, hasher_->hash( 996589584));
        CPPUNIT_ASSERT_EQUAL(210627U, hasher_->hash(1165361155));
        CPPUNIT_ASSERT_EQUAL(307494U, hasher_->hash(1337147910));
        CPPUNIT_ASSERT_EQUAL(397154U, hasher_->hash(1509196290));
        CPPUNIT_ASSERT_EQUAL(505306U, hasher_->hash(1682866202));
        CPPUNIT_ASSERT_EQUAL(987395U, hasher_->hash(fakeSuperstripId_));
        CPPUNIT_ASSERT_EQUAL(987394U, hasher_->hash(fakeSuperstripId1_));
        CPPUNIT_ASSERT_EQUAL(987393U, hasher_->hash(fakeSuperstripId2_));
    }

    void testStitch() {
        std::vector<addr_type> input;
        std::vector<std::vector<addr_type> > output;

        output.clear(); output.resize(1);
        input     = {11, 22, 33};
        output[0] = {11, 22, 33, fakeSuperstripId_, fakeSuperstripId1_, fakeSuperstripId2_};
        CPPUNIT_ASSERT(output == stitcher_->stitch(input));

        output.clear(); output.resize(1);
        input     = {11, 22, 33, 44};
        output[0] = {11, 22, 33, 44, fakeSuperstripId_, fakeSuperstripId1_};
        CPPUNIT_ASSERT(output == stitcher_->stitch(input));

        output.clear(); output.resize(1);
        input     = {11, 22, 33, 44, 55};
        output[0] = {11, 22, 33, 44, 55, fakeSuperstripId_};
        CPPUNIT_ASSERT(output == stitcher_->stitch(input));

        output.clear(); output.resize(1);
        input     = {11, 22, 33, 44, 55, 66};
        output[0] = {11, 22, 33, 44, 55, 66};
        CPPUNIT_ASSERT(output == stitcher_->stitch(input));

        input     = {11, 22, 33, 44, 55, 66, 77};
        output    = combinations(input, 6);
        CPPUNIT_ASSERT(output == stitcher_->stitch(input));

        input      = {11, 22, 33, 44, 55, 66, 77, 88};
        output    = combinations(input, 6);
        CPPUNIT_ASSERT(output == stitcher_->stitch(input));
    }
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestSuperstripOperations);
