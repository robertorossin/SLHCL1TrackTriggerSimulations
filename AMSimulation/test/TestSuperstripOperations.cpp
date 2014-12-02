#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/SuperstripArbiter.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/SuperstripStitcher.h"
using namespace slhcl1tt;

#include <cppunit/extensions/HelperMacros.h>


class TestSuperstripOperations : public CppUnit::TestFixture  {

CPPUNIT_TEST_SUITE(TestSuperstripOperations);
CPPUNIT_TEST(testArbiterModule);
CPPUNIT_TEST(testArbiterSuperstrip);
CPPUNIT_TEST(testArbiterSuperstripLuciano);
CPPUNIT_TEST(testStitcherStitch);
CPPUNIT_TEST(testStitcherStitchLayermap);
CPPUNIT_TEST_SUITE_END();

private:
    SuperstripArbiter *arbiter_;
    SuperstripStitcher *stitcher_;
    std::vector<unsigned> values_;

public:
    void setUp() {
        const unsigned nLayers = 6;
        const unsigned nFakers = 3;

        const unsigned subLadderSize = 16;
        const unsigned subModuleSize = 256;

        std::vector<unsigned> subLadderVarSize = {8, 16, 32, 16, 16, 32};
        std::vector<unsigned> subModuleVarSize = {256, 256, 256, 256, 512, 512};
        std::vector<unsigned> subLadderECVarSize = {8, 8, 8, 16, 16, 16, 16, 32, 32, 16, 16, 16, 16, 32, 32};
        std::vector<unsigned> subModuleECVarSize = {256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 512, 512, 512, 512};
        arbiter_ = new SuperstripArbiter(subLadderSize, subModuleSize);
        //arbiter_ = new SuperstripArbiter(subLadderVarSize, subModuleVarSize, subLadderECVarSize, subModuleECVarSize);
        //arbiter_->print();

        stitcher_ = new SuperstripStitcher(nLayers, nFakers);

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
        delete stitcher_;
    }

    void testArbiterModule() {
        CPPUNIT_ASSERT_EQUAL(     0U, arbiter_->module( 5,    0,    0));
        CPPUNIT_ASSERT_EQUAL(  1007U, arbiter_->module( 5, 16-1, 63-1));
        CPPUNIT_ASSERT_EQUAL(  2327U, arbiter_->module( 6, 24-1, 55-1));
        CPPUNIT_ASSERT_EQUAL(  4163U, arbiter_->module( 7, 34-1, 54-1));
        CPPUNIT_ASSERT_EQUAL(  5315U, arbiter_->module( 8, 48-1, 24-1));
        CPPUNIT_ASSERT_EQUAL(  6803U, arbiter_->module( 9, 62-1, 24-1));
        CPPUNIT_ASSERT_EQUAL(  8627U, arbiter_->module(10, 76-1, 24-1));
        CPPUNIT_ASSERT_EQUAL(  8628U, arbiter_->module(11,    0,    0));
        CPPUNIT_ASSERT_EQUAL(  8871U, arbiter_->module(11,  8-1, 40-1));
        CPPUNIT_ASSERT_EQUAL(  9551U, arbiter_->module(12,  8-1, 40-1));
        CPPUNIT_ASSERT_EQUAL( 10231U, arbiter_->module(13,  8-1, 40-1));
        CPPUNIT_ASSERT_EQUAL( 10911U, arbiter_->module(14,  8-1, 40-1));
        CPPUNIT_ASSERT_EQUAL( 11591U, arbiter_->module(15,  8-1, 40-1));
        CPPUNIT_ASSERT_EQUAL( 12271U, arbiter_->module(18,  8-1, 40-1));
        CPPUNIT_ASSERT_EQUAL( 12951U, arbiter_->module(19,  8-1, 40-1));
        CPPUNIT_ASSERT_EQUAL( 13631U, arbiter_->module(20,  8-1, 40-1));
        CPPUNIT_ASSERT_EQUAL( 14311U, arbiter_->module(21,  8-1, 40-1));
        CPPUNIT_ASSERT_EQUAL( 14991U, arbiter_->module(22,  8-1, 40-1));
        CPPUNIT_ASSERT_EQUAL(  9307U, arbiter_->module(11, 15-1, 80-1));
        CPPUNIT_ASSERT_EQUAL(  9987U, arbiter_->module(12, 15-1, 80-1));
        CPPUNIT_ASSERT_EQUAL( 10667U, arbiter_->module(13, 15-1, 80-1));
        CPPUNIT_ASSERT_EQUAL( 11347U, arbiter_->module(14, 15-1, 80-1));
        CPPUNIT_ASSERT_EQUAL( 12027U, arbiter_->module(15, 15-1, 80-1));
        CPPUNIT_ASSERT_EQUAL( 12707U, arbiter_->module(18, 15-1, 80-1));
        CPPUNIT_ASSERT_EQUAL( 13387U, arbiter_->module(19, 15-1, 80-1));
        CPPUNIT_ASSERT_EQUAL( 14067U, arbiter_->module(20, 15-1, 80-1));
        CPPUNIT_ASSERT_EQUAL( 14747U, arbiter_->module(21, 15-1, 80-1));
        CPPUNIT_ASSERT_EQUAL( 15427U, arbiter_->module(22, 15-1, 80-1));
    }

    void testArbiterSuperstrip() {
        // half-strip
        CPPUNIT_ASSERT_EQUAL(     0U, arbiter_->superstrip( 5,    0,    0,  0*2,    0*2));
        CPPUNIT_ASSERT_EQUAL(     7U, arbiter_->superstrip( 5,    0,    0, 31*2,  959*2));
        CPPUNIT_ASSERT_EQUAL(  8063U, arbiter_->superstrip( 5, 16-1, 63-1, 31*2,  959*2));
        CPPUNIT_ASSERT_EQUAL( 18623U, arbiter_->superstrip( 6, 24-1, 55-1, 31*2,  959*2));
        CPPUNIT_ASSERT_EQUAL( 33311U, arbiter_->superstrip( 7, 34-1, 54-1, 31*2,  959*2));
        CPPUNIT_ASSERT_EQUAL( 42527U, arbiter_->superstrip( 8, 48-1, 24-1,  1*2, 1015*2));
        CPPUNIT_ASSERT_EQUAL( 54431U, arbiter_->superstrip( 9, 62-1, 24-1,  1*2, 1015*2));
        CPPUNIT_ASSERT_EQUAL( 69023U, arbiter_->superstrip(10, 76-1, 24-1,  1*2, 1015*2));
        CPPUNIT_ASSERT_EQUAL( 69024U, arbiter_->superstrip(11,    0,    0,  0*2,    0*2));
        CPPUNIT_ASSERT_EQUAL( 69031U, arbiter_->superstrip(11,    0,    0, 31*2,  959*2));
        CPPUNIT_ASSERT_EQUAL( 70975U, arbiter_->superstrip(11,  8-1, 40-1, 31*2,  959*2));
        CPPUNIT_ASSERT_EQUAL( 76415U, arbiter_->superstrip(12,  8-1, 40-1, 31*2,  959*2));
        CPPUNIT_ASSERT_EQUAL( 81855U, arbiter_->superstrip(13,  8-1, 40-1, 31*2,  959*2));
        CPPUNIT_ASSERT_EQUAL( 87295U, arbiter_->superstrip(14,  8-1, 40-1, 31*2,  959*2));
        CPPUNIT_ASSERT_EQUAL( 92735U, arbiter_->superstrip(15,  8-1, 40-1, 31*2,  959*2));
        CPPUNIT_ASSERT_EQUAL( 98175U, arbiter_->superstrip(18,  8-1, 40-1, 31*2,  959*2));
        CPPUNIT_ASSERT_EQUAL(103615U, arbiter_->superstrip(19,  8-1, 40-1, 31*2,  959*2));
        CPPUNIT_ASSERT_EQUAL(109055U, arbiter_->superstrip(20,  8-1, 40-1, 31*2,  959*2));
        CPPUNIT_ASSERT_EQUAL(114495U, arbiter_->superstrip(21,  8-1, 40-1, 31*2,  959*2));
        CPPUNIT_ASSERT_EQUAL(119935U, arbiter_->superstrip(22,  8-1, 40-1, 31*2,  959*2));
        CPPUNIT_ASSERT_EQUAL( 74463U, arbiter_->superstrip(11, 15-1, 80-1,  1*2, 1015*2));
        CPPUNIT_ASSERT_EQUAL( 79903U, arbiter_->superstrip(12, 15-1, 80-1,  1*2, 1015*2));
        CPPUNIT_ASSERT_EQUAL( 85343U, arbiter_->superstrip(13, 15-1, 80-1,  1*2, 1015*2));
        CPPUNIT_ASSERT_EQUAL( 90783U, arbiter_->superstrip(14, 15-1, 80-1,  1*2, 1015*2));
        CPPUNIT_ASSERT_EQUAL( 96223U, arbiter_->superstrip(15, 15-1, 80-1,  1*2, 1015*2));
        CPPUNIT_ASSERT_EQUAL(101663U, arbiter_->superstrip(18, 15-1, 80-1,  1*2, 1015*2));
        CPPUNIT_ASSERT_EQUAL(107103U, arbiter_->superstrip(19, 15-1, 80-1,  1*2, 1015*2));
        CPPUNIT_ASSERT_EQUAL(112543U, arbiter_->superstrip(20, 15-1, 80-1,  1*2, 1015*2));
        CPPUNIT_ASSERT_EQUAL(117983U, arbiter_->superstrip(21, 15-1, 80-1,  1*2, 1015*2));
        CPPUNIT_ASSERT_EQUAL(123423U, arbiter_->superstrip(22, 15-1, 80-1,  1*2, 1015*2));

        // full-strip
        CPPUNIT_ASSERT_EQUAL( 69023U, arbiter_->superstrip(10, 76-1, 24-1,  1, 1015, false));
        CPPUNIT_ASSERT_EQUAL(119935U, arbiter_->superstrip(22,  8-1, 40-1, 31,  959, false));
        CPPUNIT_ASSERT_EQUAL(123423U, arbiter_->superstrip(22, 15-1, 80-1,  1, 1015, false));
    }

    void testArbiterSuperstripLuciano() {
        // 400x0
        CPPUNIT_ASSERT_EQUAL(     0U, arbiter_->superstrip_luciano( 5, -M_PI, -2.2, M_PI/1600., 4.4));
        CPPUNIT_ASSERT_EQUAL(  1600U, arbiter_->superstrip_luciano( 5,   0.0, -2.2, M_PI/1600., 4.4));
        CPPUNIT_ASSERT_EQUAL(  3199U, arbiter_->superstrip_luciano( 5,  M_PI, -2.2, M_PI/1600., 4.4));
        CPPUNIT_ASSERT_EQUAL(     0U, arbiter_->superstrip_luciano( 5, -M_PI,  0.0, M_PI/1600., 4.4));
        CPPUNIT_ASSERT_EQUAL(  1600U, arbiter_->superstrip_luciano( 5,   0.0,  0.0, M_PI/1600., 4.4));
        CPPUNIT_ASSERT_EQUAL(  3199U, arbiter_->superstrip_luciano( 5,  M_PI,  0.0, M_PI/1600., 4.4));
        CPPUNIT_ASSERT_EQUAL(     0U, arbiter_->superstrip_luciano( 5, -M_PI,  2.2, M_PI/1600., 4.4));
        CPPUNIT_ASSERT_EQUAL(  1600U, arbiter_->superstrip_luciano( 5,   0.0,  2.2, M_PI/1600., 4.4));
        CPPUNIT_ASSERT_EQUAL(  3199U, arbiter_->superstrip_luciano( 5,  M_PI,  2.2, M_PI/1600., 4.4));
        CPPUNIT_ASSERT_EQUAL(  6399U, arbiter_->superstrip_luciano( 6,  M_PI, -2.2, M_PI/1600., 4.4));
        CPPUNIT_ASSERT_EQUAL(  9599U, arbiter_->superstrip_luciano( 7,  M_PI, -2.2, M_PI/1600., 4.4));
        CPPUNIT_ASSERT_EQUAL( 12799U, arbiter_->superstrip_luciano( 8,  M_PI, -2.2, M_PI/1600., 4.4));
        CPPUNIT_ASSERT_EQUAL( 15999U, arbiter_->superstrip_luciano( 9,  M_PI, -2.2, M_PI/1600., 4.4));
        CPPUNIT_ASSERT_EQUAL( 19199U, arbiter_->superstrip_luciano(10,  M_PI, -2.2, M_PI/1600., 4.4));

        // 400x1
        CPPUNIT_ASSERT_EQUAL( 11200U, arbiter_->superstrip_luciano( 5,   0.0,  0.0, M_PI/1600., 4.4/6));
        CPPUNIT_ASSERT_EQUAL( 12799U, arbiter_->superstrip_luciano( 5,  M_PI,  0.0, M_PI/1600., 4.4/6));
        CPPUNIT_ASSERT_EQUAL( 31999U, arbiter_->superstrip_luciano( 6,  M_PI,  0.0, M_PI/1600., 4.4/6));
        CPPUNIT_ASSERT_EQUAL( 51199U, arbiter_->superstrip_luciano( 7,  M_PI,  0.0, M_PI/1600., 4.4/6));
        CPPUNIT_ASSERT_EQUAL( 70399U, arbiter_->superstrip_luciano( 8,  M_PI,  0.0, M_PI/1600., 4.4/6));
        CPPUNIT_ASSERT_EQUAL( 89599U, arbiter_->superstrip_luciano( 9,  M_PI,  0.0, M_PI/1600., 4.4/6));
        CPPUNIT_ASSERT_EQUAL(108799U, arbiter_->superstrip_luciano(10,  M_PI,  0.0, M_PI/1600., 4.4/6));
    }

    void testStitcherStitch() {
        std::vector<unsigned> input;
        std::vector<unsigned> output;

        input  = {5, 6, 7};
        output = {0, 1, 2, 999999, 999999, 999999};
        CPPUNIT_ASSERT(output == stitcher_->stitch(input));

        input  = {5, 6, 7, 8};
        output = {0, 1, 2, 3, 999999, 999999};
        CPPUNIT_ASSERT(output == stitcher_->stitch(input));

        input  = {5, 6, 7, 8, 9};
        output = {0, 1, 2, 3, 4, 999999};
        CPPUNIT_ASSERT(output == stitcher_->stitch(input));

        input  = {5, 6, 7, 8, 9, 10};
        output = {0, 1, 2, 3, 4, 5};
        CPPUNIT_ASSERT(output == stitcher_->stitch(input));

        input  = {5, 6, 7, 8, 9, 10, 11};
        output = {0, 1, 3, 4, 5, 6};
        CPPUNIT_ASSERT(output == stitcher_->stitch(input));

        input  = {5, 6, 7, 8, 9, 10, 11, 12};
        output = {0, 1, 3, 4, 5, 7};
        CPPUNIT_ASSERT(output == stitcher_->stitch(input));

        //std::vector<unsigned> output2 = stitcher_->stitch(input);
        //for (unsigned i=0; i<output2.size(); ++i)
        //    std::cout << output2.at(i) << ", ";
        //std::cout << std::endl;
    }

    void testStitcherStitchLayermap() {
        std::vector<unsigned> input;
        std::vector<unsigned> output;

        input  = {5, 6, 7};
        output = {0, 1, 2, 999999, 999999, 999999};
        CPPUNIT_ASSERT(output == stitcher_->stitch_layermap(input));

        input  = {5, 6, 7, 8};
        output = {0, 1, 2, 3, 999999, 999999};
        CPPUNIT_ASSERT(output == stitcher_->stitch_layermap(input));

        input  = {5, 6, 7, 8, 9};
        output = {0, 1, 2, 3, 4, 999999};
        CPPUNIT_ASSERT(output == stitcher_->stitch_layermap(input));

        input  = {5, 6, 7, 8, 9, 10};
        output = {0, 1, 2, 3, 4, 5};
        CPPUNIT_ASSERT(output == stitcher_->stitch_layermap(input));

        input  = {5, 6, 7, 8, 9, 10, 11};
        output = {0, 1, 2, 3, 4, 6};
        CPPUNIT_ASSERT(output == stitcher_->stitch_layermap(input));

        input  = {5, 6, 7, 8, 9, 10, 11, 12};
        output = {0, 1, 2, 3, 7, 6};
        CPPUNIT_ASSERT(output == stitcher_->stitch_layermap(input));

        //std::vector<unsigned> output2 = stitcher_->stitch_layermap(input);
        //for (unsigned i=0; i<output2.size(); ++i)
        //    std::cout << output2.at(i) << ", ";
        //std::cout << std::endl;
    }
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestSuperstripOperations);
