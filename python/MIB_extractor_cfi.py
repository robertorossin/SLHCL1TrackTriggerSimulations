import FWCore.ParameterSet.Config as cms

MIBextraction = cms.EDAnalyzer("RecoExtractor",


##
## First you define the name of the  output ROOTfile
##
                               
  extractedRootFile = cms.string('extracted.root'),


##
## Then the name of the input ROOTfile, if you start from already extracted file
##
                               
  inputRootFile     = cms.string('default.root'),
                               

##
## Here you tell is you start from a RECO file (True) or an extracted ROOTuple (False)
##

  fillTree = cms.untracked.bool(True),
                               
## Then the different info you want in (set to false by default)

  # Main stuff
  #doINFO           = cms.untracked.bool(True),      
  #doEvent          = cms.untracked.bool(True),
  #L1digi_tag       = cms.InputTag( "gtDigis" ),                           

  doMC             = cms.untracked.bool(False),
  doMatch          = cms.untracked.bool(False),                               

  # Put all data or just part of it
  #skimData         = cms.untracked.bool(False),
                
  # Add Pixel information                              
  doPixel          = cms.untracked.bool(False),
  pixel_tag        = cms.InputTag( "siPixelClusters" ),
                               
  doTranslation    = cms.untracked.bool(False), 
  doL1TT           = cms.untracked.bool(False),

  n_events         = cms.untracked.int32(10),  # How many events you want to analyze (only if fillTree=False)
  skip_events      = cms.untracked.int32(0),  # How many events you want to skip (only if fillTree=False)

  # The analysis settings could be whatever you want
  # 
  # Format is "STRING VALUE" where STRING is the name of the cut, and VALUE the value of the cut

  # Here we define for example the cuts for the dimuon analysis
  analysisSettings = cms.untracked.vstring()

  # Add Tracker information                              
  #doTracker        = cms.untracked.bool(False),
  #tracker_tag      = cms.InputTag( "siStripClusters" ),
     
  # Add HF information                              
  #doHF             = cms.untracked.bool(False),
  #HF_tag           = cms.InputTag( "hfreco" ),
          
  # Add PV information
  #doVertices       = cms.untracked.bool(False),
  #vertex_tag       = cms.InputTag( "pixelVertices" ),

  # Add track information
  #doTracks         = cms.untracked.bool(False),                               
  #track_tag    = cms.InputTag( "generalTracks" )                               

)
