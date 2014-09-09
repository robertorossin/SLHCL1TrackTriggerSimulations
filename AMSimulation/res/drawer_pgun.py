#!/usr/bin/env python

from rootdrawing import *

# ______________________________________________________________________________
# Configurations

samples = {}
samples["nu140"        ] = False
samples["mu140"        ] = False
samples["minbias"      ] = True
samples["minbias_remix"] = False

sections = {}
sections["genParts"   ] = False
sections["simTracks"  ] = False
sections["simVertices"] = False
sections["trkParts"   ] = False
sections["trkVertices"] = False
sections["eventInfo"  ] = False
sections["clusters"   ] = False
sections["stubs"      ] = False
sections["minbias1"   ] = False
sections["minbias140" ] = True
sections["fixme"      ] = False

drawerInit = DrawerInit()

# Muon gun
#infiles = ["/uscms_data/d2/jiafu/L1TrackTrigger/CRAB_amsim/ntuple_1_1_1TX_redux.root"]
infiles = ["/eos/uscms/store/user/l1upgrades/SLHC/GEN/620_SLHC12p1/SingleMuMinus_Barrel_20140821/SingleMu_Barrel_E2023TTI_ntuple_1_1_Fr2.root", "/eos/uscms/store/user/l1upgrades/SLHC/GEN/620_SLHC12p1/SingleMuPlus_Barrel_20140821/SingleMu_Barrel_E2023TTI_ntuple_1_1_guT.root"]

col  = TColor.GetColor("#1f78b4")  # Paired
fcol = TColor.GetColor("#a6cee3")
imgdir = "figures_pgun/"

# Neutrino gun (140PU)
if samples["nu140"]:
    infiles = ["/eos/uscms/store/user/l1upgrades/SLHC/GEN/620_SLHC12p1_ntuple/Neutrino_Pt2to20_gun_20140821/ntuple_1_1_9cK.root", "/eos/uscms/store/user/l1upgrades/SLHC/GEN/620_SLHC12p1_ntuple/Neutrino_Pt2to20_gun_20140821/ntuple_2_1_HJw.root"]
    col  = TColor.GetColor("#e31a1c")  # Paired
    fcol = TColor.GetColor("#fb9a99")
    imgdir = "figures_pgun_nu140/"

# Muon gun (140PU)
if samples["mu140"]:
    infiles = ["/eos/uscms/store/user/l1upgrades/SLHC/GEN/620_SLHC12p1_ntuple/SingleMuMinusFlatPt0p2To150_20140821/ntuple_1_1_dzk.root", "/eos/uscms/store/user/l1upgrades/SLHC/GEN/620_SLHC12p1_ntuple/SingleMuMinusFlatPt0p2To150_20140821/ntuple_2_1_j6Z.root"]
    col  = TColor.GetColor("#6a3d9a")  # Paired
    fcol = TColor.GetColor("#cab2d6")
    imgdir = "figures_pgun_mu140/"

# MinBias
if samples["minbias"]:
    infiles = get_infiles("/uscms_data/d2/jiafu/L1TrackTrigger/CRAB_amsim/input_MinBias_20140821.txt", fast=False)
    col  = TColor.GetColor("#ff7f00")  # Paired
    fcol = TColor.GetColor("#fdbf6f")
    imgdir = "figures_pgun_minbias/"

if samples["minbias_remix"]:
    infiles = ["/uscms_data/d2/jiafu/L1TrackTrigger/CRAB_amsim/Neutrino_E2023TTI_PU140_ntuple.0.root"]
    col  = TColor.GetColor("#ff7f00")  # Paired
    fcol = TColor.GetColor("#fdbf6f")
    imgdir = "figures_pgun_minbias/"


# Number of events
#nentries = 139673
#nentries = 200000
nentries = 1000

chain = TChain("ntupler/tree", "")
for f in infiles:
    chain.Add(f)
tree = chain

if not imgdir.endswith("/"):  imgdir += "/"
if gSystem.AccessPathName(imgdir):
    gSystem.mkdir(imgdir)


# ______________________________________________________________________________
# genParticles

if sections["genParts"]:
    def doit(x, imgdir=None, logy=False):
        histos = book(x)
        project(tree, histos, nentries=nentries)
        draw(histos, logy=logy)
        save(imgdir, x[0].name)
        return histos

    cut = ""

    p0 = struct("genParts_size", "genParts_size" if not cut else "Sum$(%s)" % cut, "",
        "# gen particles", 4, 0, 4, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("genParts_pt", "genParts_pt", cut,
        "gen p_{T} [GeV]", 50, 0, 200, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("genParts_oneoverpt", "1.0/genParts_pt", cut,
        "gen 1/p_{T} [1/GeV]", 50, 0, 0.5, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("genParts_eta", "genParts_eta", cut,
        "gen #eta", 60, -3, 3, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("genParts_phi", "genParts_phi", cut,
        "gen #phi [rad]", 64, -3.2, 3.2, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("genParts_E", "genParts_E", cut,
        "gen energy [GeV]", 50, 0, 1500, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("genParts_M", "genParts_M", cut,
        "gen mass [GeV]", 50, 0, 0.5, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("genParts_vx", "genParts_vx", cut,
        "gen vertex x_{0} [cm]", 50, -0.01, 0.01, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("genParts_vy", "genParts_vy", cut,
        "gen vertex y_{0} [cm]", 50, -0.01, 0.01, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("genParts_vz", "genParts_vz", cut,
        "gen vertex z_{0} [cm]", 60, -30, 30, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("genParts_pdgId", "genParts_pdgId", cut,
        "gen PDG id", 50, -25, 25, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("genParts_charge", "genParts_charge", cut,
        "gen charge", 5, -2.5, 2.5, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("genParts_status", "genParts_status", cut,
        "gen status", 5, 0, 5, col, fcol)
    histos = doit([p0], imgdir)


# ______________________________________________________________________________
# simTracks

if sections["simTracks"]:
    def doit(x, imgdir=None, logy=False):
        histos = book(x)
        project(tree, histos, nentries)
        draw(histos, logy=logy)
        save(imgdir, x[0].name)
        return histos

    cut = "simTracks_trkId==1"

    p0 = struct("simTracks_size", "simTracks_size" if not cut else "Sum$(%s)" % cut, "",
        "# simTracks", 50, 0, 100, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("simTracks_pt", "simTracks_pt", cut,
        "simTrack p_{T} [GeV]", 50, 0, 200, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("simTracks_oneoverpt", "simTracks_pt==0 ? 9999 : 1.0/simTracks_pt", cut,
        "simTrack 1/p_{T} [1/GeV]", 50, 0, 0.5, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("simTracks_eta", "simTracks_eta", cut,
        "simTrack #eta", 60, -3, 3, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("simTracks_phi", "simTracks_phi", cut,
        "simTrack #phi [rad]", 64, -3.2, 3.2, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("simTracks_pdgId", "simTracks_pdgId", cut,
        "simTrack PDG id", 50, -25, 25, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("simTracks_evtId", "simTracks_evtId", cut,
        "simTrack event id", 4, 0, 4, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("simTracks_trkId", "simTracks_trkId", cut,
        "simTrack id", 50, 0, 200, col, fcol)
    histos = doit([p0], imgdir, logy=True)
    # max = 2466515

    p0 = struct("simTracks_vtxId", "simTracks_vtxId", cut,
        "simTrack vertex id", 50, 0, 100, col, fcol)
    histos = doit([p0], imgdir, logy=True)
    # max = 1560

    p0 = struct("simTracks_genpId", "simTracks_genpId", cut,
        "simTrack genParticle id", 6, -2, 4, col, fcol)
    histos = doit([p0], imgdir)

    # Special attention

    #p0 = struct("sp_simTracks_trkId_vs_pt", "simTracks_trkId:simTracks_pt" , "") ]
    #    "simTrack p_{T} [GeV]", 50, 0, 100, col, fcol)
    #ybinnings = [ ("simTrack track id", 50, 0, 200) ]
    #histos = book_scat(params, binning, ybinnings); project_scat(params, histos); ret=draw_scat(params, histos[0])
    #save(imgdir, params[0][0])


# ______________________________________________________________________________
if sections["simVertices"]:
    def doit(x, imgdir=None, logy=False):
        histos = book(x)
        project(tree, histos, nentries)
        draw(histos, logy=logy)
        save(imgdir, x[0].name)
        return histos

    cut = "simVertices_vtxId==0"

    p0 = struct("simVertices_size", "simVertices_size" if not cut else "Sum$(%s)" % cut, "",
        "# simVertices", 50, 0, 100, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("simVertices_vx", "simVertices_vx", cut,
        "simVertex x_{0} [cm]", 50, -0.01, 0.01, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("simVertices_vy", "simVertices_vy", cut,
        "simVertex y_{0} [cm]", 50, -0.01, 0.01, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("simVertices_vz", "simVertices_vz", cut,
        "simVertex z_{0} [cm]", 60, -30, 30, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("simVertices_tof", "simVertices_tof", cut,
        "simVertex time of flight [s]", 50, -5e-12, 5e-12, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("simVertices_trkId", "simVertices_trkId", cut,
        "simVertex track id", 50, 0, 200, col, fcol)
    histos = doit([p0], imgdir, logy=True)

    p0 = struct("simVertices_vtxId", "simVertices_vtxId", cut,
        "simVertex vertex id", 50, 0, 100, col, fcol)
    histos = doit([p0], imgdir, logy=True)

    #p0 = struct("simBeamSpot_vx", "simBeamSpot_bx", cut,
    #    "simBeamSpot x_{0} [cm]", 50, -0.01, 0.01, col, fcol)
    #histos = doit([p0], imgdir)
    #
    #p0 = struct("simBeamSpot_vy", "simBeamSpot_by", cut,
    #    "simBeamSpot y_{0} [cm]", 50, -0.01, 0.01, col, fcol)
    #histos = doit([p0], imgdir)
    #
    #p0 = struct("simBeamSpot_vz", "simBeamSpot_bz", cut,
    #    "simBeamSpot z_{0} [cm]", 60, -3, 3, col, fcol)
    #histos = doit([p0], imgdir)
    #
    #p0 = struct("simBeamSpot_sigmaZ", "simBeamSpot_sigmaZ", cut,
    #    "simBeamSpot #sigma_{z} [cm]", 50, 0, 0.1, col, fcol)
    #histos = doit([p0], imgdir)


# ______________________________________________________________________________
if sections["trkParts"]:
    def doit(x, imgdir=None, logy=False):
        histos = book(x)
        project(tree, histos, nentries)
        draw(histos, logy=logy)
        save(imgdir, x[0].name)
        return histos

    cut = "trkParts_trkId==1"

    p0 = struct("trkParts_size", "trkParts_size" if not cut else "Sum$(%s)" % cut, "",
        "# trkParticles", 50, 0, 100, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("trkParts_nSimTracks", "@trkParts_trkIds.size()", cut,
        "trkParticle # simTracks", 30, 0, 30, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("trkParts_pt", "trkParts_pt", cut,
        "trkParticle p_{T} [GeV]", 50, 0, 200, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("trkParts_oneoverpt", "trkParts_pt==0 ? 9999 : 1.0/trkParts_pt", cut,
        "trkParticle 1/p_{T} [1/GeV]", 50, 0, 0.5, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("trkParts_eta", "trkParts_eta", cut,
        "trkParticle #eta", 60, -3, 3, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("trkParts_phi", "trkParts_phi", cut,
        "trkParticle #phi [rad]", 64, -3.2, 3.2, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("trkParts_vx", "trkParts_vx", cut,
        "trkParticle x_{0} [cm]", 50, -0.01, 0.01, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("trkParts_vy", "trkParts_vy", cut,
        "trkParticle y_{0} [cm]", 50, -0.01, 0.01, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("trkParts_vz", "trkParts_vz", cut,
        "trkParticle z_{0} [cm]", 60, -30, 30, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("trkParts_pdgId", "trkParts_pdgId", cut,
        "trkParticle PDG id", 50, -25, 25, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("trkParts_evtId", "trkParts_evtId", cut,
        "trkParticle event id", 4, 0, 4, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("trkParts_trkId", "trkParts_trkId", cut,
        "trkParticle simTrack id", 50, 0, 200, col, fcol)
    histos = doit([p0], imgdir, logy=True)

    p0 = struct("trkParts_genpId", "trkParts_genpId", cut,
        "trkParticle genParticle id", 6, -2, 4, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("trkParts_nhits", "trkParts_nhits", cut,
        "trkParticle # hits in tracker + muon", 50, 0, 50, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("trkParts_ntkhits", "trkParts_ntkhits", cut,
        "trkParticle # hits in tracker", 50, 0, 50, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("trkParts_ntklayers", "trkParts_ntklayers", cut,
        "trkParticle # hits in tracker layers", 50, 0, 50, col, fcol)
    histos = doit([p0], imgdir)

    # Special attention

    #p0 = struct("sp_trkParts_size_vs_simTracks_size", "trkParts_size:simTracks_size" , "") ]
    #    "# simTracks", 50, 0, 100)
    #ybinnings = [ ("# trkParticles", 50, 0, 100) ]
    #histos = book_scat(params, binning, ybinnings); project_scat(params, histos); ret=draw_scat(params, histos[0])
    #save(imgdir, params[0][0])
    #
    #p0 = struct("sp_trkParts_trkId_vs_pt", "trkParts_trkIds[][0]:trkParts_pt" , "") ]
    #    "trkParticle p_{T} [GeV]", 50, 0, 100)
    #ybinnings = [ ("trkParticle simTrack id", 50, 0, 200) ]
    #histos = book_scat(params, binning, ybinnings); project_scat(params, histos); ret=draw_scat(params, histos[0])
    #save(imgdir, params[0][0])


# ______________________________________________________________________________
if sections["trkVertices"]:
    def doit(x, imgdir=None, logy=False):
        histos = book(x)
        project(tree, histos, nentries)
        draw(histos, logy=logy)
        save(imgdir, x[0].name)
        return histos

    #cut = "trkVertices_vtxId==0"
    cut = "abs(trkVertices_vx)<1 && abs(trkVertices_vy)<1"

    p0 = struct("trkVertices_size", "trkVertices_size" if not cut else "Sum$(%s)" % cut, "",
        "# trkVertices", 50, 0, 100, col, fcol)
    histos = doit([p0], imgdir)

    #p0 = struct("trkVertices_nSimVertices", "@trkVertices_vtxIds.size()", cut,  # bugged
    #    "trkVertex # simVertices", 20, 0, 20, col, fcol)
    #histos = doit([p0], imgdir)

    p0 = struct("trkVertices_vx", "trkVertices_vx", cut,
        "trkVertex x_{0} [cm]", 50, -0.01, 0.01, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("trkVertices_vy", "trkVertices_vy", cut,
        "trkVertex y_{0} [cm]", 50, -0.01, 0.01, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("trkVertices_vz", "trkVertices_vz", cut,
        "trkVertex z_{0} [cm]", 60, -30, 30, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("trkVertices_tof", "trkVertices_tof", cut,
        "trkVertex time of flight [s]", 50, -5e-12, 5e-12, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("trkVertices_vtxId", "trkVertices_vtxId", cut,  # bugged ?
        "trkVertex simVertex id", 50, 0, 100, col, fcol)
    histos = doit([p0], imgdir, logy=True)

    #p0 = struct("trkVertices_inVolume", "trkVertices_inVolume", cut,
    #    "trkVertex simVertex inside tracker?", 4, 0, 4, col, fcol)
    #histos = doit([p0], imgdir, logy=True)

    # Special attention

    #p0 = struct("sp_trkVertices_size_vs_simVertices_size", "trkVertices_size:simVertices_size" , "") ]
    #    "# simVertices", 50, 0, 100)
    #ybinnings = [ ("# trkVertices", 50, 0, 100) ]
    #histos = book_scat(params, binning, ybinnings); project_scat(params, histos); ret=draw_scat(params, histos[0])
    #save(imgdir, params[0][0])


# ______________________________________________________________________________
if sections["eventInfo"]:
    def doit(x, imgdir=None, logy=False):
        histos = book(x)
        project(tree, histos, nentries)
        draw(histos, logy=logy)
        save(imgdir, x[0].name)
        return histos

    cut = ""

    p0 = struct("event_run", "run", cut,
        "run number", 4, 0, 4, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("event_lumi", "lumi", cut,
        "lumi section", 50, 1, 101, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("event_event", "event", cut,
        "event number", 50, 1, 10001, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("event_bx", "bx", cut,
        "bunch crossing number", 4, -3, 1, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("event_orbit", "orbit", cut,
        "orbit number", 4, -3, 1, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("gen_nPV", "gen_nPV", cut,
        "sim # PV", 50, 0, 200, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("gen_trueNPV", "gen_trueNPV", cut,
        "gen # PV", 50, 0, 200, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("gen_seed", "gen_randomSeeds[0]", cut,
        "generator seed", 64, 0, 4294967296., col, fcol)
    histos = doit([p0], imgdir)


# ______________________________________________________________________________
if sections["clusters"]:
    def doit(x, imgdir=None, logy=False):
        histos = book(x)
        project(tree, histos, nentries)
        draw(histos, logy=logy)
        save(imgdir, x[0].name)
        return histos

    cut = "TTClusters_trkId==1"
    #cut = "TTClusters_isGenuine==1"

    p0 = struct("TTClusters_size", "TTClusters_size" if not cut else "Sum$(%s)" % cut, "",
        "# clusters", 60, 0, 60, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTClusters_x", "TTClusters_x", cut,
        "Cluster global pos x [cm]", 120, -120, 120, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTClusters_y", "TTClusters_y", cut,
        "Cluster global pos y [cm]", 120, -120, 120, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTClusters_z", "TTClusters_z", cut,
        "Cluster global pos z [cm]", 120, -300, 300, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTClusters_r", "TTClusters_r", cut,
        "Cluster global pos r [cm]", 60, 0, 120, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTClusters_eta", "TTClusters_eta", cut,
        "Cluster global pos #eta", 60, -3, 3, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTClusters_phi", "TTClusters_phi", cut,
        "Cluster global pos phi [rad]", 64, -3.2, 3.2, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTClusters_localx", "TTClusters_localx", cut,
        "Cluster local pos x [unit?]", 50, -5, 5, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTClusters_localy", "TTClusters_localy", cut,
        "Cluster local pos y [unit?]", 50, -5, 5, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTClusters_coordx", "TTClusters_coordx", cut,
        "Cluster local coord x", 128, 0, 1024, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTClusters_coordy", "TTClusters_coordy", cut,
        "Cluster local coord y", 33, 0, 33, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTClusters_surfx", "TTClusters_surfx", cut,
        "Cluster surface pos x [cm]", 120, -120, 120, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTClusters_surfy", "TTClusters_surfy", cut,
        "Cluster surface pos y [cm]", 120, -120, 120, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTClusters_surfz", "TTClusters_surfz", cut,
        "Cluster surface pos z [cm]", 120, -300, 300, col, fcol)
    histos = doit([p0], imgdir)

    ##
    p0 = struct("TTClusters_simPt", "TTClusters_simPt", cut,
        "Cluster simTrack p_{T} [GeV]", 50, 0, 200, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTClusters_simOneoverpt", "TTClusters_simPt==0 ? 9999 : 1.0/TTClusters_simPt", cut,
        "Cluster simTrack 1/p_{T} [1/GeV]", 50, 0, 0.5, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTClusters_simEta", "TTClusters_simEta", cut,
        "Cluster simTrack #eta", 60, -3, 3, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTClusters_simPhi", "TTClusters_simPhi", cut,
        "Cluster simTrack #phi [rad]", 64, -3.2, 3.2, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTClusters_trkId", "TTClusters_trkId", cut,
        "Cluster simTrack track id", 50, 0, 200, col, fcol)
    histos = doit([p0], imgdir, logy=True)

    p0 = struct("TTClusters_tpId", "TTClusters_tpId", cut,
        "Cluster trkParticle id", 50, 0, 200, col, fcol)
    histos = doit([p0], imgdir, logy=True)

    p0 = struct("TTClusters_pdgId", "TTClusters_pdgId", cut,
        "Cluster PDG id", 50, -25, 25, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTClusters_isGenuine", "TTClusters_isGenuine", cut,
        "Cluster is genuine", 4, 0, 4, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTClusters_isCombinatoric", "TTClusters_isCombinatoric", cut,
        "Cluster is combinatoric", 4, 0, 4, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTClusters_isUnknown", "TTClusters_isUnknown", cut,
        "Cluster is unknown", 4, 0, 4, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTClusters_modId", "TTClusters_modId", cut,
        "Cluster module id", 120, 0, 240000, col, fcol)
    histos = doit([p0], imgdir)

    #p0 = struct("TTClusters_iModLayer", "TTClusters_iModLayer", cut,
    #    "Cluster layer number", 30, 0, 30, col, fcol)
    #histos = doit([p0], imgdir)

    #p0 = struct("TTClusters_iModLadder", "TTClusters_iModLadder", cut,
    #    "Cluster ladder number", 100, 0, 100, col, fcol)
    #histos = doit([p0], imgdir)

    #p0 = struct("TTClusters_iModModule", "TTClusters_iModModule", cut,
    #    "Cluster module number", 100, 0, 200, col, fcol)
    #histos = doit([p0], imgdir)

    p0 = struct("TTClusters_iModPitchX", "TTClusters_iModPitchX", cut,
        "Cluster pitch x [unit?]", 10, 0, 10, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTClusters_iModPitchY", "TTClusters_iModPitchY", cut,
        "Cluster pitch y [unit?]", 10, 0, 10, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTClusters_iModCols", "TTClusters_iModCols", cut,
        "Cluster # columns in module", 33, 0, 32+1, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTClusters_iModRows", "TTClusters_iModRows", cut,
        "Cluster # rows in module", 100, 950, 1050, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTClusters_nhits", "TTClusters_nhits", cut,
        "Cluster # hits", 20, 0, 20, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTClusters_hitXs", "TTClusters_hitXs", cut,
        "Cluster hit global pos x (for each hit) [cm]", 120, -120, 120, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTClusters_hitYs", "TTClusters_hitYs", cut,
        "Cluster hit global pos y (for each hit) [cm]", 120, -120, 120, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTClusters_hitZs", "TTClusters_hitZs", cut,
        "Cluster hit global pos z (for each hit) [cm]", 120, -300, 300, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTClusters_hitADCs", "TTClusters_hitADCs", cut,
        "Cluster hit ADC count (for each hit)", 64, 0, 256, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTClusters_hitChans", "TTClusters_hitChans", cut,
        "Cluster hit channel id (for each hit)", 60, 0, 2400000, col, fcol)
    histos = doit([p0], imgdir)

    #p0 = struct("TTClusters_hitCols", "TTClusters_hitCols", cut,
    #    "Cluster hit column number (for each hit)", 33, 0, 32+1, col, fcol)
    #histos = doit([p0], imgdir)

    #p0 = struct("TTClusters_hitRows", "TTClusters_hitRows", cut,
    #    "Cluster hit row number (for each hit)", 128, 0, 1024, col, fcol)
    #histos = doit([p0], imgdir)

    #p0 = struct("TTClusters_hitFracs", "TTClusters_hitFracs", cut,
    #    "Cluster hit charge fraction (for each hit)", 100, 0, 1, col, fcol)
    #histos = doit([p0], imgdir)

    p0 = struct("TTClusters_hitTrkIds", "TTClusters_hitTrkIds", cut,
        "Cluster hit simTrack track id (for each hit)", 50, 0, 200, col, fcol)
    histos = doit([p0], imgdir, logy=True)

    p0 = struct("TTClusters_hitEvtIds", "TTClusters_hitEvtIds", cut,
        "Cluster hit simTrack event id (for each hit)", 4, 0, 4, col, fcol)
    histos = doit([p0], imgdir, logy=True)

    p0 = struct("TTClusters_barrel", "TTClusters_barrel", cut,
        "Cluster is in barrel", 4, 0, 4, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTClusters_psmodule", "TTClusters_psmodule", cut,
        "Cluster is a PS module", 4, 0, 4, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTClusters_stackId", "TTClusters_stackId", cut,
        "Cluster CMSSW module id", 150, 5E8, 5.3E8, col, fcol)
    histos = doit([p0], imgdir)

    ##
    p0 = struct("TTClusters_geoId", "TTClusters_geoId", cut,
        "Cluster geographical id", 150, 3e8, 3.6e8, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTClusters_stack", "TTClusters_stack", cut,
        "Cluster stack member number", 4, 0 ,4, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTClusters_width", "TTClusters_width", cut,
        "Cluster width in xy", 16, 0 ,16, col, fcol)
    histos = doit([p0], imgdir)


# ______________________________________________________________________________
if sections["stubs"]:
    def doit(x, imgdir=None, logy=False):
        histos = book(x)
        project(tree, histos, nentries)
        draw(histos, logy=logy)
        save(imgdir, x[0].name)
        return histos

    cut = "TTStubs_trkId==1"
    #cut = "TTStubs_isGenuine==1"

    p0 = struct("TTStubs_size", "TTStubs_size" if not cut else "Sum$(%s)" % cut, "",
        "# stubs", 30, 0, 30, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTStubs_x", "TTStubs_x", cut,
        "Stub global pos x [cm]", 120, -120, 120, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTStubs_y", "TTStubs_y", cut,
        "Stub global pos y [cm]", 120, -120, 120, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTStubs_z", "TTStubs_z", cut,
        "Stub global pos z [cm]", 120, -300, 300, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTStubs_r", "TTStubs_r", cut,
        "Stub global pos r [cm]", 60, 0, 120, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTStubs_eta", "TTStubs_eta", cut,
        "Stub global pos #eta", 60, -3, 3, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTStubs_phi", "TTStubs_phi", cut,
        "Stub global pos phi [rad]", 64, -3.2, 3.2, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTStubs_dirx", "TTStubs_dirx", cut,
        "Stub global dir x", 100, -2.5, 2.5, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTStubs_diry", "TTStubs_diry", cut,
        "Stub global dir y", 100, -2.5, 2.5, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTStubs_dirz", "TTStubs_dirz", cut,
        "Stub global dir z", 100, -2.5, 2.5, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTStubs_roughPt", "TTStubs_roughPt", cut,
        "Stub 'rough' p_{T} [GeV]", 100, 0, 100, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTStubs_coordx", "TTStubs_coordx", cut,
        "Stub local coord x", 128, 0, 1024, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTStubs_coordy", "TTStubs_coordy", cut,
        "Stub local coord y", 33, 0, 33, col, fcol)
    histos = doit([p0], imgdir)

    ##
    p0 = struct("TTStubs_simPt", "TTStubs_simPt", cut,
        "Stub simTrack p_{T} [GeV]", 50, 0, 200, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTStubs_simOneoverpt", "TTStubs_simPt==0 ? 9999 : 1.0/TTStubs_simPt", cut,
        "Stub simTrack 1/p_{T} [1/GeV]", 50, 0, 0.5, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTStubs_simEta", "TTStubs_simEta", cut,
        "Stub simTrack #eta", 60, -3, 3, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTStubs_simPhi", "TTStubs_simPhi", cut,
        "Stub simTrack #phi [rad]", 64, -3.2, 3.2, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTStubs_trkId", "TTStubs_trkId", cut,
        "Stub simTrack track id", 50, 0, 200, col, fcol)
    histos = doit([p0], imgdir, logy=True)

    p0 = struct("TTStubs_tpId", "TTStubs_tpId", cut,
        "Stub trkParticle id", 50, 0, 200, col, fcol)
    histos = doit([p0], imgdir, logy=True)

    p0 = struct("TTStubs_pdgId", "TTStubs_pdgId", cut,
        "Stub PDG id", 50, -25, 25, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTStubs_isGenuine", "TTStubs_isGenuine", cut,
        "Stub is genuine", 4, 0, 4, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTStubs_isCombinatoric", "TTStubs_isCombinatoric", cut,
        "Stub is combinatoric", 4, 0, 4, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTStubs_isUnknown", "TTStubs_isUnknown", cut,
        "Stub is unknown", 4, 0, 4, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTStubs_modId", "TTStubs_modId", cut,
        "Stub module id", 120, 0, 240000, col, fcol)
    histos = doit([p0], imgdir)

    #p0 = struct("TTStubs_iModLayer", "TTStubs_iModLayer", cut,
    #    "Stub layer number", 30, 0, 30, col, fcol)
    #histos = doit([p0], imgdir)

    #p0 = struct("TTStubs_iModLadder", "TTStubs_iModLadder", cut,
    #    "Stub ladder number", 100, 0, 100, col, fcol)
    #histos = doit([p0], imgdir)

    #p0 = struct("TTStubs_iModModule", "TTStubs_iModModule", cut,
    #    "Stub module number", 100, 0, 200, col, fcol)
    #histos = doit([p0], imgdir)

    p0 = struct("TTStubs_iModPitchX", "TTStubs_iModPitchX", cut,
        "Stub pitch x [unit?]", 10, 0, 10, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTStubs_iModPitchY", "TTStubs_iModPitchY", cut,
        "Stub pitch y [unit?]", 10, 0, 10, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTStubs_iModCols", "TTStubs_iModCols", cut,
        "Stub # columns in module", 33, 0, 32+1, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTStubs_iModRows", "TTStubs_iModRows", cut,
        "Stub # rows in module", 100, 950, 1050, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTStubs_nhits", "TTStubs_nhits", cut,
        "Stub # hits", 20, 0, 20, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTStubs_hitXs", "TTStubs_hitXs", cut,
        "Stub hit global pos x (for each hit) [cm]", 120, -120, 120, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTStubs_hitYs", "TTStubs_hitYs", cut,
        "Stub hit global pos y (for each hit) [cm]", 120, -120, 120, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTStubs_hitZs", "TTStubs_hitZs", cut,
        "Stub hit global pos z (for each hit) [cm]", 120, -300, 300, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTStubs_hitADCs", "TTStubs_hitADCs", cut,
        "Stub hit ADC count (for each hit)", 64, 0, 256, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTStubs_hitChans", "TTStubs_hitChans", cut,
        "Stub hit channel id (for each hit)", 60, 0, 2400000, col, fcol)
    histos = doit([p0], imgdir)

    #p0 = struct("TTStubs_hitCols", "TTStubs_hitCols", cut,
    #    "Stub hit column number (for each hit)", 33, 0, 32+1, col, fcol)
    #histos = doit([p0], imgdir)

    #p0 = struct("TTStubs_hitRows", "TTStubs_hitRows", cut,
    #    "Stub hit row number (for each hit)", 128, 0, 1024, col, fcol)
    #histos = doit([p0], imgdir)

    #p0 = struct("TTStubs_hitFracs", "TTStubs_hitFracs", cut,
    #    "Stub hit charge fraction (for each hit)", 100, 0, 1, col, fcol)
    #histos = doit([p0], imgdir)

    p0 = struct("TTStubs_hitTrkIds", "TTStubs_hitTrkIds", cut,
        "Stub hit simTrack track id (for each hit)", 50, 0, 200, col, fcol)
    histos = doit([p0], imgdir, logy=True)

    p0 = struct("TTStubs_hitEvtIds", "TTStubs_hitEvtIds", cut,
        "Stub hit simTrack event id (for each hit)", 4, 0, 4, col, fcol)
    histos = doit([p0], imgdir, logy=True)

    p0 = struct("TTStubs_barrel", "TTStubs_barrel", cut,
        "Stub is in barrel", 4, 0, 4, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTStubs_psmodule", "TTStubs_psmodule", cut,
        "Stub is a PS module", 4, 0, 4, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTStubs_stackId", "TTStubs_stackId", cut,
        "Stub CMSSW module id", 150, 5E8, 5.3E8, col, fcol)
    histos = doit([p0], imgdir)

    ##
    p0 = struct("TTStubs_geoId0", "TTStubs_geoId0", cut,
        "Stub inner cluster geographical id", 150, 3e8, 3.6e8, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTStubs_geoId1", "TTStubs_geoId1", cut,
        "Stub outer cluster geographical id", 150, 3e8, 3.6e8, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTStubs_clusId0", "TTStubs_clusId0", cut,
        "Stub inner cluster index in the ntuple", 60, 0, 300, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTStubs_clusId1", "TTStubs_clusId1", cut,
        "Stub outer cluster index in the ntuple", 60, 0, 300, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTStubs_trigDist", "TTStubs_trigDist", cut,
        "Stub trigger displacement [strip unit]", 20, -10, 10, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTStubs_trigOffset", "TTStubs_trigOffset", cut,
        "Stub trigger offset [strip unit]", 10, -5, 5, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTStubs_trigPos", "TTStubs_trigPos", cut,
        "Stub trigger local coord x [strip unit]", 128, 0, 1024, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTStubs_trigBend", "TTStubs_trigBend", cut,
        "Stub trigger bend [strip unit]", 20, -10, 10, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTStubs_separation", "TTStubs_separation", cut,
        "Stub separation [#Delta R or #Delta z]", 50, 0, 1, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTStubs_detWindow", "TTStubs_detWindow", cut,
        "Stub window", 10, 0, 10, col, fcol)
    histos = doit([p0], imgdir)

    # Special attention

    #p0 = struct("TTStubs_passed", "TTStubs_passed", cut,
    #    "Stub pattern match (by event)", 4, 0, 4, col, fcol)
    #histos = book([p0]); project(tree, histos, nentries); draw(histos, text=True); save(imgdir, p0.name)
    #
    #p0 = struct("TTStubs_modPassed", "TTStubs_modPassed", cut,
    #    "Stub pattern match (by module)", 4, 0, 4, col, fcol)
    #histos = book([p0]); project(tree, histos, nentries); draw(histos, text=True); save(imgdir, p0.name)
    #
    #p0 = struct("TTStubs_hitPasseds", "TTStubs_hitPasseds", cut,
    #    "Stub pattern match (by hit, for each hit)", 4, 0, 4, col, fcol)
    #histos = book([p0]); project(tree, histos, nentries); draw(histos, text=True); save(imgdir, p0.name)


# ______________________________________________________________________________
if sections["minbias1"]:
    """For single min-bias event"""

    if not samples["minbias"]:
        print "ERROR"
        exit

    subsections = {}
    subsections["genParts"   ] = False
    subsections["simTracks"  ] = True


    gROOT.LoadMacro("pdgName.C")
    from ROOT import pdgName

    def doit(x, imgdir=None, logy=False):
        histos = book(x)
        project(tree, histos, nentries=nentries)
        draw(histos, logy=logy)
        save(imgdir, x[0].name)
        return histos


    # __________________________________________________________________________
    if subsections["genParts"]:
        cut = ""

        p0 = struct("genParts_all_status", "genParts_status", cut,
            "gen status", 5, 0, 5, col, fcol)
        histos = doit([p0], imgdir)

        cut = "(genParts_status==1 && abs(genParts_eta)<2.5)"  # prompt only

        p0 = struct("genParts_size", "genParts_size" if not cut else "Sum$(%s)" % cut, "",
            "# gen particles", 20, 0, 20, col, fcol)
        histos = doit([p0], imgdir)

        p0 = struct("genParts_charge", "genParts_charge", cut,
            "gen charge", 5, -2.5, 2.5, col, fcol)
        histos = doit([p0], imgdir)

        def myProject(tree, histos, nentries=1000000000):
            tree.SetBranchStatus("*"              , 0)
            tree.SetBranchStatus("genParts_status", 1)
            tree.SetBranchStatus("genParts_eta"   , 1)
            tree.SetBranchStatus("genParts_pdgId" , 1)

            for i_ievt, ievt in enumerate(tree):
                if (i_ievt == nentries):  break

                for br in izip(ievt.genParts_status, ievt.genParts_eta, ievt.genParts_pdgId):
                    status, eta, pdgId = br
                    if status==1 and abs(eta)<2.5:  # cut
                        histos[0].h.Fill(pdgName(pdgId), 1)

            histos[0].h.LabelsDeflate("X")
            histos[0].h.LabelsOption("a")
            tree.SetBranchStatus("*"              , 1)
            return

        p0 = struct("genParts_pdgId", "pdgName(genParts_pdgId)", cut,
            "gen PDG id", 50, -25, 25, col, fcol)
        if True:
            x = [p0]; logy=False
            histos = book(x)
            myProject(tree, histos, nentries=nentries)
            draw(histos, logy=logy)
            save(imgdir, x[0].name)

        cut = "(genParts_status==1 && abs(genParts_eta)<2.5 && genParts_charge != 0)"

        p0 = struct("genParts_charged_size", "genParts_charged_size" if not cut else "Sum$(%s)" % cut, "",
            "# gen_{charged} particles", 20, 0, 20, col, fcol)
        histos = doit([p0], imgdir)

        p0 = struct("genParts_charged_pt", "genParts_pt", cut,
            "gen_{charged} p_{T} [GeV]", 100, 0, 20, col, fcol)
        histos = doit([p0], imgdir, logy=True)

        p0 = struct("genParts_charged_eta", "genParts_eta", cut,
            "gen_{charged} #eta", 60, -3, 3, col, fcol)
        histos = doit([p0], imgdir)

        p0 = struct("genParts_charged_phi", "genParts_phi", cut,
            "gen_{charged} #phi [rad]", 64, -3.2, 3.2, col, fcol)
        histos = doit([p0], imgdir)

        p0 = struct("genParts_charged_vz", "genParts_vz", cut,
            "gen_{charged} vertex z_{0} [cm]", 60, -30, 30, col, fcol)
        histos = doit([p0], imgdir)


    # __________________________________________________________________________
    if subsections["simTracks"]:
        cut = "(simTracks_genpId!=-1 && abs(simTracks_eta)<2.5 && simTracks_charge!=0)"

        p0 = struct("simTracks_size", "simTracks_size" if not cut else "Sum$(%s)" % cut, "",
            "# simTracks per interaction", 50, 0, 200, col, fcol)
        histos = doit([p0], imgdir)

        p0 = struct("simTracks_pt", "simTracks_pt", cut,
            "simTrack p_{T} [GeV]", 100, 0, 20, col, fcol)
        histos = doit([p0], imgdir, logy=True)

        def myProject(tree, histos, nentries=nentries):
            #nentries = 139995
            tree.SetBranchStatus("*"               , 0)
            tree.SetBranchStatus("simTracks_genpId", 1)
            tree.SetBranchStatus("simTracks_eta"   , 1)
            tree.SetBranchStatus("simTracks_charge", 1)
            tree.SetBranchStatus("simTracks_pdgId" , 1)

            for i_ievt, ievt in enumerate(tree):
                if (i_ievt == nentries):  break

                for br in izip(ievt.simTracks_genpId, ievt.simTracks_eta, ievt.simTracks_charge, ievt.simTracks_pdgId):
                    genpId, eta, charge, pdgId = br
                    if genpId!=-1 and abs(eta)<2.5 and charge!=0:  # cut
                        histos[0].h.Fill(pdgName(pdgId), 1)

            histos[0].h.LabelsDeflate("X")
            histos[0].h.LabelsOption("a")
            tree.SetBranchStatus("*"               , 1)
            return

        p0 = struct("simTracks_pdgId", "pdgName(simTracks_pdgId)", cut,
            "simTrack PDG id", 50, -25, 25, col, fcol)
        if False:  # takes a long time
            x = [p0]; logy=False
            histos = book(x)
            myProject(tree, histos, nentries=nentries)
            draw(histos, logy=logy)
            save(imgdir, x[0].name)


        # ______________________________________________________________________
        p0 = struct("simTracks_pt0_size", "simTracks_size" if not cut else "Sum$(%s * (simTracks_pt>0))" % cut, "",
            "# simTracks_{pt>0}", 60, 0, 240, col, fcol)
        histos = doit([p0], imgdir)

        p0 = struct("simTracks_pt0p2_size", "simTracks_size" if not cut else "Sum$(%s * (simTracks_pt>0.2))" % cut, "",
            "# simTracks_{pt>0.2}", 50, 0, 200, col, fcol)
        histos = doit([p0], imgdir)

        p0 = struct("simTracks_pt0p5_size", "simTracks_size" if not cut else "Sum$(%s * (simTracks_pt>0.5))" % cut, "",
            "# simTracks_{pt>0.5}",60, 0, 120, col, fcol)
        histos = doit([p0], imgdir)

        p0 = struct("simTracks_pt1_size", "simTracks_size" if not cut else "Sum$(%s * (simTracks_pt>1))" % cut, "",
            "# simTracks_{pt>1}", 50, 0, 50, col, fcol)
        histos = doit([p0], imgdir)

        p0 = struct("simTracks_pt2_size", "simTracks_size" if not cut else "Sum$(%s * (simTracks_pt>2))" % cut, "",
            "# simTracks_{pt>2}", 20, 0, 20, col, fcol)
        histos = doit([p0], imgdir)

        p0 = struct("simTracks_pt3_size", "simTracks_size" if not cut else "Sum$(%s * (simTracks_pt>3))" % cut, "",
            "# simTracks_{pt>3}", 10, 0, 10, col, fcol)
        histos = doit([p0], imgdir)

        # ______________________________________________________________________
        cut_eta = cut.replace(" && abs(simTracks_eta)<2.5", "")

        p0 = struct("simTracks_pt0_eta_full", "simTracks_eta", "%s * (simTracks_pt>0)" % cut_eta,
            "simTrack_{pt>0} #eta", 60, -6, 6, col, fcol)
        histos = doit([p0], imgdir)

        p0 = struct("simTracks_pt0p2_eta_full", "simTracks_eta", "%s * (simTracks_pt>0.2)" % cut_eta,
            "simTrack_{pt>0.2} #eta", 60, -6, 6, col, fcol)
        histos = doit([p0], imgdir)

        p0 = struct("simTracks_pt0p5_eta_full", "simTracks_eta", "%s * (simTracks_pt>0.5)" % cut_eta,
            "simTrack_{pt>0.5} #eta", 60, -6, 6, col, fcol)
        histos = doit([p0], imgdir)

        p0 = struct("simTracks_pt1_eta_full", "simTracks_eta", "%s * (simTracks_pt>1)" % cut_eta,
            "simTrack_{pt>1} #eta", 60, -6, 6, col, fcol)
        histos = doit([p0], imgdir)

        p0 = struct("simTracks_pt2_eta_full", "simTracks_eta", "%s * (simTracks_pt>2)" % cut_eta,
            "simTrack_{pt>2} #eta", 60, -6, 6, col, fcol)
        histos = doit([p0], imgdir)

        p0 = struct("simTracks_pt3_eta_full", "simTracks_eta", "%s * (simTracks_pt>3)" % cut_eta,
            "simTrack_{pt>3} #eta", 60, -6, 6, col, fcol)
        histos = doit([p0], imgdir)

        # ______________________________________________________________________
        cut = "(simTracks_genpId!=-1 && abs(simTracks_eta)<2.5 && simTracks_charge!=0 && simTracks_pt>2)"

        p0 = struct("simTracks_pt2_charge", "simTracks_charge", cut,
            "simTracks_{pt>2} charge", 5, -2.5, 2.5, col, fcol)
        histos = doit([p0], imgdir)

        p0 = struct("simTracks_pt2_eta", "simTracks_eta", cut,
            "simTrack_{pt>2} #eta", 60, -3, 3, col, fcol)
        histos = doit([p0], imgdir)

        p0 = struct("simTracks_pt2_phi", "simTracks_phi", cut,
            "simTrack_{pt>2} #phi [rad]", 64, -3.2, 3.2, col, fcol)
        histos = doit([p0], imgdir)

        p0 = struct("simTracks_pt2_vz", "simVertices_vz[simTracks_vtxId]", cut,
            "simTrack_{pt>2} vertex z_{0} [cm]", 60, -30, 30, col, fcol)
        histos = doit([p0], imgdir)


# ______________________________________________________________________________
if sections["minbias140"]:
    """For 140 min-bias event"""

    if not samples["minbias"]:
        print "ERROR"
        exit

    subsections = {}
    subsections["simTracks"    ] = False
    subsections["simTracks_eta"] = True


    #import numpy as np
    #np.random.seed(62935)
    #ninteractions = []
    #for i in xrange(nentries):
    #    ninteractions.append(np.random.poisson(140))
    #nentries2 = sum(ninteractions)
    #print nentries2

    from poisson_ave_140 import ninteractions
    ninteractions = ninteractions[:nentries]
    nentries2 = sum(ninteractions)
    print nentries2


    def doit(x, f, region, imgdir=None, logy=False):
        histos = book(x)
        f(tree, histos, region, nentries=nentries)
        for i, ix in enumerate(x):
            draw(histos[i:i+1], logy=logy)
            save(imgdir, ix.name)
        return histos

    # __________________________________________________________________________
    if subsections["simTracks"]:

        p0 = struct("ave140_gen_nPV", "dummy", "dummy",
            "sim # interactions", 60, 80, 200, col, fcol)
        if True:
            x = [p0]; logy=False
            histos = book(x)
            for i in xrange(nentries):
                histos[0].h.Fill(ninteractions[i])
            draw(histos, logy=logy)
            save(imgdir, x[0].name)

        def myProject(tree, histos, region, nentries=1000000000):
            tree.SetBranchStatus("*"               , 0)
            tree.SetBranchStatus("simTracks_genpId", 1)
            tree.SetBranchStatus("simTracks_eta"   , 1)
            tree.SetBranchStatus("simTracks_phi"   , 1)
            tree.SetBranchStatus("simTracks_charge", 1)
            tree.SetBranchStatus("simTracks_pt"    , 1)

            j_ievt, j_i = 0, 0
            rr = [0] * 4
            nn = [0] * 6

            for i_ievt, ievt in enumerate(tree):
                if (j_ievt == nentries):  break

                #print i_ievt, j_ievt, j_i

                for br in izip(ievt.simTracks_genpId, ievt.simTracks_eta, ievt.simTracks_phi, ievt.simTracks_charge, ievt.simTracks_pt):
                    genpId, eta, phi, charge, pt = br
                    if genpId!=-1 and abs(eta)<2.5 and charge!=0:  # cut

                        rr[0] = True
                        if 0.6<phi and phi<1.6:
                            rr[1] = -0.2<eta and eta<0.8
                            rr[2] = 0.6<eta and eta<1.5
                            rr[3] = 1.3<eta and eta<2.5
                        else:
                            rr[1] = False
                            rr[2] = False
                            rr[3] = False

                        if rr[region]:
                            if pt > 0  :   nn[0] += 1
                            if pt > 0.2:   nn[1] += 1
                            if pt > 0.5:   nn[2] += 1
                            if pt > 1  :   nn[3] += 1
                            if pt > 2  :   nn[4] += 1
                            if pt > 3  :   nn[5] += 1

                j_i += 1
                if j_i == ninteractions[j_ievt]:
                    j_ievt += 1
                    j_i = 0

                    #print nn
                    for i in xrange(6):
                        histos[i].h.Fill(nn[i])
                        nn[i] = 0  # rezero

            tree.SetBranchStatus("*"               , 1)
            return

        params = [
            struct("ave140_simTracks_pt0_size"  , "dummy", "dummy", "# simTracks_{pt>0}"  , 60, 0, 6000, col, fcol),
            struct("ave140_simTracks_pt0p2_size", "dummy", "dummy", "# simTracks_{pt>0.2}", 50, 0, 5000, col, fcol),
            struct("ave140_simTracks_pt0p5_size", "dummy", "dummy", "# simTracks_{pt>0.5}", 60, 0, 3000, col, fcol),
            struct("ave140_simTracks_pt1_size"  , "dummy", "dummy", "# simTracks_{pt>1}"  , 60, 0, 1200, col, fcol),
            struct("ave140_simTracks_pt2_size"  , "dummy", "dummy", "# simTracks_{pt>2}"  , 50, 0,  250, col, fcol),
            struct("ave140_simTracks_pt3_size"  , "dummy", "dummy", "# simTracks_{pt>3}"  , 50, 0,  100, col, fcol),
            ]
        histos = doit(params, myProject, region=0, imgdir=imgdir)

        params = [
            struct("ave140_barrel_simTracks_pt0_size"  , "dummy", "dummy", "# simTracks_{pt>0, barrel}"  , 60, 0, 6000/30, col, fcol),
            struct("ave140_barrel_simTracks_pt0p2_size", "dummy", "dummy", "# simTracks_{pt>0.2, barrel}", 50, 0, 5000/25, col, fcol),
            struct("ave140_barrel_simTracks_pt0p5_size", "dummy", "dummy", "# simTracks_{pt>0.5, barrel}", 60, 0, 3000/15, col, fcol),
            struct("ave140_barrel_simTracks_pt1_size"  , "dummy", "dummy", "# simTracks_{pt>1, barrel}"  , 60, 0, 1200/20, col, fcol),
            struct("ave140_barrel_simTracks_pt2_size"  , "dummy", "dummy", "# simTracks_{pt>2, barrel}"  , 50, 0,  250/ 5, col, fcol),
            struct("ave140_barrel_simTracks_pt3_size"  , "dummy", "dummy", "# simTracks_{pt>3, barrel}"  , 50, 0,  100/ 2, col, fcol),
            ]
        for p in params:  p.fillstyle = 3004
        #histos = doit(params, myProject, region=1, imgdir=imgdir)

        params = [
            struct("ave140_hybrid_simTracks_pt0_size"  , "dummy", "dummy", "# simTracks_{pt>0, hybrid}"  , 60, 0, 6000/30, col, fcol),
            struct("ave140_hybrid_simTracks_pt0p2_size", "dummy", "dummy", "# simTracks_{pt>0.2, hybrid}", 50, 0, 5000/25, col, fcol),
            struct("ave140_hybrid_simTracks_pt0p5_size", "dummy", "dummy", "# simTracks_{pt>0.5, hybrid}", 60, 0, 3000/15, col, fcol),
            struct("ave140_hybrid_simTracks_pt1_size"  , "dummy", "dummy", "# simTracks_{pt>1, hybrid}"  , 60, 0, 1200/20, col, fcol),
            struct("ave140_hybrid_simTracks_pt2_size"  , "dummy", "dummy", "# simTracks_{pt>2, hybrid}"  , 50, 0,  250/ 5, col, fcol),
            struct("ave140_hybrid_simTracks_pt3_size"  , "dummy", "dummy", "# simTracks_{pt>3, hybrid}"  , 50, 0,  100/ 2, col, fcol),
            ]
        for p in params:  p.fillstyle = 3003
        #histos = doit(params, myProject, region=2, imgdir=imgdir)

        params = [
            struct("ave140_endcap_simTracks_pt0_size"  , "dummy", "dummy", "# simTracks_{pt>0, endcap}"  , 60, 0, 6000/30, col, fcol),
            struct("ave140_endcap_simTracks_pt0p2_size", "dummy", "dummy", "# simTracks_{pt>0.2, endcap}", 50, 0, 5000/25, col, fcol),
            struct("ave140_endcap_simTracks_pt0p5_size", "dummy", "dummy", "# simTracks_{pt>0.5, endcap}", 60, 0, 3000/15, col, fcol),
            struct("ave140_endcap_simTracks_pt1_size"  , "dummy", "dummy", "# simTracks_{pt>1, endcap}"  , 60, 0, 1200/20, col, fcol),
            struct("ave140_endcap_simTracks_pt2_size"  , "dummy", "dummy", "# simTracks_{pt>2, endcap}"  , 50, 0,  250/ 5, col, fcol),
            struct("ave140_endcap_simTracks_pt3_size"  , "dummy", "dummy", "# simTracks_{pt>3, endcap}"  , 50, 0,  100/ 2, col, fcol),
            ]
        for p in params:  p.fillstyle = 3005
        #histos = doit(params, myProject, region=3, imgdir=imgdir)


    eb = EtaBinning("#eta", 60, -3, 3)

    def doit(x, f, imgdir=None, logy=False):
        histos = bookProf(x)
        f(tree, histos, nentries=nentries)
        for i, ix in enumerate(x):
            drawProf(histos[i:i+1], ytitle="", logy=logy)
            save(imgdir, ix.name)
        return histos

    # __________________________________________________________________________
    if subsections["simTracks_eta"]:
        gStyle.SetTitleSize(0.05, "Y")
        gStyle.SetOptStat(1110)

        def myProject(tree, histos, nentries=1000000000):
            tree.SetBranchStatus("*"               , 0)
            tree.SetBranchStatus("simTracks_genpId", 1)
            tree.SetBranchStatus("simTracks_eta"   , 1)
            tree.SetBranchStatus("simTracks_phi"   , 1)
            tree.SetBranchStatus("simTracks_charge", 1)
            tree.SetBranchStatus("simTracks_pt"    , 1)

            j_ievt, j_i = 0, 0
            yrr = []
            for i in xrange(6):
                yrr.append([0] * eb.nbinsx)

            for i_ievt, ievt in enumerate(tree):
                if (j_ievt == nentries):  break

                for br in izip(ievt.simTracks_genpId, ievt.simTracks_eta, ievt.simTracks_phi, ievt.simTracks_charge, ievt.simTracks_pt):
                    genpId, eta, phi, charge, pt = br
                    if genpId!=-1 and abs(eta)<2.5 and charge!=0:  # cut

                        ix = eb.findBin(eta)
                        if pt > 0  :   yrr[0][ix] += 1
                        if pt > 0.2:   yrr[1][ix] += 1
                        if pt > 0.5:   yrr[2][ix] += 1
                        if pt > 1  :   yrr[3][ix] += 1
                        if pt > 2  :   yrr[4][ix] += 1
                        if pt > 3  :   yrr[5][ix] += 1

                j_i += 1
                if j_i == ninteractions[j_ievt]:
                    j_ievt += 1
                    j_i = 0

                    for i in xrange(6):
                        for ix in xrange(eb.nbinsx):
                            histos[i].h.Fill(eb.getBinCenter(ix), yrr[i][ix])
                            yrr[i][ix] = 0  # rezero

            tree.SetBranchStatus("*"               , 1)
            return


        params = [
            struct2D("ave140_simTracks_pt0_eta"  , "dummy", "dummy", "simTracks #eta", 60, -3, 3, "<# simTracks> per #eta unit of 0.1_{pt>0}"  , 1000, 0, 1000, col, fcol),
            struct2D("ave140_simTracks_pt0p2_eta", "dummy", "dummy", "simTracks #eta", 60, -3, 3, "<# simTracks> per #eta unit of 0.1_{pt>0.2}", 1000, 0, 1000, col, fcol),
            struct2D("ave140_simTracks_pt0p5_eta", "dummy", "dummy", "simTracks #eta", 60, -3, 3, "<# simTracks> per #eta unit of 0.1_{pt>0.5}", 1000, 0, 1000, col, fcol),
            struct2D("ave140_simTracks_pt1_eta"  , "dummy", "dummy", "simTracks #eta", 60, -3, 3, "<# simTracks> per #eta unit of 0.1_{pt>1}"  , 1000, 0, 1000, col, fcol),
            struct2D("ave140_simTracks_pt2_eta"  , "dummy", "dummy", "simTracks #eta", 60, -3, 3, "<# simTracks> per #eta unit of 0.1_{pt>2}"  , 1000, 0, 1000, col, fcol),
            struct2D("ave140_simTracks_pt3_eta"  , "dummy", "dummy", "simTracks #eta", 60, -3, 3, "<# simTracks> per #eta unit of 0.1_{pt>3}"  , 1000, 0, 1000, col, fcol),
            ]

        histos = doit(params, myProject, imgdir=imgdir)

# ______________________________________________________________________________
if sections["fixme"]:

    if True:
        nPV = []
        for ievt in tree:
            nPV.append(ievt.gen_nPV)
        print "ninteractions =", nPV
