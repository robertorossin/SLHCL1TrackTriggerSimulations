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
#include "SimDataFormats/TrackingHit/interface/PSimHit.h"

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

  int getTP_ID(int i)      {return m_part_pdgId->at(i);}
  float getTP_x(int i)     {return m_part_x->at(i);}
  float getTP_y(int i)     {return m_part_y->at(i);}
  float getTP_z(int i)     {return m_part_z->at(i);}
  float getTP_r(int i)     {return sqrt(m_part_x->at(i)*m_part_x->at(i)+m_part_y->at(i)*m_part_y->at(i));}
  float getTP_eta(int i)   {return m_part_eta->at(i);}
  float getTP_px(int i)    {return m_part_px->at(i);}
  float getTP_py(int i)    {return m_part_py->at(i);}
  float getTP_pz(int i)    {return m_part_pz->at(i);}
  float getTP_pt(int i)    {return sqrt(m_part_px->at(i)*m_part_px->at(i)+m_part_py->at(i)*m_part_py->at(i));}


  void printhits(float x, float y, float z);

  void clearTP(float ptmin,float rmax);

  void findMatchingTP(const int &stID, const int &evtID,
		      int &itp, bool verb);

 private:
 			      

  void getGenInfo(const edm::Event *event); 
 
  // Rootuple parameters

  TTree* m_tree_new;
  TTree* m_tree_retrieved;
  
  bool m_OK;
  std::vector<int>      *m_part_used;
  std::vector<int>      *m_hits_used;

  /*
    List of the branches contained in the MC tree

    m_tree_new->Branch("gen_n",   &m_gen_n);
    m_tree_new->Branch("gen_proc",&m_gen_proc);
    m_tree_new->Branch("gen_pdg", &m_gen_pdg);
    m_tree_new->Branch("gen_px",  &m_gen_px);
    m_tree_new->Branch("gen_py",  &m_gen_py);
    m_tree_new->Branch("gen_pz",  &m_gen_pz);
    m_tree_new->Branch("gen_x",   &m_gen_x);
    m_tree_new->Branch("gen_y",   &m_gen_y);
    m_tree_new->Branch("gen_z",   &m_gen_z);
    
    // Infos related to the subsequent tracking particles
  
    m_tree_new->Branch("subpart_n",            &m_part_n);
    m_tree_new->Branch("subpart_hits",         &m_hits);
    m_tree_new->Branch("subpart_st",           &m_st);
    m_tree_new->Branch("subpart_pdgId",        &m_part_pdgId);
    m_tree_new->Branch("subpart_px",           &m_part_px);
    m_tree_new->Branch("subpart_py",           &m_part_py);
    m_tree_new->Branch("subpart_pz",           &m_part_pz);
    m_tree_new->Branch("subpart_eta",          &m_part_eta);
    m_tree_new->Branch("subpart_phi",          &m_part_phi);
    m_tree_new->Branch("subpart_x",            &m_part_x);
    m_tree_new->Branch("subpart_y",            &m_part_y);
    m_tree_new->Branch("subpart_z",            &m_part_z);
    
    
  */
  
  int    		m_gen_n;       // Number of particles generated
 
  // Size of the following vectors is m_gen_n
  std::vector<float>    *m_gen_x;     // x-origin of particle i (in cm)
  std::vector<float>    *m_gen_y;     // y-origin of particle i (in cm)
  std::vector<float>    *m_gen_z;     // z-origin of particle i (in cm)
  std::vector<float> 	*m_gen_px;    // px-origin of particle i (in cm)
  std::vector<float> 	*m_gen_py;    // py-origin of particle i (in cm)
  std::vector<float>    *m_gen_pz;    // pz-origin of particle i (in cm)
  std::vector<int> 	*m_gen_proc;  // UNUSED 
  std::vector<int>      *m_gen_pdg;   // PDG code of particle


  int    		m_part_n;     // Number of TrackingParticles retrieved

  // Size of the following vectors is m_part_n
  std::vector<int>      *m_part_pdgId;// PDG code of TP i
  std::vector<int>      *m_part_evtId;// 
  std::vector< std::vector<int> >      *m_part_stId;// 
  std::vector<float> 	*m_part_px;   // px-origin of TP i (in cm)
  std::vector<float> 	*m_part_py;   // py-origin of TP i (in cm)
  std::vector<float>	*m_part_pz;   // pz-origin of TP i (in cm)
  std::vector<float>    *m_part_eta;  // eta-origin of TP i (in cm)
  std::vector<float> 	*m_part_phi;  // phi-origin of TP i (in cm)
  std::vector<float>    *m_part_x;    // x-origin of TP i (in cm)
  std::vector<float>    *m_part_y;    // y-origin of TP i (in cm)
  std::vector<float>    *m_part_z;    // z-origin of TP i (in cm)
  std::vector<int>      *m_st;        // Number of simtracks involved in TP i 
  std::vector<int>      *m_hits;      // Number of SimHits of TP i 




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


  std::vector<int>      the_ids;
  float x,y,z;
};


#endif
