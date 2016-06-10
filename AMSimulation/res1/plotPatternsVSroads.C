/*
 * plotPatternsVSroads.C
 *
 *  Created on: Mar 9, 2016
 *      Author: rossin
 */
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TMultiGraph.h>
#include <TLegend.h>
#include <TLegendEntry.h>
#include <TMath.h>

#include <iostream>
#include <map>

class ssConfiguration{
public:
	TString name_      ;
	float   roadsMean_ ;
//	float   roads90_   ;
	float   roads95_   ;
	float   combsMean_ ;
//	float   combs90_   ;
	float   combs95_   ;
	unsigned nPatterns_;

	ssConfiguration(
			TString name,
			float roadsMean,
			float roads95,
			float combsMean,
			float combs95,
			unsigned nPatterns) :
		name_(name)          ,
		roadsMean_(roadsMean),
		roads95_(roads95)    ,
		combsMean_(combsMean),
		combs95_(combs95)    ,
		nPatterns_(nPatterns)
	{}
};

class mergedConfiguration{
public:
	TString var_       ;
	TString conf_      ;
 	float   varMean_ ;
	float   var90_   ;
	float   var95_   ;
	float   var99_   ;
	unsigned nPatterns_;
	float varmax_;

	mergedConfiguration(
			TString var     ,
			TString conf    ,
			TString dummy   ,
			float   varMean ,
			TString dummy2  ,
			float   var90   ,
			float   var95   ,
			float   var99   ,
			unsigned nPatterns):
				var_     (var    ),
				conf_    (conf   ),
				varMean_ (varMean),
				var90_   (var90  ),
				var95_   (var95  ),
				var99_   (var99  ),
				nPatterns_(nPatterns),
	            varmax_(-1)
	{}
    mergedConfiguration(
            TString var     ,
            TString conf    ,
            TString dummy   ,
            float   varMean ,
            TString dummy2  ,
            float   var90   ,
            float   var95   ,
            float   var99   ,
            float    max    ,
            unsigned nPatterns) :
                var_     (var    ),
                conf_    (conf   ),
                varMean_ (varMean),
                var90_   (var90  ),
                var95_   (var95  ),
                var99_   (var99  ),
                nPatterns_(nPatterns),
                varmax_(max)
    {}
};


void plotPatternsVSroads (unsigned pu=140, bool isTTbar = 0, bool savePlots = 0) {

    unsigned vcolors[] = {1,2,3,4,95,6,64,8,40,41,42,43,44,44,45,46};

	std::vector <ssConfiguration> vssC;
//  PU140 - 95% coverage
//	vssC.push_back(ssConfiguration ("sf1_nz4"	         ,  16.66, 58.38	, 41.45	, 161.1,	1862700));
//	vssC.push_back(ssConfiguration ("sf1_nz444555"       ,  16.72, 57.6     , 39.88 , 154.3,	2442300));
//	vssC.push_back(ssConfiguration ("sf1_nz544443"       ,  16.95, 57.59	, 42.89	, 165.1,	1966700));
//	vssC.push_back(ssConfiguration ("sf1_nz555333"       ,  17.46, 59.67	, 43.76	, 168.3,	1886400));
//	vssC.push_back(ssConfiguration ("sf1_nz555444"       ,  16.01, 56.49	, 37.8	, 144.5,	2357500));
//	vssC.push_back(ssConfiguration ("sf1_nz555555"       ,  15.4,   54.7	, 34.53	, 131.4,	2922800));
//	vssC.push_back(ssConfiguration ("sf1_nz666222"       ,  19.05, 64.03	, 52.26	, 201  ,    1693700));
//	vssC.push_back(ssConfiguration ("sf1_nz666333"       ,  16.85, 58.1	    , 40.27	, 154.9,	2338200));
//	vssC.push_back(ssConfiguration ("sf1_nz666444"       ,  15.77, 55.78	, 35.66	, 139.2,	2958900));
//	vssC.push_back(ssConfiguration ("sf1_nz666555"       ,  15.12, 53.47	, 32.43	, 127.1,	3627500));
//	vssC.push_back(ssConfiguration ("sf1_nz6"	         ,  14.34, 51.69	, 29.93	, 117.4,	3905700));
//	vssC.push_back(ssConfiguration ("sf1_nz777111"       ,  32.89, 106.2	, 119.6	, 470.9,	1546000));
//	vssC.push_back(ssConfiguration ("sf1_nz777222"       ,  19	 , 64.34	, 50.88	, 194.2,	2142300));
//	vssC.push_back(ssConfiguration ("sf1_nz777333"       ,  16.45, 56.95	, 38.55	, 149.6,	2836000));
//	vssC.push_back(ssConfiguration ("sf1_nz888111"       ,  31.86, 103.6	, 114.8	, 460  ,    1899700));
//	vssC.push_back(ssConfiguration ("sf1_nz888222"       ,  18.41, 62.71	, 48.6	, 185.9,	2585300));
//	vssC.push_back(ssConfiguration ("sf1_nz8"	         ,  13.26, 48.42	, 25.95	, 102.2,	6712700));

	unsigned coverage = 0;
	unsigned nConfs = 1;
//  PU140 - 95% coverage
	if (pu==140) {
		coverage = 95;
		vssC.push_back(ssConfiguration ("sf1_nz4     "       , 16.52 , 57.81    , 41.23 ,161.9 ,    1764800)); //14k
		vssC.push_back(ssConfiguration ("sf1_nz4_L0x2"       , 17.06 , 58.68    , 47.47 ,183.8 ,    1309600)); //14k
		vssC.push_back(ssConfiguration ("sf1_nz4_L0x2_L5x2"  , 18.05 , 61.02    , 54.56 ,210.1 ,    1001100)); //14k
		vssC.push_back(ssConfiguration ("sf1p26_nz4"         , 18.98 , 64.8     , 55.58 ,214.8 ,    1053200)); //14k
		vssC.push_back(ssConfiguration ("sf1_nz6"	         , 13.88 , 50.48	, 28.91	, 114  ,    3905700)); //14k
		vssC.push_back(ssConfiguration ("sf1_nz6_L0x2"       , 13.79 , 49.56    , 31.45 , 123  ,    2851200)); //14k
		vssC.push_back(ssConfiguration ("sf1_nz6_L0x2_L5x2"  , 14.25 , 50.58    , 34.84 , 134  ,    2180400)); //14k
		vssC.push_back(ssConfiguration ("sf1p26_nz6"         , 15.11 , 53.46    , 35.82 ,137.8 ,    2304700)); //14k
		vssC.push_back(ssConfiguration ("sf1_nz8"	         , 12.88 , 47.03    , 25.12 , 98.64,    6712700)); //14k
		vssC.push_back(ssConfiguration ("sf1_nz8_L0x2"       , 12.65 , 46.15    , 26.79 ,104.2 ,    4953500)); //14k
		vssC.push_back(ssConfiguration ("sf1_nz8_L0x2_L5x2"  , 12.87 , 46.05    , 28.56 ,111.1 ,    3775200)); //14k
		vssC.push_back(ssConfiguration ("sf1p26_nz8"         , 13.52 , 48.63    , 29.12 ,114.2 ,    3990200)); //14k
		nConfs = 4;
	}

//  PU200 - 90% coverage
	if (pu==200) {
		coverage = 90;
		vssC.push_back(ssConfiguration ("sf1_nz4     "       ,  29.04, 84.25    , 95.49 ,324.2 ,    1184200));
		vssC.push_back(ssConfiguration ("sf1_nz4_L0x2"       ,  33.02, 91.53    , 120.1 ,399.1 ,     882300));
		vssC.push_back(ssConfiguration ("sf1_nz4_L0x2_L5x2"  ,  35.54, 100      , 144.3 ,487.2 ,     675500));
		vssC.push_back(ssConfiguration ("sf1_nz6"	         ,  20.41, 62.8 	, 54.35	, 190  ,    2593800));
		vssC.push_back(ssConfiguration ("sf1_nz6_L0x2"       ,  21.8 , 66.68    , 62.86 ,215.8 ,    1915900));
		vssC.push_back(ssConfiguration ("sf1_nz6_L0x2_L5x2"  ,  23.55, 69.87    , 73.91 ,254.5 ,    1469600));
		vssC.push_back(ssConfiguration ("sf1_nz8"	         ,  17.15, 54.78    , 41.74 ,  143 ,    4521400));
		vssC.push_back(ssConfiguration ("sf1_nz8_L0x2"       ,  17.64, 56       , 46.62 ,  163 ,    3374200));
		vssC.push_back(ssConfiguration ("sf1_nz8_L0x2_L5x2"  ,  19.22, 59.58    , 52.48 ,185.7 ,    2586500));
//		vssC.push_back(ssConfiguration ("sf1_nz8_L0x3_L5x2"  ,   0.0 , 64       , 0.0   ,  215 ,    2314700));
//		vssC.push_back(ssConfiguration ("sf1opt_nz8"	     ,  17.71, 55.47    , 44.94 ,159.4 ,    4625000));
        nConfs = 3;
	}

	if (pu==250) { // TODO    FAKE FOR NOW!!!!
        coverage = 95;
        vssC.push_back(ssConfiguration ("sf1_nz4     "       , 16.52 , 57.81    , 41.23 ,161.9 ,    1764800)); //14k
        vssC.push_back(ssConfiguration ("sf1_nz4_L0x2"       , 17.06 , 58.68    , 47.47 ,183.8 ,    1309600)); //14k
        vssC.push_back(ssConfiguration ("sf1_nz4_L0x2_L5x2"  , 18.05 , 61.02    , 54.56 ,210.1 ,    1001100)); //14k
        vssC.push_back(ssConfiguration ("sf1p26_nz4"         , 18.98 , 64.8     , 55.58 ,214.8 ,    1053200)); //14k
        vssC.push_back(ssConfiguration ("sf1_nz6"            , 13.88 , 50.48    , 28.91 , 114  ,    3905700)); //14k
        vssC.push_back(ssConfiguration ("sf1_nz6_L0x2"       , 13.79 , 49.56    , 31.45 , 123  ,    2851200)); //14k
        vssC.push_back(ssConfiguration ("sf1_nz6_L0x2_L5x2"  , 14.25 , 50.58    , 34.84 , 134  ,    2180400)); //14k
        vssC.push_back(ssConfiguration ("sf1p26_nz6"         , 15.11 , 53.46    , 35.82 ,137.8 ,    2304700)); //14k
        vssC.push_back(ssConfiguration ("sf1_nz8"            , 12.88 , 47.03    , 25.12 , 98.64,    6712700)); //14k
        vssC.push_back(ssConfiguration ("sf1_nz8_L0x2"       , 12.65 , 46.15    , 26.79 ,104.2 ,    4953500)); //14k
        vssC.push_back(ssConfiguration ("sf1_nz8_L0x2_L5x2"  , 12.87 , 46.05    , 28.56 ,111.1 ,    3775200)); //14k
        vssC.push_back(ssConfiguration ("sf1p26_nz8"         , 13.52 , 48.63    , 29.12 ,114.2 ,    3990200)); //14k
    }

	unsigned merging=0;
	// grep Mean meanlog.txt | awk '{print "\"" $1"\" , \"" $2 "\" , \""$3 "\"" ", " $4 " , ""\"" $5 "\"", ", " $6 " , " $7 " , " $8, ",", $10}'
	//grep Mean m8_TTbar_PU.log  | awk '{print "vmc.push_back(mergedConfiguration(\"" $1"\" , \"" $2 "\" , \""$3 "\"" ", " $4 " , ""\"" $5 "\"", ", " $6 " , " $7 " , " $8, ",", $10, ",", $12,"));"}'
	std::vector<mergedConfiguration> vmc;
	if (pu==140) {
	    if (isTTbar) {
	        vmc.push_back(mergedConfiguration("nRoads" , "TTbar_PU140_sf1L0x2L5x2_nz6" , "Mean=", 22.83 , "Q[0.90,0.95,0.99]=" , 57.88 , 93.24 , 196.41 , 2828 , 2180279                  ));
	        vmc.push_back(mergedConfiguration("nRoadsMerged_x8_" , "TTbar_PU140_sf1L0x2L5x2_nz6" , "Mean=", 15.37 , "Q[0.90,0.95,0.99]=" , 37.95 , 60.60 , 130.86 , 1576 , 880864         ));
	        vmc.push_back(mergedConfiguration("nCombsPerBX" , "TTbar_PU140_sf1L0x2L5x2_nz6" , "Mean=", 52.72 , "Q[0.90,0.95,0.99]=" , 123.25 , 217.53 , 571.64 , 41126 , 2180279          ));
	        vmc.push_back(mergedConfiguration("nCombsPerBXMerged_x8_" , "TTbar_PU140_sf1L0x2L5x2_nz6" , "Mean=", 40.43 , "Q[0.90,0.95,0.99]=" , 92.17 , 162.80 , 470.80 , 32433 , 880864  ));
	        vmc.push_back(mergedConfiguration("nRoads" , "TTbar_PU140_sf1L0x2L5x2_nz8" , "Mean=", 21.05 , "Q[0.90,0.95,0.99]=" , 54.86 , 85.27 , 191.36 , 2247 , 3774867                  ));
	        vmc.push_back(mergedConfiguration("nRoadsMerged_x8_" , "TTbar_PU140_sf1L0x2L5x2_nz8" , "Mean=", 13.95 , "Q[0.90,0.95,0.99]=" , 35.82 , 54.89 , 118.85 , 1292 , 1510120        ));
	        vmc.push_back(mergedConfiguration("nCombsPerBX" , "TTbar_PU140_sf1L0x2L5x2_nz8" , "Mean=", 44.99 , "Q[0.90,0.95,0.99]=" , 106.40 , 185.37 , 520.84 , 27037 , 3774867          ));
	        vmc.push_back(mergedConfiguration("nCombsPerBXMerged_x8_" , "TTbar_PU140_sf1L0x2L5x2_nz8" , "Mean=", 33.58 , "Q[0.90,0.95,0.99]=" , 76.65 , 137.31 , 418.76 , 20201 , 1510120 ));
	        vmc.push_back(mergedConfiguration("nRoads" , "TTbar_PU140_sf1L0x2_nz6" , "Mean=", 21.97 , "Q[0.90,0.95,0.99]=" , 56.37 , 90.80 , 184.27 , 2869 , 2851008                      ));
	        vmc.push_back(mergedConfiguration("nRoadsMerged_x8_" , "TTbar_PU140_sf1L0x2_nz6" , "Mean=", 14.84 , "Q[0.90,0.95,0.99]=" , 37.48 , 58.79 , 123.62 , 1590 , 1157114            ));
	        vmc.push_back(mergedConfiguration("nCombsPerBX" , "TTbar_PU140_sf1L0x2_nz6" , "Mean=", 48.93 , "Q[0.90,0.95,0.99]=" , 115.27 , 200.00 , 526.80 , 27943 , 2851008              ));
	        vmc.push_back(mergedConfiguration("nCombsPerBXMerged_x8_" , "TTbar_PU140_sf1L0x2_nz6" , "Mean=", 36.52 , "Q[0.90,0.95,0.99]=" , 84.62 , 149.22 , 404.38 , 21357 , 1157114     ));
	        vmc.push_back(mergedConfiguration("nRoads" , "TTbar_PU140_sf1L0x2_nz8" , "Mean=", 20.58 , "Q[0.90,0.95,0.99]=" , 54.70 , 82.76 , 180.34 , 2402 , 4953044                      ));
	        vmc.push_back(mergedConfiguration("nRoadsMerged_x8_" , "TTbar_PU140_sf1L0x2_nz8" , "Mean=", 13.61 , "Q[0.90,0.95,0.99]=" , 35.71 , 53.83 , 116.43 , 1356 , 1999061            ));
	        vmc.push_back(mergedConfiguration("nCombsPerBX" , "TTbar_PU140_sf1L0x2_nz8" , "Mean=", 42.06 , "Q[0.90,0.95,0.99]=" , 101.37 , 172.66 , 464.78 , 19832 , 4953044              ));
	        vmc.push_back(mergedConfiguration("nCombsPerBXMerged_x8_" , "TTbar_PU140_sf1L0x2_nz8" , "Mean=", 31.05 , "Q[0.90,0.95,0.99]=" , 73.93 , 124.81 , 346.74 , 14596 , 1999061     ));
	        vmc.push_back(mergedConfiguration("nCombsPerRoad" , "TTbar_PU140_sf1L0x2L5x2_nz6" , "Mean=", 2.71 , "Q[0.90,0.95,0.99]=" , 5.81 , 8.49 , 19.81 , 2180279                      ));
	        vmc.push_back(mergedConfiguration("nCombsPerRoadMerged_x8_" , "TTbar_PU140_sf1L0x2L5x2_nz6" , "Mean=", 3.04 , "Q[0.90,0.95,0.99]=" , 6.74 , 9.44 , 24.65 , 880864             ));
	        vmc.push_back(mergedConfiguration("nCombsPerRoad" , "TTbar_PU140_sf1L0x2L5x2_nz8" , "Mean=", 2.49 , "Q[0.90,0.95,0.99]=" , 5.51 , 7.87 , 17.66 , 3774867                      ));
	        vmc.push_back(mergedConfiguration("nCombsPerRoadMerged_x8_" , "TTbar_PU140_sf1L0x2L5x2_nz8" , "Mean=", 2.77 , "Q[0.90,0.95,0.99]=" , 5.94 , 8.77 , 20.81 , 1510120            ));
	        vmc.push_back(mergedConfiguration("nCombsPerRoad" , "TTbar_PU140_sf1L0x2_nz6" , "Mean=", 2.52 , "Q[0.90,0.95,0.99]=" , 5.55 , 7.84 , 17.77 , 2851008                          ));
	        vmc.push_back(mergedConfiguration("nCombsPerRoadMerged_x8_" , "TTbar_PU140_sf1L0x2_nz6" , "Mean=", 2.82 , "Q[0.90,0.95,0.99]=" , 6.00 , 8.90 , 20.80 , 1157114                ));
	        vmc.push_back(mergedConfiguration("nCombsPerRoad" , "TTbar_PU140_sf1L0x2_nz8" , "Mean=", 2.31 , "Q[0.90,0.95,0.99]=" , 5.22 , 7.25 , 16.29 , 4953044                          ));
	        vmc.push_back(mergedConfiguration("nCombsPerRoadMerged_x8_" , "TTbar_PU140_sf1L0x2_nz8" , "Mean=", 2.58 , "Q[0.90,0.95,0.99]=" , 5.69 , 8.20 , 18.52 , 1999061                ));
// 100c. Full bank merging
	        vmc.push_back(mergedConfiguration("nRoads" , "TTbar_PU140_sf1L0x2L5x2_nz4_100c" , "Mean=", 26.77 , "Q[0.90,0.95,0.99]=" , 68.25 , 106.15 , 226.21 , 2319 , 1001026 ));
	        vmc.push_back(mergedConfiguration("nRoadsMerged_x8_" , "TTbar_PU140_sf1L0x2L5x2_nz4_100c" , "Mean=", 18.11 , "Q[0.90,0.95,0.99]=" , 44.32 , 68.71 , 153.65 , 1199 , 238245 ));
	        vmc.push_back(mergedConfiguration("nCombsPerRoad" , "TTbar_PU140_sf1L0x2L5x2_nz4_100c" , "Mean=", 3.04 , "Q[0.90,0.95,0.99]=" , 6.58 , 8.95 , 24.14 , 0 , 1001026 ));
	        vmc.push_back(mergedConfiguration("nCombsPerRoadMerged_x8_" , "TTbar_PU140_sf1L0x2L5x2_nz4_100c" , "Mean=", 3.70 , "Q[0.90,0.95,0.99]=" , 8.37 , 12.50 , 27.94 , 0 , 238245 ));
	        vmc.push_back(mergedConfiguration("nCombsPerBX" , "TTbar_PU140_sf1L0x2L5x2_nz4_100c" , "Mean=", 71.62 , "Q[0.90,0.95,0.99]=" , 179.73 , 312.22 , 792.58 , 51964 , 1001026 ));
	        vmc.push_back(mergedConfiguration("nCombsPerBXMerged_x8_" , "TTbar_PU140_sf1L0x2L5x2_nz4_100c" , "Mean=", 61.41 , "Q[0.90,0.95,0.99]=" , 142.55 , 259.50 , 757.00 , 46756 , 238245 ));
	        vmc.push_back(mergedConfiguration("nRoads" , "TTbar_PU140_sf1L0x2L5x2_nz6_100c" , "Mean=", 22.83 , "Q[0.90,0.95,0.99]=" , 57.84 , 93.24 , 196.41 , 2828 , 2180280 ));
	        vmc.push_back(mergedConfiguration("nRoadsMerged_x8_" , "TTbar_PU140_sf1L0x2L5x2_nz6_100c" , "Mean=", 14.91 , "Q[0.90,0.95,0.99]=" , 36.59 , 57.88 , 126.91 , 1305 , 479558 ));
	        vmc.push_back(mergedConfiguration("nCombsPerRoad" , "TTbar_PU140_sf1L0x2L5x2_nz6_100c" , "Mean=", 2.59 , "Q[0.90,0.95,0.99]=" , 4.92 , 8.21 , 18.02 , 0 , 2180280 ));
	        vmc.push_back(mergedConfiguration("nCombsPerRoadMerged_x8_" , "TTbar_PU140_sf1L0x2L5x2_nz6_100c" , "Mean=", 3.07 , "Q[0.90,0.95,0.99]=" , 6.61 , 8.97 , 24.04 , 0 , 479558 ));
	        vmc.push_back(mergedConfiguration("nCombsPerBX" , "TTbar_PU140_sf1L0x2L5x2_nz6_100c" , "Mean=", 52.72 , "Q[0.90,0.95,0.99]=" , 123.25 , 217.53 , 571.64 , 41126 , 2180280 ));
	        vmc.push_back(mergedConfiguration("nCombsPerBXMerged_x8_" , "TTbar_PU140_sf1L0x2L5x2_nz6_100c" , "Mean=", 41.72 , "Q[0.90,0.95,0.99]=" , 94.55 , 169.14 , 482.27 , 37133 , 479558 ));
	        vmc.push_back(mergedConfiguration("nRoads" , "TTbar_PU140_sf1L0x2L5x2_nz8_100c" , "Mean=", 21.05 , "Q[0.90,0.95,0.99]=" , 54.86 , 85.27 , 191.36 , 2247 , 3774869 ));
	        vmc.push_back(mergedConfiguration("nRoadsMerged_x8_" , "TTbar_PU140_sf1L0x2L5x2_nz8_100c" , "Mean=", 13.50 , "Q[0.90,0.95,0.99]=" , 34.62 , 52.47 , 118.11 , 1136 , 840069 ));
	        vmc.push_back(mergedConfiguration("nCombsPerRoad" , "TTbar_PU140_sf1L0x2L5x2_nz8_100c" , "Mean=", 2.40 , "Q[0.90,0.95,0.99]=" , 4.76 , 6.91 , 16.50 , 0 , 3774869 ));
	        vmc.push_back(mergedConfiguration("nCombsPerRoadMerged_x8_" , "TTbar_PU140_sf1L0x2L5x2_nz8_100c" , "Mean=", 2.75 , "Q[0.90,0.95,0.99]=" , 6.11 , 8.58 , 18.86 , 0 , 840069 ));
	        vmc.push_back(mergedConfiguration("nCombsPerBX" , "TTbar_PU140_sf1L0x2L5x2_nz8_100c" , "Mean=", 44.99 , "Q[0.90,0.95,0.99]=" , 106.40 , 185.37 , 520.84 , 27037 , 3774869 ));
	        vmc.push_back(mergedConfiguration("nCombsPerBXMerged_x8_" , "TTbar_PU140_sf1L0x2L5x2_nz8_100c" , "Mean=", 34.03 , "Q[0.90,0.95,0.99]=" , 78.23 , 139.38 , 403.38 , 21259 , 840069 ));

	        vmc.push_back(mergedConfiguration("nRoads" , "TTbar_PU140_sf1L0x2_nz4_100c" , "Mean=", 25.77 , "Q[0.90,0.95,0.99]=" , 66.07 , 100.78 , 211.37 , 2381 , 1309491 ));
	        vmc.push_back(mergedConfiguration("nRoadsMerged_x8_" , "TTbar_PU140_sf1L0x2_nz4_100c" , "Mean=", 16.95 , "Q[0.90,0.95,0.99]=" , 41.85 , 64.87 , 141.53 , 1185 , 301885 ));
	        vmc.push_back(mergedConfiguration("nCombsPerRoad" , "TTbar_PU140_sf1L0x2_nz4_100c" , "Mean=", 2.82 , "Q[0.90,0.95,0.99]=" , 6.22 , 8.66 , 18.79 , 0 , 1309491 ));
	        vmc.push_back(mergedConfiguration("nCombsPerRoadMerged_x8_" , "TTbar_PU140_sf1L0x2_nz4_100c" , "Mean=", 3.41 , "Q[0.90,0.95,0.99]=" , 7.25 , 12.12 , 24.74 , 0 , 301885 ));
	        vmc.push_back(mergedConfiguration("nCombsPerBX" , "TTbar_PU140_sf1L0x2_nz4_100c" , "Mean=", 64.22 , "Q[0.90,0.95,0.99]=" , 158.16 , 278.55 , 689.02 , 34659 , 1309491 ));
	        vmc.push_back(mergedConfiguration("nCombsPerBXMerged_x8_" , "TTbar_PU140_sf1L0x2_nz4_100c" , "Mean=", 53.48 , "Q[0.90,0.95,0.99]=" , 126.60 , 222.48 , 637.44 , 31432 , 301885 ));
	        vmc.push_back(mergedConfiguration("nRoads" , "TTbar_PU140_sf1L0x2_nz6_100c" , "Mean=", 21.97 , "Q[0.90,0.95,0.99]=" , 56.37 , 90.80 , 184.27 , 2869 , 2851009 ));
	        vmc.push_back(mergedConfiguration("nRoadsMerged_x8_" , "TTbar_PU140_sf1L0x2_nz6_100c" , "Mean=", 14.22 , "Q[0.90,0.95,0.99]=" , 35.61 , 55.72 , 117.43 , 1322 , 628819 ));
	        vmc.push_back(mergedConfiguration("nCombsPerRoad" , "TTbar_PU140_sf1L0x2_nz6_100c" , "Mean=", 2.44 , "Q[0.90,0.95,0.99]=" , 4.79 , 6.90 , 16.59 , 0 , 2851009 ));
	        vmc.push_back(mergedConfiguration("nCombsPerRoadMerged_x8_" , "TTbar_PU140_sf1L0x2_nz6_100c" , "Mean=", 2.86 , "Q[0.90,0.95,0.99]=" , 6.26 , 8.69 , 20.27 , 0 , 628819 ));
	        vmc.push_back(mergedConfiguration("nCombsPerBX" , "TTbar_PU140_sf1L0x2_nz6_100c" , "Mean=", 48.93 , "Q[0.90,0.95,0.99]=" , 115.27 , 200.00 , 526.80 , 27943 , 2851009 ));
	        vmc.push_back(mergedConfiguration("nCombsPerBXMerged_x8_" , "TTbar_PU140_sf1L0x2_nz6_100c" , "Mean=", 37.13 , "Q[0.90,0.95,0.99]=" , 85.42 , 151.53 , 407.38 , 24191 , 628819 ));
	        vmc.push_back(mergedConfiguration("nRoads" , "TTbar_PU140_sf1L0x2_nz8_100c" , "Mean=", 20.58 , "Q[0.90,0.95,0.99]=" , 54.70 , 82.76 , 180.34 , 2389 , 4953047 ));
	        vmc.push_back(mergedConfiguration("nRoadsMerged_x8_" , "TTbar_PU140_sf1L0x2_nz8_100c" , "Mean=", 13.19 , "Q[0.90,0.95,0.99]=" , 34.25 , 51.58 , 110.27 , 1154 , 1144092 ));
	        vmc.push_back(mergedConfiguration("nCombsPerRoad" , "TTbar_PU140_sf1L0x2_nz8_100c" , "Mean=", 2.25 , "Q[0.90,0.95,0.99]=" , 4.62 , 6.60 , 12.98 , 0 , 4953047 ));
	        vmc.push_back(mergedConfiguration("nCombsPerRoadMerged_x8_" , "TTbar_PU140_sf1L0x2_nz8_100c" , "Mean=", 2.58 , "Q[0.90,0.95,0.99]=" , 4.93 , 8.21 , 16.96 , 0 , 1144092 ));
	        vmc.push_back(mergedConfiguration("nCombsPerBX" , "TTbar_PU140_sf1L0x2_nz8_100c" , "Mean=", 42.06 , "Q[0.90,0.95,0.99]=" , 101.37 , 172.66 , 464.78 , 19832 , 4953047 ));
	        vmc.push_back(mergedConfiguration("nCombsPerBXMerged_x8_" , "TTbar_PU140_sf1L0x2_nz8_100c" , "Mean=", 31.37 , "Q[0.90,0.95,0.99]=" , 73.41 , 128.26 , 341.57 , 15493 , 1144092 ));

	    }
	    else {
        vmc.push_back(mergedConfiguration("nRoads" , "PU140_sf1L0x2L5x2_nz4" , "Mean=", 17.79 , "Q[0.90,0.95,0.99]=" , 45.30 , 60.31 , 96.78 , 1001026              ));
        vmc.push_back(mergedConfiguration("nCombsPerRoad" , "PU140_sf1L0x2L5x2_nz4" , "Mean=", 2.84 , "Q[0.90,0.95,0.99]=" , 6.48 , 8.80 , 16.89 , 1001026          ));
        vmc.push_back(mergedConfiguration("nCombsPerBX" , "PU140_sf1L0x2L5x2_nz4" , "Mean=", 50.48 , "Q[0.90,0.95,0.99]=" , 132.84 , 201.31 , 379.82 , 1001026      ));
        vmc.push_back(mergedConfiguration("nRoads" , "PU140_sf1L0x2L5x2_nz6" , "Mean=", 14.15 , "Q[0.90,0.95,0.99]=" , 37.72 , 50.35 , 78.90 , 2180279              ));
        vmc.push_back(mergedConfiguration("nCombsPerRoad" , "PU140_sf1L0x2L5x2_nz6" , "Mean=", 2.34 , "Q[0.90,0.95,0.99]=" , 4.82 , 6.85 , 12.90 , 2180279          ));
        vmc.push_back(mergedConfiguration("nCombsPerBX" , "PU140_sf1L0x2L5x2_nz6" , "Mean=", 33.13 , "Q[0.90,0.95,0.99]=" , 88.30 , 130.72 , 264.64 , 2180279       ));
        vmc.push_back(mergedConfiguration("nRoads" , "PU140_sf1L0x2L5x2_nz8" , "Mean=", 12.85 , "Q[0.90,0.95,0.99]=" , 34.90 , 46.00 , 73.67 , 3774867              ));
        vmc.push_back(mergedConfiguration("nCombsPerRoad" , "PU140_sf1L0x2L5x2_nz8" , "Mean=", 2.16 , "Q[0.90,0.95,0.99]=" , 4.61 , 6.42 , 12.42 , 3774867          ));
        vmc.push_back(mergedConfiguration("nCombsPerBX" , "PU140_sf1L0x2L5x2_nz8" , "Mean=", 27.81 , "Q[0.90,0.95,0.99]=" , 72.94 , 110.30 , 237.08 , 3774867       ));
        vmc.push_back(mergedConfiguration("nRoads" , "PU140_sf1L0x2_nz4" , "Mean=", 16.87 , "Q[0.90,0.95,0.99]=" , 43.04 , 58.13 , 93.44 , 1309491                  ));
        vmc.push_back(mergedConfiguration("nCombsPerRoad" , "PU140_sf1L0x2_nz4" , "Mean=", 2.65 , "Q[0.90,0.95,0.99]=" , 6.14 , 8.47 , 16.39 , 1309491              ));
        vmc.push_back(mergedConfiguration("nCombsPerBX" , "PU140_sf1L0x2_nz4" , "Mean=", 44.76 , "Q[0.90,0.95,0.99]=" , 116.10 , 177.61 , 345.37 , 1309491          ));
        vmc.push_back(mergedConfiguration("nRoads" , "PU140_sf1L0x2_nz6" , "Mean=", 13.70 , "Q[0.90,0.95,0.99]=" , 37.15 , 49.45 , 78.18 , 2851008                  ));
        vmc.push_back(mergedConfiguration("nCombsPerRoad" , "PU140_sf1L0x2_nz6" , "Mean=", 2.23 , "Q[0.90,0.95,0.99]=" , 4.69 , 6.58 , 12.54 , 2851008              ));
        vmc.push_back(mergedConfiguration("nCombsPerBX" , "PU140_sf1L0x2_nz6" , "Mean=", 30.62 , "Q[0.90,0.95,0.99]=" , 82.21 , 121.35 , 250.15 , 2851008           ));
        vmc.push_back(mergedConfiguration("nRoads" , "PU140_sf1L0x2_nz8" , "Mean=", 12.62 , "Q[0.90,0.95,0.99]=" , 34.21 , 46.08 , 74.15 , 4953044                  ));
        vmc.push_back(mergedConfiguration("nCombsPerRoad" , "PU140_sf1L0x2_nz8" , "Mean=", 2.07 , "Q[0.90,0.95,0.99]=" , 4.50 , 6.15 , 10.91 , 4953044              ));
        vmc.push_back(mergedConfiguration("nCombsPerBX" , "PU140_sf1L0x2_nz8" , "Mean=", 26.22 , "Q[0.90,0.95,0.99]=" , 68.65 , 103.57 , 219.08 , 4953044           ));
        vmc.push_back(mergedConfiguration("nRoadsMerged_x2_" , "PU140_sf1L0x2L5x2_nz4" , "Mean=", 14.97 , "Q[0.90,0.95,0.99]=" , 37.80 , 50.09 , 79.01 , 595679         ));
        vmc.push_back(mergedConfiguration("nCombsPerRoadMerged_x2_" , "PU140_sf1L0x2L5x2_nz4" , "Mean=", 3.01 , "Q[0.90,0.95,0.99]=" , 6.73 , 9.20 , 18.42 , 595679     ));
        vmc.push_back(mergedConfiguration("nCombsPerBXMerged_x2_" , "PU140_sf1L0x2L5x2_nz4" , "Mean=", 46.04 , "Q[0.90,0.95,0.99]=" , 120.78 , 183.31 , 350.55 , 595679 ));
        vmc.push_back(mergedConfiguration("nRoadsMerged_x2_" , "PU140_sf1L0x2L5x2_nz6" , "Mean=", 11.91 , "Q[0.90,0.95,0.99]=" , 31.64 , 41.76 , 66.06 , 1298337        ));
        vmc.push_back(mergedConfiguration("nCombsPerRoadMerged_x2_" , "PU140_sf1L0x2L5x2_nz6" , "Mean=", 2.46 , "Q[0.90,0.95,0.99]=" , 4.94 , 8.02 , 14.68 , 1298337    ));
        vmc.push_back(mergedConfiguration("nCombsPerBXMerged_x2_" , "PU140_sf1L0x2L5x2_nz6" , "Mean=", 29.76 , "Q[0.90,0.95,0.99]=" , 79.03 , 117.60 , 242.40 , 1298337 ));
        vmc.push_back(mergedConfiguration("nRoadsMerged_x2_" , "PU140_sf1L0x2L5x2_nz8" , "Mean=", 10.81 , "Q[0.90,0.95,0.99]=" , 29.36 , 38.57 , 61.17 , 2249397        ));
        vmc.push_back(mergedConfiguration("nCombsPerRoadMerged_x2_" , "PU140_sf1L0x2L5x2_nz8" , "Mean=", 2.25 , "Q[0.90,0.95,0.99]=" , 4.72 , 6.63 , 12.61 , 2249397    ));
        vmc.push_back(mergedConfiguration("nCombsPerBXMerged_x2_" , "PU140_sf1L0x2L5x2_nz8" , "Mean=", 24.76 , "Q[0.90,0.95,0.99]=" , 64.41 , 97.90 , 216.11 , 2249397  ));
        vmc.push_back(mergedConfiguration("nRoadsMerged_x2_" , "PU140_sf1L0x2_nz4" , "Mean=", 14.15 , "Q[0.90,0.95,0.99]=" , 35.89 , 47.90 , 76.44 , 776321             ));
        vmc.push_back(mergedConfiguration("nCombsPerRoadMerged_x2_" , "PU140_sf1L0x2_nz4" , "Mean=", 2.82 , "Q[0.90,0.95,0.99]=" , 6.44 , 8.73 , 16.79 , 776321         ));
        vmc.push_back(mergedConfiguration("nCombsPerBXMerged_x2_" , "PU140_sf1L0x2_nz4" , "Mean=", 40.56 , "Q[0.90,0.95,0.99]=" , 104.52 , 160.19 , 319.15 , 776321     ));
        vmc.push_back(mergedConfiguration("nRoadsMerged_x2_" , "PU140_sf1L0x2_nz6" , "Mean=", 11.49 , "Q[0.90,0.95,0.99]=" , 31.03 , 40.73 , 64.88 , 1693108            ));
        vmc.push_back(mergedConfiguration("nCombsPerRoadMerged_x2_" , "PU140_sf1L0x2_nz6" , "Mean=", 2.34 , "Q[0.90,0.95,0.99]=" , 4.82 , 6.82 , 12.78 , 1693108        ));
        vmc.push_back(mergedConfiguration("nCombsPerBXMerged_x2_" , "PU140_sf1L0x2_nz6" , "Mean=", 27.27 , "Q[0.90,0.95,0.99]=" , 72.88 , 107.57 , 221.10 , 1693108     ));
        vmc.push_back(mergedConfiguration("nRoadsMerged_x2_" , "PU140_sf1L0x2_nz8" , "Mean=", 10.63 , "Q[0.90,0.95,0.99]=" , 28.89 , 38.51 , 60.73 , 2945176            ));
        vmc.push_back(mergedConfiguration("nCombsPerRoadMerged_x2_" , "PU140_sf1L0x2_nz8" , "Mean=", 2.16 , "Q[0.90,0.95,0.99]=" , 4.62 , 6.42 , 12.27 , 2945176        ));
        vmc.push_back(mergedConfiguration("nCombsPerBXMerged_x2_" , "PU140_sf1L0x2_nz8" , "Mean=", 23.12 , "Q[0.90,0.95,0.99]=" , 60.53 , 92.03 , 198.81 , 2945176      ));
        vmc.push_back(mergedConfiguration("nRoadsMerged_x4_" , "PU140_sf1L0x2L5x2_nz4" , "Mean=", 13.35 , "Q[0.90,0.95,0.99]=" , 33.65 , 44.57 , 71.38 , 477291         ));
        vmc.push_back(mergedConfiguration("nCombsPerRoadMerged_x4_" , "PU140_sf1L0x2L5x2_nz4" , "Mean=", 3.07 , "Q[0.90,0.95,0.99]=" , 6.83 , 9.75 , 18.57 , 477291     ));
        vmc.push_back(mergedConfiguration("nCombsPerBXMerged_x4_" , "PU140_sf1L0x2L5x2_nz4" , "Mean=", 42.32 , "Q[0.90,0.95,0.99]=" , 110.53 , 168.12 , 330.59 , 477291 ));
        vmc.push_back(mergedConfiguration("nRoadsMerged_x4_" , "PU140_sf1L0x2L5x2_nz6" , "Mean=", 10.46 , "Q[0.90,0.95,0.99]=" , 27.54 , 36.52 , 56.70 , 1018838        ));
        vmc.push_back(mergedConfiguration("nCombsPerRoadMerged_x4_" , "PU140_sf1L0x2L5x2_nz6" , "Mean=", 2.52 , "Q[0.90,0.95,0.99]=" , 5.00 , 8.18 , 15.75 , 1018838    ));
        vmc.push_back(mergedConfiguration("nCombsPerBXMerged_x4_" , "PU140_sf1L0x2L5x2_nz6" , "Mean=", 26.83 , "Q[0.90,0.95,0.99]=" , 71.31 , 104.33 , 222.19 , 1018838 ));
        vmc.push_back(mergedConfiguration("nRoadsMerged_x4_" , "PU140_sf1L0x2L5x2_nz8" , "Mean=", 9.44 , "Q[0.90,0.95,0.99]=" , 25.51 , 33.47 , 53.33 , 1750212         ));
        vmc.push_back(mergedConfiguration("nCombsPerRoadMerged_x4_" , "PU140_sf1L0x2L5x2_nz8" , "Mean=", 2.30 , "Q[0.90,0.95,0.99]=" , 4.79 , 6.77 , 12.72 , 1750212    ));
        vmc.push_back(mergedConfiguration("nCombsPerBXMerged_x4_" , "PU140_sf1L0x2L5x2_nz8" , "Mean=", 22.26 , "Q[0.90,0.95,0.99]=" , 58.26 , 87.34 , 196.03 , 1750212  ));
        vmc.push_back(mergedConfiguration("nRoadsMerged_x4_" , "PU140_sf1L0x2_nz4" , "Mean=", 12.54 , "Q[0.90,0.95,0.99]=" , 31.63 , 42.50 , 67.86 , 623341             ));
        vmc.push_back(mergedConfiguration("nCombsPerRoadMerged_x4_" , "PU140_sf1L0x2_nz4" , "Mean=", 2.89 , "Q[0.90,0.95,0.99]=" , 6.56 , 8.82 , 16.91 , 623341         ));
        vmc.push_back(mergedConfiguration("nCombsPerBXMerged_x4_" , "PU140_sf1L0x2_nz4" , "Mean=", 37.22 , "Q[0.90,0.95,0.99]=" , 95.42 , 146.55 , 290.13 , 623341      ));
        vmc.push_back(mergedConfiguration("nRoadsMerged_x4_" , "PU140_sf1L0x2_nz6" , "Mean=", 10.12 , "Q[0.90,0.95,0.99]=" , 27.19 , 35.62 , 56.00 , 1337203            ));
        vmc.push_back(mergedConfiguration("nCombsPerRoadMerged_x4_" , "PU140_sf1L0x2_nz6" , "Mean=", 2.40 , "Q[0.90,0.95,0.99]=" , 4.88 , 6.93 , 12.91 , 1337203        ));
        vmc.push_back(mergedConfiguration("nCombsPerBXMerged_x4_" , "PU140_sf1L0x2_nz6" , "Mean=", 24.66 , "Q[0.90,0.95,0.99]=" , 65.53 , 96.66 , 203.70 , 1337203      ));
        vmc.push_back(mergedConfiguration("nRoadsMerged_x4_" , "PU140_sf1L0x2_nz8" , "Mean=", 9.27 , "Q[0.90,0.95,0.99]=" , 24.95 , 33.38 , 52.73 , 2309055             ));
        vmc.push_back(mergedConfiguration("nCombsPerRoadMerged_x4_" , "PU140_sf1L0x2_nz8" , "Mean=", 2.22 , "Q[0.90,0.95,0.99]=" , 4.70 , 6.58 , 12.42 , 2309055        ));
        vmc.push_back(mergedConfiguration("nCombsPerBXMerged_x4_" , "PU140_sf1L0x2_nz8" , "Mean=", 20.78 , "Q[0.90,0.95,0.99]=" , 54.46 , 82.28 , 181.02 , 2309055      ));
//        vmc.push_back(mergedConfiguration("nRoads" , "PU140_sf1L0x2L5x2__nz8_OR" , "Mean=", 11.11 , "Q[0.90,0.95,0.99]=" , 31.07 , 40.47 , 64.63 , 3671262                  ));
//        vmc.push_back(mergedConfiguration("nRoadsMerged_x4_" , "PU140_sf1L0x2L5x2__nz8_OR" , "Mean=", 8.22 , "Q[0.90,0.95,0.99]=" , 22.72 , 29.70 , 46.89 , 1706341         ));
//        vmc.push_back(mergedConfiguration("nCombsPerRoad" , "PU140_sf1L0x2L5x2__nz8_OR" , "Mean=", 1.93 , "Q[0.90,0.95,0.99]=" , 4.28 , 4.99 , 10.49 , 3671262              ));
//        vmc.push_back(mergedConfiguration("nCombsPerRoadMerged_x4_" , "PU140_sf1L0x2L5x2__nz8_OR" , "Mean=", 2.05 , "Q[0.90,0.95,0.99]=" , 4.48 , 6.16 , 12.21 , 1706341    ));
//        vmc.push_back(mergedConfiguration("nCombsPerBX" , "PU140_sf1L0x2L5x2__nz8_OR" , "Mean=", 21.67 , "Q[0.90,0.95,0.99]=" , 57.26 , 86.53 , 183.02 , 3671262            ));
//        vmc.push_back(mergedConfiguration("nCombsPerBXMerged_x4_" , "PU140_sf1L0x2L5x2__nz8_OR" , "Mean=", 17.22 , "Q[0.90,0.95,0.99]=" , 44.89 , 67.68 , 143.51 , 1706341  ));
        vmc.push_back(mergedConfiguration("nRoadsMerged_x8_" , "PU140_sf1L0x2L5x2_nz6" , "Mean=", 9.42 , "Q[0.90,0.95,0.99]=" , 24.88 , 32.86 , 51.00 , 880864              ));
        vmc.push_back(mergedConfiguration("nCombsPerRoadMerged_x8_" , "PU140_sf1L0x2L5x2_nz6" , "Mean=", 2.59 , "Q[0.90,0.95,0.99]=" , 5.71 , 8.33 , 16.24 , 880864         ));
        vmc.push_back(mergedConfiguration("nCombsPerBXMerged_x8_" , "PU140_sf1L0x2L5x2_nz6" , "Mean=", 24.98 , "Q[0.90,0.95,0.99]=" , 65.61 , 97.53 , 205.73 , 880864       ));
        vmc.push_back(mergedConfiguration("nRoadsMerged_x8_" , "PU140_sf1L0x2L5x2_nz8" , "Mean=", 8.45 , "Q[0.90,0.95,0.99]=" , 22.72 , 29.83 , 47.56 , 1510120             ));
        vmc.push_back(mergedConfiguration("nCombsPerRoadMerged_x8_" , "PU140_sf1L0x2L5x2_nz8" , "Mean=", 2.36 , "Q[0.90,0.95,0.99]=" , 4.84 , 6.87 , 12.83 , 1510120        ));
        vmc.push_back(mergedConfiguration("nCombsPerBXMerged_x8_" , "PU140_sf1L0x2L5x2_nz8" , "Mean=", 20.42 , "Q[0.90,0.95,0.99]=" , 52.93 , 80.14 , 171.03 , 1510120      ));
        vmc.push_back(mergedConfiguration("nRoadsMerged_x8_" , "PU140_sf1L0x2_nz6" , "Mean=", 9.10 , "Q[0.90,0.95,0.99]=" , 24.41 , 31.90 , 49.85 , 1157114                 ));
        vmc.push_back(mergedConfiguration("nCombsPerRoadMerged_x8_" , "PU140_sf1L0x2_nz6" , "Mean=", 2.46 , "Q[0.90,0.95,0.99]=" , 4.95 , 8.03 , 14.17 , 1157114            ));
        vmc.push_back(mergedConfiguration("nCombsPerBXMerged_x8_" , "PU140_sf1L0x2_nz6" , "Mean=", 23.02 , "Q[0.90,0.95,0.99]=" , 60.71 , 89.43 , 192.02 , 1157114          ));
        vmc.push_back(mergedConfiguration("nRoadsMerged_x8_" , "PU140_sf1L0x2_nz8" , "Mean=", 8.30 , "Q[0.90,0.95,0.99]=" , 22.42 , 29.90 , 47.09 , 1999061                 ));
        vmc.push_back(mergedConfiguration("nCombsPerRoadMerged_x8_" , "PU140_sf1L0x2_nz8" , "Mean=", 2.27 , "Q[0.90,0.95,0.99]=" , 4.76 , 6.69 , 12.55 , 1999061            ));
        vmc.push_back(mergedConfiguration("nCombsPerBXMerged_x8_" , "PU140_sf1L0x2_nz8" , "Mean=", 19.10 , "Q[0.90,0.95,0.99]=" , 50.16 , 76.02 , 166.77 , 1999061          ));
	}
	}
	if (pu==200) {
        if (isTTbar) {
            vmc.push_back(mergedConfiguration("nRoads" , "TTbar_PU200_sf1L0x2L5x2_nz6" , "Mean=", 39.20 , "Q[0.90,0.95,0.99]=" , 91.51 , 134.77 , 294.08 , 1874 , 2180279                   ));
            vmc.push_back(mergedConfiguration("nRoadsMerged_x8_" , "TTbar_PU200_sf1L0x2L5x2_nz6" , "Mean=", 26.17 , "Q[0.90,0.95,0.99]=" , 60.62 , 87.43 , 189.33 , 970 , 880864            ));
            vmc.push_back(mergedConfiguration("nCombsPerBX" , "TTbar_PU200_sf1L0x2L5x2_nz6" , "Mean=", 106.77 , "Q[0.90,0.95,0.99]=" , 247.04 , 389.26 , 1028.08 , 19097 , 2180279          ));
            vmc.push_back(mergedConfiguration("nCombsPerBXMerged_x8_" , "TTbar_PU200_sf1L0x2L5x2_nz6" , "Mean=", 83.26 , "Q[0.90,0.95,0.99]=" , 188.65 , 303.76 , 764.08 , 13958 , 880864   ));
            vmc.push_back(mergedConfiguration("nRoads" , "TTbar_PU200_sf1L0x2L5x2_nz8" , "Mean=", 32.63 , "Q[0.90,0.95,0.99]=" , 77.58 , 119.61 , 248.41 , 2106 , 3774867                   ));
            vmc.push_back(mergedConfiguration("nRoadsMerged_x8_" , "TTbar_PU200_sf1L0x2L5x2_nz8" , "Mean=", 21.54 , "Q[0.90,0.95,0.99]=" , 50.51 , 76.67 , 157.33 , 1036 , 1510120          ));
            vmc.push_back(mergedConfiguration("nCombsPerBX" , "TTbar_PU200_sf1L0x2L5x2_nz8" , "Mean=", 79.69 , "Q[0.90,0.95,0.99]=" , 182.52 , 308.12 , 760.12 , 9555 , 3774867             ));
            vmc.push_back(mergedConfiguration("nCombsPerBXMerged_x8_" , "TTbar_PU200_sf1L0x2L5x2_nz8" , "Mean=", 61.01 , "Q[0.90,0.95,0.99]=" , 135.92 , 224.02 , 596.04 , 7236 , 1510120   ));
            vmc.push_back(mergedConfiguration("nRoads" , "TTbar_PU200_sf1L0x2_nz6" , "Mean=", 36.84 , "Q[0.90,0.95,0.99]=" , 86.33 , 126.16 , 273.34 , 1885 , 2851008                       ));
            vmc.push_back(mergedConfiguration("nRoadsMerged_x8_" , "TTbar_PU200_sf1L0x2_nz6" , "Mean=", 24.31 , "Q[0.90,0.95,0.99]=" , 56.69 , 81.27 , 175.00 , 980 , 1157114               ));
            vmc.push_back(mergedConfiguration("nCombsPerBX" , "TTbar_PU200_sf1L0x2_nz6" , "Mean=", 93.90 , "Q[0.90,0.95,0.99]=" , 214.97 , 339.37 , 872.04 , 14725 , 2851008                ));
            vmc.push_back(mergedConfiguration("nCombsPerBXMerged_x8_" , "TTbar_PU200_sf1L0x2_nz6" , "Mean=", 71.66 , "Q[0.90,0.95,0.99]=" , 164.61 , 260.02 , 668.02 , 10936 , 1157114      ));
            vmc.push_back(mergedConfiguration("nRoads" , "TTbar_PU200_sf1L0x2_nz8" , "Mean=", 31.05 , "Q[0.90,0.95,0.99]=" , 74.51 , 113.28 , 230.01 , 1947 , 4953044                       ));
            vmc.push_back(mergedConfiguration("nRoadsMerged_x8_" , "TTbar_PU200_sf1L0x2_nz8" , "Mean=", 20.46 , "Q[0.90,0.95,0.99]=" , 48.29 , 72.56 , 147.33 , 1012 , 1999061              ));
            vmc.push_back(mergedConfiguration("nCombsPerBX" , "TTbar_PU200_sf1L0x2_nz8" , "Mean=", 72.72 , "Q[0.90,0.95,0.99]=" , 164.40 , 276.40 , 692.00 , 7117 , 4953044                 ));
            vmc.push_back(mergedConfiguration("nCombsPerBXMerged_x8_" , "TTbar_PU200_sf1L0x2_nz8" , "Mean=", 54.25 , "Q[0.90,0.95,0.99]=" , 124.40 , 207.20 , 512.00 , 5341 , 1999061       ));
            vmc.push_back(mergedConfiguration("nCombsPerRoad" , "TTbar_PU200_sf1L0x2L5x2_nz6" , "Mean=", 2.80 , "Q[0.90,0.95,0.99]=" , 6.31 , 8.93 , 18.31 , 2180279                        ));
            vmc.push_back(mergedConfiguration("nCombsPerRoadMerged_x8_" , "TTbar_PU200_sf1L0x2L5x2_nz6" , "Mean=", 3.24 , "Q[0.90,0.95,0.99]=" , 7.50 , 9.87 , 24.39 , 880864               ));
            vmc.push_back(mergedConfiguration("nCombsPerRoad" , "TTbar_PU200_sf1L0x2L5x2_nz8" , "Mean=", 2.49 , "Q[0.90,0.95,0.99]=" , 5.72 , 8.06 , 16.27 , 3774867                        ));
            vmc.push_back(mergedConfiguration("nCombsPerRoadMerged_x8_" , "TTbar_PU200_sf1L0x2L5x2_nz8" , "Mean=", 2.86 , "Q[0.90,0.95,0.99]=" , 6.50 , 9.07 , 18.74 , 1510120              ));
            vmc.push_back(mergedConfiguration("nCombsPerRoad" , "TTbar_PU200_sf1L0x2_nz6" , "Mean=", 2.58 , "Q[0.90,0.95,0.99]=" , 5.84 , 8.29 , 16.81 , 2851008                            ));
            vmc.push_back(mergedConfiguration("nCombsPerRoadMerged_x8_" , "TTbar_PU200_sf1L0x2_nz6" , "Mean=", 2.98 , "Q[0.90,0.95,0.99]=" , 6.92 , 9.38 , 19.85 , 1157114                  ));
            vmc.push_back(mergedConfiguration("nCombsPerRoad" , "TTbar_PU200_sf1L0x2_nz8" , "Mean=", 2.32 , "Q[0.90,0.95,0.99]=" , 5.46 , 7.48 , 13.51 , 4953044                            ));
            vmc.push_back(mergedConfiguration("nCombsPerRoadMerged_x8_" , "TTbar_PU200_sf1L0x2_nz8" , "Mean=", 2.65 , "Q[0.90,0.95,0.99]=" , 5.95 , 8.54 , 17.10 , 1999061                  ));
//100c. full bank merging
            vmc.push_back(mergedConfiguration("nRoads" , "TTbar_PU200_sf1L0x2L5x2_nz4_100c" , "Mean=", 55.67 , "Q[0.90,0.95,0.99]=" , 127.70 , 185.14 , 358.82 , 1819 , 1001026 ));
            vmc.push_back(mergedConfiguration("nRoadsMerged_x8_" , "TTbar_PU200_sf1L0x2L5x2_nz4_100c" , "Mean=", 39.51 , "Q[0.90,0.95,0.99]=" , 87.81 , 127.25 , 242.00 , 1113 , 238245 ));
            vmc.push_back(mergedConfiguration("nCombsPerRoad" , "TTbar_PU200_sf1L0x2L5x2_nz4_100c" , "Mean=", 3.44 , "Q[0.90,0.95,0.99]=" , 8.02 , 12.06 , 24.40 , 0 , 1001026 ));
            vmc.push_back(mergedConfiguration("nCombsPerRoadMerged_x8_" , "TTbar_PU200_sf1L0x2L5x2_nz4_100c" , "Mean=", 4.34 , "Q[0.90,0.95,0.99]=" , 8.96 , 14.43 , 32.52 , 0 , 238245 ));
            vmc.push_back(mergedConfiguration("nCombsPerBX" , "TTbar_PU200_sf1L0x2L5x2_nz4_100c" , "Mean=", 190.03 , "Q[0.90,0.95,0.99]=" , 455.29 , 681.93 , 1622.14 , 16198 , 1001026 ));
            vmc.push_back(mergedConfiguration("nCombsPerBXMerged_x8_" , "TTbar_PU200_sf1L0x2L5x2_nz4_100c" , "Mean=", 173.53 , "Q[0.90,0.95,0.99]=" , 408.95 , 629.53 , 1526.12 , 14164 , 238245 ));
            vmc.push_back(mergedConfiguration("nRoads" , "TTbar_PU200_sf1L0x2L5x2_nz6_100c" , "Mean=", 39.20 , "Q[0.90,0.95,0.99]=" , 91.51 , 134.77 , 294.08 , 1873 , 2180280 ));
            vmc.push_back(mergedConfiguration("nRoadsMerged_x8_" , "TTbar_PU200_sf1L0x2L5x2_nz6_100c" , "Mean=", 25.57 , "Q[0.90,0.95,0.99]=" , 58.64 , 85.56 , 181.00 , 898 , 479558 ));
            vmc.push_back(mergedConfiguration("nCombsPerRoad" , "TTbar_PU200_sf1L0x2L5x2_nz6_100c" , "Mean=", 2.78 , "Q[0.90,0.95,0.99]=" , 6.16 , 8.53 , 18.04 , 0 , 2180280 ));
            vmc.push_back(mergedConfiguration("nCombsPerRoadMerged_x8_" , "TTbar_PU200_sf1L0x2L5x2_nz6_100c" , "Mean=", 3.47 , "Q[0.90,0.95,0.99]=" , 8.06 , 12.02 , 24.61 , 0 , 479558 ));
            vmc.push_back(mergedConfiguration("nCombsPerBX" , "TTbar_PU200_sf1L0x2L5x2_nz6_100c" , "Mean=", 106.77 , "Q[0.90,0.95,0.99]=" , 247.04 , 389.26 , 1028.08 , 19097 , 2180280 ));
            vmc.push_back(mergedConfiguration("nCombsPerBXMerged_x8_" , "TTbar_PU200_sf1L0x2L5x2_nz6_100c" , "Mean=", 88.63 , "Q[0.90,0.95,0.99]=" , 200.73 , 318.91 , 854.69 , 14094 , 479558 ));
            vmc.push_back(mergedConfiguration("nRoads" , "TTbar_PU200_sf1L0x2L5x2_nz8_100c" , "Mean=", 32.63 , "Q[0.90,0.95,0.99]=" , 77.58 , 119.61 , 248.41 , 2106 , 3774869 ));
            vmc.push_back(mergedConfiguration("nRoadsMerged_x8_" , "TTbar_PU200_sf1L0x2L5x2_nz8_100c" , "Mean=", 20.93 , "Q[0.90,0.95,0.99]=" , 49.49 , 73.75 , 149.60 , 1004 , 840069 ));
            vmc.push_back(mergedConfiguration("nCombsPerRoad" , "TTbar_PU200_sf1L0x2L5x2_nz8_100c" , "Mean=", 2.48 , "Q[0.90,0.95,0.99]=" , 4.88 , 8.03 , 16.10 , 0 , 3774869 ));
            vmc.push_back(mergedConfiguration("nCombsPerRoadMerged_x8_" , "TTbar_PU200_sf1L0x2L5x2_nz8_100c" , "Mean=", 3.00 , "Q[0.90,0.95,0.99]=" , 6.55 , 8.85 , 18.96 , 0 , 840069 ));
            vmc.push_back(mergedConfiguration("nCombsPerBX" , "TTbar_PU200_sf1L0x2L5x2_nz8_100c" , "Mean=", 79.69 , "Q[0.90,0.95,0.99]=" , 182.52 , 308.12 , 760.12 , 9555 , 3774869 ));
            vmc.push_back(mergedConfiguration("nCombsPerBXMerged_x8_" , "TTbar_PU200_sf1L0x2L5x2_nz8_100c" , "Mean=", 63.09 , "Q[0.90,0.95,0.99]=" , 138.17 , 240.42 , 622.02 , 7404 , 840069 ));

            vmc.push_back(mergedConfiguration("nRoads" , "TTbar_PU200_sf1L0x2_nz4_100c" , "Mean=", 50.47 , "Q[0.90,0.95,0.99]=" , 115.32 , 167.76 , 324.69 , 1635 , 1309491 ));
            vmc.push_back(mergedConfiguration("nRoadsMerged_x8_" , "TTbar_PU200_sf1L0x2_nz4_100c" , "Mean=", 34.90 , "Q[0.90,0.95,0.99]=" , 78.13 , 111.90 , 214.40 , 951 , 301885 ));
            vmc.push_back(mergedConfiguration("nCombsPerRoad" , "TTbar_PU200_sf1L0x2_nz4_100c" , "Mean=", 3.13 , "Q[0.90,0.95,0.99]=" , 6.69 , 8.96 , 18.92 , 0 , 1309491 ));
            vmc.push_back(mergedConfiguration("nCombsPerRoadMerged_x8_" , "TTbar_PU200_sf1L0x2_nz4_100c" , "Mean=", 3.94 , "Q[0.90,0.95,0.99]=" , 8.61 , 12.65 , 28.20 , 0 , 301885 ));
            vmc.push_back(mergedConfiguration("nCombsPerBX" , "TTbar_PU200_sf1L0x2_nz4_100c" , "Mean=", 157.36 , "Q[0.90,0.95,0.99]=" , 375.42 , 576.10 , 1352.08 , 13223 , 1309491 ));
            vmc.push_back(mergedConfiguration("nCombsPerBXMerged_x8_" , "TTbar_PU200_sf1L0x2_nz4_100c" , "Mean=", 141.14 , "Q[0.90,0.95,0.99]=" , 324.71 , 507.58 , 1208.04 , 10790 , 301885 ));
            vmc.push_back(mergedConfiguration("nRoads" , "TTbar_PU200_sf1L0x2_nz6_100c" , "Mean=", 36.84 , "Q[0.90,0.95,0.99]=" , 86.33 , 126.16 , 273.34 , 1885 , 2851009 ));
            vmc.push_back(mergedConfiguration("nRoadsMerged_x8_" , "TTbar_PU200_sf1L0x2_nz6_100c" , "Mean=", 23.49 , "Q[0.90,0.95,0.99]=" , 53.94 , 78.20 , 163.50 , 900 , 628819 ));
            vmc.push_back(mergedConfiguration("nCombsPerRoad" , "TTbar_PU200_sf1L0x2_nz6_100c" , "Mean=", 2.57 , "Q[0.90,0.95,0.99]=" , 4.94 , 8.16 , 16.38 , 0 , 2851009 ));
            vmc.push_back(mergedConfiguration("nCombsPerRoadMerged_x8_" , "TTbar_PU200_sf1L0x2_nz6_100c" , "Mean=", 3.17 , "Q[0.90,0.95,0.99]=" , 6.75 , 9.30 , 24.02 , 0 , 628819 ));
            vmc.push_back(mergedConfiguration("nCombsPerBX" , "TTbar_PU200_sf1L0x2_nz6_100c" , "Mean=", 93.90 , "Q[0.90,0.95,0.99]=" , 214.97 , 339.37 , 872.04 , 14725 , 2851009 ));
            vmc.push_back(mergedConfiguration("nCombsPerBXMerged_x8_" , "TTbar_PU200_sf1L0x2_nz6_100c" , "Mean=", 74.55 , "Q[0.90,0.95,0.99]=" , 168.78 , 276.02 , 680.02 , 10835 , 628819 ));
            vmc.push_back(mergedConfiguration("nRoads" , "TTbar_PU200_sf1L0x2_nz8_100c" , "Mean=", 31.04 , "Q[0.90,0.95,0.99]=" , 74.46 , 113.28 , 230.01 , 1947 , 4953047 ));
            vmc.push_back(mergedConfiguration("nRoadsMerged_x8_" , "TTbar_PU200_sf1L0x2_nz8_100c" , "Mean=", 19.88 , "Q[0.90,0.95,0.99]=" , 47.13 , 69.64 , 141.50 , 984 , 1144092 ));
            vmc.push_back(mergedConfiguration("nCombsPerRoad" , "TTbar_PU200_sf1L0x2_nz8_100c" , "Mean=", 2.32 , "Q[0.90,0.95,0.99]=" , 4.74 , 6.74 , 12.75 , 0 , 4953047 ));
            vmc.push_back(mergedConfiguration("nCombsPerRoadMerged_x8_" , "TTbar_PU200_sf1L0x2_nz8_100c" , "Mean=", 2.74 , "Q[0.90,0.95,0.99]=" , 6.15 , 8.49 , 16.83 , 0 , 1144092 ));
            vmc.push_back(mergedConfiguration("nCombsPerBX" , "TTbar_PU200_sf1L0x2_nz8_100c" , "Mean=", 72.73 , "Q[0.90,0.95,0.99]=" , 164.40 , 276.40 , 692.00 , 7117 , 4953047 ));
            vmc.push_back(mergedConfiguration("nCombsPerBXMerged_x8_" , "TTbar_PU200_sf1L0x2_nz8_100c" , "Mean=", 55.22 , "Q[0.90,0.95,0.99]=" , 125.69 , 208.92 , 524.00 , 5665 , 1144092 ));
        }
        else {
        vmc.push_back(mergedConfiguration("nRoads" , "PU200_sf1L0x2L5x2_nz4" , "Mean=", 50.88 , "Q[0.90,0.95,0.99]=" , 109.85 , 140.03 , 207.31 , 1001026                ));
        vmc.push_back(mergedConfiguration("nCombsPerRoad" , "PU200_sf1L0x2L5x2_nz4" , "Mean=", 3.82 , "Q[0.90,0.95,0.99]=" , 8.68 , 12.68 , 24.80 , 1001026              ));
        vmc.push_back(mergedConfiguration("nCombsPerBX" , "PU200_sf1L0x2L5x2_nz4" , "Mean=", 204.19 , "Q[0.90,0.95,0.99]=" , 487.82 , 688.55 , 1207.06 , 1001026         ));
        vmc.push_back(mergedConfiguration("nRoads" , "PU200_sf1L0x2L5x2_nz6" , "Mean=", 32.13 , "Q[0.90,0.95,0.99]=" , 74.19 , 94.73 , 139.80 , 2180279                  ));
        vmc.push_back(mergedConfiguration("nCombsPerRoad" , "PU200_sf1L0x2L5x2_nz6" , "Mean=", 3.03 , "Q[0.90,0.95,0.99]=" , 6.67 , 9.09 , 20.18 , 2180279               ));
        vmc.push_back(mergedConfiguration("nCombsPerBX" , "PU200_sf1L0x2L5x2_nz6" , "Mean=", 101.31 , "Q[0.90,0.95,0.99]=" , 241.50 , 340.06 , 710.10 , 2180279          ));
        vmc.push_back(mergedConfiguration("nRoads" , "PU200_sf1L0x2L5x2_nz8" , "Mean=", 25.31 , "Q[0.90,0.95,0.99]=" , 60.98 , 76.96 , 117.50 , 3774867                  ));
        vmc.push_back(mergedConfiguration("nCombsPerRoad" , "PU200_sf1L0x2L5x2_nz8" , "Mean=", 2.68 , "Q[0.90,0.95,0.99]=" , 6.08 , 8.48 , 16.81 , 3774867               ));
        vmc.push_back(mergedConfiguration("nCombsPerBX" , "PU200_sf1L0x2L5x2_nz8" , "Mean=", 69.62 , "Q[0.90,0.95,0.99]=" , 170.93 , 247.41 , 488.06 , 3774867           ));
        vmc.push_back(mergedConfiguration("nRoads" , "PU200_sf1L0x2_nz4" , "Mean=", 45.93 , "Q[0.90,0.95,0.99]=" , 100.43 , 126.88 , 190.79 , 1309491                    ));
        vmc.push_back(mergedConfiguration("nCombsPerRoad" , "PU200_sf1L0x2_nz4" , "Mean=", 3.51 , "Q[0.90,0.95,0.99]=" , 8.26 , 12.23 , 24.37 , 1309491                  ));
        vmc.push_back(mergedConfiguration("nCombsPerBX" , "PU200_sf1L0x2_nz4" , "Mean=", 168.25 , "Q[0.90,0.95,0.99]=" , 397.49 , 555.40 , 1034.44 , 1309491             ));
        vmc.push_back(mergedConfiguration("nRoads" , "PU200_sf1L0x2_nz6" , "Mean=", 29.66 , "Q[0.90,0.95,0.99]=" , 69.28 , 89.33 , 130.50 , 2851008                      ));
        vmc.push_back(mergedConfiguration("nCombsPerRoad" , "PU200_sf1L0x2_nz6" , "Mean=", 2.83 , "Q[0.90,0.95,0.99]=" , 6.36 , 8.69 , 18.01 , 2851008                   ));
        vmc.push_back(mergedConfiguration("nCombsPerBX" , "PU200_sf1L0x2_nz6" , "Mean=", 85.88 , "Q[0.90,0.95,0.99]=" , 209.56 , 292.70 , 562.04 , 2851008               ));
        vmc.push_back(mergedConfiguration("nRoads" , "PU200_sf1L0x2_nz8" , "Mean=", 23.43 , "Q[0.90,0.95,0.99]=" , 56.52 , 72.50 , 113.00 , 4953044                      ));
        vmc.push_back(mergedConfiguration("nCombsPerRoad" , "PU200_sf1L0x2_nz8" , "Mean=", 2.61 , "Q[0.90,0.95,0.99]=" , 5.47 , 8.29 , 16.56 , 4953044                   ));
        vmc.push_back(mergedConfiguration("nCombsPerBX" , "PU200_sf1L0x2_nz8" , "Mean=", 61.88 , "Q[0.90,0.95,0.99]=" , 152.80 , 215.27 , 456.04 , 4953044               ));
        vmc.push_back(mergedConfiguration("nRoadsMerged_x2_" , "PU200_sf1L0x2L5x2_nz4" , "Mean=", 42.97 , "Q[0.90,0.95,0.99]=" , 92.17 , 117.46 , 176.03 , 595679        ));
        vmc.push_back(mergedConfiguration("nCombsPerRoadMerged_x2_" , "PU200_sf1L0x2L5x2_nz4" , "Mean=", 4.09 , "Q[0.90,0.95,0.99]=" , 8.97 , 12.95 , 27.10 , 595679     ));
        vmc.push_back(mergedConfiguration("nCombsPerBXMerged_x2_" , "PU200_sf1L0x2L5x2_nz4" , "Mean=", 188.54 , "Q[0.90,0.95,0.99]=" , 452.13 , 634.03 , 1132.84 , 595679));
        vmc.push_back(mergedConfiguration("nRoadsMerged_x2_" , "PU200_sf1L0x2L5x2_nz6" , "Mean=", 26.96 , "Q[0.90,0.95,0.99]=" , 61.56 , 78.12 , 116.83 , 1298337        ));
        vmc.push_back(mergedConfiguration("nCombsPerRoadMerged_x2_" , "PU200_sf1L0x2L5x2_nz6" , "Mean=", 3.23 , "Q[0.90,0.95,0.99]=" , 6.93 , 10.45 , 21.32 , 1298337    ));
        vmc.push_back(mergedConfiguration("nCombsPerBXMerged_x2_" , "PU200_sf1L0x2L5x2_nz6" , "Mean=", 91.77 , "Q[0.90,0.95,0.99]=" , 217.14 , 307.85 , 654.02 , 1298337 ));
        vmc.push_back(mergedConfiguration("nRoadsMerged_x2_" , "PU200_sf1L0x2L5x2_nz8" , "Mean=", 21.29 , "Q[0.90,0.95,0.99]=" , 50.75 , 63.63 , 96.50 , 2249397         ));
        vmc.push_back(mergedConfiguration("nCombsPerRoadMerged_x2_" , "PU200_sf1L0x2L5x2_nz8" , "Mean=", 2.83 , "Q[0.90,0.95,0.99]=" , 6.35 , 8.72 , 18.32 , 2249397     ));
        vmc.push_back(mergedConfiguration("nCombsPerBXMerged_x2_" , "PU200_sf1L0x2L5x2_nz8" , "Mean=", 62.33 , "Q[0.90,0.95,0.99]=" , 153.62 , 219.85 , 440.04 , 2249397 ));
        vmc.push_back(mergedConfiguration("nRoadsMerged_x2_" , "PU200_sf1L0x2_nz4" , "Mean=", 38.66 , "Q[0.90,0.95,0.99]=" , 83.41 , 105.58 , 160.67 , 776321            ));
        vmc.push_back(mergedConfiguration("nCombsPerRoadMerged_x2_" , "PU200_sf1L0x2_nz4" , "Mean=", 3.77 , "Q[0.90,0.95,0.99]=" , 8.60 , 12.56 , 24.70 , 776321         ));
        vmc.push_back(mergedConfiguration("nCombsPerBXMerged_x2_" , "PU200_sf1L0x2_nz4" , "Mean=", 154.34 , "Q[0.90,0.95,0.99]=" , 364.95 , 512.86 , 952.34 , 776321     ));
        vmc.push_back(mergedConfiguration("nRoadsMerged_x2_" , "PU200_sf1L0x2_nz6" , "Mean=", 24.84 , "Q[0.90,0.95,0.99]=" , 57.48 , 74.00 , 107.00 , 1693108            ));
        vmc.push_back(mergedConfiguration("nCombsPerRoadMerged_x2_" , "PU200_sf1L0x2_nz6" , "Mean=", 3.02 , "Q[0.90,0.95,0.99]=" , 6.65 , 8.95 , 18.83 , 1693108         ));
        vmc.push_back(mergedConfiguration("nCombsPerBXMerged_x2_" , "PU200_sf1L0x2_nz6" , "Mean=", 77.50 , "Q[0.90,0.95,0.99]=" , 187.23 , 261.12 , 515.34 , 1693108     ));
        vmc.push_back(mergedConfiguration("nRoadsMerged_x2_" , "PU200_sf1L0x2_nz8" , "Mean=", 19.72 , "Q[0.90,0.95,0.99]=" , 47.18 , 59.93 , 91.00 , 2945176             ));
        vmc.push_back(mergedConfiguration("nCombsPerRoadMerged_x2_" , "PU200_sf1L0x2_nz8" , "Mean=", 2.76 , "Q[0.90,0.95,0.99]=" , 6.19 , 8.59 , 16.99 , 2945176         ));
        vmc.push_back(mergedConfiguration("nCombsPerBXMerged_x2_" , "PU200_sf1L0x2_nz8" , "Mean=", 56.24 , "Q[0.90,0.95,0.99]=" , 136.80 , 191.80 , 422.00 , 2945176     ));
        vmc.push_back(mergedConfiguration("nRoadsMerged_x4_" , "PU200_sf1L0x2L5x2_nz4" , "Mean=", 38.68 , "Q[0.90,0.95,0.99]=" , 82.18 , 106.25 , 159.67 , 477291        ));
        vmc.push_back(mergedConfiguration("nCombsPerRoadMerged_x4_" , "PU200_sf1L0x2L5x2_nz4" , "Mean=", 4.20 , "Q[0.90,0.95,0.99]=" , 9.38 , 14.58 , 28.44 , 477291     ));
        vmc.push_back(mergedConfiguration("nCombsPerBXMerged_x4_" , "PU200_sf1L0x2L5x2_nz4" , "Mean=", 176.41 , "Q[0.90,0.95,0.99]=" , 419.10 , 599.85 , 1097.34 , 477291));
        vmc.push_back(mergedConfiguration("nRoadsMerged_x4_" , "PU200_sf1L0x2L5x2_nz6" , "Mean=", 23.86 , "Q[0.90,0.95,0.99]=" , 54.20 , 68.79 , 102.40 , 1018838        ));
        vmc.push_back(mergedConfiguration("nCombsPerRoadMerged_x4_" , "PU200_sf1L0x2L5x2_nz6" , "Mean=", 3.33 , "Q[0.90,0.95,0.99]=" , 8.00 , 10.97 , 24.15 , 1018838    ));
        vmc.push_back(mergedConfiguration("nCombsPerBXMerged_x4_" , "PU200_sf1L0x2L5x2_nz6" , "Mean=", 84.73 , "Q[0.90,0.95,0.99]=" , 200.76 , 286.86 , 592.02 , 1018838 ));
        vmc.push_back(mergedConfiguration("nRoadsMerged_x4_" , "PU200_sf1L0x2L5x2_nz8" , "Mean=", 18.71 , "Q[0.90,0.95,0.99]=" , 44.48 , 55.80 , 85.00 , 1750212         ));
        vmc.push_back(mergedConfiguration("nCombsPerRoadMerged_x4_" , "PU200_sf1L0x2L5x2_nz8" , "Mean=", 2.93 , "Q[0.90,0.95,0.99]=" , 6.50 , 8.87 , 18.90 , 1750212     ));
        vmc.push_back(mergedConfiguration("nCombsPerBXMerged_x4_" , "PU200_sf1L0x2L5x2_nz8" , "Mean=", 56.93 , "Q[0.90,0.95,0.99]=" , 139.27 , 205.27 , 418.04 , 1750212 ));
        vmc.push_back(mergedConfiguration("nRoadsMerged_x4_" , "PU200_sf1L0x2_nz4" , "Mean=", 34.76 , "Q[0.90,0.95,0.99]=" , 74.60 , 94.75 , 146.67 , 623341             ));
        vmc.push_back(mergedConfiguration("nCombsPerRoadMerged_x4_" , "PU200_sf1L0x2_nz4" , "Mean=", 3.88 , "Q[0.90,0.95,0.99]=" , 8.72 , 12.69 , 24.81 , 623341         ));
        vmc.push_back(mergedConfiguration("nCombsPerBXMerged_x4_" , "PU200_sf1L0x2_nz4" , "Mean=", 143.42 , "Q[0.90,0.95,0.99]=" , 338.54 , 476.92 , 916.17 , 623341     ));
        vmc.push_back(mergedConfiguration("nRoadsMerged_x4_" , "PU200_sf1L0x2_nz6" , "Mean=", 22.00 , "Q[0.90,0.95,0.99]=" , 50.50 , 64.53 , 95.00 , 1337203             ));
        vmc.push_back(mergedConfiguration("nCombsPerRoadMerged_x4_" , "PU200_sf1L0x2_nz6" , "Mean=", 3.12 , "Q[0.90,0.95,0.99]=" , 6.78 , 9.52 , 20.36 , 1337203         ));
        vmc.push_back(mergedConfiguration("nCombsPerBXMerged_x4_" , "PU200_sf1L0x2_nz6" , "Mean=", 71.32 , "Q[0.90,0.95,0.99]=" , 172.51 , 243.01 , 474.02 , 1337203     ));
        vmc.push_back(mergedConfiguration("nRoadsMerged_x4_" , "PU200_sf1L0x2_nz8" , "Mean=", 17.32 , "Q[0.90,0.95,0.99]=" , 41.24 , 52.70 , 79.33 , 2309055             ));
        vmc.push_back(mergedConfiguration("nCombsPerRoadMerged_x4_" , "PU200_sf1L0x2_nz8" , "Mean=", 2.83 , "Q[0.90,0.95,0.99]=" , 6.32 , 8.71 , 18.41 , 2309055         ));
        vmc.push_back(mergedConfiguration("nCombsPerBXMerged_x4_" , "PU200_sf1L0x2_nz8" , "Mean=", 50.60 , "Q[0.90,0.95,0.99]=" , 121.88 , 177.12 , 368.02 , 2309055     ));
//        vmc.push_back(mergedConfiguration("nRoads" , "PU200_sf1L0x2L5x2__nz8_OR" , "Mean=", 21.51 , "Q[0.90,0.95,0.99]=" , 52.07 , 67.29 , 101.67 , 3671262                 ));
//        vmc.push_back(mergedConfiguration("nRoadsMerged_x4_" , "PU200_sf1L0x2L5x2__nz8_OR" , "Mean=", 15.96 , "Q[0.90,0.95,0.99]=" , 38.27 , 48.87 , 72.44 , 1706341        ));
//        vmc.push_back(mergedConfiguration("nCombsPerRoad" , "PU200_sf1L0x2L5x2__nz8_OR" , "Mean=", 2.41 , "Q[0.90,0.95,0.99]=" , 4.87 , 7.00 , 16.06 , 3671262              ));
//        vmc.push_back(mergedConfiguration("nCombsPerRoadMerged_x4_" , "PU200_sf1L0x2L5x2__nz8_OR" , "Mean=", 2.62 , "Q[0.90,0.95,0.99]=" , 5.72 , 8.40 , 16.83 , 1706341    ));
//        vmc.push_back(mergedConfiguration("nCombsPerBX" , "PU200_sf1L0x2L5x2__nz8_OR" , "Mean=", 52.95 , "Q[0.90,0.95,0.99]=" , 133.01 , 191.10 , 380.01 , 3671262          ));
//        vmc.push_back(mergedConfiguration("nCombsPerBXMerged_x4_" , "PU200_sf1L0x2L5x2__nz8_OR" , "Mean=", 43.13 , "Q[0.90,0.95,0.99]=" , 107.10 , 156.59 , 310.00 , 1706341));
//        vmc.push_back(mergedConfiguration("nRoadsMerged_x8_" , "PU200_sf1L0x2L5x2__nz8_OR" , "Mean=", 14.52 , "Q[0.90,0.95,0.99]=" , 34.93 , 44.36 , 65.00 , 1492254        ));
//        vmc.push_back(mergedConfiguration("nCombsPerRoadMerged_x8_" , "PU200_sf1L0x2L5x2__nz8_OR" , "Mean=", 2.69 , "Q[0.90,0.95,0.99]=" , 6.08 , 8.52 , 18.01 , 1492254    ));
//        vmc.push_back(mergedConfiguration("nCombsPerBXMerged_x8_" , "PU200_sf1L0x2L5x2__nz8_OR" , "Mean=", 40.38 , "Q[0.90,0.95,0.99]=" , 100.23 , 147.33 , 295.33 , 1492254));
        vmc.push_back(mergedConfiguration("nRoadsMerged_x8_" , "PU200_sf1L0x2L5x2_nz6" , "Mean=", 21.68 , "Q[0.90,0.95,0.99]=" , 49.02 , 62.71 , 91.67 , 880864             ));
        vmc.push_back(mergedConfiguration("nCombsPerRoadMerged_x8_" , "PU200_sf1L0x2L5x2_nz6" , "Mean=", 3.43 , "Q[0.90,0.95,0.99]=" , 8.16 , 12.16 , 24.33 , 880864        ));
        vmc.push_back(mergedConfiguration("nCombsPerBXMerged_x8_" , "PU200_sf1L0x2L5x2_nz6" , "Mean=", 80.12 , "Q[0.90,0.95,0.99]=" , 188.93 , 271.01 , 570.01 , 880864     ));
        vmc.push_back(mergedConfiguration("nRoadsMerged_x8_" , "PU200_sf1L0x2L5x2_nz8" , "Mean=", 16.92 , "Q[0.90,0.95,0.99]=" , 40.08 , 50.28 , 75.33 , 1510120            ));
        vmc.push_back(mergedConfiguration("nCombsPerRoadMerged_x8_" , "PU200_sf1L0x2L5x2_nz8" , "Mean=", 3.01 , "Q[0.90,0.95,0.99]=" , 6.61 , 8.99 , 20.33 , 1510120        ));
        vmc.push_back(mergedConfiguration("nCombsPerBXMerged_x8_" , "PU200_sf1L0x2L5x2_nz8" , "Mean=", 53.24 , "Q[0.90,0.95,0.99]=" , 128.85 , 189.01 , 391.01 , 1510120    ));
        vmc.push_back(mergedConfiguration("nRoadsMerged_x8_" , "PU200_sf1L0x2_nz6" , "Mean=", 19.94 , "Q[0.90,0.95,0.99]=" , 45.84 , 58.09 , 86.00 , 1157114                ));
        vmc.push_back(mergedConfiguration("nCombsPerRoadMerged_x8_" , "PU200_sf1L0x2_nz6" , "Mean=", 3.21 , "Q[0.90,0.95,0.99]=" , 6.90 , 10.19 , 20.83 , 1157114           ));
        vmc.push_back(mergedConfiguration("nCombsPerBXMerged_x8_" , "PU200_sf1L0x2_nz6" , "Mean=", 67.13 , "Q[0.90,0.95,0.99]=" , 161.44 , 230.40 , 446.00 , 1157114        ));
        vmc.push_back(mergedConfiguration("nRoadsMerged_x8_" , "PU200_sf1L0x2_nz8" , "Mean=", 15.62 , "Q[0.90,0.95,0.99]=" , 36.78 , 47.67 , 69.67 , 1999061                ));
        vmc.push_back(mergedConfiguration("nCombsPerRoadMerged_x8_" , "PU200_sf1L0x2_nz8" , "Mean=", 2.91 , "Q[0.90,0.95,0.99]=" , 6.49 , 8.84 , 18.85 , 1999061            ));
        vmc.push_back(mergedConfiguration("nCombsPerBXMerged_x8_" , "PU200_sf1L0x2_nz8" , "Mean=", 47.48 , "Q[0.90,0.95,0.99]=" , 114.67 , 165.00 , 346.00 , 1999061        ));
	}
	}
    if (pu==250) {
        vmc.push_back(mergedConfiguration("nRoads" , "PU250_sf1L0x2L5x2_nz4" , "Mean=", 117.60 , "Q[0.90,0.95,0.99]=" , 230.51 , 292.06 , 434.06 , 1001026                  ));
        vmc.push_back(mergedConfiguration("nCombsPerRoad" , "PU250_sf1L0x2L5x2_nz4" , "Mean=", 4.63 , "Q[0.90,0.95,0.99]=" , 10.65 , 16.34 , 32.81 , 1001026                ));
        vmc.push_back(mergedConfiguration("nCombsPerBX" , "PU250_sf1L0x2L5x2_nz4" , "Mean=", 576.90 , "Q[0.90,0.95,0.99]=" , 1286.33 , 1755.50 , 2840.30 , 1001026          ));
        vmc.push_back(mergedConfiguration("nRoads" , "PU250_sf1L0x2L5x2_nz6" , "Mean=", 63.31 , "Q[0.90,0.95,0.99]=" , 131.45 , 167.00 , 226.00 , 2180279                   ));
        vmc.push_back(mergedConfiguration("nCombsPerRoad" , "PU250_sf1L0x2L5x2_nz6" , "Mean=", 3.70 , "Q[0.90,0.95,0.99]=" , 8.43 , 12.46 , 24.89 , 2180279                 ));
        vmc.push_back(mergedConfiguration("nCombsPerBX" , "PU250_sf1L0x2L5x2_nz6" , "Mean=", 248.53 , "Q[0.90,0.95,0.99]=" , 552.00 , 756.00 , 1548.00 , 2180279            ));
        vmc.push_back(mergedConfiguration("nRoads" , "PU250_sf1L0x2L5x2_nz8" , "Mean=", 44.02 , "Q[0.90,0.95,0.99]=" , 95.78 , 118.00 , 180.00 , 3774867                    ));
        vmc.push_back(mergedConfiguration("nCombsPerRoad" , "PU250_sf1L0x2L5x2_nz8" , "Mean=", 3.22 , "Q[0.90,0.95,0.99]=" , 6.81 , 10.03 , 24.02 , 3774867                 ));
        vmc.push_back(mergedConfiguration("nCombsPerBX" , "PU250_sf1L0x2L5x2_nz8" , "Mean=", 147.29 , "Q[0.90,0.95,0.99]=" , 336.00 , 473.00 , 874.00 , 3774867             ));
        vmc.push_back(mergedConfiguration("nRoads" , "PU250_sf1L0x2_nz4" , "Mean=", 103.58 , "Q[0.90,0.95,0.99]=" , 205.49 , 254.55 , 380.02 , 1309491                      ));
        vmc.push_back(mergedConfiguration("nCombsPerRoad" , "PU250_sf1L0x2_nz4" , "Mean=", 4.20 , "Q[0.90,0.95,0.99]=" , 8.95 , 12.91 , 30.36 , 1309491                     ));
        vmc.push_back(mergedConfiguration("nCombsPerBX" , "PU250_sf1L0x2_nz4" , "Mean=", 458.36 , "Q[0.90,0.95,0.99]=" , 1023.50 , 1416.25 , 2326.10 , 1309491              ));
        vmc.push_back(mergedConfiguration("nRoads" , "PU250_sf1L0x2_nz6" , "Mean=", 56.66 , "Q[0.90,0.95,0.99]=" , 120.44 , 146.50 , 205.00 , 2851008                       ));
        vmc.push_back(mergedConfiguration("nCombsPerRoad" , "PU250_sf1L0x2_nz6" , "Mean=", 3.39 , "Q[0.90,0.95,0.99]=" , 7.85 , 12.01 , 24.35 , 2851008                     ));
        vmc.push_back(mergedConfiguration("nCombsPerBX" , "PU250_sf1L0x2_nz6" , "Mean=", 200.52 , "Q[0.90,0.95,0.99]=" , 450.00 , 596.00 , 1124.00 , 2851008                ));
        vmc.push_back(mergedConfiguration("nRoads" , "PU250_sf1L0x2_nz8" , "Mean=", 40.58 , "Q[0.90,0.95,0.99]=" , 89.23 , 110.75 , 164.00 , 4953044                        ));
        vmc.push_back(mergedConfiguration("nCombsPerRoad" , "PU250_sf1L0x2_nz8" , "Mean=", 3.00 , "Q[0.90,0.95,0.99]=" , 6.53 , 8.88 , 18.79 , 4953044                      ));
        vmc.push_back(mergedConfiguration("nCombsPerBX" , "PU250_sf1L0x2_nz8" , "Mean=", 123.12 , "Q[0.90,0.95,0.99]=" , 279.40 , 406.05 , 662.02 , 4953044                 ));
        vmc.push_back(mergedConfiguration("nRoadsMerged_x2_" , "PU250_sf1L0x2L5x2_nz4" , "Mean=", 99.41 , "Q[0.90,0.95,0.99]=" , 192.40 , 244.00 , 363.00 , 595679          ));
        vmc.push_back(mergedConfiguration("nCombsPerRoadMerged_x2_" , "PU250_sf1L0x2L5x2_nz4" , "Mean=", 4.99 , "Q[0.90,0.95,0.99]=" , 12.24 , 16.95 , 36.27 , 595679       ));
        vmc.push_back(mergedConfiguration("nCombsPerBXMerged_x2_" , "PU250_sf1L0x2L5x2_nz4" , "Mean=", 538.11 , "Q[0.90,0.95,0.99]=" , 1200.84 , 1659.10 , 2736.22 , 595679 ));
        vmc.push_back(mergedConfiguration("nRoadsMerged_x2_" , "PU250_sf1L0x2L5x2_nz6" , "Mean=", 53.44 , "Q[0.90,0.95,0.99]=" , 109.88 , 140.00 , 188.00 , 1298337         ));
        vmc.push_back(mergedConfiguration("nCombsPerRoadMerged_x2_" , "PU250_sf1L0x2L5x2_nz6" , "Mean=", 3.96 , "Q[0.90,0.95,0.99]=" , 8.73 , 12.75 , 28.58 , 1298337       ));
        vmc.push_back(mergedConfiguration("nCombsPerBXMerged_x2_" , "PU250_sf1L0x2L5x2_nz6" , "Mean=", 227.70 , "Q[0.90,0.95,0.99]=" , 503.00 , 684.00 , 1410.00 , 1298337  ));
        vmc.push_back(mergedConfiguration("nRoadsMerged_x2_" , "PU250_sf1L0x2L5x2_nz8" , "Mean=", 37.25 , "Q[0.90,0.95,0.99]=" , 80.67 , 97.80 , 151.20 , 2249397           ));
        vmc.push_back(mergedConfiguration("nCombsPerRoadMerged_x2_" , "PU250_sf1L0x2L5x2_nz8" , "Mean=", 3.42 , "Q[0.90,0.95,0.99]=" , 7.63 , 12.10 , 24.38 , 2249397       ));
        vmc.push_back(mergedConfiguration("nCombsPerBXMerged_x2_" , "PU250_sf1L0x2L5x2_nz8" , "Mean=", 133.55 , "Q[0.90,0.95,0.99]=" , 307.33 , 428.00 , 806.00 , 2249397   ));
        vmc.push_back(mergedConfiguration("nRoadsMerged_x2_" , "PU250_sf1L0x2_nz4" , "Mean=", 87.11 , "Q[0.90,0.95,0.99]=" , 170.86 , 213.50 , 321.00 , 776321              ));
        vmc.push_back(mergedConfiguration("nCombsPerRoadMerged_x2_" , "PU250_sf1L0x2_nz4" , "Mean=", 4.55 , "Q[0.90,0.95,0.99]=" , 10.27 , 16.13 , 32.39 , 776321           ));
        vmc.push_back(mergedConfiguration("nCombsPerBXMerged_x2_" , "PU250_sf1L0x2_nz4" , "Mean=", 423.80 , "Q[0.90,0.95,0.99]=" , 950.80 , 1308.40 , 2142.08 , 776321      ));
        vmc.push_back(mergedConfiguration("nRoadsMerged_x2_" , "PU250_sf1L0x2_nz6" , "Mean=", 47.73 , "Q[0.90,0.95,0.99]=" , 100.20 , 121.33 , 176.00 , 1693108             ));
        vmc.push_back(mergedConfiguration("nCombsPerRoadMerged_x2_" , "PU250_sf1L0x2_nz6" , "Mean=", 3.64 , "Q[0.90,0.95,0.99]=" , 8.34 , 12.35 , 24.75 , 1693108           ));
        vmc.push_back(mergedConfiguration("nCombsPerBXMerged_x2_" , "PU250_sf1L0x2_nz6" , "Mean=", 183.31 , "Q[0.90,0.95,0.99]=" , 408.00 , 554.00 , 1005.00 , 1693108      ));
        vmc.push_back(mergedConfiguration("nRoadsMerged_x2_" , "PU250_sf1L0x2_nz8" , "Mean=", 34.27 , "Q[0.90,0.95,0.99]=" , 75.08 , 90.91 , 138.00 , 2945176               ));
        vmc.push_back(mergedConfiguration("nCombsPerRoadMerged_x2_" , "PU250_sf1L0x2_nz8" , "Mean=", 3.19 , "Q[0.90,0.95,0.99]=" , 6.77 , 9.74 , 20.80 , 2945176            ));
        vmc.push_back(mergedConfiguration("nCombsPerBXMerged_x2_" , "PU250_sf1L0x2_nz8" , "Mean=", 111.33 , "Q[0.90,0.95,0.99]=" , 255.24 , 365.05 , 608.01 , 2945176       ));
        vmc.push_back(mergedConfiguration("nRoadsMerged_x4_" , "PU250_sf1L0x2L5x2_nz4" , "Mean=", 89.53 , "Q[0.90,0.95,0.99]=" , 174.40 , 219.50 , 316.00 , 477291          ));
        vmc.push_back(mergedConfiguration("nCombsPerRoadMerged_x4_" , "PU250_sf1L0x2L5x2_nz4" , "Mean=", 5.12 , "Q[0.90,0.95,0.99]=" , 12.36 , 18.21 , 36.46 , 477291       ));
        vmc.push_back(mergedConfiguration("nCombsPerBXMerged_x4_" , "PU250_sf1L0x2L5x2_nz4" , "Mean=", 506.12 , "Q[0.90,0.95,0.99]=" , 1133.80 , 1576.90 , 2664.18 , 477291 ));
        vmc.push_back(mergedConfiguration("nRoadsMerged_x4_" , "PU250_sf1L0x2L5x2_nz6" , "Mean=", 47.54 , "Q[0.90,0.95,0.99]=" , 96.29 , 125.33 , 170.00 , 1018838          ));
        vmc.push_back(mergedConfiguration("nCombsPerRoadMerged_x4_" , "PU250_sf1L0x2L5x2_nz6" , "Mean=", 4.09 , "Q[0.90,0.95,0.99]=" , 8.88 , 12.90 , 30.11 , 1018838       ));
        vmc.push_back(mergedConfiguration("nCombsPerBXMerged_x4_" , "PU250_sf1L0x2L5x2_nz6" , "Mean=", 210.37 , "Q[0.90,0.95,0.99]=" , 472.67 , 626.00 , 1266.00 , 1018838  ));
        vmc.push_back(mergedConfiguration("nRoadsMerged_x4_" , "PU250_sf1L0x2L5x2_nz8" , "Mean=", 32.95 , "Q[0.90,0.95,0.99]=" , 71.07 , 86.50 , 136.00 , 1750212           ));
        vmc.push_back(mergedConfiguration("nCombsPerRoadMerged_x4_" , "PU250_sf1L0x2L5x2_nz8" , "Mean=", 3.55 , "Q[0.90,0.95,0.99]=" , 8.17 , 12.29 , 24.63 , 1750212       ));
        vmc.push_back(mergedConfiguration("nCombsPerBXMerged_x4_" , "PU250_sf1L0x2L5x2_nz8" , "Mean=", 122.90 , "Q[0.90,0.95,0.99]=" , 278.40 , 398.00 , 702.00 , 1750212   ));
        vmc.push_back(mergedConfiguration("nRoadsMerged_x4_" , "PU250_sf1L0x2_nz4" , "Mean=", 78.50 , "Q[0.90,0.95,0.99]=" , 152.57 , 192.00 , 288.00 , 623341              ));
        vmc.push_back(mergedConfiguration("nCombsPerRoadMerged_x4_" , "PU250_sf1L0x2_nz4" , "Mean=", 4.68 , "Q[0.90,0.95,0.99]=" , 10.78 , 16.32 , 32.78 , 623341           ));
        vmc.push_back(mergedConfiguration("nCombsPerBXMerged_x4_" , "PU250_sf1L0x2_nz4" , "Mean=", 395.95 , "Q[0.90,0.95,0.99]=" , 879.40 , 1217.47 , 2068.08 , 623341      ));
        vmc.push_back(mergedConfiguration("nRoadsMerged_x4_" , "PU250_sf1L0x2_nz6" , "Mean=", 42.45 , "Q[0.90,0.95,0.99]=" , 88.67 , 109.40 , 154.00 , 1337203              ));
        vmc.push_back(mergedConfiguration("nCombsPerRoadMerged_x4_" , "PU250_sf1L0x2_nz6" , "Mean=", 3.76 , "Q[0.90,0.95,0.99]=" , 8.50 , 12.50 , 24.89 , 1337203           ));
        vmc.push_back(mergedConfiguration("nCombsPerBXMerged_x4_" , "PU250_sf1L0x2_nz6" , "Mean=", 169.23 , "Q[0.90,0.95,0.99]=" , 379.00 , 505.00 , 964.00 , 1337203       ));
        vmc.push_back(mergedConfiguration("nRoadsMerged_x4_" , "PU250_sf1L0x2_nz8" , "Mean=", 30.25 , "Q[0.90,0.95,0.99]=" , 65.18 , 79.57 , 120.00 , 2309055               ));
        vmc.push_back(mergedConfiguration("nCombsPerRoadMerged_x4_" , "PU250_sf1L0x2_nz8" , "Mean=", 3.30 , "Q[0.90,0.95,0.99]=" , 6.91 , 10.45 , 22.74 , 2309055           ));
        vmc.push_back(mergedConfiguration("nCombsPerBXMerged_x4_" , "PU250_sf1L0x2_nz8" , "Mean=", 103.56 , "Q[0.90,0.95,0.99]=" , 232.00 , 339.20 , 548.00 , 2309055       ));
//        vmc.push_back(mergedConfiguration("nRoads" , "PU250_sf1L0x2L5x2__nz8_OR" , "Mean=", 37.00 , "Q[0.90,0.95,0.99]=" , 79.82 , 100.92 , 153.00 , 3671262                ));
//        vmc.push_back(mergedConfiguration("nRoadsMerged_x4_" , "PU250_sf1L0x2L5x2__nz8_OR" , "Mean=", 27.78 , "Q[0.90,0.95,0.99]=" , 58.71 , 74.14 , 113.00 , 1706341       ));
//        vmc.push_back(mergedConfiguration("nCombsPerRoad" , "PU250_sf1L0x2L5x2__nz8_OR" , "Mean=", 2.92 , "Q[0.90,0.95,0.99]=" , 6.39 , 8.84 , 20.28 , 3671262              ));
//        vmc.push_back(mergedConfiguration("nCombsPerRoadMerged_x4_" , "PU250_sf1L0x2L5x2__nz8_OR" , "Mean=", 3.20 , "Q[0.90,0.95,0.99]=" , 6.79 , 10.33 , 24.10 , 1706341   ));
//        vmc.push_back(mergedConfiguration("nCombsPerBX" , "PU250_sf1L0x2L5x2__nz8_OR" , "Mean=", 110.39 , "Q[0.90,0.95,0.99]=" , 254.50 , 350.00 , 634.00 , 3671262         ));
//        vmc.push_back(mergedConfiguration("nCombsPerBXMerged_x4_" , "PU250_sf1L0x2L5x2__nz8_OR" , "Mean=", 91.53 , "Q[0.90,0.95,0.99]=" , 208.75 , 290.00 , 540.00 , 1706341));
        vmc.push_back(mergedConfiguration("nRoadsMerged_x8_" , "PU250_sf1L0x2L5x2_nz6" , "Mean=", 43.51 , "Q[0.90,0.95,0.99]=" , 87.64 , 112.67 , 154.00 , 880864           ));
        vmc.push_back(mergedConfiguration("nCombsPerRoadMerged_x8_" , "PU250_sf1L0x2L5x2_nz6" , "Mean=", 4.24 , "Q[0.90,0.95,0.99]=" , 9.08 , 14.14 , 30.82 , 880864        ));
        vmc.push_back(mergedConfiguration("nCombsPerBXMerged_x8_" , "PU250_sf1L0x2L5x2_nz6" , "Mean=", 201.32 , "Q[0.90,0.95,0.99]=" , 452.80 , 604.00 , 1211.00 , 880864   ));
        vmc.push_back(mergedConfiguration("nRoadsMerged_x8_" , "PU250_sf1L0x2L5x2_nz8" , "Mean=", 29.94 , "Q[0.90,0.95,0.99]=" , 63.90 , 77.87 , 120.00 , 1510120           ));
        vmc.push_back(mergedConfiguration("nCombsPerRoadMerged_x8_" , "PU250_sf1L0x2L5x2_nz8" , "Mean=", 3.67 , "Q[0.90,0.95,0.99]=" , 8.36 , 12.47 , 24.79 , 1510120       ));
        vmc.push_back(mergedConfiguration("nCombsPerBXMerged_x8_" , "PU250_sf1L0x2L5x2_nz8" , "Mean=", 116.44 , "Q[0.90,0.95,0.99]=" , 266.50 , 374.80 , 703.00 , 1510120   ));
        vmc.push_back(mergedConfiguration("nRoadsMerged_x8_" , "PU250_sf1L0x2_nz6" , "Mean=", 38.66 , "Q[0.90,0.95,0.99]=" , 79.89 , 98.86 , 142.00 , 1157114               ));
        vmc.push_back(mergedConfiguration("nCombsPerRoadMerged_x8_" , "PU250_sf1L0x2_nz6" , "Mean=", 3.90 , "Q[0.90,0.95,0.99]=" , 8.65 , 12.64 , 27.03 , 1157114           ));
        vmc.push_back(mergedConfiguration("nCombsPerBXMerged_x8_" , "PU250_sf1L0x2_nz6" , "Mean=", 160.62 , "Q[0.90,0.95,0.99]=" , 356.67 , 488.00 , 906.00 , 1157114       ));
        vmc.push_back(mergedConfiguration("nRoadsMerged_x8_" , "PU250_sf1L0x2_nz8" , "Mean=", 27.60 , "Q[0.90,0.95,0.99]=" , 59.44 , 71.90 , 110.00 , 1999061               ));
        vmc.push_back(mergedConfiguration("nCombsPerRoadMerged_x8_" , "PU250_sf1L0x2_nz8" , "Mean=", 3.40 , "Q[0.90,0.95,0.99]=" , 7.85 , 11.69 , 24.12 , 1999061           ));
        vmc.push_back(mergedConfiguration("nCombsPerBXMerged_x8_" , "PU250_sf1L0x2_nz8" , "Mean=", 97.99 , "Q[0.90,0.95,0.99]=" , 221.00 , 318.00 , 552.00 , 1999061        ));
    }

	TMultiGraph *mgRoads = new TMultiGraph();
	char hname [200];
	char htitle[200];
	sprintf(hname ,"patterns%dVSroads95",coverage);
	sprintf(htitle,"PU%d - Pattern(%dc) vs Roads 95%%",pu,coverage);
	mgRoads->SetNameTitle(hname,htitle);

	TMultiGraph *mgCombs = new TMultiGraph();
	sprintf(hname ,"patterns%dVScombs95",coverage);
	sprintf(htitle,"PU%d - Pattern(%dc) vs Combinations 95%%",pu,coverage);
//	mgCombs->SetNameTitle("patterns90VScombs95","PU200 - Pattern(90c) vs Combinations 95%");
	mgCombs->SetNameTitle(hname,htitle);

	TGraph* vgr[15][2];
	Double_t x[1], xx[1], y[1];
	for (unsigned issC=0; issC<vssC.size(); ++issC) {
		y[0] = vssC.at(issC).nPatterns_; x[0] = vssC.at(issC).roads95_; xx[0] = vssC.at(issC).combs95_;
		for (unsigned ii=0; ii<2; ++ii) {
			if (ii==0) vgr[issC][ii] = new TGraph(1,x ,y); //roads
			else       vgr[issC][ii] = new TGraph(1,xx,y); //combs
			vgr[issC][ii]->SetMarkerStyle(20+(issC)%nConfs);
			vgr[issC][ii]->SetMarkerColor(1+issC/nConfs);
			vgr[issC][ii]->SetFillStyle(0);
			vgr[issC][ii]->SetLineColor(0);
			vgr[issC][ii]->SetNameTitle(vssC.at(issC).name_,vssC.at(issC).name_);
			if (ii==0) mgRoads->Add(vgr[issC][ii]);
			else       mgCombs->Add(vgr[issC][ii]);
		}
	}
	TCanvas *cmgRoads = new TCanvas("cmgRoads","cmgRoads",0,0,800, 600);
	cmgRoads->SetGrid();
	mgRoads ->Draw("acp");
	mgRoads->GetXaxis()->SetTitle("# roads/BX");
	sprintf(hname ,"# patterns (%d%% cov)",coverage);
	mgRoads->GetYaxis()->SetTitle(hname);
	mgRoads ->Draw("acp");
	cmgRoads->BuildLegend(0.5,0.4,0.9,0.9);

	TCanvas *cmgCombs = new TCanvas("cmgCombs","cmgCombs",100,100,800, 600);
	cmgCombs->SetGrid();
	mgCombs ->Draw("acp");
	mgCombs->GetXaxis()->SetTitle("# combinations/BX");
	mgCombs->GetYaxis()->SetTitle(hname);
	mgCombs ->Draw("acp");
	TLegend* tl = cmgCombs->BuildLegend(0.45,0.4,0.9,0.9);

	TMultiGraph *mergedMultiGraph[3];
	TString varNames[3];
	for (unsigned ivar=0; ivar<3; ++ivar) mergedMultiGraph[ivar]= new TMultiGraph();
	const unsigned nconf = 12;
	TGraph* mergedGraphs[4][nconf][3];
//	bool firstDone = false;
	for (unsigned iv=0; iv<vmc.size(); ++iv) {
		unsigned imerge=0;
		if (vmc.at(iv).var_.Contains("Merged_x2")) imerge=1;
		if (vmc.at(iv).var_.Contains("Merged_x4")) imerge=2;
		if (vmc.at(iv).var_.Contains("Merged_x8")) imerge=3;
		unsigned ivar=0;
		if (vmc.at(iv).var_.BeginsWith("nCombsPerRoad")) ivar=1;
		if (vmc.at(iv).var_.BeginsWith("nCombsPerBX"))   ivar=2;
		if (imerge==0) varNames[ivar]=vmc.at(iv).var_+TString("_95perc");
		unsigned iconf=10;
		if (vmc.at(iv).conf_.Contains("sf1L0x2_nz4"))     iconf=0;
		if (vmc.at(iv).conf_.Contains("sf1L0x2_nz6"))     iconf=1;
		if (vmc.at(iv).conf_.Contains("sf1L0x2_nz8"))     iconf=2;
		if (vmc.at(iv).conf_.Contains("sf1L0x2L5x2_nz4")) iconf=3;
		if (vmc.at(iv).conf_.Contains("sf1L0x2L5x2_nz6")) iconf=4;
		if (vmc.at(iv).conf_.Contains("sf1L0x2L5x2_nz8")) iconf=5;
		if (vmc.at(iv).conf_.Contains(    "sf1L0x2_nz4_100c")) iconf=6;
		if (vmc.at(iv).conf_.Contains(    "sf1L0x2_nz6_100c")) iconf=7;
		if (vmc.at(iv).conf_.Contains(    "sf1L0x2_nz8_100c")) iconf=8;
		if (vmc.at(iv).conf_.Contains("sf1L0x2L5x2_nz4_100c")) iconf=9;
		if (vmc.at(iv).conf_.Contains("sf1L0x2L5x2_nz6_100c")) iconf=10;
		if (vmc.at(iv).conf_.Contains("sf1L0x2L5x2_nz8_100c")) iconf=11;
//        if (vmc.at(iv).conf_.Contains("sf1L0x2L5x2__nz8_OR")) iconf=6;
		TString m("");
		if (imerge==1) m=TString("_mx2");
		if (imerge==2) m=TString("_mx4");
		if (imerge==3) m=TString("_mx8");
		y[0] = vmc.at(iv).nPatterns_; x[0] = vmc.at(iv).var95_;
//		std::cout << imerge << " " << iconf << " " << ivar << std::endl;
		mergedGraphs[imerge][iconf][ivar] = new TGraph(1,x,y);
		mergedGraphs[imerge][iconf][ivar]->SetMarkerStyle(20+imerge);
		mergedGraphs[imerge][iconf][ivar]->SetMarkerColor(vcolors[iconf]);
		mergedGraphs[imerge][iconf][ivar]->SetFillStyle(0);
		mergedGraphs[imerge][iconf][ivar]->SetLineColor(0);
		mergedGraphs[imerge][iconf][ivar]->SetNameTitle(vmc.at(iv).conf_+m,vmc.at(iv).conf_+m);
//		            std::cout << imerge << "\t" << iconf << "\t" << ivar << std::endl;
        if (!imerge) {
//            std::cout << imerge << "\t" << iconf << "\t" << ivar << std::endl;
//            std::cout << TString("Patterns VS ")+vmc.at(iv).var_+TString(";")+vmc.at(iv).var_+TString(" 95%;")+TString("# patterns (95% cov)") << std::endl;
            mergedMultiGraph[ivar]->SetTitle(TString("Patterns VS ")+vmc.at(iv).var_+TString(";")+vmc.at(iv).var_+TString(" 95%;")+TString("# patterns (95% cov)"));
//            firstDone = true;
        }
	}
	for (unsigned ivar=0; ivar<3; ++ivar) {
	    for (unsigned iconf=0; iconf<nconf; ++iconf) {
	        for (unsigned imerge=0; imerge<4; ++imerge) {
//	            if (imerge==2 or imerge==1) continue;
	            char htitle[100];
//	            sprintf(htitle,"PU%d - Pattern(95%%c) vs ",pu);
//                std::cout << imerge << " " << iconf << " " << ivar << std::endl;
//	            if (imerge==3 and ((iconf==3) or (iconf==0))) continue;
	            if (isTTbar and (imerge==1 or imerge==2 or (imerge==0 and iconf<6) or (imerge==3 and (iconf<6 and iconf%3==0)))) continue;
	            std::cout << imerge << "\t" << iconf << "\t" << ivar << std::endl;
	            mergedMultiGraph[ivar]->Add(mergedGraphs[imerge][iconf][ivar]);
	        }
	    }
//        mergedMultiGraph[ivar]->Add(mergedGraphs[2][6][ivar]);
//        mergedMultiGraph[ivar]->Add(mergedGraphs[3][6][ivar]);
	}
//    return;
	TCanvas *cmerged[3];
	for (unsigned ivar=0; ivar<3; ++ivar) {
		char title[100];
		sprintf(title,"cmerged_%d",ivar);
		cmerged[ivar]= new TCanvas(title,title,200,200,800, 600);
	}
	for (unsigned ivar=0; ivar<3; ++ivar) {
		cmerged[ivar]->cd();
		gPad->SetGrid();
		mergedMultiGraph[ivar]->Draw("acp");
		TLegend* tl= gPad->BuildLegend(0.58,0.45,0.98,0.95);
		tl->SetFillColor(0);
	}
	if (savePlots) {
		TString outDir("/home/rossin/Dropbox/TT/Work/figures_SSoptimization/");
		TString ext[2] = {".pdf",".png"};
		TString sPU("_PU140");
        if (pu==200) sPU=TString("_PU200");
        if (pu==250) sPU=TString("_PU250");
        if (isTTbar) sPU=TString("_TTbar")+sPU;
		for (unsigned iext=0; iext<2; ++iext) {
			cmgRoads->                                           SaveAs(outDir+mgRoads->GetName()+sPU+ext[iext]);
			cmgCombs->                                           SaveAs(outDir+mgCombs->GetName()+sPU+ext[iext]);
			for (unsigned ivar=0; ivar<3; ++ivar) cmerged[ivar]->SaveAs(outDir+varNames[ivar]    +sPU+ext[iext]);
		}
	}
	return;
}
