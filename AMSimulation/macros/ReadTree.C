#define ReadTree_cxx
#include "ReadTree.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <string>
#include <vector>
#include <TLegend.h>
#include <TLatex.h>

static const unsigned MAX_NTOWERS = 6 * 8;
bool requirePtCut3 = true;

unsigned int decodeLayer( unsigned int moduleId) {
	return moduleId / 10000;
}

unsigned int decodeLadder( unsigned int moduleId) {
	return (moduleId / 100) % 100;
}

unsigned int decodeModule( unsigned int moduleId) {
	return moduleId % 100;
}

int getTriggerTowerMap(TString src, std::map<unsigned, std::vector<unsigned> >& ttmap) {
	if (!src.EndsWith(".csv")) {
		std::cout << "Input filename must be .csv" << std::endl;
		return 1;
	}   
	std::vector<unsigned> values;
	unsigned j = 0;
	std::string line, issline;
	std::ifstream ifs(src.Data());
	while (std::getline(ifs, line)) {
		std::istringstream iss(line);
		while (std::getline(iss, issline, ',')) { // split by commas
			if (j == 0) continue; // skip the first line
			values.push_back(std::stoi(issline));
		}   
		++j;
	}   
	if (values.empty()) {
		std::cout << "Failed to read any trigger tower" << std::endl;
		return 1;
	}   
	ttmap.clear();
	unsigned ttId = 0;
	for (unsigned i=0; i<values.size(); ++i) {
		if (i == 0) continue; // skip the first index
		if (values.at(i-1) <= 6 && values.at(i) <= 8) { // eta_idx, phi_idx
			ttId = (values.at(i-1)-1) * 8 + (values.at(i)-1); // mapped to 0-47
			ttmap.insert(std::make_pair(ttId, std::vector<unsigned>()));
		} else if (values.at(i) > 10000) {
			ttmap.at(ttId).push_back(values.at(i));
		}
	}
	return 0;
}

void checkOverLap(unsigned TowerA_ID, unsigned TowerB_ID, std::map<unsigned, std::vector<unsigned> >& trackModuleIdMap_, std::map<unsigned, std::vector<unsigned> >& triggerTowerReverseMap_, TH1F* CheckOverLapArea) {
	for (auto it: trackModuleIdMap_) {
		//Module_perTrack->Fill(it.second.size());
		// it.first (key, here is the trackId); trackModuleIdMap_[it.first] (value, here is a vector of moduleIds) 
		bool InterestedArea = true;
		bool TowerAFlagOnly = false;
		bool TowerBFlagOnly = false;
		//loop on all the modules in a track, to check if the track stays in only one tower:
		for (unsigned module=0; module<trackModuleIdMap_[it.first].size(); module++) {
			bool TowerAFlag = false;
			bool TowerBFlag = false;
			//loop on all the towers relates to the module, to check if the module fire the interested tower:: 
			for (unsigned twr=0; twr<triggerTowerReverseMap_[trackModuleIdMap_[it.first].at(module)].size(); twr++) {
				if (triggerTowerReverseMap_[trackModuleIdMap_[it.first].at(module)].at(twr) == TowerA_ID) TowerAFlag = true;
				if (triggerTowerReverseMap_[trackModuleIdMap_[it.first].at(module)].at(twr) == TowerB_ID) TowerBFlag = true;
			}
			if (!TowerAFlag && !TowerBFlag) InterestedArea = false;
			if (TowerAFlag && !TowerBFlag) TowerAFlagOnly = true;
			if (TowerBFlag && !TowerAFlag) TowerBFlagOnly = true;
		}
		if (InterestedArea) {
			int flyToOtherTower = 0;
			if ( TowerAFlagOnly && TowerBFlagOnly) flyToOtherTower = 1;
			CheckOverLapArea->Fill(flyToOtherTower);
		}
	}
}

bool pass3GeVCut(unsigned moduleId, float bend) {
	bool passCut=false;
	unsigned int lay = decodeLayer(moduleId); //layer
	unsigned int lad = decodeLadder(moduleId); //phi module for barrel; r rings for endcaps
	switch (lay) {
		case 5: if (fabs(bend)<=1.5) {/*std:cout<<"bend="<<bend<<", "<<std::endl; */passCut=true; } break;
		case 6: if (fabs(bend)<=1.5) passCut=true;break;
		case 7: if (fabs(bend)<=2.0) passCut=true;break;
		case 8: if (fabs(bend)<=3.0) passCut=true;break;
		case 9: if (fabs(bend)<=3.5) passCut=true;break;
		case 10: if (fabs(bend)<=4.5) passCut=true;break;
		case 11: 
		case 18: 
			if (lad<=1 && (fabs(bend)<=1)) passCut=true;
			if (lad>1 && lad<=4 && (fabs(bend)<=1.5)) passCut=true;
			if (lad>4 && lad<=7 && (fabs(bend)<=2)) passCut=true;
			if (lad>7 && lad<=8 && (fabs(bend)<=2.5)) passCut=true;
			if (lad>8 && lad<=9 && (fabs(bend)<=3)) passCut=true;
			if (lad>9 && lad<=10 && (fabs(bend)<=2)) passCut=true;
			if (lad>10 && lad<=11 && (fabs(bend)<=2.5)) passCut=true;
			if (lad>11 && lad<=13 && (fabs(bend)<=3)) passCut=true;
			if (lad>13 && lad<=14 && (fabs(bend)<=3.5)) passCut=true;
			break;
		case 12: 
		case 19:
         if (lad<=1 && (fabs(bend)<=1)) passCut=true;
         if (lad>1 && lad<=5 && (fabs(bend)<=1.5)) passCut=true;
         if (lad>5 && lad<=8 && (fabs(bend)<=2)) passCut=true;
         if (lad>8 && lad<=9 && (fabs(bend)<=3)) passCut=true;
         if (lad>9 && lad<=11 && (fabs(bend)<=2)) passCut=true;
         if (lad>11 && lad<=12 && (fabs(bend)<=2.5)) passCut=true;
         if (lad>12 && lad<=14 && (fabs(bend)<=3)) passCut=true;
			break;	
		case 13: 
		case 20:
         if (lad<=2 && (fabs(bend)<=1)) passCut=true;
         if (lad>2 && lad<=6 && (fabs(bend)<=1.5)) passCut=true;
         if (lad>6 && lad<=8 && (fabs(bend)<=2)) passCut=true;
         if (lad>8 && lad<=9 && (fabs(bend)<=2.5)) passCut=true;
         if (lad>9 && lad<=10 && (fabs(bend)<=3)) passCut=true;
         if (lad>10 && lad<=12 && (fabs(bend)<=2)) passCut=true;
         if (lad>12 && lad<=13 && (fabs(bend)<=2.5)) passCut=true;
         if (lad>13 && lad<=14 && (fabs(bend)<=3)) passCut=true;
         break;
		case 14: 
		case 21:
         if (lad<=4 && (fabs(bend)<=1)) passCut=true;
         if (lad>4 && lad<=7 && (fabs(bend)<=1.5)) passCut=true;
         if (lad>7 && lad<=9 && (fabs(bend)<=2)) passCut=true;
         if (lad>9 && lad<=10 && (fabs(bend)<=2.5)) passCut=true;
         if (lad>10 && lad<=14 && (fabs(bend)<=2)) passCut=true;
         break;
		case 15: 
		case 22:
         if (lad<=6 && (fabs(bend)<=1)) passCut=true;
         if (lad>6 && lad<=8 && (fabs(bend)<=1.5)) passCut=true;
         if (lad>8 && lad<=10 && (fabs(bend)<=2)) passCut=true;
         if (lad>10 && lad<=11 && (fabs(bend)<=2.5)) passCut=true;
         if (lad>11 && lad<=14 && (fabs(bend)<=2)) passCut=true;
			break;
		default: break;
	}
	return passCut;
}


int readTriggerTowerFile(TString src, std::map<unsigned, std::vector<unsigned> >& triggerTowerMap_, std::map<unsigned, std::vector<unsigned> >& triggerTowerReverseMap_) {
	if (getTriggerTowerMap(src, triggerTowerMap_)) {
		std::cout << "Failed to parse the trigger tower csv file." << std::endl;
		return 1;
	}
	assert(triggerTowerMap_.size() == MAX_NTOWERS);
	// Reverse trigger tower map
	triggerTowerReverseMap_.clear();
	for (unsigned i=0; i<MAX_NTOWERS; i++) { // loop over input trigger towers
		const std::vector<unsigned>& moduleIds = triggerTowerMap_[i];
		std::cout << "Tower " << i << " has " << moduleIds.size() << " modules." << std::endl;
		for (auto it2: moduleIds) { // loop over the moduleIds in the tower
			triggerTowerReverseMap_[it2].push_back(i);
		}
	}
	//for (auto it: triggerTowerReverseMap_) std::cout << "Module " << it.first << " is in " << it.second.size() << " towers." << std::endl;
	return 0;
}

void ReadTree::Loop()
{
	if (fChain == 0) return;

	std::map<unsigned, std::vector<unsigned> > triggerTowerMap_; // key: towerId; value: moduleIds in the tower
	std::map<unsigned, std::vector<unsigned> > triggerTowerReverseMap_; // key: moduleId; value: towerIds containing the module
	readTriggerTowerFile("trigger_sector_map.csv",triggerTowerMap_,triggerTowerReverseMap_);
	if (triggerTowerMap_.empty() || triggerTowerReverseMap_.empty()) {
		std::cout << "Failed to load trigger tower maps" << std::endl;
		return;
	}   

	std::map<int, int> ring_nModule_Map_; // key: ring ID; value: # of phi modules in the ring
   ring_nModule_Map_[0]=20;
   ring_nModule_Map_[1]=24;
   ring_nModule_Map_[2]=28;
   ring_nModule_Map_[3]=28;
   ring_nModule_Map_[4]=32;
   ring_nModule_Map_[5]=36;
   ring_nModule_Map_[6]=36;
   ring_nModule_Map_[7]=40;
   ring_nModule_Map_[8]=40;
   ring_nModule_Map_[9]=52;
   ring_nModule_Map_[10]=56;
   ring_nModule_Map_[11]=64;
   ring_nModule_Map_[12]=68;
   ring_nModule_Map_[13]=76;
   ring_nModule_Map_[14]=80;

	TFile *newfile = new TFile("Histograms.root", "RECREATE");

	Long64_t nentries = fChain->GetEntriesFast();
	//Long64_t nentries = 1;
	Long64_t nbytes = 0, nb = 0;

	Char_t str[100];
	unsigned int moduleId, lay, lad, mod, localZ; 
	float stub_r, stub_phi, stub_z, stub_eta, stub_trigBend, stub_x, stub_y, localPhi;

   TH1F *Tower_perModule = new TH1F("Tower_perModule","Tower_perModule",10,0,10);
   Tower_perModule->GetXaxis()->SetTitle("# of connections");
   Tower_perModule->GetYaxis()->SetTitle("Modules");
   TH1F *Module_inEachTower = new TH1F("Module_inEachTower","Module_inEachTower",50,0,50);
   Module_inEachTower->GetXaxis()->SetTitle("Trigger Tower ID");
   Module_inEachTower->GetYaxis()->SetTitle("Modules");
   TH1F *Module_inEachTower_merge = new TH1F("Module_inEachTower_merge","Module_inEachTower_merge",50,0,50);
   Module_inEachTower_merge->GetXaxis()->SetTitle("Trigger Tower ID");
   Module_inEachTower_merge->GetYaxis()->SetTitle("Modules");

	TH1F *Module_perTrack = new TH1F("Module_perTrack","Module_perTrack",50,0,50);
	Module_perTrack->GetXaxis()->SetTitle("# of modules per track");
	Module_perTrack->GetYaxis()->SetTitle("Tracks");

	TH1F *nSTUB_perLayer = new TH1F("nSTUB_perLayer","nSTUB_perLayer",20,4,24);
	nSTUB_perLayer->GetXaxis()->SetTitle("Layer ID");
	nSTUB_perLayer->GetYaxis()->SetTitle("# of stubs per event");

	TH1F *nSTUB_perTower = new TH1F("nSTUB_perTower","nSTUB_perTower",50,0,50);
	nSTUB_perTower->GetXaxis()->SetTitle("Trigger Tower ID");
	nSTUB_perTower->GetYaxis()->SetTitle("# of stubs per event");

	TH1F *CheckOverLapArea_0_8 = new TH1F("CheckOverLapArea_0_8","CheckOverLapArea_0_8",5,0,5);
	CheckOverLapArea_0_8->GetXaxis()->SetTitle("0: stay in one tower; 1: fly to other tower");
	CheckOverLapArea_0_8->GetYaxis()->SetTitle("Tracks");
	TH1F *CheckOverLapArea_8_16 = new TH1F("CheckOverLapArea_8_16","CheckOverLapArea_8_16",5,0,5);
	CheckOverLapArea_8_16->GetXaxis()->SetTitle("0: stay in one tower; 1: fly to other tower");
	CheckOverLapArea_8_16->GetYaxis()->SetTitle("Tracks");
	TH1F *CheckOverLapArea_16_24 = new TH1F("CheckOverLapArea_16_24","CheckOverLapArea_16_24",5,0,5);
	CheckOverLapArea_16_24->GetXaxis()->SetTitle("0: stay in one tower; 1: fly to other tower");
	CheckOverLapArea_16_24->GetYaxis()->SetTitle("Tracks");
	TH1F *CheckOverLapArea_0_1 = new TH1F("CheckOverLapArea_0_1","CheckOverLapArea_0_1",5,0,5);
	CheckOverLapArea_0_1->GetXaxis()->SetTitle("0: stay in one tower; 1: fly to other tower");
	CheckOverLapArea_0_1->GetYaxis()->SetTitle("Tracks");
	TH1F *CheckOverLapArea_8_9 = new TH1F("CheckOverLapArea_8_9","CheckOverLapArea_8_9",5,0,5);
	CheckOverLapArea_8_9->GetXaxis()->SetTitle("0: stay in one tower; 1: fly to other tower");
	CheckOverLapArea_8_9->GetYaxis()->SetTitle("Tracks");
	TH1F *CheckOverLapArea_16_17 = new TH1F("CheckOverLapArea_16_17","CheckOverLapArea_16_17",5,0,5);
	CheckOverLapArea_16_17->GetXaxis()->SetTitle("0: stay in one tower; 1: fly to other tower");
	CheckOverLapArea_16_17->GetYaxis()->SetTitle("Tracks");


	TH1F *nSTUB_Layer5 = new TH1F("nSTUB_Layer5","nSTUB_Layer5",70,0,70);
	nSTUB_Layer5->GetXaxis()->SetTitle("module ID (z)");
	nSTUB_Layer5->GetYaxis()->SetTitle("# of stubs per event per module (average over #phi)");
	TH1F *nSTUB_Layer6 = new TH1F("nSTUB_Layer6","nSTUB_Layer6",70,0,70);
	TH1F *nSTUB_Layer7 = new TH1F("nSTUB_Layer7","nSTUB_Layer7",70,0,70);
	TH1F *nSTUB_Layer8 = new TH1F("nSTUB_Layer8","nSTUB_Layer8",70,0,70);
	TH1F *nSTUB_Layer9 = new TH1F("nSTUB_Layer9","nSTUB_Layer9",70,0,70);
	TH1F *nSTUB_Layer10 = new TH1F("nSTUB_Layer10","nSTUB_Layer10",70,0,70);

   TH1F *nSTUB_Layer5_centralZ = new TH1F("nSTUB_Layer5_centralZ","nSTUB_Layer5_centralZ",80,0,80);
   nSTUB_Layer5_centralZ->GetXaxis()->SetTitle("ladder ID (#phi)");
   nSTUB_Layer5_centralZ->GetYaxis()->SetTitle("# of stubs per event per module");
   TH1F *nSTUB_Layer6_centralZ = new TH1F("nSTUB_Layer6_centralZ","nSTUB_Layer6_centralZ",80,0,80);
   TH1F *nSTUB_Layer7_centralZ = new TH1F("nSTUB_Layer7_centralZ","nSTUB_Layer7_centralZ",80,0,80);
   TH1F *nSTUB_Layer8_centralZ = new TH1F("nSTUB_Layer8_centralZ","nSTUB_Layer8_centralZ",80,0,80);
   TH1F *nSTUB_Layer9_centralZ = new TH1F("nSTUB_Layer9_centralZ","nSTUB_Layer9_centralZ",80,0,80);
   TH1F *nSTUB_Layer10_centralZ = new TH1F("nSTUB_Layer10_centralZ","nSTUB_Layer10_centralZ",80,0,80);

	TH1F *nSTUB_Layer11 = new TH1F("nSTUB_Layer11","nSTUB_Layer11",20,0,20);
   nSTUB_Layer11->GetXaxis()->SetTitle("ladder ID (r)");
   nSTUB_Layer11->GetYaxis()->SetTitle("# of stubs per event per module (average over #phi)");
	TH1F *nSTUB_Layer12 = new TH1F("nSTUB_Layer12","nSTUB_Layer12",20,0,20);
	TH1F *nSTUB_Layer13 = new TH1F("nSTUB_Layer13","nSTUB_Layer13",20,0,20);
	TH1F *nSTUB_Layer14 = new TH1F("nSTUB_Layer14","nSTUB_Layer14",20,0,20);
	TH1F *nSTUB_Layer15 = new TH1F("nSTUB_Layer15","nSTUB_Layer15",20,0,20);

	TH1F *nSTUB_ring0 = new TH1F("nSTUB_ring0","nSTUB_ring0",100,0,100);
   nSTUB_ring0->GetXaxis()->SetTitle("module ID (#phi)");
	nSTUB_ring0->GetYaxis()->SetTitle("# of stubs per event per module");
	TH1F *nSTUB_ring1 = new TH1F("nSTUB_ring1","nSTUB_ring1",100,0,100);
	TH1F *nSTUB_ring2 = new TH1F("nSTUB_ring2","nSTUB_ring2",100,0,100);
	TH1F *nSTUB_ring3 = new TH1F("nSTUB_ring3","nSTUB_ring3",100,0,100);
	TH1F *nSTUB_ring4 = new TH1F("nSTUB_ring4","nSTUB_ring4",100,0,100);
	TH1F *nSTUB_ring5 = new TH1F("nSTUB_ring5","nSTUB_ring5",100,0,100);
	TH1F *nSTUB_ring6 = new TH1F("nSTUB_ring6","nSTUB_ring6",100,0,100);
	TH1F *nSTUB_ring7 = new TH1F("nSTUB_ring7","nSTUB_ring7",100,0,100);
	TH1F *nSTUB_ring8 = new TH1F("nSTUB_ring8","nSTUB_ring8",100,0,100);
	TH1F *nSTUB_ring9 = new TH1F("nSTUB_ring9","nSTUB_ring9",100,0,100);
	TH1F *nSTUB_ring10 = new TH1F("nSTUB_ring10","nSTUB_ring10",100,0,100);
	TH1F *nSTUB_ring11 = new TH1F("nSTUB_ring11","nSTUB_ring11",100,0,100);
	TH1F *nSTUB_ring12 = new TH1F("nSTUB_ring12","nSTUB_ring12",100,0,100);
	TH1F *nSTUB_ring13 = new TH1F("nSTUB_ring13","nSTUB_ring13",100,0,100);
	TH1F *nSTUB_ring14 = new TH1F("nSTUB_ring14","nSTUB_ring14",100,0,100);

	TH2F *STUB_z_r = new TH2F("STUB_z_r","STUB_z_r",600,-300,300,120,0,120);
	STUB_z_r->GetXaxis()->SetTitle("STUB_z [cm]");
	STUB_z_r->GetYaxis()->SetTitle("STUB_r [cm]");
	TH2F *STUB_z_r_overlap = new TH2F("STUB_z_r_overlap","STUB_z_r_overlap",600,-300.,300,120,0,120);
   TH2F *STUB_x_y_barrel = new TH2F("STUB_x_y_barrel","STUB_x_y_barrel",240,-120,120,240,-120,120);
   STUB_x_y_barrel->GetXaxis()->SetTitle("STUB_x [cm]");
   STUB_x_y_barrel->GetYaxis()->SetTitle("STUB_y [cm]");
   TH2F *STUB_x_y_endcap = new TH2F("STUB_x_y_endcap","STUB_x_y_endcap",240,-120,120,240,-120,120);
   STUB_x_y_endcap->GetXaxis()->SetTitle("STUB_x [cm]");
   STUB_x_y_endcap->GetYaxis()->SetTitle("STUB_y [cm]");   
	//TH2F *STUB_x_y_overlap = new TH2F("STUB_x_y_overlap","STUB_x_y_overlap",240,-120,120,240,-120,120);
   TH2F *STUB_phi_mod = new TH2F("STUB_phi_mod","STUB_phi_mod",100,0,100,100,-3.14,3.14);
   STUB_phi_mod->GetXaxis()->SetTitle("STUB_mod");
   STUB_phi_mod->GetYaxis()->SetTitle("STUB_phi");


	TH2F *occupancyTower16Layer5 = new TH2F("occupancyTower16Layer5", "occupancyTower16Layer5", 10,24,34,10,10,20);
	occupancyTower16Layer5->GetXaxis()->SetTitle("module ID (z)");
	occupancyTower16Layer5->GetYaxis()->SetTitle("ladder ID (#phi)");
	TH2F *occupancyTower16Layer6 = new TH2F("occupancyTower16Layer6", "occupancyTower16Layer6", 10,19,29,10,15,25);
	occupancyTower16Layer6->GetXaxis()->SetTitle("module ID (z)");
	occupancyTower16Layer6->GetYaxis()->SetTitle("ladder ID (#phi)");
	TH2F *occupancyTower16Layer7 = new TH2F("occupancyTower16Layer7", "occupancyTower16Layer7", 15,15,30,15,20,35);
	occupancyTower16Layer7->GetXaxis()->SetTitle("module ID (z)");
	occupancyTower16Layer7->GetYaxis()->SetTitle("ladder ID (#phi)");
	TH2F *occupancyTower16Layer8 = new TH2F("occupancyTower16Layer8", "occupancyTower16Layer8", 10,4,14,10,35,45);
	occupancyTower16Layer8->GetXaxis()->SetTitle("module ID (z)");
	occupancyTower16Layer8->GetYaxis()->SetTitle("ladder ID (#phi)");
	TH2F *occupancyTower16Layer9 = new TH2F("occupancyTower16Layer9", "occupancyTower16Layer9", 10,4,14,10,46,56);
	occupancyTower16Layer9->GetXaxis()->SetTitle("module ID (z)");
	occupancyTower16Layer9->GetYaxis()->SetTitle("ladder ID (#phi)");
	TH2F *occupancyTower16Layer10 = new TH2F("occupancyTower16Layer10", "occupancyTower16Layer10", 15,0,15,15,55,70);
	occupancyTower16Layer10->GetXaxis()->SetTitle("module ID (z)");
	occupancyTower16Layer10->GetYaxis()->SetTitle("ladder ID (#phi)");

   TH2F *occupancyTower27Layer5 = new TH2F("occupancyTower27Layer5", "occupancyTower27Layer5", 10,29,39,10,0,10);
   occupancyTower27Layer5->GetXaxis()->SetTitle("module ID (z)");
   occupancyTower27Layer5->GetYaxis()->SetTitle("ladder ID (#phi)");
   TH2F *occupancyTower27Layer6 = new TH2F("occupancyTower27Layer6", "occupancyTower27Layer6", 10,26,36,10,0,10);
   occupancyTower27Layer6->GetXaxis()->SetTitle("module ID (z)");
   occupancyTower27Layer6->GetYaxis()->SetTitle("ladder ID (#phi)");
   TH2F *occupancyTower27Layer7 = new TH2F("occupancyTower27Layer7", "occupancyTower27Layer7", 15,25,40,15,0,15);
   occupancyTower27Layer7->GetXaxis()->SetTitle("module ID (z)");
   occupancyTower27Layer7->GetYaxis()->SetTitle("ladder ID (#phi)");
   TH2F *occupancyTower27Layer8 = new TH2F("occupancyTower27Layer8", "occupancyTower27Layer8", 10,10,20,10,5,15);
   occupancyTower27Layer8->GetXaxis()->SetTitle("module ID (z)");
   occupancyTower27Layer8->GetYaxis()->SetTitle("ladder ID (#phi)");
   TH2F *occupancyTower27Layer9 = new TH2F("occupancyTower27Layer9", "occupancyTower27Layer9", 10,10,20,10,7,17);
   occupancyTower27Layer9->GetXaxis()->SetTitle("module ID (z)");
   occupancyTower27Layer9->GetYaxis()->SetTitle("ladder ID (#phi)");
   TH2F *occupancyTower27Layer10 = new TH2F("occupancyTower27Layer10", "occupancyTower27Layer10", 15,10,25,15,8,23);
   occupancyTower27Layer10->GetXaxis()->SetTitle("module ID (z)");
   occupancyTower27Layer10->GetYaxis()->SetTitle("ladder ID (#phi)");

	TH1F *nStubRoads = new TH1F("nStubRoads","nStubRoads",100,0,100);
	nStubRoads->GetXaxis()->SetTitle("# of stubs per road");
	nStubRoads->GetYaxis()->SetTitle("Entries");

   std::vector<TH1F*> nStubTowerN;
	for (int t=0;t<48;t++){
		sprintf(str,"nStubTower%02d",t);
		nStubTowerN.push_back(new TH1F(str, str, 800, 0, 800));
		nStubTowerN[t]->GetXaxis()->SetTitle("# of stubs per tower");
		nStubTowerN[t]->GetYaxis()->SetTitle("Entries");
	}

	std::vector< std::vector<TH1F*> > nStubTowerNLayerN;
	for (int t=0;t<48;t++) { 
		std::vector<TH1F*> nStubLayerN;
		for (int l=0;l<24;l++) {
			sprintf(str,"nStubTower%02dLayer%02d",t,l);
			nStubLayerN.push_back(new TH1F(str, str, 200, 0, 200));
			nStubLayerN[l]->GetXaxis()->SetTitle("# of stubs per tower per layer");
			nStubLayerN[l]->GetYaxis()->SetTitle("Entries");
		}
		nStubTowerNLayerN.push_back(nStubLayerN);
	}
/*
	std::vector< std::vector<TH1F*> > nStubTower16Layer5ladNmodN;
	for (int l=0;l<76;l++) {
		std::vector<TH1F*> nStubTower16Layer5modN;
		for (int m=0;m<82;m++) {
			sprintf(str,"nStubTower16Layer5lad%02dmod%02d",l,m);
			nStubTower16Layer5modN.push_back(new TH1F(str, str, 30, 0, 30));
			nStubTower16Layer5modN[m]->GetXaxis()->SetTitle("# of stubs per mudole");
			nStubTower16Layer5modN[m]->GetYaxis()->SetTitle("Entries");
		}
		nStubTower16Layer5ladNmodN.push_back(nStubTower16Layer5modN);
	}
*/
   std::vector< std::vector< std::vector<TH1F*> > > nStubTower16LayerNladNmodN;
   for (int i=0;i<25;i++) {
      std::vector< std::vector<TH1F*> > nStubTower16ladNmodN;
      for (int l=0;l<76;l++) {
         std::vector<TH1F*> nStubTower16modN;
         for (int m=0;m<82;m++) {
            sprintf(str,"nStubTower16Layer%02dlad%02dmod%02d",i,l,m);
            nStubTower16modN.push_back(new TH1F(str, str, 100, 0, 100));
            nStubTower16modN[m]->GetXaxis()->SetTitle("# of stubs per mudole");
            nStubTower16modN[m]->GetYaxis()->SetTitle("Entries");
         }    
         nStubTower16ladNmodN.push_back(nStubTower16modN);
      }    
      nStubTower16LayerNladNmodN.push_back(nStubTower16ladNmodN);
   }   

   std::vector< std::vector< std::vector<TH1F*> > > nStubTower27LayerNladNmodN;
   for (int i=0;i<25;i++) {
		std::vector< std::vector<TH1F*> > nStubTower27ladNmodN;
		for (int l=0;l<76;l++) {
      	std::vector<TH1F*> nStubTower27modN;
      	for (int m=0;m<82;m++) {
         	sprintf(str,"nStubTower27Layer%02dlad%02dmod%02d",i,l,m);
         	nStubTower27modN.push_back(new TH1F(str, str, 20, 0, 20));
         	nStubTower27modN[m]->GetXaxis()->SetTitle("# of stubs per mudole");
         	nStubTower27modN[m]->GetYaxis()->SetTitle("Entries");
      	}   
      	nStubTower27ladNmodN.push_back(nStubTower27modN);
   	}
		nStubTower27LayerNladNmodN.push_back(nStubTower27ladNmodN);
	}


	for (Long64_t jentry=0; jentry<nentries;jentry++) 
	{
		Long64_t ientry = LoadTree(jentry);
		if (ientry < 0) break;
		nb = fChain->GetEntry(jentry);   nbytes += nb;
		// if (Cut(ientry) < 0) continue;
		const unsigned nstubs = TTStubs_modId->size();
		if (jentry%100==0) std::cout<<"Processing event #"<<jentry<<", nstubs="<<nstubs<<std::endl;
		if (!nstubs) { // skip if no stub
			continue;
		}
		if (nstubs > 100000) {
			std::cout << "Way too many stubs: " << nstubs << std::endl;
			return;
		}
/*
		std::map<unsigned, std::vector<unsigned> > trackModuleIdMap_; // key: trackId, value: moduleIds in the track
		for (unsigned l=0; l<nstubs; ++l) {
			if (requirePtCut3 && !pass3GeVCut(TTStubs_modId->at(l),TTStubs_trigBend->at(l))) continue;
			if (TTStubs_tpId->at(l) != -1 && trkParts_primary->at(TTStubs_tpId->at(l)) && genParts_pt->at(TTStubs_tpId->at(l))>3 ) {
				trackModuleIdMap_.insert(std::make_pair(TTStubs_tpId->at(l), std::vector<unsigned>()));
			}   
		}   
		for (unsigned l=0; l<nstubs; ++l) {
			if (requirePtCut3 && !pass3GeVCut(TTStubs_modId->at(l),TTStubs_trigBend->at(l))) continue;
			if (TTStubs_tpId->at(l) != -1 && trkParts_primary->at(TTStubs_tpId->at(l)) && genParts_pt->at(TTStubs_tpId->at(l))>3 ) {
				trackModuleIdMap_.at(TTStubs_tpId->at(l)).push_back(TTStubs_modId->at(l));
			}
		}
		for (auto it: trackModuleIdMap_) {
			Module_perTrack->Fill(it.second.size());
		}
		checkOverLap(0,8,trackModuleIdMap_,triggerTowerReverseMap_,CheckOverLapArea_0_8);
		checkOverLap(8,16,trackModuleIdMap_,triggerTowerReverseMap_,CheckOverLapArea_8_16);
		checkOverLap(16,24,trackModuleIdMap_,triggerTowerReverseMap_,CheckOverLapArea_16_24);
		checkOverLap(0,1,trackModuleIdMap_,triggerTowerReverseMap_,CheckOverLapArea_0_1);
		checkOverLap(8,9,trackModuleIdMap_,triggerTowerReverseMap_,CheckOverLapArea_8_9);
		checkOverLap(16,17,trackModuleIdMap_,triggerTowerReverseMap_,CheckOverLapArea_16_17);
		trackModuleIdMap_.clear();
*/
		int stub_ttN[48]={0};
		int stub_ttN_lN[48][24]={{0}};
		//int stub_tt16_l5_ladN_modN[76][82]={{0}};
		int stub_tt16_lN_ladN_modN[25][76][82]={{{0}}};
		int stub_tt27_lN_ladN_modN[25][76][82]={{{0}}};
		int stub_roads=0;

		for (unsigned l=0; l<nstubs; ++l) {   
			//module ID
			moduleId = TTStubs_modId->at(l);
			lay      = decodeLayer(moduleId); //layer
			lad      = decodeLadder(moduleId); //phi module for barrel; r rings for endcaps 
			mod      = decodeModule(moduleId); //z module for barrel; phi module for endcaps 
			//local coordinates
			localPhi = TTStubs_coordx->at(l);
			localZ   = TTStubs_coordy->at(l); 
			//std::cout<<moduleId<<" "<<lay<<" "<<lad<<" "<<mod<<std::endl;
			//global coordinates
			stub_r   = TTStubs_r->at(l);
			stub_phi = TTStubs_phi->at(l);
			stub_z   = TTStubs_z->at(l);
			stub_eta = TTStubs_eta->at(l);
			stub_trigBend = TTStubs_trigBend->at(l);
			stub_x   = TTStubs_x->at(l);
			stub_y   = TTStubs_y->at(l);
			std::cout<<"localPhi="<<localPhi<<"*2="<<int(localPhi*2)<<", localZ="<<localZ<<", stub_trigBend="<<stub_trigBend<<std::endl;
         if (triggerTowerReverseMap_[moduleId].size()==0) continue;
			if (requirePtCut3 && !pass3GeVCut(moduleId,stub_trigBend)) continue;

			nSTUB_perLayer->Fill(lay,1./nentries);

			if (stub_z>250) STUB_phi_mod->Fill(mod,stub_phi);
			if (fabs(stub_z)<120) STUB_x_y_barrel->Fill(stub_x,stub_y);
			if (stub_z>120 && stub_z<140) STUB_x_y_endcap->Fill(stub_x,stub_y);
			STUB_z_r->Fill(stub_z,stub_r);
			std::vector<unsigned> overlap0_8; overlap0_8.push_back(0); overlap0_8.push_back(8);
			std::vector<unsigned> overlap8_16; overlap8_16.push_back(8); overlap8_16.push_back(16);
			std::vector<unsigned> overlap16_24; overlap16_24.push_back(16); overlap16_24.push_back(24);
			std::vector<unsigned> overlap24_32; overlap24_32.push_back(24); overlap24_32.push_back(32);
			std::vector<unsigned> overlap32_40; overlap32_40.push_back(32); overlap32_40.push_back(40);
			if (triggerTowerReverseMap_[moduleId] == overlap0_8
					|| triggerTowerReverseMap_[moduleId] == overlap8_16 
					|| triggerTowerReverseMap_[moduleId] == overlap16_24
					|| triggerTowerReverseMap_[moduleId] == overlap24_32
					|| triggerTowerReverseMap_[moduleId] == overlap32_40)
				STUB_z_r_overlap->Fill(stub_z,stub_r);

			switch (lay) {
				//barrel
				case 5: 
					nSTUB_Layer5->Fill(mod,1./nentries/16);
					if (mod==31) nSTUB_Layer5_centralZ->Fill(lad,1./nentries);
					break;
				case 6: 
					nSTUB_Layer6->Fill(mod,1./nentries/24);
					if (mod==27) nSTUB_Layer6_centralZ->Fill(lad,1./nentries);
					break;
				case 7: 
					nSTUB_Layer7->Fill(mod,1./nentries/34);
					if (mod==26) nSTUB_Layer7_centralZ->Fill(lad,1./nentries);
					break;
				case 8: 
					nSTUB_Layer8->Fill(mod,1./nentries/48);
					if (mod==12) nSTUB_Layer8_centralZ->Fill(lad,1./nentries);
					break;
				case 9: 
					nSTUB_Layer9->Fill(mod,1./nentries/62);
					if (mod==12) nSTUB_Layer9_centralZ->Fill(lad,1./nentries);
					break;
				case 10: 
					nSTUB_Layer10->Fill(mod,1./nentries/76);
					if (mod==12) nSTUB_Layer10_centralZ->Fill(lad,1./nentries);
					break;
				
				//endcap
				case 11: 
					nSTUB_Layer11->Fill(lad,1./nentries/ring_nModule_Map_[lad]);
					switch(lad) {
						case 0: nSTUB_ring0->Fill(mod,1./nentries);break;
						case 1: nSTUB_ring1->Fill(mod,1./nentries);break;
						case 2: nSTUB_ring2->Fill(mod,1./nentries);break;
						case 3: nSTUB_ring3->Fill(mod,1./nentries);break;
						case 4: nSTUB_ring4->Fill(mod,1./nentries);break;
						case 5: nSTUB_ring5->Fill(mod,1./nentries);break;
						case 6: nSTUB_ring6->Fill(mod,1./nentries);break;
						case 7: nSTUB_ring7->Fill(mod,1./nentries);break;
						case 8: nSTUB_ring8->Fill(mod,1./nentries);break;
						case 9: nSTUB_ring9->Fill(mod,1./nentries);break;
						case 10: nSTUB_ring10->Fill(mod,1./nentries);break;
						case 11: nSTUB_ring11->Fill(mod,1./nentries);break;
						case 12: nSTUB_ring12->Fill(mod,1./nentries);break;
						case 13: nSTUB_ring13->Fill(mod,1./nentries);break;
						case 14: nSTUB_ring14->Fill(mod,1./nentries);break;
						default:break;
					}
					break;
				case 12: nSTUB_Layer12->Fill(lad,1./nentries/ring_nModule_Map_[lad]);break;
				case 13: nSTUB_Layer13->Fill(lad,1./nentries/ring_nModule_Map_[lad]);break;
				case 14: nSTUB_Layer14->Fill(lad,1./nentries/ring_nModule_Map_[lad]);break;
				case 15: nSTUB_Layer15->Fill(lad,1./nentries/ring_nModule_Map_[lad]);break;
				
				default: break;
			}

			bool inTowerN[48] = {false}; //Endcap: 0-7,40,47; Hybird: 8-15,32-39; Barrel: 16-23,24-31; 
			for (unsigned j=0; j<triggerTowerReverseMap_[moduleId].size(); j++) {
				int getTower = triggerTowerReverseMap_[moduleId].at(j);
				nSTUB_perTower->Fill(getTower,1./nentries);
				for (int i=0; i<48; i++) {
					if (getTower == i) inTowerN[i] = true;
				}
			}
			if (inTowerN[16]) {
				stub_ttN[16]++;
				switch (lay) {
					case 5: 
						occupancyTower16Layer5->Fill(mod,lad,1./nentries);
						stub_ttN_lN[16][5]++;
						if (lad==12) {
							switch (mod) {
                        case 25: stub_tt16_lN_ladN_modN[5][12][25]++;break;
                        case 26: stub_tt16_lN_ladN_modN[5][12][26]++;break;
                        case 27: stub_tt16_lN_ladN_modN[5][12][27]++;break;
                        case 28: stub_tt16_lN_ladN_modN[5][12][28]++;break;
                        case 29: stub_tt16_lN_ladN_modN[5][12][29]++;break;
                        case 30: stub_tt16_lN_ladN_modN[5][12][30]++;break;
                        case 31: stub_tt16_lN_ladN_modN[5][12][31]++;break;
                        case 32: stub_tt16_lN_ladN_modN[5][12][32]++;break;
								default: break;
							}
						}
                  if (lad==13) {
                     switch (mod) {
                        case 25: stub_tt16_lN_ladN_modN[5][13][25]++;break;
                        case 26: stub_tt16_lN_ladN_modN[5][13][26]++;break;
                        case 27: stub_tt16_lN_ladN_modN[5][13][27]++;break;
                        case 28: stub_tt16_lN_ladN_modN[5][13][28]++;break;
                        case 29: stub_tt16_lN_ladN_modN[5][13][29]++;break;
                        case 30: stub_tt16_lN_ladN_modN[5][13][30]++;break;
                        case 31: stub_tt16_lN_ladN_modN[5][13][31]++;break;
                        case 32: stub_tt16_lN_ladN_modN[5][13][32]++;break;
                        default: break;
                     }
						}   
                  if (lad==14) {
                     switch (mod) {
                        case 25: stub_tt16_lN_ladN_modN[5][14][25]++;break;
                        case 26: stub_tt16_lN_ladN_modN[5][14][26]++;break;
                        case 27: stub_tt16_lN_ladN_modN[5][14][27]++;break;
                        case 28: stub_tt16_lN_ladN_modN[5][14][28]++;break;
                        case 29: stub_tt16_lN_ladN_modN[5][14][29]++;break;
                        case 30: stub_tt16_lN_ladN_modN[5][14][30]++;break;
                        case 31: stub_tt16_lN_ladN_modN[5][14][31]++;break;
                        case 32: stub_tt16_lN_ladN_modN[5][14][32]++;break;
                        default: break;
                     }
						}
						break;

               case 6:
                  occupancyTower16Layer6->Fill(mod,lad,1./nentries);
                  stub_ttN_lN[16][6]++;
                  if (lad==18) {
                     switch (mod) {
                        case 23: stub_tt16_lN_ladN_modN[6][18][23]++;break;
                        case 24: stub_tt16_lN_ladN_modN[6][18][24]++;break;
                        case 25: stub_tt16_lN_ladN_modN[6][18][25]++;break;
                        case 26: stub_tt16_lN_ladN_modN[6][18][26]++;break;
                        case 27: stub_tt16_lN_ladN_modN[6][18][27]++;break;
                        case 28: stub_tt16_lN_ladN_modN[6][18][28]++;break;
                        default: break;
                     }
                  }
                  break;
               case 7:
                  occupancyTower16Layer7->Fill(mod,lad,1./nentries);
                  stub_ttN_lN[16][7]++;
                  if (lad==25) {
                     switch (mod) {
                        case 23: stub_tt16_lN_ladN_modN[7][25][23]++;break;
                        case 24: stub_tt16_lN_ladN_modN[7][25][24]++;break;
                        case 25: stub_tt16_lN_ladN_modN[7][25][25]++;break;
                        case 26: stub_tt16_lN_ladN_modN[7][25][26]++;break;
                        case 27: stub_tt16_lN_ladN_modN[7][25][27]++;break;
                        case 28: stub_tt16_lN_ladN_modN[7][25][28]++;break;
                        default: break;
                     }
                  }
                  break;
               case 8:
                  occupancyTower16Layer8->Fill(mod,lad,1./nentries);
                  stub_ttN_lN[16][8]++;
                  if (lad==36) {
                     switch (mod) {
                        case 7: stub_tt16_lN_ladN_modN[8][36][7]++;break;
                        case 8: stub_tt16_lN_ladN_modN[8][36][8]++;break;
                        case 9: stub_tt16_lN_ladN_modN[8][36][9]++;break;
                        case 10: stub_tt16_lN_ladN_modN[8][36][10]++;break;
                        case 11: stub_tt16_lN_ladN_modN[8][36][11]++;break;
                        case 12: stub_tt16_lN_ladN_modN[8][36][12]++;break;
                        default: break;
                     }
                  }
                  break;

               case 9:
                  occupancyTower16Layer9->Fill(mod,lad,1./nentries);
                  stub_ttN_lN[16][9]++;
                  if (lad==46) {
                     switch (mod) {
                        case 7: stub_tt16_lN_ladN_modN[9][46][7]++;break;
                        case 8: stub_tt16_lN_ladN_modN[9][46][8]++;break;
                        case 9: stub_tt16_lN_ladN_modN[9][46][9]++;break;
                        case 10: stub_tt16_lN_ladN_modN[9][46][10]++;break;
                        case 11: stub_tt16_lN_ladN_modN[9][46][11]++;break;
                        case 12: stub_tt16_lN_ladN_modN[9][46][12]++;break;
                        default: break;
                     }
                  }
                  break;
               case 10:
                  occupancyTower16Layer10->Fill(mod,lad,1./nentries);
                  stub_ttN_lN[16][10]++;
                  if (lad==55) {
                     switch (mod) {
                        case 6: stub_tt16_lN_ladN_modN[10][55][6]++;break;
                        case 7: stub_tt16_lN_ladN_modN[10][55][7]++;break;
                        case 8: stub_tt16_lN_ladN_modN[10][55][8]++;break;
                        case 9: stub_tt16_lN_ladN_modN[10][55][9]++;break;
                        case 10: stub_tt16_lN_ladN_modN[10][55][10]++;break;
                        case 11: stub_tt16_lN_ladN_modN[10][55][11]++;break;
                        default: break;
                     }
                  }
                  break;

					default: break;
				}
			}

			if (inTowerN[27]) {
				//stub_ttN[27]++;
				switch (lay) {
					case 5: 
						occupancyTower27Layer5->Fill(mod,lad,1./nentries);
						stub_ttN_lN[27][5]++;
						if (lad==3 && mod==33) {stub_tt27_lN_ladN_modN[5][3][33]++;stub_roads++;}
						if (lad==3 && mod==34) {stub_tt27_lN_ladN_modN[5][3][34]++;stub_roads++;}
						break;
               case 6: 
						occupancyTower27Layer6->Fill(mod,lad,1./nentries);
						stub_ttN_lN[27][6]++;
						if (lad==4 && mod==29) {stub_tt27_lN_ladN_modN[6][4][29]++;stub_roads++;}
						if (lad==4 && mod==30) {stub_tt27_lN_ladN_modN[6][4][30]++;stub_roads++;}
						break;
               case 7: 
						occupancyTower27Layer7->Fill(mod,lad,1./nentries);
						stub_ttN_lN[27][7]++;
						if (lad==6 && mod==30) {stub_tt27_lN_ladN_modN[7][6][30]++;stub_roads++;}
						if (lad==6 && mod==31) {stub_tt27_lN_ladN_modN[7][6][31]++;stub_roads++;}
						break;
               case 8: 
						occupancyTower27Layer8->Fill(mod,lad,1./nentries);
						stub_ttN_lN[27][8]++;
						if (lad==8 && mod==14) {stub_tt27_lN_ladN_modN[8][8][14]++;stub_roads++;}
						if (lad==9 && mod==14) {stub_tt27_lN_ladN_modN[8][9][14]++;stub_roads++;}
						break;
               case 9: 
						occupancyTower27Layer9->Fill(mod,lad,1./nentries);
						stub_ttN_lN[27][9]++;
						if (lad==10 && mod==15) {stub_tt27_lN_ladN_modN[9][10][15]++;stub_roads++;}
						if (lad==11 && mod==15) {stub_tt27_lN_ladN_modN[9][11][15]++;stub_roads++;}
						break;
               case 10: 
						occupancyTower27Layer10->Fill(mod,lad,1./nentries);
						stub_ttN_lN[27][10]++;
						if (lad==12 && mod==15) {stub_tt27_lN_ladN_modN[10][12][15]++;stub_roads++;}
						if (lad==13 && mod==15) {stub_tt27_lN_ladN_modN[10][13][15]++;stub_roads++;}
						if (lad==14 && mod==15) {stub_tt27_lN_ladN_modN[10][14][15]++;stub_roads++;}
						if (lad==12 && mod==16) {stub_tt27_lN_ladN_modN[10][12][16]++;stub_roads++;}
						if (lad==13 && mod==16) {stub_tt27_lN_ladN_modN[10][13][16]++;stub_roads++;}
						if (lad==14 && mod==16) {stub_tt27_lN_ladN_modN[10][14][16]++;stub_roads++;}
						break;
               default: break;
				}
			}
			if (inTowerN[8]) stub_ttN[8]++;
			if (inTowerN[0]) stub_ttN[0]++;
		}
		nStubTowerN[16]->Fill(stub_ttN[16]);
		nStubTowerN[8]->Fill(stub_ttN[8]);
		nStubTowerN[0]->Fill(stub_ttN[0]);
		nStubTowerNLayerN[16][5]->Fill(stub_ttN_lN[16][5]);
		nStubTowerNLayerN[16][6]->Fill(stub_ttN_lN[16][6]);
		nStubTowerNLayerN[16][7]->Fill(stub_ttN_lN[16][7]);
		nStubTowerNLayerN[16][8]->Fill(stub_ttN_lN[16][8]);
		nStubTowerNLayerN[16][9]->Fill(stub_ttN_lN[16][9]);
		nStubTowerNLayerN[16][10]->Fill(stub_ttN_lN[16][10]);
      nStubTowerNLayerN[27][5]->Fill(stub_ttN_lN[27][5]);
      nStubTowerNLayerN[27][6]->Fill(stub_ttN_lN[27][6]);
      nStubTowerNLayerN[27][7]->Fill(stub_ttN_lN[27][7]);
      nStubTowerNLayerN[27][8]->Fill(stub_ttN_lN[27][8]);
      nStubTowerNLayerN[27][9]->Fill(stub_ttN_lN[27][9]);
      nStubTowerNLayerN[27][10]->Fill(stub_ttN_lN[27][10]);


      nStubTower16LayerNladNmodN[5][12][25]->Fill(stub_tt16_lN_ladN_modN[5][12][25]);
      nStubTower16LayerNladNmodN[5][12][26]->Fill(stub_tt16_lN_ladN_modN[5][12][26]);
      nStubTower16LayerNladNmodN[5][12][27]->Fill(stub_tt16_lN_ladN_modN[5][12][27]);
      nStubTower16LayerNladNmodN[5][12][28]->Fill(stub_tt16_lN_ladN_modN[5][12][28]);
      nStubTower16LayerNladNmodN[5][12][29]->Fill(stub_tt16_lN_ladN_modN[5][12][29]);
      nStubTower16LayerNladNmodN[5][12][30]->Fill(stub_tt16_lN_ladN_modN[5][12][30]);
      nStubTower16LayerNladNmodN[5][12][31]->Fill(stub_tt16_lN_ladN_modN[5][12][31]);
      nStubTower16LayerNladNmodN[5][12][32]->Fill(stub_tt16_lN_ladN_modN[5][12][32]);
      nStubTower16LayerNladNmodN[5][13][25]->Fill(stub_tt16_lN_ladN_modN[5][13][25]);
      nStubTower16LayerNladNmodN[5][13][26]->Fill(stub_tt16_lN_ladN_modN[5][13][26]);
      nStubTower16LayerNladNmodN[5][13][27]->Fill(stub_tt16_lN_ladN_modN[5][13][27]);
      nStubTower16LayerNladNmodN[5][13][28]->Fill(stub_tt16_lN_ladN_modN[5][13][28]);
      nStubTower16LayerNladNmodN[5][13][29]->Fill(stub_tt16_lN_ladN_modN[5][13][29]);
      nStubTower16LayerNladNmodN[5][13][30]->Fill(stub_tt16_lN_ladN_modN[5][13][30]);
      nStubTower16LayerNladNmodN[5][13][31]->Fill(stub_tt16_lN_ladN_modN[5][13][31]);
      nStubTower16LayerNladNmodN[5][13][32]->Fill(stub_tt16_lN_ladN_modN[5][13][32]);
      nStubTower16LayerNladNmodN[5][14][25]->Fill(stub_tt16_lN_ladN_modN[5][14][25]);
      nStubTower16LayerNladNmodN[5][14][26]->Fill(stub_tt16_lN_ladN_modN[5][14][26]);
      nStubTower16LayerNladNmodN[5][14][27]->Fill(stub_tt16_lN_ladN_modN[5][14][27]);
      nStubTower16LayerNladNmodN[5][14][28]->Fill(stub_tt16_lN_ladN_modN[5][14][28]);
      nStubTower16LayerNladNmodN[5][14][29]->Fill(stub_tt16_lN_ladN_modN[5][14][29]);
      nStubTower16LayerNladNmodN[5][14][30]->Fill(stub_tt16_lN_ladN_modN[5][14][30]);
      nStubTower16LayerNladNmodN[5][14][31]->Fill(stub_tt16_lN_ladN_modN[5][14][31]);
      nStubTower16LayerNladNmodN[5][14][32]->Fill(stub_tt16_lN_ladN_modN[5][14][32]);

      nStubTower16LayerNladNmodN[6][18][23]->Fill(stub_tt16_lN_ladN_modN[6][18][23]);
      nStubTower16LayerNladNmodN[6][18][24]->Fill(stub_tt16_lN_ladN_modN[6][18][24]);
      nStubTower16LayerNladNmodN[6][18][25]->Fill(stub_tt16_lN_ladN_modN[6][18][25]);
      nStubTower16LayerNladNmodN[6][18][26]->Fill(stub_tt16_lN_ladN_modN[6][18][26]);
      nStubTower16LayerNladNmodN[6][18][27]->Fill(stub_tt16_lN_ladN_modN[6][18][27]);
      nStubTower16LayerNladNmodN[6][18][28]->Fill(stub_tt16_lN_ladN_modN[6][18][28]);

      nStubTower16LayerNladNmodN[7][25][23]->Fill(stub_tt16_lN_ladN_modN[7][25][23]);
      nStubTower16LayerNladNmodN[7][25][24]->Fill(stub_tt16_lN_ladN_modN[7][25][24]);
      nStubTower16LayerNladNmodN[7][25][25]->Fill(stub_tt16_lN_ladN_modN[7][25][25]);
      nStubTower16LayerNladNmodN[7][25][26]->Fill(stub_tt16_lN_ladN_modN[7][25][26]);
      nStubTower16LayerNladNmodN[7][25][27]->Fill(stub_tt16_lN_ladN_modN[7][25][27]);
      nStubTower16LayerNladNmodN[7][25][28]->Fill(stub_tt16_lN_ladN_modN[7][25][28]);

      nStubTower16LayerNladNmodN[8][36][7]->Fill(stub_tt16_lN_ladN_modN[8][36][7]);
      nStubTower16LayerNladNmodN[8][36][8]->Fill(stub_tt16_lN_ladN_modN[8][36][8]);
      nStubTower16LayerNladNmodN[8][36][9]->Fill(stub_tt16_lN_ladN_modN[8][36][9]);
      nStubTower16LayerNladNmodN[8][36][10]->Fill(stub_tt16_lN_ladN_modN[8][36][10]);
      nStubTower16LayerNladNmodN[8][36][11]->Fill(stub_tt16_lN_ladN_modN[8][36][11]);
      nStubTower16LayerNladNmodN[8][36][12]->Fill(stub_tt16_lN_ladN_modN[8][36][12]);

      nStubTower16LayerNladNmodN[9][46][7]->Fill(stub_tt16_lN_ladN_modN[9][46][7]);
      nStubTower16LayerNladNmodN[9][46][8]->Fill(stub_tt16_lN_ladN_modN[9][46][8]);
      nStubTower16LayerNladNmodN[9][46][9]->Fill(stub_tt16_lN_ladN_modN[9][46][9]);
      nStubTower16LayerNladNmodN[9][46][10]->Fill(stub_tt16_lN_ladN_modN[9][46][10]);
      nStubTower16LayerNladNmodN[9][46][11]->Fill(stub_tt16_lN_ladN_modN[9][46][11]);
      nStubTower16LayerNladNmodN[9][46][12]->Fill(stub_tt16_lN_ladN_modN[9][46][12]);

      nStubTower16LayerNladNmodN[10][55][6]->Fill(stub_tt16_lN_ladN_modN[10][55][6]);
      nStubTower16LayerNladNmodN[10][55][7]->Fill(stub_tt16_lN_ladN_modN[10][55][7]);
      nStubTower16LayerNladNmodN[10][55][8]->Fill(stub_tt16_lN_ladN_modN[10][55][8]);
      nStubTower16LayerNladNmodN[10][55][9]->Fill(stub_tt16_lN_ladN_modN[10][55][9]);
      nStubTower16LayerNladNmodN[10][55][10]->Fill(stub_tt16_lN_ladN_modN[10][55][10]);
      nStubTower16LayerNladNmodN[10][55][11]->Fill(stub_tt16_lN_ladN_modN[10][55][11]);


		nStubTower27LayerNladNmodN[5][3][33]->Fill(stub_tt27_lN_ladN_modN[5][3][33]);
		nStubTower27LayerNladNmodN[5][3][34]->Fill(stub_tt27_lN_ladN_modN[5][3][34]);
		nStubTower27LayerNladNmodN[6][4][29]->Fill(stub_tt27_lN_ladN_modN[6][4][29]);
		nStubTower27LayerNladNmodN[6][4][30]->Fill(stub_tt27_lN_ladN_modN[6][4][30]);
		nStubTower27LayerNladNmodN[7][6][30]->Fill(stub_tt27_lN_ladN_modN[7][6][30]);
		nStubTower27LayerNladNmodN[7][6][31]->Fill(stub_tt27_lN_ladN_modN[7][6][31]);
		nStubTower27LayerNladNmodN[8][8][14]->Fill(stub_tt27_lN_ladN_modN[8][8][14]);
		nStubTower27LayerNladNmodN[8][9][14]->Fill(stub_tt27_lN_ladN_modN[8][9][14]);
		nStubTower27LayerNladNmodN[9][10][15]->Fill(stub_tt27_lN_ladN_modN[9][10][15]);
		nStubTower27LayerNladNmodN[9][11][15]->Fill(stub_tt27_lN_ladN_modN[9][11][15]);
		nStubTower27LayerNladNmodN[10][12][15]->Fill(stub_tt27_lN_ladN_modN[10][12][15]);
		nStubTower27LayerNladNmodN[10][13][15]->Fill(stub_tt27_lN_ladN_modN[10][13][15]);
		nStubTower27LayerNladNmodN[10][14][15]->Fill(stub_tt27_lN_ladN_modN[10][14][15]);
		nStubTower27LayerNladNmodN[10][12][16]->Fill(stub_tt27_lN_ladN_modN[10][12][16]);
		nStubTower27LayerNladNmodN[10][13][16]->Fill(stub_tt27_lN_ladN_modN[10][13][16]);
		nStubTower27LayerNladNmodN[10][14][16]->Fill(stub_tt27_lN_ladN_modN[10][14][16]);
		nStubRoads->Fill(stub_roads);
	}


   TDirectory *DirLayer[24];
   for (int i=0;i<25;i++) {
      sprintf(str,"Layer%02d",i);
      DirLayer[i] = newfile->mkdir(str);
      DirLayer[i]->cd();
      for (int l=0;l<76;l++) {
         for (int m=0;m<82;m++) {
            nStubTower16LayerNladNmodN[i][l][m]->Write();
         }    
      }
   }  

   //Loop with the geometry map
	for (auto it: triggerTowerReverseMap_) {
      Tower_perModule->Fill(it.second.size());
      // it.first (key, here is the moduleId); triggerTowerReverseMap_[it.first] (value, here is a vector of towers) 
      // it.second.size() == triggerTowerReverseMap_[it.first].size()
      for (unsigned j=0; j<triggerTowerReverseMap_[it.first].size(); j++) {
         Module_inEachTower->Fill(triggerTowerReverseMap_[it.first].at(j));
         if (decodeLayer(it.first)==10 //barral
            || (decodeLayer(it.first)>10 && decodeLadder(it.first)>8) //endcaps
            || (decodeLayer(it.first)==9 && (decodeModule(it.first)<5 || decodeModule(it.first)>18) )//hybird
            )   
            Module_inEachTower_merge->Fill(triggerTowerReverseMap_[it.first].at(j),0.5);
         else Module_inEachTower_merge->Fill(triggerTowerReverseMap_[it.first].at(j));
      }   
   }   

   TLatex L1;
   L1.SetNDC();

   TCanvas *c0 = new TCanvas("c0","c0");
   c0->cd();
   Tower_perModule->SetFillColor(kYellow-9);
   Tower_perModule->Draw("TEXT00");
   Tower_perModule->Draw("same");
   c0->SaveAs("plots/Tower_perModule.pdf");

   TCanvas *c1 = new TCanvas("c1","c1");
   c1->cd();
   Module_inEachTower->SetFillColor(kYellow-9);
   Module_inEachTower->Draw();
   c1->SaveAs("plots/Module_inEachTower.pdf");

   TCanvas *c2 = new TCanvas("c2","c2");
   c2->cd();
   Module_inEachTower->SetFillColor(kYellow-9);
   Module_inEachTower->Draw();
   Module_inEachTower_merge->SetFillColor(kCyan-8);
   Module_inEachTower_merge->Draw("same");
   c2->SaveAs("plots/Module_inEachTower_merge.pdf");

	TCanvas *c3 = new TCanvas("c3","c3");
	c3->cd();
	nSTUB_perLayer->Draw();
	c3->SaveAs("plots/nSTUB_perLayer.pdf");

	TCanvas *c4 = new TCanvas("c4","c4",1200,300);
	TPad* pad = (TPad*)c4->GetPad(0); 
	pad->SetLeftMargin(0.06); 
	pad->SetRightMargin(0.1); 
	STUB_z_r->GetYaxis()->SetTitleOffset(0.5);
	STUB_z_r->Draw("colz");
	//STUB_z_r_overlap->SetMarkerSize(0.2);
	//STUB_z_r_overlap->SetMarkerColor(kRed);
	//STUB_z_r_overlap->Draw("same");
	c4->SaveAs("plots/STUB_z_r_overlap.pdf");

   TCanvas *c4b = new TCanvas("c4b","c4b");
   //STUB_phi_mod->Draw("colz");
   STUB_x_y_barrel->Draw("colz");
   //STUB_x_y_overlap->SetMarkerSize(0.2);
   //STUB_x_y_overlap->SetMarkerColor(kRed);
   //STUB_x_y_overlap->Draw("same");
   c4b->SaveAs("plots/STUB_x_y_barrel.pdf");
	STUB_x_y_endcap->Draw("colz");
	c4b->SaveAs("plots/STUB_x_y_endcap.pdf");
	

	TCanvas *c5 = new TCanvas("c5","c5", 800,800);
	c5->Divide(2,2);
	c5->cd(1);
	nSTUB_Layer5->Draw();
	nSTUB_Layer6->SetLineColor(kRed);
	nSTUB_Layer6->Draw("same");
	nSTUB_Layer7->SetLineColor(kBlue);
	nSTUB_Layer7->Draw("same");
	nSTUB_Layer8->SetLineColor(kYellow+1);
	nSTUB_Layer8->Draw("same");
	nSTUB_Layer9->SetLineColor(kGreen+2);
	nSTUB_Layer9->Draw("same");
	nSTUB_Layer10->SetLineColor(kMagenta);
	nSTUB_Layer10->Draw("same");
	L1.DrawLatex(0.4,0.95,"Barrel");
	TLegend *leg1 = new TLegend(0.7,0.65,0.9,0.9);
	//leg1->AddEntry((TObject*)0,"average over #phi","");
	leg1->AddEntry(nSTUB_Layer10,"Layer 10","l");
	leg1->AddEntry(nSTUB_Layer9,"Layer 9","l");
	leg1->AddEntry(nSTUB_Layer8,"Layer 8","l");
	leg1->AddEntry(nSTUB_Layer7,"Layer 7","l");
	leg1->AddEntry(nSTUB_Layer6,"Layer 6","l");
	leg1->AddEntry(nSTUB_Layer5,"Layer 5","l");
	leg1->Draw();
	c5->cd(2);
   nSTUB_Layer11->Draw();
   nSTUB_Layer12->SetLineColor(kRed);
   nSTUB_Layer12->Draw("same");
   nSTUB_Layer13->SetLineColor(kBlue);
   nSTUB_Layer13->Draw("same");
   nSTUB_Layer14->SetLineColor(kYellow+1);
   nSTUB_Layer14->Draw("same");
   nSTUB_Layer15->SetLineColor(kGreen+2);
   nSTUB_Layer15->Draw("same");
	L1.DrawLatex(0.4,0.95,"Endcap");
   TLegend *leg2 = new TLegend(0.7,0.65,0.9,0.9);
   //leg2->AddEntry((TObject*)0,"average over #phi","");
   leg2->AddEntry(nSTUB_Layer11,"Layer 11","l");
   leg2->AddEntry(nSTUB_Layer12,"Layer 12","l");
   leg2->AddEntry(nSTUB_Layer13,"Layer 13","l");
   leg2->AddEntry(nSTUB_Layer14,"Layer 14","l");
   leg2->AddEntry(nSTUB_Layer15,"Layer 15","l");
   leg2->Draw();
	c5->cd(3);
   nSTUB_Layer5_centralZ->Draw();
   nSTUB_Layer6_centralZ->SetLineColor(kRed);
   nSTUB_Layer6_centralZ->Draw("same");
   nSTUB_Layer7_centralZ->SetLineColor(kBlue);
   nSTUB_Layer7_centralZ->Draw("same");
   nSTUB_Layer8_centralZ->SetLineColor(kYellow+1);
   nSTUB_Layer8_centralZ->Draw("same");
   nSTUB_Layer9_centralZ->SetLineColor(kGreen+2);
   nSTUB_Layer9_centralZ->Draw("same");
   nSTUB_Layer10_centralZ->SetLineColor(kMagenta);
   nSTUB_Layer10_centralZ->Draw("same");
   L1.DrawLatex(0.4,0.95,"Barrel, modules near z=0");
   TLegend *leg3 = new TLegend(0.4,0.65,0.9,0.9);
   leg3->AddEntry(nSTUB_Layer10_centralZ,"Layer 10, moduleID(z)=12","l");
   leg3->AddEntry(nSTUB_Layer9_centralZ,"Layer 9, moduleID(z)=12","l");
   leg3->AddEntry(nSTUB_Layer8_centralZ,"Layer 8, moduleID(z)=12","l");
   leg3->AddEntry(nSTUB_Layer7_centralZ,"Layer 7, moduleID(z)=26","l");
   leg3->AddEntry(nSTUB_Layer6_centralZ,"Layer 6, moduleID(z)=27","l");
   leg3->AddEntry(nSTUB_Layer5_centralZ,"Layer 5, moduleID(z)=31","l");
   leg3->Draw();
	c5->cd(4);
	nSTUB_ring0->Draw();
	nSTUB_ring1->SetLineColor(kRed);
	nSTUB_ring1->Draw("same");
   nSTUB_ring2->SetLineColor(kBlue);
   nSTUB_ring2->Draw("same");
   nSTUB_ring3->SetLineColor(kYellow+1);
   nSTUB_ring3->Draw("same");
   nSTUB_ring4->SetLineColor(kGreen+1);
   nSTUB_ring4->Draw("same");
   nSTUB_ring5->SetLineColor(kMagenta);
   nSTUB_ring5->Draw("same");
   nSTUB_ring6->SetLineColor(kOrange);
   nSTUB_ring6->Draw("same");
   nSTUB_ring7->SetLineColor(kCyan);
   nSTUB_ring7->Draw("same");
   nSTUB_ring8->SetLineColor(kPink+1);
   nSTUB_ring8->Draw("same");
   nSTUB_ring9->SetLineColor(kViolet+1);
   nSTUB_ring9->Draw("same");
   nSTUB_ring10->SetLineColor(kSpring+1);
   nSTUB_ring10->Draw("same");
   nSTUB_ring11->SetLineColor(kAzure+1);
   nSTUB_ring11->Draw("same");
   nSTUB_ring12->SetLineColor(kTeal+1);
   nSTUB_ring12->Draw("same");
   nSTUB_ring13->SetLineColor(kGray+1);
   nSTUB_ring13->Draw("same");
   nSTUB_ring14->SetLineColor(kBlue-10);
   nSTUB_ring14->Draw("same");
	L1.DrawLatex(0.4,0.95,"Endcap, innermost layer (layer 11)");
	TLegend *leg4 = new TLegend(0.7,0.2,0.9,0.9);
	leg4->AddEntry(nSTUB_ring0,"Ring 0","l");
	leg4->AddEntry(nSTUB_ring1,"Ring 1","l");
	leg4->AddEntry(nSTUB_ring2,"Ring 2","l");
	leg4->AddEntry(nSTUB_ring3,"Ring 3","l");
	leg4->AddEntry(nSTUB_ring4,"Ring 4","l");
	leg4->AddEntry(nSTUB_ring5,"Ring 5","l");
	leg4->AddEntry(nSTUB_ring6,"Ring 6","l");
	leg4->AddEntry(nSTUB_ring7,"Ring 7","l");
	leg4->AddEntry(nSTUB_ring8,"Ring 8","l");
	leg4->AddEntry(nSTUB_ring9,"Ring 9","l");
	leg4->AddEntry(nSTUB_ring10,"Ring 10","l");
	leg4->AddEntry(nSTUB_ring11,"Ring 11","l");
	leg4->AddEntry(nSTUB_ring12,"Ring 12","l");
	leg4->AddEntry(nSTUB_ring13,"Ring 13","l");
	leg4->AddEntry(nSTUB_ring14,"Ring 14","l");
	leg4->Draw();
	c5->SaveAs("plots/nSTUB_moduleZ.pdf");

	TCanvas *c6 = new TCanvas("c6","c6",900,600);
	c6->Divide(3,2);
	c6->cd(1);
	occupancyTower16Layer5->Draw("text30");
	L1.DrawLatex(0.4,0.95,"TriggerTower16, Layer5");
	c6->cd(2);
	occupancyTower16Layer6->Draw("text30");
	L1.DrawLatex(0.4,0.95,"TriggerTower16, Layer6");
	c6->cd(3);
	occupancyTower16Layer7->Draw("text30");
	L1.DrawLatex(0.4,0.95,"TriggerTower16, Layer7");
	c6->cd(4);
	occupancyTower16Layer8->Draw("text30");
	L1.DrawLatex(0.4,0.95,"TriggerTower16, Layer8");
	c6->cd(5);
	occupancyTower16Layer9->Draw("text30");
	L1.DrawLatex(0.4,0.95,"TriggerTower16, Layer9");
	c6->cd(6);
	occupancyTower16Layer10->Draw("text30");
	L1.DrawLatex(0.4,0.95,"TriggerTower16, Layer10");
	c6->SaveAs("plots/occupancyTower16.pdf");
   c6->cd(1);
   occupancyTower27Layer5->Draw("text30");
   L1.DrawLatex(0.4,0.95,"TriggerTower27, Layer5");
   c6->cd(2);
   occupancyTower27Layer6->Draw("text30");
   L1.DrawLatex(0.4,0.95,"TriggerTower27, Layer6");
   c6->cd(3);
   occupancyTower27Layer7->Draw("text30");
   L1.DrawLatex(0.4,0.95,"TriggerTower27, Layer7");
   c6->cd(4);
   occupancyTower27Layer8->Draw("text30");
   L1.DrawLatex(0.4,0.95,"TriggerTower27, Layer8");
   c6->cd(5);
   occupancyTower27Layer9->Draw("text30");
   L1.DrawLatex(0.4,0.95,"TriggerTower27, Layer9");
   c6->cd(6);
   occupancyTower27Layer10->Draw("text30");
   L1.DrawLatex(0.4,0.95,"TriggerTower27, Layer10");
   c6->SaveAs("plots/occupancyTower27.pdf");

	TCanvas *c7 = new TCanvas("c7","c7");
	c7->cd();
	Module_perTrack->SetFillColor(kAzure-9);
	Module_perTrack->Draw();
	c7->SaveAs("plots/Module_perTrack.pdf");
/*
	TCanvas *c8 = new TCanvas("c8","c8",900,600);
	c8->Divide(3,2); 
	c8->cd(1);
	CheckOverLapArea_0_8->SetFillColor(kAzure-9);
	CheckOverLapArea_0_8->Draw("text00");
	CheckOverLapArea_0_8->Draw("same");
	L1.DrawLatex(0.4,0.95,"TriggerTower 0 & 8");
	sprintf(str,"%03f",float(CheckOverLapArea_0_8->GetBinContent(2))/(CheckOverLapArea_0_8->GetBinContent(1)+CheckOverLapArea_0_8->GetBinContent(2)));
	L1.DrawLatex(0.4,0.5,str);
	c8->cd(2);
	CheckOverLapArea_8_16->SetFillColor(kAzure-9);
	CheckOverLapArea_8_16->Draw("text00");
	CheckOverLapArea_8_16->Draw("same");
	L1.DrawLatex(0.4,0.95,"TriggerTower 8 & 16");
   sprintf(str,"%03f",float(CheckOverLapArea_8_16->GetBinContent(2))/(CheckOverLapArea_8_16->GetBinContent(1)+CheckOverLapArea_8_16->GetBinContent(2)));
   L1.DrawLatex(0.4,0.5,str);
	c8->cd(3);
	CheckOverLapArea_16_24->SetFillColor(kAzure-9);
	CheckOverLapArea_16_24->Draw("text00");
	CheckOverLapArea_16_24->Draw("same");
	L1.DrawLatex(0.4,0.95,"TriggerTower 16 & 24");
   sprintf(str,"%03f",float(CheckOverLapArea_16_24->GetBinContent(2))/(CheckOverLapArea_16_24->GetBinContent(1)+CheckOverLapArea_16_24->GetBinContent(2)));
   L1.DrawLatex(0.4,0.5,str);
	c8->cd(4);
	CheckOverLapArea_0_1->SetFillColor(kAzure-9);
	CheckOverLapArea_0_1->Draw("text00");
	CheckOverLapArea_0_1->Draw("same");
	L1.DrawLatex(0.4,0.95,"TriggerTower 0 & 1");
   sprintf(str,"%03f",float(CheckOverLapArea_0_1->GetBinContent(2))/(CheckOverLapArea_0_1->GetBinContent(1)+CheckOverLapArea_0_1->GetBinContent(2)));
   L1.DrawLatex(0.4,0.5,str);
	c8->cd(5);
	CheckOverLapArea_8_9->SetFillColor(kAzure-9);
	CheckOverLapArea_8_9->Draw("text00");
	CheckOverLapArea_8_9->Draw("same");
	L1.DrawLatex(0.4,0.95,"TriggerTower 8 & 9");
   sprintf(str,"%03f",float(CheckOverLapArea_8_9->GetBinContent(2))/(CheckOverLapArea_8_9->GetBinContent(1)+CheckOverLapArea_8_9->GetBinContent(2)));
   L1.DrawLatex(0.4,0.5,str);
	c8->cd(6);
	CheckOverLapArea_16_17->SetFillColor(kAzure-9);
	CheckOverLapArea_16_17->Draw("text00");
	CheckOverLapArea_16_17->Draw("same");
	L1.DrawLatex(0.4,0.95,"TriggerTower 16 & 17");
   sprintf(str,"%03f",float(CheckOverLapArea_16_17->GetBinContent(2))/(CheckOverLapArea_16_17->GetBinContent(1)+CheckOverLapArea_16_17->GetBinContent(2)));
   L1.DrawLatex(0.4,0.5,str);
	c8->SaveAs("plots/CheckOverLapArea.pdf");
*/
	TCanvas *c9 = new TCanvas("c9","c9");
	c9->cd();
	nSTUB_perTower->Draw();
	c9->SaveAs("plots/nSTUB_perTower.pdf");

	gStyle->SetOptStat(1111);	

   TCanvas *c10 = new TCanvas("c10","c10",900,600);
   c10->Divide(3,2);
	for (int i=0;i<6;i++){
		c10->cd(i+1);
		nStubTowerNLayerN[16][i+5]->SetFillColor(kMagenta-8);
		nStubTowerNLayerN[16][i+5]->Draw();
		sprintf(str,"TriggerTower16, Layer%02d",i+5);
		L1.DrawLatex(0.2,0.95,str);
	}
	c10->SaveAs("plots/nStubTower16allLayers.pdf");
   for (int i=0;i<6;i++){
      c10->cd(i+1);
      nStubTowerNLayerN[27][i+5]->SetFillColor(kMagenta-8);
      nStubTowerNLayerN[27][i+5]->Draw();
      sprintf(str,"TriggerTower27, Layer%02d",i+5);
      L1.DrawLatex(0.2,0.95,str);
   }   
   c10->SaveAs("plots/nStubTower27allLayers.pdf");


	TCanvas *c11 = new TCanvas("c11","c11",1600,600);
	c11->Divide(8,3);
	for (int l=0;l<3;l++){
		for (int m=0;m<8;m++){
			c11->cd(l*8+m+1);
			nStubTower16LayerNladNmodN[5][l+12][m+25]->SetFillColor(kMagenta-8);
			nStubTower16LayerNladNmodN[5][l+12][m+25]->Draw();
			nStubTower16LayerNladNmodN[5][l+12][m+25]->Write(); 
			sprintf(str,"TT16, Layer5, Ladder%02d, Module%02d",l+12,m+25);
			L1.DrawLatex(0.1,0.95,str);
		}
	}
	c11->SaveAs("plots/nStubTower16Layer5allModules.pdf");
		
	TCanvas *c12 = new TCanvas("c12","c12",1200,400);
	c12->Divide(3,1);
	for (int i=0; i<3; i++) {
		c12->cd(i+1);
		nStubTowerN[i*8]->SetFillColor(kMagenta-8);
		nStubTowerN[i*8]->Draw();
		sprintf(str,"TriggerTower%02d",i*8);
		L1.DrawLatex(0.4,0.95,str);
	}
	c12->SaveAs("plots/nStubFor3Towers.pdf");

	for (int i=0;i<25;i++){
		for (int l=0;l<76;l++){
			for (int m=0;m<82;m++){
				nStubTower27LayerNladNmodN[i][l][m]->SetFillColor(kMagenta-8);
			}
		}
	}
	TCanvas *c13 = new TCanvas("c13","c13",1200,600);
	c13->Divide(6,3);
	c13->cd(1);
	nStubTower27LayerNladNmodN[5][3][33]->Draw();
	L1.DrawLatex(0.1,0.95,"TT27, Layer5, Ladder3, Module33");
   c13->cd(2);
   nStubTower27LayerNladNmodN[5][3][34]->Draw();
   L1.DrawLatex(0.1,0.95,"TT27, Layer5, Ladder3, Module34");
   c13->cd(3);
   nStubTower27LayerNladNmodN[6][4][29]->Draw();
   L1.DrawLatex(0.1,0.95,"TT27, Layer6, Ladder4, Module29");
   c13->cd(4);
   nStubTower27LayerNladNmodN[6][4][30]->Draw();
   L1.DrawLatex(0.1,0.95,"TT27, Layer6, Ladder4, Module30");
   c13->cd(5);
   nStubTower27LayerNladNmodN[7][6][30]->Draw();
   L1.DrawLatex(0.1,0.95,"TT27, Layer7, Ladder6, Module30");
   c13->cd(6);
   nStubTower27LayerNladNmodN[7][6][31]->Draw();
   L1.DrawLatex(0.1,0.95,"TT27, Layer7, Ladder6, Module31");
   c13->cd(7);
   nStubTower27LayerNladNmodN[8][8][14]->Draw();
   L1.DrawLatex(0.1,0.95,"TT27, Layer8, Ladder8, Module14");
   c13->cd(8);
   nStubTower27LayerNladNmodN[8][9][14]->Draw();
   L1.DrawLatex(0.1,0.95,"TT27, Layer8, Ladder9, Module14");
   c13->cd(9);
   nStubTower27LayerNladNmodN[9][10][15]->Draw();
   L1.DrawLatex(0.1,0.95,"TT27, Layer9, Ladder10, Module15");
   c13->cd(10);
   nStubTower27LayerNladNmodN[9][11][15]->Draw();
   L1.DrawLatex(0.1,0.95,"TT27, Layer9, Ladder11, Module15");
   c13->cd(13);
   nStubTower27LayerNladNmodN[10][12][15]->Draw();
   L1.DrawLatex(0.1,0.95,"TT27, Layer10, Ladder12, Module15");
   c13->cd(14);
   nStubTower27LayerNladNmodN[10][13][15]->Draw();
   L1.DrawLatex(0.1,0.95,"TT27, Layer10, Ladder13, Module15");
   c13->cd(15);
   nStubTower27LayerNladNmodN[10][14][15]->Draw();
   L1.DrawLatex(0.1,0.95,"TT27, Layer10, Ladder14, Module15");
   c13->cd(16);
   nStubTower27LayerNladNmodN[10][12][16]->Draw();
   L1.DrawLatex(0.1,0.95,"TT27, Layer10, Ladder12, Module16");
   c13->cd(17);
   nStubTower27LayerNladNmodN[10][13][16]->Draw();
   L1.DrawLatex(0.1,0.95,"TT27, Layer10, Ladder13, Module16");
   c13->cd(18);
   nStubTower27LayerNladNmodN[10][14][16]->Draw();
   L1.DrawLatex(0.1,0.95,"TT27, Layer10, Ladder14, Module16");
	c13->SaveAs("plots/nStubTower27someModules.pdf");

	TCanvas *c14 = new TCanvas("c14","c14");
	c14->cd();
	nStubRoads->SetFillColor(kMagenta-8);
	nStubRoads->Draw();
	c14->SaveAs("plots/nStubRoads.pdf");

}
