#!/usr/bin/env python

from rootdrawing import *

# ______________________________________________________________________________
# Configurations

sections = {}
sections["genParts"   ] = False
sections["simTracks"  ] = False
sections["simVertices"] = False
sections["trkParts"   ] = False
sections["trkVertices"] = False
sections["eventInfo"  ] = False
sections["clusters"   ] = False
sections["stubs"      ] = False
sections["fixme"      ] = True

drawerInit = DrawerInit()
infiles = get_infiles("input.txt", fast=True)

chain = TChain("ntupler/tree", "")
for f in infiles:
    chain.Add(f)
tree = chain

imgdir = "figures_pgun/"
if not imgdir.endswith("/"):  imgdir += "/"
if gSystem.AccessPathName(imgdir):
    gSystem.mkdir(imgdir)


# ______________________________________________________________________________
# genParticles

if sections["genParts"]:
    def doit(x, imgdir=None, logy=False):
        histos = book(x)
        project(tree, histos)
        draw(histos, logy=logy)
        if imgdir:  save(imgdir, x[0].name)
        return histos

    cut = ""
    col = sBase03
    fcol = ssBase3

    p0 = struct("genParts_size", "genParts_size" if not cut else "Sum$(%s)" % cut, "",
        "muon # gen particles", 4, 0, 4, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("genParts_pt", "genParts_pt", cut,
        "muon gen p_{T} [GeV]", 50, 0, 100, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("genParts_oneoverpt", "1.0/genParts_pt", cut,
        "muon gen 1/p_{T} [1/GeV]", 50, 0, 1, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("genParts_eta", "genParts_eta", cut,
        "muon gen #eta", 60, -3, 3, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("genParts_phi", "genParts_phi", cut,
        "muon gen #phi [rad]", 64, -3.2, 3.2, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("genParts_E", "genParts_E", cut,
        "muon gen energy [GeV]", 50, 0, 1000, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("genParts_M", "genParts_M", cut,
        "muon gen mass [GeV]", 50, 0, 1, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("genParts_vx", "genParts_vx", cut,
        "muon gen vertex x_{0} [mm]", 60, -3, 3, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("genParts_vy", "genParts_vy", cut,
        "muon gen vertex y_{0} [mm]", 60, -3, 3, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("genParts_vz", "genParts_vz", cut,
        "muon gen vertex z_{0} [mm]", 60, -30, 30, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("genParts_pdgId", "genParts_pdgId", cut,
        "muon gen PDG id", 50, -25, 25, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("genParts_charge", "genParts_charge", cut,
        "muon gen charge", 8, -4, 4, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("genParts_status", "genParts_status", cut,
        "muon gen status", 4, 0, 4, col, fcol)
    histos = doit([p0], imgdir)


# ______________________________________________________________________________
# simTracks

if sections["simTracks"]:
    def doit(x, imgdir=None, logy=False):
        histos = book(x)
        project(tree, histos)
        draw(histos, logy=logy)
        if imgdir:  save(imgdir, x[0].name)
        return histos

    cut = "simTracks_trkId==1"
    col = sBase03
    fcol = ssBase4

    p0 = struct("simTracks_size", "simTracks_size" if not cut else "Sum$(%s)" % cut, "",
        "# simTracks", 50, 0, 100, col, fcol)
    histos = doit([p0], imgdir)
    # max = 1930, 1853, 1782

    p0 = struct("simTracks_pt", "simTracks_pt", cut,
        "simTrack p_{T} [GeV]", 50, 0, 100, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("simTracks_oneoverpt", "simTracks_pt==0 ? 9999 : 1.0/simTracks_pt", cut,
        "simTrack 1/p_{T} [1/GeV]", 50, 0, 1, col, fcol)
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
        project(tree, histos)
        draw(histos, logy=logy)
        if imgdir:  save(imgdir, x[0].name)
        return histos

    cut = "simVertices_vtxId==0"
    col = sBase03
    fcol = ssBase4

    p0 = struct("simVertices_size", "simVertices_size" if not cut else "Sum$(%s)" % cut, "",
        "# simVertices", 50, 0, 100, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("simVertices_vx", "simVertices_vx", cut,
        "simVertex x_{0} [mm]", 60, -3, 3, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("simVertices_vy", "simVertices_vy", cut,
        "simVertex y_{0} [mm]", 60, -3, 3, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("simVertices_vz", "simVertices_vz", cut,
        "simVertex z_{0} [mm]", 60, -30, 30, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("simVertices_tof", "simVertices_tof", cut,
        "simVertex time of flight [s]", 50, -1e-7, 1e-7, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("simVertices_trkId", "simVertices_trkId", cut,
        "simVertex track id", 50, 0, 200, col, fcol)
    histos = doit([p0], imgdir, logy=True)

    p0 = struct("simVertices_vtxId", "simVertices_vtxId", cut,
        "simVertex vertex id", 50, 0, 100, col, fcol)
    histos = doit([p0], imgdir, logy=True)

    #p0 = struct("simBeamSpot_vx", "simBeamSpot_bx", cut,
    #    "simBeamSpot x_{0} [mm]", 60, -3, 3, col, fcol)
    #histos = doit([p0], imgdir)
    #
    #p0 = struct("simBeamSpot_vy", "simBeamSpot_by", cut,
    #    "simBeamSpot y_{0} [mm]", 60, -3, 3, col, fcol)
    #histos = doit([p0], imgdir)
    #
    #p0 = struct("simBeamSpot_vz", "simBeamSpot_bz", cut,
    #    "simBeamSpot z_{0} [mm]", 60, -3, 3, col, fcol)
    #histos = doit([p0], imgdir)
    #
    #p0 = struct("simBeamSpot_sigmaZ", "simBeamSpot_sigmaZ", cut,
    #    "simBeamSpot #sigma_{z} [mm]", 50, 0, 0.1, col, fcol)
    #histos = doit([p0], imgdir)


# ______________________________________________________________________________
if sections["trkParts"]:
    def doit(x, imgdir=None, logy=False):
        histos = book(x)
        project(tree, histos)
        draw(histos, logy=logy)
        if imgdir:  save(imgdir, x[0].name)
        return histos

    cut = "trkParts_trkId==1"
    col = sBase03
    fcol = ssBase4

    p0 = struct("trkParts_size", "trkParts_size" if not cut else "Sum$(%s)" % cut, "",
        "# trkParticles", 50, 0, 100, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("trkParts_nSimTracks", "@trkParts_trkIds.size()", cut,
        "trkParticle # simTracks", 20, 0, 20, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("trkParts_pt", "trkParts_pt", cut,
        "trkParticle p_{T} [GeV]", 50, 0, 100, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("trkParts_oneoverpt", "trkParts_pt==0 ? 9999 : 1.0/trkParts_pt", cut,
        "trkParticle 1/p_{T} [1/GeV]", 50, 0, 1, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("trkParts_eta", "trkParts_eta", cut,
        "trkParticle #eta", 60, -3, 3, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("trkParts_phi", "trkParts_phi", cut,
        "trkParticle #phi [rad]", 64, -3.2, 3.2, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("trkParts_vx", "trkParts_vx", cut,
        "trkParticle x_{0} [mm]", 60, -3, 3, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("trkParts_vy", "trkParts_vy", cut,
        "trkParticle y_{0} [mm]", 60, -3, 3, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("trkParts_vz", "trkParts_vz", cut,
        "trkParticle z_{0} [mm]", 60, -30, 30, col, fcol)
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
        project(tree, histos)
        draw(histos, logy=logy)
        if imgdir:  save(imgdir, x[0].name)
        return histos

    #cut = "trkVertices_vtxId==0"
    cut = "abs(trkVertices_vx)<1 && abs(trkVertices_vy)<1"
    col = sBase03
    fcol = ssBase4

    p0 = struct("trkVertices_size", "trkVertices_size" if not cut else "Sum$(%s)" % cut, "",
        "# trkVertices", 50, 0, 100, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("trkVertices_nSimVertices", "@trkVertices_vtxIds.size()", cut,
        "trkVertex # simVertices", 20, 0, 20, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("trkVertices_vx", "trkVertices_vx", cut,
        "trkVertex x_{0} [mm]", 60, -3, 3, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("trkVertices_vy", "trkVertices_vy", cut,
        "trkVertex y_{0} [mm]", 60, -3, 3, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("trkVertices_vz", "trkVertices_vz", cut,
        "trkVertex z_{0} [mm]", 60, -30, 30, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("trkVertices_tof", "trkVertices_tof", cut,
        "trkVertex time of flight [s]", 50, -1e-7, 1e-7, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("trkVertices_vtxId", "trkVertices_vtxId", cut,
        "trkVertex simVertex id", 50, 0, 100, col, fcol)
    histos = doit([p0], imgdir, logy=True)

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
        project(tree, histos)
        draw(histos, logy=logy)
        if imgdir:  save(imgdir, x[0].name)
        return histos

    cut = ""
    col = sBase03
    fcol = ssBase3

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
        "bunch crossing", 4, -3, 1, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("gen_nPV", "gen_nPV", cut,
        "sim # PV", 50, 0, 150, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("gen_trueNPV", "gen_trueNPV", cut,
        "gen # PV", 50, 0, 150, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("gen_randomSeed", "gen_randomSeed", cut,  # bugged
        "generator seed", 50, 0, 100000, col, fcol)
    histos = doit([p0], imgdir)


# ______________________________________________________________________________
if sections["clusters"]:
    def doit(x, imgdir=None, logy=False):
        histos = book(x)
        project(tree, histos)
        draw(histos, logy=logy)
        if imgdir:  save(imgdir, x[0].name)
        return histos

    cut = "TTClusters_trkId==1"
    col = sBase03
    fcol = ssBase4

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
        "Cluster local coord x", 129, 0, 1024+8, col, fcol)
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
        "Cluster simTrack p_{T} [GeV]", 50, 0, 100, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTClusters_simOneoverpt", "TTClusters_simPt==0 ? 9999 : 1.0/TTClusters_simPt", cut,
        "Cluster simTrack 1/p_{T} [1/GeV]", 50, 0, 1, col, fcol)
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
        "Cluster module id", 120, 0, 300000, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTClusters_iModLayer", "TTClusters_iModLayer", cut,
        "Cluster layer number", 30, 0, 30, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTClusters_iModLadder", "TTClusters_iModLadder", cut,
        "Cluster ladder number", 100, 0, 100, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTClusters_iModModule", "TTClusters_iModModule", cut,
        "Cluster module number", 100, 0, 200, col, fcol)
    histos = doit([p0], imgdir)

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

    p0 = struct("TTClusters_hitADCs", "TTClusters_hitADCs", cut,
        "Cluster hit ADC count (for each hit)", 129, 0, 256+2, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTClusters_hitChans", "TTClusters_hitChans", cut,
        "Cluster hit channel id (for each hit)", 125, 0, 2500000, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTClusters_hitCols", "TTClusters_hitCols", cut,
        "Cluster hit column number (for each hit)", 33, 0, 32+1, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTClusters_hitRows", "TTClusters_hitRows", cut,
        "Cluster hit row number (for each hit)", 129, 0, 1024+8, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTClusters_hitFracs", "TTClusters_hitFracs", cut,
        "Cluster hit charge fraction (for each hit)", 100, 0, 1, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTClusters_hitTrkIds", "TTClusters_hitTrkIds", cut,
        "Cluster hit simTrack track id (for each hit)", 50, 0, 200, col, fcol)
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
        project(tree, histos)
        draw(histos, logy=logy)
        if imgdir:  save(imgdir, x[0].name)
        return histos

    cut = "TTStubs_trkId==1"
    col = sBase03
    fcol = ssBase4


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

    ##
    p0 = struct("TTStubs_simPt", "TTStubs_simPt", cut,
        "Stub simTrack p_{T} [GeV]", 50, 0, 100, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTStubs_simOneoverpt", "TTStubs_simPt==0 ? 9999 : 1.0/TTStubs_simPt", cut,
        "Stub simTrack 1/p_{T} [1/GeV]", 50, 0, 1, col, fcol)
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
        "Stub module id", 120, 0, 300000, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTStubs_iModLayer", "TTStubs_iModLayer", cut,
        "Stub layer number", 30, 0, 30, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTStubs_iModLadder", "TTStubs_iModLadder", cut,
        "Stub ladder number", 100, 0, 100, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTStubs_iModModule", "TTStubs_iModModule", cut,
        "Stub module number", 100, 0, 200, col, fcol)
    histos = doit([p0], imgdir)

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

    p0 = struct("TTStubs_hitADCs", "TTStubs_hitADCs", cut,
        "Stub hit ADC count (for each hit)", 129, 0, 256+2, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTStubs_hitChans", "TTStubs_hitChans", cut,
        "Stub hit channel id (for each hit)", 125, 0, 2500000, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTStubs_hitCols", "TTStubs_hitCols", cut,
        "Stub hit column number (for each hit)", 33, 0, 32+1, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTStubs_hitRows", "TTStubs_hitRows", cut,
        "Stub hit row number (for each hit)", 129, 0, 1024+8, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTStubs_hitFracs", "TTStubs_hitFracs", cut,
        "Stub hit charge fraction (for each hit)", 100, 0, 1, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTStubs_hitTrkIds", "TTStubs_hitTrkIds", cut,
        "Stub hit simTrack track id (for each hit)", 50, 0, 200, col, fcol)
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
        "Stub trigger local coord x [strip unit]", 129, 0, 1024+8, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTStubs_trigBend", "TTStubs_trigBend", cut,
        "Stub trigger bend [strip unit]", 20, -10, 10, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTStubs_separation", "TTStubs_separation", cut,
        "Stub separation [#Delta R or #Delta z]", 50, 0, 1, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTStubs_detWindow", "TTStubs_detWindow", cut,
        "Stub window (ignore for now)", 80, -1000, 3000, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTStubs_trivial", "TTStubs_trivial", cut,
        "Stub trivial match", 4, 0, 4, col, fcol)
    histos = book([p0]); project(tree, histos); draw(histos, text=True); save(imgdir, p0.name)

    p0 = struct("TTStubs_passed", "TTStubs_passed", cut,
        "Stub pattern match (by event)", 4, 0, 4, col, fcol)
    histos = book([p0]); project(tree, histos); draw(histos, text=True); save(imgdir, p0.name)

    p0 = struct("TTStubs_modPassed", "TTStubs_modPassed", cut,
        "Stub pattern match (by module)", 4, 0, 4, col, fcol)
    histos = book([p0]); project(tree, histos); draw(histos, text=True); save(imgdir, p0.name)

    p0 = struct("TTStubs_hitPasseds", "TTStubs_hitPasseds", cut,
        "Stub pattern match (by hit, for each hit)", 4, 0, 4, col, fcol)
    histos = book([p0]); project(tree, histos); draw(histos, text=True); save(imgdir, p0.name)


# ______________________________________________________________________________
if sections["fixme"]:
    cut = "TTStubs_trkId==1"
    col = sBase03

    fcol = ssBase3
    p0 = struct("TTStubs_size", "TTStubs_size", "",
        "# stubs", 30, 0, 30, col, fcol)
    histos = book([p0]); project(tree, histos); draws(histos, logy=True); save(imgdir, p0.name+"_logy")

    fcol = ssBase4
    p0 = struct("TTStubs_size", "Sum$(%s)" % cut, "",
        "# stubs {simTrack id==1}", 30, 0, 30, col, fcol)
    histos = book([p0]); project(tree, histos); draws(histos, logy=True); save(imgdir, p0.name+"_id1_logy")

    fcol = ssBase4
    p0 = struct("TTStubs_size", "Sum$(%s)" % cut, "",
        "# stubs {# simTracks==1}", 30, 0, 30, col, fcol)
    histos = book([p0]); project(tree, histos); draws(histos, logy=True); save(imgdir, p0.name+"_ns1_logy")

    #fcol = ssBase3
    #p0 = struct("TTStubs_pdgId", "TTStubs_pdgId", "",
    #    "Stub PDG id", 50, -25, 25, col, fcol)
    #histos = book([p0]); project(tree, histos); draws(histos, logy=True); save(imgdir, p0.name+"_logy")
