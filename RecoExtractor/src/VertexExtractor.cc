#include "../interface/VertexExtractor.h"


VertexExtractor::VertexExtractor(edm::InputTag tag)
{
  //std::cout << "VertexExtractor objet is created" << std::endl;


  m_tag = tag;

  // Tree definition

  m_tree     = new TTree("Vertices","RECO PV info") ;

  // Branches definition

  m_tree->Branch("n_vertices",   &m_n_vertices,   "n_vertices/I");  
  m_tree->Branch("vertex_vx",    &m_vtx_vx,       "vertex_vx[n_vertices]/F");  
  m_tree->Branch("vertex_vy",    &m_vtx_vy,       "vertex_vy[n_vertices]/F");  
  m_tree->Branch("vertex_vz",    &m_vtx_vz,       "vertex_vz[n_vertices]/F"); 
  m_tree->Branch("vertex_isFake",&m_vtx_isFake,   "vertex_isFake[n_vertices]/B"); 
  m_tree->Branch("vertex_ndof",  &m_vtx_ndof,     "vertex_ndof[n_vertices]/F"); 
  m_tree->Branch("vertex_chi2",  &m_vtx_normChi2, "vertex_chi2[n_vertices]/F"); 

  // Set everything to 0

  VertexExtractor::reset();
}

VertexExtractor::~VertexExtractor()
{}



//
// Method filling the main particle tree
//

void VertexExtractor::writeInfo(const edm::Event *event) 
{
  edm::Handle<reco::VertexCollection> vertexHandle;
  event->getByLabel(m_tag, vertexHandle);
  const reco::VertexCollection vertexCollection = *(vertexHandle.product());

  VertexExtractor::reset();
  VertexExtractor::fillSize(static_cast<int>(vertexCollection.size()));

  if (VertexExtractor::getSize())
  {
    for(int i=0; i<VertexExtractor::getSize(); ++i) 
      VertexExtractor::writeInfo(&vertexCollection.at(i),i); 
  }

  VertexExtractor::fillTree();
}

void VertexExtractor::writeInfo(const reco::Vertex *part, int index) 
{
  if (index>=m_vertices_MAX) return;

  m_vtx_vx[index]      = part->position().x();
  m_vtx_vy[index]      = part->position().y();
  m_vtx_vz[index]      = part->position().z();
  m_vtx_isFake[index]  = static_cast<int>(part->isFake());
  m_vtx_ndof[index]    = part->ndof();
  m_vtx_normChi2[index]= part->normalizedChi2();

}


// Method initializing everything (to do for each event)

void VertexExtractor::reset()
{
  m_n_vertices = 0;

  for (int i=0;i<m_vertices_MAX;++i) 
  {
    m_vtx_vx[i]       = 0.;
    m_vtx_vy[i]       = 0.;
    m_vtx_vz[i]       = 0.;
    m_vtx_isFake[i]   = 0;
    m_vtx_ndof[i]     = 0.;
    m_vtx_normChi2[i] = 0.;
  }
}


void VertexExtractor::fillTree()
{
  m_tree->Fill(); 
}
 
void VertexExtractor::fillSize(int size)
{
  m_n_vertices=size;
}

int  VertexExtractor::getSize()
{
  return m_n_vertices;
}

