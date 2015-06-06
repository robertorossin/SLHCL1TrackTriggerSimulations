#define MakeHists_cxx
#include "MakeHists.h"
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

void MakeHists::Loop(bool requirePtCut3)
{
//   In a ROOT session, you can do:
//      Root > .L MakeHists.C
//      Root > MakeHists t
//      Root > t.GetEntry(12); // Fill t data members with entry number 12
//      Root > t.Show();       // Show values of entry 12
//      Root > t.Show(16);     // Read and show values of entry 16
//      Root > t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
   if (fChain == 0) return;

   std::map<unsigned, std::vector<unsigned> > triggerTowerMap_; // key: towerId; value: moduleIds in the tower
   std::map<unsigned, std::vector<unsigned> > triggerTowerReverseMap_; // key: moduleId; value: towerIds containing the module
   readTriggerTowerFile("trigger_sector_map.csv",triggerTowerMap_,triggerTowerReverseMap_);
   if (triggerTowerMap_.empty() || triggerTowerReverseMap_.empty()) {
      std::cout << "Failed to load trigger tower maps" << std::endl;
      return;
   }

	Char_t str[100];
   unsigned int moduleId, lay, lad, mod, localPhi, localZ;
	float stub_r, stub_phi, stub_z, stub_eta, stub_trigBend, stub_x, stub_y;


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

/*   std::vector< std::vector<TH1F*> > nStubTower16Layer5ladNmodN;
   for (int l=0;l<76;l++) {
      std::vector<TH1F*> nStubTower16Layer5modN;
      for (int m=0;m<82;m++) {
         sprintf(str,"nStubTower16Layer5lad%02dmod%02d",l,m);
         nStubTower16Layer5modN.push_back(new TH1F(str, str, 50, 0, 50));
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

	TFile *newfile = new TFile("Histograms.root", "RECREATE");

   //Long64_t nentries = fChain->GetEntriesFast();
	Long64_t nentries = 1;

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;

      const unsigned nstubs = TTStubs_modId_new->size();
      if (jentry%100==0) std::cout<<"Processing event #"<<jentry<<", nstubs="<<nstubs<<std::endl;
      if (!nstubs) { // skip if no stub
         continue;
      }
      if (nstubs > 1000000) {
         std::cout << "Way too many stubs: " << nstubs << std::endl;
         return;
      }

      int stub_ttN[48]={0};
      int stub_ttN_lN[48][24]={{0}};
      //int stub_tt16_l5_ladN_modN[76][82]={{0}};
		int stub_tt16_lN_ladN_modN[25][76][82]={{{0}}};

      for (unsigned l=0; l<nstubs; ++l) {
         //module ID
         moduleId = TTStubs_modId_new->at(l);
         lay      = decodeLayer(moduleId); //layer
         lad      = decodeLadder(moduleId); //phi module for barrel; r rings for endcaps 
         mod      = decodeModule(moduleId); //z module for barrel; phi module for endcaps 
			stub_trigBend = TTStubs_trigBend_new->at(l);
         if (triggerTowerReverseMap_[moduleId].size()==0) continue;
         if (requirePtCut3 && !pass3GeVCut(moduleId,stub_trigBend)) continue;

         bool inTowerN[48] = {false}; //Endcap: 0-7,40,47; Hybird: 8-15,32-39; Barrel: 16-23,24-31; 
         for (unsigned j=0; j<triggerTowerReverseMap_[moduleId].size(); j++) {
            int getTower = triggerTowerReverseMap_[moduleId].at(j);
            //nSTUB_perTower->Fill(getTower,1./nentries);
            for (int i=0; i<48; i++) {
               if (getTower == i) inTowerN[i] = true;
            }
         }

         if (inTowerN[16]) {
				stub_ttN[16]++;
            switch (lay) {
               case 5:
                  //occupancyTower16Layer5->Fill(mod,lad,1./nentries);
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
						/*occupancyTower16Layer6->Fill(mod,lad,1./nentries);*/
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
						/*occupancyTower16Layer7->Fill(mod,lad,1./nentries);*/
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
						/*occupancyTower16Layer8->Fill(mod,lad,1./nentries);*/
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
						/*occupancyTower16Layer9->Fill(mod,lad,1./nentries);*/
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
						/*occupancyTower16Layer10->Fill(mod,lad,1./nentries);*/
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
		}


      nStubTowerNLayerN[16][5]->Fill(stub_ttN_lN[16][5]);
      nStubTowerNLayerN[16][6]->Fill(stub_ttN_lN[16][6]);
      nStubTowerNLayerN[16][7]->Fill(stub_ttN_lN[16][7]);
      nStubTowerNLayerN[16][8]->Fill(stub_ttN_lN[16][8]);
      nStubTowerNLayerN[16][9]->Fill(stub_ttN_lN[16][9]);
      nStubTowerNLayerN[16][10]->Fill(stub_ttN_lN[16][10]);

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

      // if (Cut(ientry) < 0) continue;
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


/*
   TLatex L1;
   L1.SetNDC();

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

   TCanvas *c11 = new TCanvas("c11","c11",1600,600);
   c11->Divide(8,3);
   for (int l=0;l<3;l++){
      for (int m=0;m<8;m++){
         c11->cd(l*8+m+1);
         nStubTower16Layer5ladNmodN[l+12][m+25]->SetFillColor(kMagenta-8);
         nStubTower16Layer5ladNmodN[l+12][m+25]->Draw();
         sprintf(str,"TT16, Layer5, Ladder%02d, Module%02d",l+12,m+25);
         L1.DrawLatex(0.1,0.95,str);
      }
   }
   c11->SaveAs("plots/nStubTower16Layer5allModules.pdf");
*/
}
