#ifndef MCExtractor_h
#define MCExtractor_h

/** \class MCExtractor
 *  Extract the MC info
 */

// Framework stuff
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Utilities/interface/InputTag.h"

// Geometry info, needed for hit recovery

#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "Geometry/CommonTopologies/interface/PixelTopology.h"
#include "Geometry/TrackerGeometryBuilder/interface/PixelGeomDetUnit.h"
#include "Geometry/TrackerGeometryBuilder/interface/StripGeomDetUnit.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"
#include "Geometry/Records/interface/CaloGeometryRecord.h"
#include "Geometry/DTGeometry/interface/DTGeometry.h"
#include "Geometry/CSCGeometry/interface/CSCGeometry.h"
#include "Geometry/RPCGeometry/interface/RPCGeometry.h"
#include "Geometry/Records/interface/MuonGeometryRecord.h"

#include "SimDataFormats/TrackingAnalysis/interface/TrackingParticle.h"
#include "SimDataFormats/Vertex/interface/SimVertexContainer.h"
#include "SimDataFormats/CaloHit/interface/PCaloHitContainer.h"

#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h" 
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/SiPixelDetId/interface/PixelSubdetector.h"
#include "DataFormats/SiPixelCluster/interface/SiPixelCluster.h"
#include "DataFormats/SiStripDetId/interface/TIBDetId.h"
#include "DataFormats/SiStripDetId/interface/TIDDetId.h"
#include "DataFormats/SiStripDetId/interface/TOBDetId.h"
#include "DataFormats/SiStripDetId/interface/TECDetId.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/SiPixelDetId/interface/PixelBarrelName.h"
#include "DataFormats/SiPixelDetId/interface/PixelEndcapName.h"
#include "DataFormats/Candidate/interface/CandidateFwd.h"
#include "DataFormats/SiPixelDetId/interface/PXBDetId.h"
#include "DataFormats/SiPixelDetId/interface/PXFDetId.h"

//#include "CommonTools/RecoAlgos/interface/TrackingParticleSelector.h"

//std C++
#include <iostream>
#include <vector>

// ROOT stuff
#include "TMath.h"
#include "TTree.h"
#include "TFile.h"

class MCExtractor
{
 public:
  /// Constructor
  MCExtractor(bool doTree);
  MCExtractor(TFile *a_file);
  /// Destructor
  virtual ~MCExtractor(){}

  void writeInfo(const edm::Event *event); 
  void init(const edm::EventSetup *setup);

  void reset();
  void fillTree(); 
  void createTree(); 
  void fillSize(int size);
  int  getSize();
  void getInfo(int ievt); 

  // Setters/Getters

  bool isOK() {return m_OK;}

  int getNGen() {return m_gen_n;}
  int getNTP() {return m_part_n;}
  int getNHITS() {return m_hits_x->size();}

  int getTP_ID(int i)      {return m_part_pdgId->at(i);}
  int getTP_hits(int i)    {return m_hits->at(i);}
  int getTP_st(int i)      {return m_st->at(i);}
  float getTP_x(int i)     {return m_part_x->at(i);}
  float getTP_y(int i)     {return m_part_y->at(i);}
  float getTP_z(int i)     {return m_part_z->at(i);}
  float getTP_r(int i)     {return sqrt(m_part_x->at(i)*m_part_x->at(i)+m_part_y->at(i)*m_part_y->at(i));}
  float getTP_eta(int i)   {return m_part_eta->at(i);}
  float getTP_px(int i)    {return m_part_px->at(i);}
  float getTP_py(int i)    {return m_part_py->at(i);}
  float getTP_pz(int i)    {return m_part_pz->at(i);}
  float getTP_pt(int i)    {return sqrt(m_part_px->at(i)*m_part_px->at(i)+m_part_py->at(i)*m_part_py->at(i));}
  float getTP_hitsx(int i) {return m_hits_x->at(i);}
  float getTP_hitsy(int i) {return m_hits_y->at(i);}
  float getTP_hitsz(int i) {return m_hits_z->at(i);}
  float getTP_hitse(int i) {return m_hits_e->at(i);}
  float getTP_hitsid(int i)  {return m_hits_id->at(i);}
  int   getTP_stid(int i)    {return m_st_id->at(i);}
  int   getTP_hitslad(int i) {return m_hits_ladder->at(i);}
  int   getTP_hitslay(int i) {return m_hits_layer->at(i);}
  int   getTP_hitsmod(int i) {return m_hits_module->at(i);}

  void printhits(float x, float y, float z);

  void clearTP(float ptmin,float rmax);

  void findMatchingTP(const int &stID,
		      const int &layer,const int &ladder,const int &module, 
		      const int &nseg, 
		      const float &x, const float &y, const float &z, 
		      int &itp, int &ihit, bool verb);

 private:
 			      

  void getGenInfo(const edm::Event *event); 
 
  // Rootuple parameters

  TTree* m_tree_new;
  TTree* m_tree_retrieved;
  
  bool m_OK;


  int    		m_gen_n;
  int    		m_part_n;
  int    		m_part_nhit;
  int                   m_part_nstracks;


  std::vector<float>    *m_gen_x;
  std::vector<float>    *m_gen_y;
  std::vector<float>    *m_gen_z;
  std::vector<float> 	*m_gen_px;
  std::vector<float> 	*m_gen_py;
  std::vector<float>    *m_gen_pz;
  std::vector<int> 	*m_gen_proc;
  std::vector<int>      *m_gen_pdg;


  // Tracking particles for a given event, along with their 
  // simulated hits



  std::vector<int>      *m_part_pdgId;
  std::vector<float> 	*m_part_px;
  std::vector<float> 	*m_part_py;
  std::vector<float>	*m_part_pz;
  std::vector<float>    *m_part_eta;
  std::vector<float> 	*m_part_phi;
  std::vector<float>    *m_part_x;
  std::vector<float>    *m_part_y;
  std::vector<float>    *m_part_z;
  std::vector<int>      *m_part_used;

  std::vector<int>      *m_hits;
  std::vector<float>    *m_hits_x;
  std::vector<float>    *m_hits_y;
  std::vector<float>    *m_hits_z;
  std::vector<float>    *m_hits_e;
  std::vector<int>      *m_hits_id;
  std::vector<int>      *m_hits_layer;
  std::vector<int>      *m_hits_ladder;
  std::vector<int>      *m_hits_module;
  std::vector<int>      *m_hits_used;

  std::vector<int>      *m_st;
  std::vector<int>      *m_st_id;

  // Finally the geometry information

  edm::ESHandle<DTGeometry> dtGeometry;
  edm::ESHandle<CSCGeometry> cscGeometry;
  edm::ESHandle<RPCGeometry> rpcGeometry;
  edm::ESHandle<TrackerGeometry> theTrackerGeometry;
  edm::ESHandle<CaloGeometry> caloGeometry;

  const CaloSubdetectorGeometry* HFgeom;
  const CaloSubdetectorGeometry* HEgeom;
  const CaloSubdetectorGeometry* HBgeom;
  const CaloSubdetectorGeometry* EEgeom;
  const CaloSubdetectorGeometry* EBgeom;


  int n_hit_part;
  int n_st_part;
  bool match;
  
  double resol_r;
  double resol_phi;
  double resol_x;
  double resol_y;

  double chi_max;
  double chi_lim;
  double chi;
  
  // int itp_failsafe = -1;
  // int ihit_failsafe = -1;
  
  int n_st;
  int n_hit;
  int n_st2;
  int n_hit2;
  int n_TP;
  int start;

  bool sch;
  
  double dx;
  double dy;
  double dz;
  double dr;
  double dphi;


  float x,y,z;
};


#endif
