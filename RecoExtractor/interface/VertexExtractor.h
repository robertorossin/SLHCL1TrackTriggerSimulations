#ifndef VERTEXEXTRACTOR_H
#define VERTEXEXTRACTOR_H

/**
 * VertexExtractor
 * \brief: Base class for extracting vertex info
 */


//Include RECO info
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

//Include std C++
#include <iostream>

#include "TMath.h"
#include "TTree.h"
#include "TFile.h"
#include "TLorentzVector.h"
#include "TClonesArray.h"

class VertexExtractor
{

 public:

  VertexExtractor(edm::InputTag tag);
  ~VertexExtractor();


  void writeInfo(const reco::Vertex *part, int index); 
  void writeInfo(const edm::Event *event); 

  void reset();
  void fillTree(); 
  void fillSize(int size);
  int  getSize();

 private:
  
  TTree* m_tree;

  static const int 	m_vertices_MAX   = 50;

  edm::InputTag m_tag;

  int   m_n_vertices;
  float	m_vtx_vx[m_vertices_MAX];
  float	m_vtx_vy[m_vertices_MAX];
  float	m_vtx_vz[m_vertices_MAX];
  int   m_vtx_isFake[m_vertices_MAX];
  float m_vtx_ndof[m_vertices_MAX];
  float m_vtx_normChi2[m_vertices_MAX];
};

#endif 
