#define ProduceFile_cxx
#include "ProduceFile.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <bitset>

unsigned int decodeLayer( unsigned int moduleId) {
	return moduleId / 10000;
}

unsigned int decodeLadder( unsigned int moduleId) {
	return (moduleId / 100) % 100; 
}

unsigned int decodeModule( unsigned int moduleId) {
	return moduleId % 100; 
}

int fillTreeWithString(int board, TTree *tree, std::bitset<256> BitsCICL[40], std::bitset<256> BitsCICR[40]);
int fillTreeWithULong64(int board, TTree *tree, std::bitset<256> BitsCICL[40], std::bitset<256> BitsCICR[40]);


void ProduceFile::Loop()
{
	TFile *outputfile = new TFile("DataSourcingTree.root", "RECREATE");
	/*TDirectory *Tower[48];  
	  sprintf(str,"Tower%02d",1);
	  Tower[0] = outputfile->mkdir(str);
	  Tower[0]->cd();*/
	TTree *tree[10]; //create 10 trees, each for a board 

	unsigned OutputModule[10][40]={{0}};
	std::ifstream ifs ("ModuleList.txt", std::ifstream::in);
	for (int i=0; i<40; i++) {
		ifs >> OutputModule[0][i];
	}
	ifs.close();

	if (fChain == 0) return;
	//Long64_t nentries = fChain->GetEntriesFast();
	Long64_t nentries = 1;
	Long64_t nbytes = 0, nb = 0;

	for (Long64_t jentry=0; jentry<nentries;jentry++) {
		Long64_t ientry = LoadTree(jentry);
		if (ientry < 0) break;
		nb = fChain->GetEntry(jentry);   nbytes += nb;
		const unsigned nstubs = TTStubs_modId->size();
		//if (jentry%100==0) 
		std::cout<<"Processing event #"<<jentry<<", nstubs="<<nstubs<<std::endl;
		if (!nstubs) { // skip if no stub
			continue;
		}    
		if (nstubs > 100000) {
			std::cout << "Way too many stubs: " << nstubs << std::endl;
			return;
		} 		
		if (fabs(genParts_vz->at(0))>4) continue;

		int nStub2SL[10][40]={{0}};
		int nStub2SR[10][40]={{0}};
		int nStubPSL[10][40]={{0}};
		int nStubPSR[10][40]={{0}};
		//define header block
		std::bitset<26> headerCICL, headerCICR;
		//define final output block
		std::bitset<256> BitsCICL[10][40], BitsCICR[10][40];

		//write stubs into the output block
		for (unsigned l=0; l<nstubs; ++l) {
			//global coordinates
			unsigned moduleId   = TTStubs_modId->at(l); std::cout<<std::endl<<"moduleID="<<moduleId<<std::endl;
			unsigned lay        = decodeLayer(moduleId);  //layer
			//unsigned lad        = decodeLadder(moduleId); //phi ladder for barrel; r ring for endcaps 
			//unsigned mod        = decodeModule(moduleId); //z module for barrel; phi module for endcaps 
			//local coordinates
			float localZ        = TTStubs_coordy->at(l);  //0-31(5bits) for PS (4bits for each CIC); 0-1 for 2S
			float localPhi      = TTStubs_coordx->at(l);  //960 strips for PS, 1016 strips for 2S. 2024 values in halfStrip = 11bits = 3bits(chipID)+8bits(stubAddress)
			float stub_trigBend = TTStubs_trigBend->at(l);//-8 to 8, 16 strips, 32 values(5bits) in halfStrip

			std::bitset<3>  bxID;
			std::bitset<11> stubAddress; stubAddress |= int(localPhi*2); //chipID+stubAddress
			std::bitset<5>  stubBend2S;  stubBend2S |= int(stub_trigBend*2);  // int-->bitset-->ullong; [0,16]-->[00000,10000]-->[0,16]; [-16,-1]-->[10000,11111]-->[16,31]
			std::bitset<5>  stubBendPS;  stubBendPS |= int(stub_trigBend*2);
			std::bitset<5>  stubZpos;    stubZpos |= int(localZ);
			int nBitsFor2S = bxID.size()+stubAddress.size()+stubBend2S.size();
			int nBitsForPS = bxID.size()+stubAddress.size()+stubBendPS.size()+stubZpos.size();

			for (int board=0; board<1; board++) {        //loop 10 boards in this trigger tower (for now, only 1 board)
				for (int m=0; m<40; m++){                 //loop 40 modules in this board
					if (moduleId == OutputModule[board][m]) { //find out which module this stub belongs to
						if (lay>7) { //2S modules (layer 8,9,10)
							if (localZ<1) { //CIC L
								std::cout<<"The "<<nStub2SL[board][m]<<"th fired stub in this module:"<<std::endl;
								for (unsigned b=0;b<stubAddress.size();b++) { //write chipID+stubAddress (11bits) for this stub into the output block
									BitsCICL[board][m].set(headerCICL.size() + nStub2SL[board][m]*nBitsFor2S + bxID.size() + b, stubAddress.test(b));
								}
								std::cout<<"Set chipID+stubAddress: "<<std::endl<<BitsCICL[board][m]<<std::endl;
								for (unsigned b=0;b<stubBend2S.size();b++) { //write the stubBend bits for this stub into the output block
									BitsCICL[board][m].set(headerCICL.size() + nStub2SL[board][m]*nBitsFor2S + bxID.size() + stubAddress.size() + b, stubBend2S.test(b));
								}
								std::cout<<"Set stubBend: "<<std::endl<<BitsCICL[board][m]<<std::endl;
								nStub2SL[board][m]++;
							}
							else { //CIC R
								for (unsigned b=0;b<stubAddress.size();b++) {
									BitsCICR[board][m].set(headerCICR.size() + nStub2SR[board][m]*nBitsFor2S + bxID.size() + b, stubAddress.test(b));
								}
								for (unsigned b=0;b<stubBend2S.size();b++) {
									BitsCICR[board][m].set(headerCICR.size() + nStub2SR[board][m]*nBitsFor2S + bxID.size() + stubAddress.size() + b, stubBend2S.test(b));
								}
								nStub2SR[board][m]++;
							}
						}
						else { //PS modules (layer 5,6,7)
							if (localZ<16) { //CIC L
								std::cout<<"The "<<nStubPSL[board][m]<<"th fired stub in this module:"<<std::endl;
								for (unsigned b=0;b<stubAddress.size();b++) {
									BitsCICL[board][m].set(headerCICL.size() + nStubPSL[board][m]*nBitsForPS + bxID.size() + b, stubAddress.test(b));
								}
								std::cout<<"Set chipID+stubAddress: "<<std::endl<<BitsCICL[board][m]<<std::endl;
								for (unsigned b=0;b<stubBendPS.size();b++) {
									BitsCICL[board][m].set(headerCICL.size() + nStubPSL[board][m]*nBitsForPS + bxID.size() + stubAddress.size() + b, stubBendPS.test(b));
								}
								std::cout<<"Set stubBend: "<<std::endl<<BitsCICL[board][m]<<std::endl;
								for (unsigned b=0;b<stubZpos.size();b++) { //write the stubZposition (4bits) for this stub into the output block
									BitsCICL[board][m].set(headerCICL.size() + nStubPSL[board][m]*nBitsForPS + bxID.size() + stubAddress.size() + stubBendPS.size() + b, stubZpos.test(b));
								}
								std::cout<<"Set z position: "<<std::endl<<BitsCICL[board][m]<<std::endl;
								nStubPSL[board][m]++;
							}
							else { //CIC R
								for (unsigned b=0;b<stubAddress.size();b++) {
									BitsCICR[board][m].set(headerCICR.size() + nStubPSR[board][m]*nBitsForPS + bxID.size() + b, stubAddress.test(b));
								}
								for (unsigned b=0;b<stubBendPS.size();b++) {
									BitsCICR[board][m].set(headerCICR.size() + nStubPSR[board][m]*nBitsForPS + bxID.size() + stubAddress.size()+b, stubBendPS.test(b));
								}
								for (unsigned b=0;b<stubZpos.size();b++) {
									BitsCICR[board][m].set(headerCICR.size() + nStubPSR[board][m]*nBitsForPS + bxID.size() + stubAddress.size() + stubBendPS.size() + b, stubZpos.test(b));
								}   
								nStubPSR[board][m]++;
							}
						}
					}
				}
			}
		}

		//create header, then copy header into the output block, then fill the output tree
		for (int board=0; board<1; board++) {
			for (int m=0; m<40; m++){
				//create header
				std::bitset<4> NStubL,NStubR;
				if (decodeLayer(OutputModule[board][m])>7) { //2S module  
					//set module type
					headerCICL.set(0,0); 
					headerCICR.set(0,0); 
					//set error/status bit
					//set bx ID
					//set number of stubs
					//std::cout<<"nStubL,R = "<<nStub2SL[board][m]<<", "<<nStub2SR[board][m]<<std::endl;
					NStubL |= nStub2SL[board][m];
					NStubR |= nStub2SR[board][m];
				}
				else { //PS module
					headerCICL.set(0,1);
					headerCICR.set(0,1);
					NStubL |= nStubPSL[board][m];
					NStubR |= nStubPSR[board][m];
				}
				//writer number of stubs into header
				for (unsigned b=0;b<NStubL.size();b++) {
					headerCICL.set(headerCICL.size()-NStubL.size()+b, NStubL.test(b));
					headerCICR.set(headerCICR.size()-NStubR.size()+b, NStubR.test(b));
				}   
				std::cout<<"Module "<<OutputModule[board][m]<<": HeaderCICL="<<headerCICL<<", HeaderCICR="<<headerCICR<<std::endl;

				//copy header into the output block
				for (unsigned b=0;b<headerCICL.size();b++) {
					BitsCICL[board][m].set(b,headerCICL.test(b));
					BitsCICR[board][m].set(b,headerCICR.test(b));
				}
			}

			//fill tree for this board
			fillTreeWithULong64(board,tree[board],BitsCICL[board],BitsCICR[board]);
			//fillTreeWithString(board,tree[board],BitsCICL[board],BitsCICR[board]);
		}
	}

	outputfile->Write();
}


int fillTreeWithString(int board, TTree *tree, std::bitset<256> BitsCICL[40], std::bitset<256> BitsCICR[40]) {
	char str[20];
	sprintf(str,"Board%02d",board);
	tree = new TTree(str,str);

   std::vector< std::vector<string> > OutputData(40);
   for (int m=0; m<40; m++) {
      OutputData[m].push_back(BitsCICL[m].to_string());
      OutputData[m].push_back(BitsCICR[m].to_string());
   }   	

	std::ifstream ifs ("ModuleList.txt", std::ifstream::in);
	for (int m=0; m<40; m++) { //create 40 branches in a borad, each for a module
		TString ModuleID;
		ifs >> ModuleID;
		tree->Branch("Module_"+ModuleID,&OutputData[m]);
	}
	ifs.close();	
	tree->Fill();
	return 0;
}


//Convert bitset to an array of ULong64. Then save the array into the output tree
int fillTreeWithULong64(int board, TTree *tree, std::bitset<256> BitsCICL[40], std::bitset<256> BitsCICR[40]) {
   char str[20];
   sprintf(str,"Board%02d",board);
   tree = new TTree(str,str);

	ULong64_t OutputData[40][8]={{0}}; //an array of 64 bits unsigned integer for each link
   std::bitset<64> tmpL,tmpR;
   for (int m=0; m<40; m++) {
      for (int i=0; i<4; i++) { 
         tmpL.reset();
         tmpR.reset();
         for (int b=0; b<64; b++) { 
            tmpL.set(b, BitsCICL[m].test(b+i*64));
            tmpR.set(b, BitsCICR[m].test(b+i*64));
         }   
         OutputData[m][7-i] = tmpL.to_ullong();
         OutputData[m][3-i] = tmpR.to_ullong();
      }   
   }

   std::ifstream ifs ("ModuleList.txt", std::ifstream::in);
   for (int m=0; m<40; m++) { //create 40 branches in a borad, each for a module
      TString ModuleID;
      ifs >> ModuleID;
      tree->Branch("Module_"+ModuleID, OutputData[m], "Module_"+ModuleID+"[8]/l");
   }
   ifs.close();
   tree->Fill();
   return 0;
} 

