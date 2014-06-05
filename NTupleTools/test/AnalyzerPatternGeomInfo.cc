#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "Geometry/Records/interface/StackedTrackerGeometryRecord.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "Geometry/TrackerGeometryBuilder/interface/StackedTrackerGeometry.h"
#include "Geometry/TrackerGeometryBuilder/interface/StackedTrackerDetUnit.h"
#include "DataFormats/SiPixelDetId/interface/PXBDetId.h"
#include "DataFormats/SiPixelDetId/interface/PXFDetId.h"
#include "DataFormats/SiPixelDetId/interface/StackedTrackerDetId.h"
//#include "Geometry/TrackerGeometryBuilder/interface/PixelGeomDetUnit.h"
//#include "Geometry/TrackerNumberingBuilder/interface/GeometricDet.h"

#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
//#include "TH2D.h"
#include "TH2F.h"


// Fibonacci recursion
Long64_t fib(Long64_t x) {
    if (x <= 0)
        return 0;
    if (x == 1)
        return 1;
    if (x == 2)  // need this, otherwise '1' occurs twice
        return 2;
    return fib(x-1) + fib(x-2);
}


class AnalyzerPatternGeomInfo : public edm::EDAnalyzer {
  public:
    /// Constructor/destructor
    explicit AnalyzerPatternGeomInfo(const edm::ParameterSet&);
    virtual ~AnalyzerPatternGeomInfo();

  private:
    virtual void beginRun(const edm::Run&, const edm::EventSetup&);
    virtual void endRun(const edm::Run&, const edm::EventSetup&);

    virtual void beginJob();
    virtual void endJob();
    virtual void analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup);

  private:
    /// For event setup
    const TrackerGeometry * theGeometry;
    const StackedTrackerGeometry * theStackedGeometry;

    /// Maps
    std::map<uint32_t, uint32_t> moduleId0ToStackId;
    std::map<uint32_t, uint32_t> moduleId1ToStackId;
    std::map<uint32_t, uint32_t> moduleId0ToGeoId;
    std::map<uint32_t, uint32_t> moduleId1ToGeoId;

    /// Patterns
    std::vector<std::vector<uint32_t> > superstripIds;

    /// Histograms
    TH2F* hPXB_PS_ZR;
    TH2F* hPXB_2S_ZR;
    TH2F* hPXF_PS_ZR;
    TH2F* hPXF_2S_ZR;

    TH2F* hPXB_PS_XY;
    TH2F* hPXB_2S_XY;
    TH2F* hPXF_PS_XY;
    TH2F* hPXF_2S_XY;

    TH2F* hPXB_PS_UV;
    TH2F* hPXB_2S_UV;
    TH2F* hPXF_PS_UV;
    TH2F* hPXF_2S_UV;

    std::vector<TH2F*> vecPXA_ind_ZR;
    std::vector<TH2F*> vecPXA_sum_ZR;
    std::vector<TH2F*> vecPXB_ind_XY;
    std::vector<TH2F*> vecPXB_sum_XY;
    std::vector<TH2F*> vecPXF_ind_XY;
    std::vector<TH2F*> vecPXF_sum_XY;
    std::vector<TH2F*> vecPXB_ind_UV;
    std::vector<TH2F*> vecPXB_sum_UV;
    std::vector<TH2F*> vecPXF_ind_UV;
    std::vector<TH2F*> vecPXF_sum_UV;

    /// Configurations
    std::string bankfile_;
    int verbose_;
};


AnalyzerPatternGeomInfo::AnalyzerPatternGeomInfo(const edm::ParameterSet& iConfig)
: bankfile_(iConfig.getParameter<std::string>("bank") ),
  verbose_(iConfig.getParameter<int>("verbosity") ) {}

AnalyzerPatternGeomInfo::~AnalyzerPatternGeomInfo() {}


// Here we make the layout of the detector
void AnalyzerPatternGeomInfo::beginRun(const edm::Run& iRun, const edm::EventSetup& iSetup) {
    /// Geometry setup
    /// Set pointers to Geometry
    edm::ESHandle<TrackerGeometry> geometryHandle;
    iSetup.get<TrackerDigiGeometryRecord>().get(geometryHandle);
    theGeometry = geometryHandle.product();

    /// Set pointers to Stacked Modules
    edm::ESHandle<StackedTrackerGeometry> stackedGeometryHandle;
    iSetup.get<StackedTrackerGeometryRecord>().get(stackedGeometryHandle);
    theStackedGeometry = stackedGeometryHandle.product();

    /// Clear maps
    moduleId0ToStackId.clear();
    moduleId1ToStackId.clear();
    moduleId0ToGeoId.clear();
    moduleId1ToGeoId.clear();

    /// Loop over the detector elements
    StackedTrackerGeometry::StackContainerIterator  stkIterator;
    for (stkIterator = theStackedGeometry->stacks().begin();
         stkIterator != theStackedGeometry->stacks().end();
         ++stkIterator) {

        StackedTrackerDetUnit* stackDetUnit = *stkIterator;
        StackedTrackerDetId stackDetId = stackDetUnit->Id();
        assert(stackDetUnit == theStackedGeometry->idToStack(stackDetId));

        /// GeomDet and GeomDetUnit are needed to access each
        /// DetId and topology and geometric features
        /// Convert to specific DetId
        const GeomDet* det0 = theStackedGeometry->idToDet(stackDetId, 0);
        const GeomDet* det1 = theStackedGeometry->idToDet(stackDetId, 1);
        const GeomDetUnit* detUnit0 = theStackedGeometry->idToDetUnit(stackDetId, 0);
        const GeomDetUnit* detUnit1 = theStackedGeometry->idToDetUnit(stackDetId, 1);

        /// Barrel
        if (stackDetId.isBarrel() ) {
            uint32_t geoId0 = det0->geographicalId().rawId();
            uint32_t geoId1 = det1->geographicalId().rawId();
            PXBDetId detId0(geoId0);
            PXBDetId detId1(geoId1);

            /// Get the Stack, iPhi, iZ from StackedTrackerDetId
            uint32_t iStack = stackDetId.iLayer();
            uint32_t iPhi   = stackDetId.iPhi();
            uint32_t iZ     = stackDetId.iZ();

            /// Get index-coordinates of each sensor from DetId
            uint32_t iLayer0  = detId0.layer();
            uint32_t iLayer1  = detId1.layer();
            uint32_t iRod0    = detId0.ladder();
            uint32_t iRod1    = detId1.ladder();
            uint32_t iModule0 = detId0.module();
            uint32_t iModule1 = detId1.module();

            uint32_t moduleId0 = 10000*iLayer0 + 100*(iRod0-1) + (iModule0-1)/2;
            uint32_t moduleId1 = 10000*iLayer1 + 100*(iRod1-1) + (iModule1-1)/2;

            if (verbose_ > 1) {
                std::cout << "Stack: " << iStack << " from Layers:  " << iLayer0 << " " << iLayer1 << std::endl;
                std::cout << "Phi:   " << iPhi <<   " from Rods:    " << iRod0 << " " << iRod1 << std::endl;
                std::cout << "Z:     " << iZ <<     " from Modules: " << iModule0 << " " << iModule1 << " >>> " << (iModule0 == iModule1 + 1) << std::endl;
                std::cout << "modId: " << moduleId0 << " " << moduleId1 << std::endl;
            }

            if (moduleId0ToGeoId.find(moduleId0) == moduleId0ToGeoId.end() )
                moduleId0ToGeoId.insert(std::make_pair(moduleId0, geoId0) );
            else
                std::cout << "Error: This pair already exists in map! moduleId0: " << moduleId0 << " geoId0: " << geoId0 << " existing value in map: " << moduleId0ToGeoId.at(moduleId0) << std::endl;

            if (moduleId1ToGeoId.find(moduleId1) == moduleId1ToGeoId.end() )
                moduleId1ToGeoId.insert(std::make_pair(moduleId1, geoId1) );
            else
                std::cout << "Error: This pair already exists in map! moduleId1: " << moduleId1 << " geoId1: " << geoId1 << " existing value in map: " << moduleId1ToGeoId.at(moduleId1) << std::endl;

            if (moduleId0ToStackId.find(moduleId0) == moduleId0ToStackId.end() )
                moduleId0ToStackId.insert(std::make_pair(moduleId0, stackDetId.rawId()) );
            else
                std::cout << "Error: This pair already exists in map! moduleId0: " << moduleId0 << " stackId: " << stackDetId.rawId() << " existing value in map: " << moduleId0ToStackId.at(moduleId0) << std::endl;

            if (moduleId1ToStackId.find(moduleId1) == moduleId1ToStackId.end() )
                moduleId1ToStackId.insert(std::make_pair(moduleId1, stackDetId.rawId()) );
            else
                std::cout << "Error: This pair already exists in map! moduleId1: " << moduleId1 << " stackId: " << stackDetId.rawId() << " existing value in map: " << moduleId1ToStackId.at(moduleId1) << std::endl;


            /// Find pixel topology related information
            const PixelGeomDetUnit* pixUnit0 = dynamic_cast<const PixelGeomDetUnit*>(detUnit0);
            //const PixelGeomDetUnit* pixUnit1 = dynamic_cast<const PixelGeomDetUnit*>(detUnit1);
            const PixelTopology* pixTopo0 = dynamic_cast<const PixelTopology*>(&(pixUnit0->specificTopology()) );
            //const PixelTopology* pixTopo1 = dynamic_cast<const PixelTopology*>(&(pixUnit1->specificTopology()) );

            for (int col=0; col!=pixTopo0->ncolumns(); ++col) {
                for (int row=0; row!=pixTopo0->nrows(); ++row) {
                    MeasurementPoint mp(row + 0.5, col + 0.5);
                    GlobalPoint gp = detUnit0->surface().toGlobal(pixTopo0->localPosition(mp) );
                    double signed_r = gp.y() >= 0 ? gp.perp() : -gp.perp();

                    if (theStackedGeometry->isPSModule(stackDetId)) {
                        hPXB_PS_ZR->Fill(gp.z()*10., signed_r*10.);
                        hPXB_PS_XY->Fill(gp.x()*10., gp.y()*10.);
                        hPXB_PS_UV->Fill(2e5*0.1*gp.x()/gp.perp2(), 2e5*0.1*gp.y()/gp.perp2());
                    } else {
                        hPXB_2S_ZR->Fill(gp.z()*10., signed_r*10.);
                        hPXB_2S_XY->Fill(gp.x()*10., gp.y()*10.);
                        hPXB_2S_UV->Fill(2e5*0.1*gp.x()/gp.perp2(), 2e5*0.1*gp.y()/gp.perp2());
                    }
                }
            }
        }  // end if barrel

        /// Endcap
        if (stackDetId.isEndcap() ) {
            uint32_t geoId0 = det0->geographicalId().rawId();
            uint32_t geoId1 = det1->geographicalId().rawId();
            PXFDetId detId0(geoId0);
            PXFDetId detId1(geoId1);

            /// Get the Stack, iPhi, iZ from StackedTrackerDetId
            uint32_t iSide  = stackDetId.iSide();
            uint32_t iStack = stackDetId.iDisk();
            uint32_t iRing  = stackDetId.iRing();
            uint32_t iPhi   = stackDetId.iPhi();

            /// Get index-coordinates of each sensor from DetId
            uint32_t iSide0   = detId0.side();
            uint32_t iDisk0   = detId0.disk();
            uint32_t iRing0   = detId0.ring();
            uint32_t iModule0 = detId0.module();
            uint32_t iSide1   = detId1.side();
            uint32_t iDisk1   = detId1.disk();
            uint32_t iRing1   = detId1.ring();
            uint32_t iModule1 = detId1.module();

            uint32_t moduleId0 = 10000*((iSide0 == 2) ? iDisk0 : iDisk0+7) + 100*(iRing0-1) + (iModule0-1)/2;
            uint32_t moduleId1 = 10000*((iSide1 == 2) ? iDisk1 : iDisk1+7) + 100*(iRing1-1) + (iModule1-1)/2;

            if (verbose_ > 1) {
                std::cout << "Side:  " << iSide <<  " from Sides:   " << iSide0 << " " << iSide1 << std::endl;
                std::cout << "Stack: " << iStack << " from Disks:   " << iDisk0 << " " << iDisk1 << std::endl;
                std::cout << "Ring:  " << iRing <<  " from Rings:   " << iRing0 << " " << iRing1 << std::endl;
                std::cout << "Phi:   " << iPhi <<   " from Modules: " << iModule0 << " " << iModule1 << " >>> " << (iModule0 == iModule1 + 1) << std::endl;
                std::cout << "modId: " << moduleId0 << " " << moduleId1 << std::endl;
            }

            if (moduleId0ToGeoId.find(moduleId0) == moduleId0ToGeoId.end() )
                moduleId0ToGeoId.insert(std::make_pair(moduleId0, geoId0) );
            else
                std::cout << "Error: This pair already exists in map! moduleId0: " << moduleId0 << " geoId0: " << geoId0 << " existing value in map: " << moduleId0ToGeoId.at(moduleId0) << std::endl;

            if (moduleId1ToGeoId.find(moduleId1) == moduleId1ToGeoId.end() )
                moduleId1ToGeoId.insert(std::make_pair(moduleId1, geoId1) );
            else
                std::cout << "Error: This pair already exists in map! moduleId1: " << moduleId1 << " geoId1: " << geoId1 << " existing value in map: " << moduleId1ToGeoId.at(moduleId1) << std::endl;

            if (moduleId0ToStackId.find(moduleId0) == moduleId0ToStackId.end() )
                moduleId0ToStackId.insert(std::make_pair(moduleId0, stackDetId.rawId()) );
            else
                std::cout << "Error: This pair already exists in map! moduleId0: " << moduleId0 << " stackId: " << stackDetId.rawId() << " existing value in map: " << moduleId0ToStackId.at(moduleId0) << std::endl;

            if (moduleId1ToStackId.find(moduleId1) == moduleId1ToStackId.end() )
                moduleId1ToStackId.insert(std::make_pair(moduleId1, stackDetId.rawId()) );
            else
                std::cout << "Error: This pair already exists in map! moduleId1: " << moduleId1 << " stackId: " << stackDetId.rawId() << " existing value in map: " << moduleId1ToStackId.at(moduleId1) << std::endl;


            /// Find pixel topology related information
            const PixelGeomDetUnit* pixUnit0 = dynamic_cast<const PixelGeomDetUnit*>(detUnit0);
            //const PixelGeomDetUnit* pixUnit1 = dynamic_cast<const PixelGeomDetUnit*>(detUnit1);
            const PixelTopology* pixTopo0 = dynamic_cast<const PixelTopology*>(&(pixUnit0->specificTopology()) );
            //const PixelTopology* pixTopo1 = dynamic_cast<const PixelTopology*>(&(pixUnit1->specificTopology()) );

            for (int col=0; col!=pixTopo0->ncolumns(); ++col) {
                for (int row=0; row!=pixTopo0->nrows(); ++row) {
                    MeasurementPoint mp(row + 0.5, col + 0.5);
                    GlobalPoint gp = detUnit0->surface().toGlobal(pixTopo0->localPosition(mp) );
                    double signed_r = gp.y() >= 0 ? gp.perp() : -gp.perp();

                    if (theStackedGeometry->isPSModule(stackDetId)) {
                        hPXF_PS_ZR->Fill(gp.z()*10., signed_r*10.);
                        hPXF_PS_XY->Fill(gp.x()*10., gp.y()*10.);
                        hPXF_PS_UV->Fill(2e5*0.1*gp.x()/gp.perp2(), 2e5*0.1*gp.y()/gp.perp2());
                    } else {
                        hPXF_2S_ZR->Fill(gp.z()*10., signed_r*10.);
                        hPXF_2S_XY->Fill(gp.x()*10., gp.y()*10.);
                        hPXF_2S_UV->Fill(2e5*0.1*gp.x()/gp.perp2(), 2e5*0.1*gp.y()/gp.perp2());
                    }
                }
            }
        }  // end if endcap
    }
}

void AnalyzerPatternGeomInfo::endRun(const edm::Run& iRun, const edm::EventSetup& iSetup) {}


// Here we read the patterns and setup histograms
void AnalyzerPatternGeomInfo::beginJob() {
    /// Read superstrip id
    TFile* tfile = TFile::Open(bankfile_.c_str());
    TTree* ttree = (TTree*) tfile->Get("patternBank");

    std::vector<unsigned> * superstripIdsForBranch = 0;
    ttree->SetBranchAddress("superstripIds", &superstripIdsForBranch);

    Long64_t nentries = ttree->GetEntries();
    for (Long64_t ievt=0; ievt<nentries; ++ievt) {
        ttree->GetEntry(ievt);
        unsigned nsuperstrips = superstripIdsForBranch->size();

        std::vector<uint32_t> superstripIds_ievt;
        for (unsigned i=0; i<nsuperstrips; ++i) {
            uint32_t superstripId = superstripIdsForBranch->at(i);
            superstripIds_ievt.push_back(superstripId);
        }

        superstripIds.push_back(superstripIds_ievt);
    }
    assert((Long64_t) superstripIds.size() == nentries);
    tfile->Close();
    delete tfile;


    /// Set up histograms
    std::ostringstream histoName;
    std::ostringstream histoTitle;

    edm::Service<TFileService> fs;

    int nbinsZ = 295;
    int nbinsR = 121;
    int nbinsX = 121;
    int nbinsY = 121;
    int nbinsU = 88;
    int nbinsV = 88;
    double viewportMinZ = -2950.;
    double viewportMaxZ =  2950.;
    double viewportMinR = -1210.;
    double viewportMaxR =  1210.;
    double viewportMinX = -1210.;
    double viewportMaxX =  1210.;
    double viewportMinY = -1210.;
    double viewportMaxY =  1210.;
    double viewportMinU = -880.;
    double viewportMaxU =  880.;
    double viewportMinV = -880.;
    double viewportMaxV =  880.;

    hPXB_PS_ZR = fs->make<TH2F>("hPXB_PS_ZR", "; z [mm]; signed r [mm]", nbinsZ, viewportMinZ, viewportMaxZ, nbinsR, viewportMinR, viewportMaxR);
    hPXB_PS_ZR->Sumw2();
    hPXB_2S_ZR = fs->make<TH2F>("hPXB_2S_ZR", "; z [mm]; signed r [mm]", nbinsZ, viewportMinZ, viewportMaxZ, nbinsR, viewportMinR, viewportMaxR);
    hPXB_2S_ZR->Sumw2();
    hPXF_PS_ZR = fs->make<TH2F>("hPXF_PS_ZR", "; z [mm]; signed r [mm]", nbinsZ, viewportMinZ, viewportMaxZ, nbinsR, viewportMinR, viewportMaxR);
    hPXF_PS_ZR->Sumw2();
    hPXF_2S_ZR = fs->make<TH2F>("hPXF_2S_ZR", "; z [mm]; signed r [mm]", nbinsZ, viewportMinZ, viewportMaxZ, nbinsR, viewportMinR, viewportMaxR);
    hPXF_2S_ZR->Sumw2();

    hPXB_PS_XY = fs->make<TH2F>("hPXB_PS_XY", "; x [mm]; y [mm]", nbinsX, viewportMinX, viewportMaxX, nbinsY, viewportMinY, viewportMaxY);
    hPXB_PS_XY->Sumw2();
    hPXB_2S_XY = fs->make<TH2F>("hPXB_2S_XY", "; x [mm]; y [mm]", nbinsX, viewportMinX, viewportMaxX, nbinsY, viewportMinY, viewportMaxY);
    hPXB_2S_XY->Sumw2();
    hPXF_PS_XY = fs->make<TH2F>("hPXF_PS_XY", "; x [mm]; y [mm]", nbinsX, viewportMinX, viewportMaxX, nbinsY, viewportMinY, viewportMaxY);
    hPXF_PS_XY->Sumw2();
    hPXF_2S_XY = fs->make<TH2F>("hPXF_2S_XY", "; x [mm]; y [mm]", nbinsX, viewportMinX, viewportMaxX, nbinsY, viewportMinY, viewportMaxY);
    hPXF_2S_XY->Sumw2();

    hPXB_PS_UV = fs->make<TH2F>("hPXB_PS_UV", "; u [2e5/mm]; v [2e5/mm]", nbinsU, viewportMinU, viewportMaxU, nbinsV, viewportMinV, viewportMaxV);
    hPXB_PS_UV->Sumw2();
    hPXB_2S_UV = fs->make<TH2F>("hPXB_2S_UV", "; u [2e5/mm]; v [2e5/mm]", nbinsU, viewportMinU, viewportMaxU, nbinsV, viewportMinV, viewportMaxV);
    hPXB_2S_UV->Sumw2();
    hPXF_PS_UV = fs->make<TH2F>("hPXF_PS_UV", "; u [2e5/mm]; v [2e5/mm]", nbinsU, viewportMinU, viewportMaxU, nbinsV, viewportMinV, viewportMaxV);
    hPXF_PS_UV->Sumw2();
    hPXF_2S_UV = fs->make<TH2F>("hPXF_2S_UV", "; u [2e5/mm]; v [2e5/mm]", nbinsU, viewportMinU, viewportMaxU, nbinsV, viewportMinV, viewportMaxV);
    hPXF_2S_UV->Sumw2();

    Long64_t fib_x = 0;
    Long64_t fib_ret = fib(fib_x);
    for (Long64_t ievt=0; ievt<nentries; ++ievt) {

        if (ievt == fib_ret) {
            histoName.str("");  histoName << "hPXA_ind_ZR_" << ievt;
            histoTitle.str(""); histoTitle << "; z [mm]; signed r [mm]";
            vecPXA_ind_ZR.push_back( fs->make<TH2F>(histoName.str().c_str(), histoTitle.str().c_str(), nbinsZ, viewportMinZ, viewportMaxZ, nbinsR, viewportMinR, viewportMaxR) );
            vecPXA_ind_ZR.back()->Sumw2();

            histoName.str("");  histoName << "hPXA_sum_ZR_" << ievt;
            histoTitle.str(""); histoTitle << "; z [mm]; signed r [mm]";
            vecPXA_sum_ZR.push_back( fs->make<TH2F>(histoName.str().c_str(), histoTitle.str().c_str(), nbinsZ, viewportMinZ, viewportMaxZ, nbinsR, viewportMinR, viewportMaxR) );
            vecPXA_sum_ZR.back()->Sumw2();

            histoName.str("");  histoName << "hPXB_ind_XY_" << ievt;
            histoTitle.str(""); histoTitle << "; x [mm]; y [mm]";
            vecPXB_ind_XY.push_back( fs->make<TH2F>(histoName.str().c_str(), histoTitle.str().c_str(), nbinsX, viewportMinX, viewportMaxX, nbinsY, viewportMinY, viewportMaxY) );
            vecPXB_ind_XY.back()->Sumw2();

            histoName.str("");  histoName << "hPXB_sum_XY_" << ievt;
            histoTitle.str(""); histoTitle << "; x [mm]; y [mm]";
            vecPXB_sum_XY.push_back( fs->make<TH2F>(histoName.str().c_str(), histoTitle.str().c_str(), nbinsX, viewportMinX, viewportMaxX, nbinsY, viewportMinY, viewportMaxY) );
            vecPXB_sum_XY.back()->Sumw2();

            histoName.str("");  histoName << "hPXF_ind_XY_" << ievt;
            histoTitle.str(""); histoTitle << "; x [mm]; y [mm]";
            vecPXF_ind_XY.push_back( fs->make<TH2F>(histoName.str().c_str(), histoTitle.str().c_str(), nbinsX, viewportMinX, viewportMaxX, nbinsY, viewportMinY, viewportMaxY) );
            vecPXF_ind_XY.back()->Sumw2();

            histoName.str("");  histoName << "hPXF_sum_XY_" << ievt;
            histoTitle.str(""); histoTitle << "; x [mm]; y [mm]";
            vecPXF_sum_XY.push_back( fs->make<TH2F>(histoName.str().c_str(), histoTitle.str().c_str(), nbinsX, viewportMinX, viewportMaxX, nbinsY, viewportMinY, viewportMaxY) );
            vecPXF_sum_XY.back()->Sumw2();

            histoName.str("");  histoName << "hPXB_ind_UV_" << ievt;
            histoTitle.str(""); histoTitle << "; u [1/mm]; v [1/mm]";
            vecPXB_ind_UV.push_back( fs->make<TH2F>(histoName.str().c_str(), histoTitle.str().c_str(), nbinsU, viewportMinU, viewportMaxU, nbinsV, viewportMinV, viewportMaxV) );
            vecPXB_ind_UV.back()->Sumw2();

            histoName.str("");  histoName << "hPXB_sum_UV_" << ievt;
            histoTitle.str(""); histoTitle << "; u [1/mm]; v [1/mm]";
            vecPXB_sum_UV.push_back( fs->make<TH2F>(histoName.str().c_str(), histoTitle.str().c_str(), nbinsU, viewportMinU, viewportMaxU, nbinsV, viewportMinV, viewportMaxV) );
            vecPXB_sum_UV.back()->Sumw2();

            histoName.str("");  histoName << "hPXF_ind_UV_" << ievt;
            histoTitle.str(""); histoTitle << "; u [1/mm]; v [1/mm]";
            vecPXF_ind_UV.push_back( fs->make<TH2F>(histoName.str().c_str(), histoTitle.str().c_str(), nbinsU, viewportMinU, viewportMaxU, nbinsV, viewportMinV, viewportMaxV) );
            vecPXF_ind_UV.back()->Sumw2();

            histoName.str("");  histoName << "hPXF_sum_UV_" << ievt;
            histoTitle.str(""); histoTitle << "; u [1/mm]; v [1/mm]";
            vecPXF_sum_UV.push_back( fs->make<TH2F>(histoName.str().c_str(), histoTitle.str().c_str(), nbinsU, viewportMinU, viewportMaxU, nbinsV, viewportMinV, viewportMaxV) );
            vecPXF_sum_UV.back()->Sumw2();

            // Increment
            fib_x += 1;
            fib_ret = fib(fib_x);
        }
    }
}

void AnalyzerPatternGeomInfo::endJob() {}


// ANALYZE
void AnalyzerPatternGeomInfo::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
    if (verbose_ > 2) {
        for (auto i: moduleId0ToStackId)
            std::cout << i.first << " " << i.second << std::endl;
        for (auto i: moduleId0ToGeoId)
            std::cout << i.first << " " << i.second << std::endl;
    }

    Long64_t nentries = superstripIds.size();

    Long64_t fib_x = 0;
    Long64_t fib_ret = fib(fib_x);
    for (Long64_t ievt=0; ievt<nentries; ++ievt) {
        unsigned nsuperstrips = superstripIds.at(ievt).size();

        for (unsigned i=0; i<nsuperstrips; ++i) {
            uint32_t superstripId = superstripIds.at(ievt).at(i);
            if (superstripId == 0)  continue;
            if (superstripId == 4294967295)  continue;  // fake superstrip

            // Always check whether the decoding is correct!
            uint32_t moduleId = (superstripId >> 14) & 0x3FFFF;
            uint32_t col = (superstripId >> 9 ) & 0x1F;
            uint32_t row = (superstripId >> 0 ) & 0x1FF;

            if (verbose_ > 0) {
                std::cout << "ievt: " << ievt << " superstripId: " << superstripId << "  moduleId: " << moduleId << " col: "  << col << " row: " << row << std::endl;
            }

            uint32_t stackId = moduleId0ToStackId.at(moduleId);
            //uint32_t geoId = moduleId0ToGeoId.at(moduleId);

            StackedTrackerDetId stackDetId(stackId);
            //const GeomDet* det0 = theStackedGeometry->idToDet(stackDetId, 0);
            //const GeomDet* det1 = theStackedGeometry->idToDet(stackDetId, 1);
            const GeomDetUnit* detUnit0 = theStackedGeometry->idToDetUnit(stackDetId, 0);
            const GeomDetUnit* detUnit1 = theStackedGeometry->idToDetUnit(stackDetId, 1);

            const PixelGeomDetUnit* pixUnit0 = dynamic_cast<const PixelGeomDetUnit*>(detUnit0);
            //const PixelGeomDetUnit* pixUnit1 = dynamic_cast<const PixelGeomDetUnit*>(detUnit1);
            const PixelTopology* pixTopo0 = dynamic_cast<const PixelTopology*>(&(pixUnit0->specificTopology()) );
            //const PixelTopology* pixTopo1 = dynamic_cast<const PixelTopology*>(&(pixUnit1->specificTopology()) );

            MeasurementPoint mp(row + 0.5, col + 0.5);
            GlobalPoint gp = detUnit0->surface().toGlobal(pixTopo0->localPosition(mp) );
            double signed_r = gp.y() >= 0 ? gp.perp() : -gp.perp();

            // Individual event display
            if (ievt == fib_ret) {
                vecPXA_ind_ZR.at(fib_x)->Fill(gp.z()*10., signed_r*10.);

                if (stackDetId.isBarrel()) {
                    vecPXB_ind_XY.at(fib_x)->Fill(gp.x()*10., gp.y()*10.);
                    vecPXB_ind_UV.at(fib_x)->Fill(2e5*0.1*gp.x()/gp.perp2(), 2e5*0.1*gp.y()/gp.perp2());
                }

                if (stackDetId.isEndcap()) {
                    vecPXF_ind_XY.at(fib_x)->Fill(gp.x()*10., gp.y()*10.);
                    vecPXF_ind_UV.at(fib_x)->Fill(2e5*0.1*gp.x()/gp.perp2(), 2e5*0.1*gp.y()/gp.perp2());
                }
            }

            // Sum event display
            vecPXA_sum_ZR.at(fib_x)->Fill(gp.z()*10., signed_r*10.);

            if (stackDetId.isBarrel()) {
                vecPXB_sum_XY.at(fib_x)->Fill(gp.x()*10., gp.y()*10.);
                vecPXB_sum_UV.at(fib_x)->Fill(2e5*0.1*gp.x()/gp.perp2(), 2e5*0.1*gp.y()/gp.perp2());
            }

            if (stackDetId.isEndcap()) {
                vecPXF_sum_XY.at(fib_x)->Fill(gp.x()*10., gp.y()*10.);
                vecPXF_sum_UV.at(fib_x)->Fill(2e5*0.1*gp.x()/gp.perp2(), 2e5*0.1*gp.y()/gp.perp2());
            }
        }

        if (ievt == fib_ret) {
            if (ievt != 0) {
                vecPXA_sum_ZR.at(fib_x)->Add(vecPXA_sum_ZR.at(fib_x-1));

                vecPXB_sum_XY.at(fib_x)->Add(vecPXB_sum_XY.at(fib_x-1));
                vecPXB_sum_UV.at(fib_x)->Add(vecPXB_sum_UV.at(fib_x-1));

                vecPXF_sum_XY.at(fib_x)->Add(vecPXF_sum_XY.at(fib_x-1));
                vecPXF_sum_UV.at(fib_x)->Add(vecPXF_sum_UV.at(fib_x-1));

                if (verbose_ > 1) {
                    std::cout << "ievt: " << ievt << " fib_x: " << fib_x << " integral ind: " << vecPXA_ind_ZR.at(fib_x)->Integral() << " sum: " << vecPXA_sum_ZR.at(fib_x)->Integral() << std::endl;
                }
            }

            // Increment
            fib_x += 1;
            fib_ret = fib(fib_x);
        }
        if (fib_x == (Long64_t) vecPXF_sum_UV.size()) {
            // truncate the following events
            break;
        }
    }
}

// DEFINE THIS AS A PLUG-IN
#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(AnalyzerPatternGeomInfo);
