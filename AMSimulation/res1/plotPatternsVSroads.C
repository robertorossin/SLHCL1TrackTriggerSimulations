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

	mergedConfiguration(
			TString var     ,
			TString conf    ,
			TString dummy   ,
			float   varMean ,
			TString dummy2  ,
			float   var90   ,
			float   var95   ,
			float   var99   ,
			unsigned nPatterns) :
				var_     (var    ),
				conf_    (conf   ),
				varMean_ (varMean),
				var90_   (var90  ),
				var95_   (var95  ),
				var99_   (var99  ),
				nPatterns_(nPatterns)
	{}
};


void plotPatternsVSroads (unsigned pu=140, bool savePlots = 0) {


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
//  PU140 - 95% coverage
	if (pu==140) {
		coverage = 95;
		vssC.push_back(ssConfiguration ("sf1_nz4     "       , 16.52 , 57.81    , 41.23 ,161.9 ,    1764800)); //14k
		vssC.push_back(ssConfiguration ("sf1_nz4_L5x2"       , 17.06 , 58.68    , 47.47 ,183.8 ,    1309600)); //14k
		vssC.push_back(ssConfiguration ("sf1_nz4_L5x2_L10x2" , 18.05 , 61.02    , 54.56 ,210.1 ,    1001100)); //14k
		vssC.push_back(ssConfiguration ("sf1p26_nz4"         , 18.98 , 64.8     , 55.58 ,214.8 ,    1053200)); //14k
		vssC.push_back(ssConfiguration ("sf1_nz6"	         , 13.88 , 50.48	, 28.91	, 114  ,    3905700)); //14k
		vssC.push_back(ssConfiguration ("sf1_nz6_L5x2"       , 13.79 , 49.56    , 31.45 , 123  ,    2851200)); //14k
		vssC.push_back(ssConfiguration ("sf1_nz6_L5x2_L10x2" , 14.25 , 50.58    , 34.84 , 134  ,    2180400)); //14k
		vssC.push_back(ssConfiguration ("sf1p26_nz6"         , 15.11 , 53.46    , 35.82 ,137.8 ,    2304700)); //14k
		vssC.push_back(ssConfiguration ("sf1_nz8"	         , 12.88 , 47.03    , 25.12 , 98.64,    6712700)); //14k
		vssC.push_back(ssConfiguration ("sf1_nz8_L5x2"       , 12.65 , 46.15    , 26.79 ,104.2 ,    4953500)); //14k
		vssC.push_back(ssConfiguration ("sf1_nz8_L5x2_L10x2" , 12.87 , 46.05    , 28.56 ,111.1 ,    3775200)); //14k
		vssC.push_back(ssConfiguration ("sf1p26_nz8"         , 13.52 , 48.63    , 29.12 ,114.2 ,    3990200)); //14k
	}

//  PU200 - 90% coverage
	if (pu==200) {
		coverage = 90;
		vssC.push_back(ssConfiguration ("sf1_nz4     "       ,  29.04, 84.25    , 95.49 ,324.2 ,    1184200));
		vssC.push_back(ssConfiguration ("sf1_nz4_L5x2"       ,  33.02, 91.53    , 120.1 ,399.1 ,     882300));
		vssC.push_back(ssConfiguration ("sf1_nz4_L5x2_L10x2" ,  35.54, 100      , 144.3 ,487.2 ,     675500));
		vssC.push_back(ssConfiguration ("sf1_nz6"	         ,  20.41, 62.8 	, 54.35	, 190  ,    2593800));
		vssC.push_back(ssConfiguration ("sf1_nz6_L5x2"       ,  21.8 , 66.68    , 62.86 ,215.8 ,    1915900));
		vssC.push_back(ssConfiguration ("sf1_nz6_L5x2_L10x2" ,  23.55, 69.87    , 73.91 ,254.5 ,    1469600));
		vssC.push_back(ssConfiguration ("sf1_nz8"	         ,  17.15, 54.78    , 41.74 ,  143 ,    4521400));
		vssC.push_back(ssConfiguration ("sf1_nz8_L5x2"       ,  17.64, 56       , 46.62 ,  163 ,    3374200));
		vssC.push_back(ssConfiguration ("sf1_nz8_L5x2_L10x2" ,  19.22, 59.58    , 52.48 ,185.7 ,    2586500));
		vssC.push_back(ssConfiguration ("sf1_nz8_L5x3_L10x2" ,   0.0 , 64       , 0.0   ,  215 ,    2314700));
		vssC.push_back(ssConfiguration ("sf1opt_nz8"	     ,  17.71, 55.47    , 44.94 ,159.4 ,    4625000));
	}

	unsigned merging=0;
	// grep Mean meanlog.txt | awk '{print "\"" $1"\" , \"" $2 "\" , \""$3 "\"" ", " $4 " , ""\"" $5 "\"", ", " $6 " , " $7 " , " $8, ",", $10}'
	std::vector<mergedConfiguration> vmc;
	if (pu==140) {
//		if (merging==2) {
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
//		}
//		if (merging==4) {
//			vmc.push_back(mergedConfiguration("nRoads" , "PU140_sf1L0x2L5x2_nz4" , "Mean=", 17.79 , "Q[0.90,0.95,0.99]=" , 45.30 , 60.31 , 96.78 , 1001026                  ));
//			vmc.push_back(mergedConfiguration("nRoadsMerged_x4_" , "PU140_sf1L0x2L5x2_nz4" , "Mean=", 13.35 , "Q[0.90,0.95,0.99]=" , 33.65 , 44.57 , 71.38 , 477291         ));
//			vmc.push_back(mergedConfiguration("nCombsPerRoad" , "PU140_sf1L0x2L5x2_nz4" , "Mean=", 2.84 , "Q[0.90,0.95,0.99]=" , 6.48 , 8.80 , 16.89 , 1001026              ));
//			vmc.push_back(mergedConfiguration("nCombsPerRoadMerged_x4_" , "PU140_sf1L0x2L5x2_nz4" , "Mean=", 3.07 , "Q[0.90,0.95,0.99]=" , 6.83 , 9.75 , 18.57 , 477291     ));
//			vmc.push_back(mergedConfiguration("nCombsPerBX" , "PU140_sf1L0x2L5x2_nz4" , "Mean=", 50.48 , "Q[0.90,0.95,0.99]=" , 132.84 , 201.31 , 379.82 , 1001026          ));
//			vmc.push_back(mergedConfiguration("nCombsPerBXMerged_x4_" , "PU140_sf1L0x2L5x2_nz4" , "Mean=", 42.32 , "Q[0.90,0.95,0.99]=" , 110.53 , 168.12 , 330.59 , 477291 ));
//			vmc.push_back(mergedConfiguration("nRoads" , "PU140_sf1L0x2L5x2_nz6" , "Mean=", 14.15 , "Q[0.90,0.95,0.99]=" , 37.72 , 50.35 , 78.90 , 2180279                  ));
//			vmc.push_back(mergedConfiguration("nRoadsMerged_x4_" , "PU140_sf1L0x2L5x2_nz6" , "Mean=", 10.46 , "Q[0.90,0.95,0.99]=" , 27.54 , 36.52 , 56.70 , 1018838        ));
//			vmc.push_back(mergedConfiguration("nCombsPerRoad" , "PU140_sf1L0x2L5x2_nz6" , "Mean=", 2.34 , "Q[0.90,0.95,0.99]=" , 4.82 , 6.85 , 12.90 , 2180279              ));
//			vmc.push_back(mergedConfiguration("nCombsPerRoadMerged_x4_" , "PU140_sf1L0x2L5x2_nz6" , "Mean=", 2.52 , "Q[0.90,0.95,0.99]=" , 5.00 , 8.18 , 15.75 , 1018838    ));
//			vmc.push_back(mergedConfiguration("nCombsPerBX" , "PU140_sf1L0x2L5x2_nz6" , "Mean=", 33.13 , "Q[0.90,0.95,0.99]=" , 88.30 , 130.72 , 264.64 , 2180279           ));
//			vmc.push_back(mergedConfiguration("nCombsPerBXMerged_x4_" , "PU140_sf1L0x2L5x2_nz6" , "Mean=", 26.83 , "Q[0.90,0.95,0.99]=" , 71.31 , 104.33 , 222.19 , 1018838 ));
//			vmc.push_back(mergedConfiguration("nRoads" , "PU140_sf1L0x2L5x2_nz8" , "Mean=", 12.85 , "Q[0.90,0.95,0.99]=" , 34.90 , 46.00 , 73.67 , 3774867                  ));
//			vmc.push_back(mergedConfiguration("nRoadsMerged_x4_" , "PU140_sf1L0x2L5x2_nz8" , "Mean=", 9.44 , "Q[0.90,0.95,0.99]=" , 25.51 , 33.47 , 53.33 , 1750212         ));
//			vmc.push_back(mergedConfiguration("nCombsPerRoad" , "PU140_sf1L0x2L5x2_nz8" , "Mean=", 2.16 , "Q[0.90,0.95,0.99]=" , 4.61 , 6.42 , 12.42 , 3774867              ));
//			vmc.push_back(mergedConfiguration("nCombsPerRoadMerged_x4_" , "PU140_sf1L0x2L5x2_nz8" , "Mean=", 2.30 , "Q[0.90,0.95,0.99]=" , 4.79 , 6.77 , 12.72 , 1750212    ));
//			vmc.push_back(mergedConfiguration("nCombsPerBX" , "PU140_sf1L0x2L5x2_nz8" , "Mean=", 27.81 , "Q[0.90,0.95,0.99]=" , 72.94 , 110.30 , 237.08 , 3774867           ));
//			vmc.push_back(mergedConfiguration("nCombsPerBXMerged_x4_" , "PU140_sf1L0x2L5x2_nz8" , "Mean=", 22.26 , "Q[0.90,0.95,0.99]=" , 58.26 , 87.34 , 196.03 , 1750212  ));
//			vmc.push_back(mergedConfiguration("nRoads" , "PU140_sf1L0x2_nz4" , "Mean=", 16.87 , "Q[0.90,0.95,0.99]=" , 43.04 , 58.13 , 93.44 , 1309491                      ));
//			vmc.push_back(mergedConfiguration("nRoadsMerged_x4_" , "PU140_sf1L0x2_nz4" , "Mean=", 12.54 , "Q[0.90,0.95,0.99]=" , 31.63 , 42.50 , 67.86 , 623341             ));
//			vmc.push_back(mergedConfiguration("nCombsPerRoad" , "PU140_sf1L0x2_nz4" , "Mean=", 2.65 , "Q[0.90,0.95,0.99]=" , 6.14 , 8.47 , 16.39 , 1309491                  ));
//			vmc.push_back(mergedConfiguration("nCombsPerRoadMerged_x4_" , "PU140_sf1L0x2_nz4" , "Mean=", 2.89 , "Q[0.90,0.95,0.99]=" , 6.56 , 8.82 , 16.91 , 623341         ));
//			vmc.push_back(mergedConfiguration("nCombsPerBX" , "PU140_sf1L0x2_nz4" , "Mean=", 44.76 , "Q[0.90,0.95,0.99]=" , 116.10 , 177.61 , 345.37 , 1309491              ));
//			vmc.push_back(mergedConfiguration("nCombsPerBXMerged_x4_" , "PU140_sf1L0x2_nz4" , "Mean=", 37.22 , "Q[0.90,0.95,0.99]=" , 95.42 , 146.55 , 290.13 , 623341      ));
//			vmc.push_back(mergedConfiguration("nRoads" , "PU140_sf1L0x2_nz6" , "Mean=", 13.70 , "Q[0.90,0.95,0.99]=" , 37.15 , 49.45 , 78.18 , 2851008                      ));
//			vmc.push_back(mergedConfiguration("nRoadsMerged_x4_" , "PU140_sf1L0x2_nz6" , "Mean=", 10.12 , "Q[0.90,0.95,0.99]=" , 27.19 , 35.62 , 56.00 , 1337203            ));
//			vmc.push_back(mergedConfiguration("nCombsPerRoad" , "PU140_sf1L0x2_nz6" , "Mean=", 2.23 , "Q[0.90,0.95,0.99]=" , 4.69 , 6.58 , 12.54 , 2851008                  ));
//			vmc.push_back(mergedConfiguration("nCombsPerRoadMerged_x4_" , "PU140_sf1L0x2_nz6" , "Mean=", 2.40 , "Q[0.90,0.95,0.99]=" , 4.88 , 6.93 , 12.91 , 1337203        ));
//			vmc.push_back(mergedConfiguration("nCombsPerBX" , "PU140_sf1L0x2_nz6" , "Mean=", 30.62 , "Q[0.90,0.95,0.99]=" , 82.21 , 121.35 , 250.15 , 2851008               ));
//			vmc.push_back(mergedConfiguration("nCombsPerBXMerged_x4_" , "PU140_sf1L0x2_nz6" , "Mean=", 24.66 , "Q[0.90,0.95,0.99]=" , 65.53 , 96.66 , 203.70 , 1337203      ));
//			vmc.push_back(mergedConfiguration("nRoads" , "PU140_sf1L0x2_nz8" , "Mean=", 12.62 , "Q[0.90,0.95,0.99]=" , 34.21 , 46.08 , 74.15 , 4953044                      ));
//			vmc.push_back(mergedConfiguration("nRoadsMerged_x4_" , "PU140_sf1L0x2_nz8" , "Mean=", 9.27 , "Q[0.90,0.95,0.99]=" , 24.95 , 33.38 , 52.73 , 2309055             ));
//			vmc.push_back(mergedConfiguration("nCombsPerRoad" , "PU140_sf1L0x2_nz8" , "Mean=", 2.07 , "Q[0.90,0.95,0.99]=" , 4.50 , 6.15 , 10.91 , 4953044                  ));
//			vmc.push_back(mergedConfiguration("nCombsPerRoadMerged_x4_" , "PU140_sf1L0x2_nz8" , "Mean=", 2.22 , "Q[0.90,0.95,0.99]=" , 4.70 , 6.58 , 12.42 , 2309055        ));
//			vmc.push_back(mergedConfiguration("nCombsPerBX" , "PU140_sf1L0x2_nz8" , "Mean=", 26.22 , "Q[0.90,0.95,0.99]=" , 68.65 , 103.57 , 219.08 , 4953044               ));
//			vmc.push_back(mergedConfiguration("nCombsPerBXMerged_x4_" , "PU140_sf1L0x2_nz8" , "Mean=", 20.78 , "Q[0.90,0.95,0.99]=" , 54.46 , 82.28 , 181.02 , 2309055      ));
//		}
	}
	if (pu==200) {
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

	TGraph* vgr[10][2];
	Double_t x[1], xx[1], y[1];
	for (unsigned issC=0; issC<vssC.size(); ++issC) {
		y[0] = vssC.at(issC).nPatterns_; x[0] = vssC.at(issC).roads95_; xx[0] = vssC.at(issC).combs95_;
		for (unsigned ii=0; ii<2; ++ii) {
			if (ii==0) vgr[issC][ii] = new TGraph(1,x ,y); //roads
			else       vgr[issC][ii] = new TGraph(1,xx,y); //combs
			vgr[issC][ii]->SetMarkerStyle(20+issC/4);
			vgr[issC][ii]->SetMarkerColor(1+(issC)%4);
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
	TLegend* tl = cmgCombs->BuildLegend(0.5,0.4,0.9,0.9);

	TMultiGraph *mergedMultiGraph[3];
	TString varNames[3];
	for (unsigned ivar=0; ivar<3; ++ivar) mergedMultiGraph[ivar]= new TMultiGraph();
	TGraph* mergedGraphs[3][6][3];
	for (unsigned iv=0; iv<vmc.size(); ++iv) {
		unsigned imerge=0;
		if (vmc.at(iv).var_.Contains("Merged_x2")) imerge=1;
		if (vmc.at(iv).var_.Contains("Merged_x4")) imerge=2;
		unsigned ivar=0;
		if (vmc.at(iv).var_.BeginsWith("nCombsPerRoad")) ivar=1;
		if (vmc.at(iv).var_.BeginsWith("nCombsPerBX"))   ivar=2;
		if (imerge==0) varNames[ivar]=vmc.at(iv).var_+TString("_95perc");
		unsigned iconf=0;
		if (vmc.at(iv).conf_.Contains("sf1L0x2_nz6"))     iconf=1;
		if (vmc.at(iv).conf_.Contains("sf1L0x2_nz8"))     iconf=2;
		if (vmc.at(iv).conf_.Contains("sf1L0x2L5x2_nz4")) iconf=3;
		if (vmc.at(iv).conf_.Contains("sf1L0x2L5x2_nz6")) iconf=4;
		if (vmc.at(iv).conf_.Contains("sf1L0x2L5x2_nz8")) iconf=5;
		TString m("");
		if (imerge==1) m=TString("_mx2");
		if (imerge==2) m=TString("_mx4");
		y[0] = vmc.at(iv).nPatterns_; x[0] = vmc.at(iv).var95_;
		mergedGraphs[imerge][iconf][ivar] = new TGraph(1,x,y);
		mergedGraphs[imerge][iconf][ivar]->SetMarkerStyle(20+imerge);
		mergedGraphs[imerge][iconf][ivar]->SetMarkerColor(1+iconf);
		mergedGraphs[imerge][iconf][ivar]->SetFillStyle(0);
		mergedGraphs[imerge][iconf][ivar]->SetLineColor(0);
		mergedGraphs[imerge][iconf][ivar]->SetNameTitle(vmc.at(iv).conf_+m,vmc.at(iv).conf_+m);
		mergedMultiGraph[ivar]->Add(mergedGraphs[imerge][iconf][ivar]);
		char htitle[100];
		sprintf(htitle,"PU%d - Pattern(95%%c) vs ",pu);
		if (iconf==0 && imerge==0) mergedMultiGraph[ivar]->SetTitle(TString(htitle)+vmc.at(iv).var_+TString(";")+vmc.at(iv).var_+TString(" 95%;")+TString("# patterns (95% cov)"));
	}
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
		gPad->BuildLegend(0.58,0.45,0.98,0.95);
	}


	if (savePlots) {
		TString outDir("/home/rossin/Dropbox/TT/Work/figures_SSoptimization/");
		TString ext[2] = {".pdf",".png"};
		TString sPU("_PU140");
		if (pu==200) sPU=TString("_PU200");
		for (unsigned iext=0; iext<2; ++iext) {
			cmgRoads->                                           SaveAs(outDir+mgRoads->GetName()+sPU+ext[iext]);
			cmgCombs->                                           SaveAs(outDir+mgCombs->GetName()+sPU+ext[iext]);
			for (unsigned ivar=0; ivar<3; ++ivar) cmerged[ivar]->SaveAs(outDir+varNames[ivar]    +sPU+ext[iext]);
		}
	}

	return;
}
