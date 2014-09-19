#!/usr/bin/env python

from rootdrawing import *
from roothelper import *
from operator import itemgetter

# ______________________________________________________________________________
# Configurations

sections = {}
sections["params"]  = False
sections["digis"]   = True
sections["stubs"]   = False

drawerInit = DrawerInit()


file1 = "/uscms_data/d2/jiafu/L1TrackTrigger/CMSSW_6_2_0_SLHC12_patch1/src/SLHCL1TrackTriggerSimulations/Configuration/test/zeroMaterial/SingleMu_Barrel_E2023TTI_ntuple.root"
file2 = "/uscms_data/d2/jiafu/L1TrackTrigger/CMSSW_6_2_0_SLHC12_patch1/src/SLHCL1TrackTriggerSimulations/Configuration/test/zeroMaterial/SingleMu_Barrel_ZeroMat_E2023TTI_ntuple.root"


col1 = TColor.GetColor("#1f78b4")
col2 = TColor.GetColor("#ff9c1f")
imgdir = "figures_zeroMaterial/"

# Number of events
nentries = 20000

chain1 = TChain("ntupler/tree")
chain2 = TChain("ntupler/tree")

chain1.Add(file1)
chain2.Add(file2)
chain1.AddFriend(chain2, "friend")
tree = chain1

if not imgdir.endswith("/"):  imgdir += "/"
if gSystem.AccessPathName(imgdir):
    gSystem.mkdir(imgdir)


if sections["params"]:

    def doit(x, imgdir=None, logy=False):
        histos = book(x)
        project(tree, histos, nentries=nentries)
        draw(histos, logy=logy)
        save(imgdir, x[0].name)
        return histos

    cut = ""

    p0 = struct("genParts_size", "genParts_size - friend.genParts_size", "",
        "Difference in # genParticles", 50, -25, 25, col1, col1)
    p0.fillstyle = 0
    histos = doit([p0], imgdir, logy=True)

    p0 = struct("genParts_pt", "genParts_pt - friend.genParts_pt", cut,
        "Difference in genPart p_{T} [GeV]", 50, -1e-6, 1e-6, col1, col1)
    p0.fillstyle = 0
    histos = doit([p0], imgdir, logy=True)

    p0 = struct("genParts_eta", "genParts_eta - friend.genParts_eta", cut,
        "Difference in genPart #eta", 50, -1e-6, 1e-6, col1, col1)
    p0.fillstyle = 0
    histos = doit([p0], imgdir, logy=True)

    p0 = struct("genParts_phi", "genParts_phi - friend.genParts_phi", cut,
        "Difference in genPart #phi", 50, -1e-6, 1e-6, col1, col1)
    p0.fillstyle = 0
    histos = doit([p0], imgdir, logy=True)

    p0 = struct("genParts_vz", "genParts_vz - friend.genParts_vz", cut,
        "Difference in genPart z_{0}", 50, -1e-6, 1e-6, col1, col1)
    p0.fillstyle = 0
    histos = doit([p0], imgdir, logy=True)

    cut = "simTracks_trkId==1"

    p0 = struct("simTracks_size", "simTracks_size - friend.simTracks_size", "",
        "Difference in # simTracks", 50, -25, 25, col1, col1)
    p0.fillstyle = 0
    histos = doit([p0], imgdir, logy=True)

    p0 = struct("simTracks_pt", "simTracks_pt - friend.simTracks_pt", cut,
        "Difference in simTrk p_{T} [GeV]", 50, -1e-6, 1e-6, col1, col1)
    p0.fillstyle = 0
    histos = doit([p0], imgdir, logy=True)

    p0 = struct("simTracks_eta", "simTracks_eta - friend.simTracks_eta", cut,
        "Difference in simTrk #eta", 50, -1e-6, 1e-6, col1, col1)
    p0.fillstyle = 0
    histos = doit([p0], imgdir, logy=True)

    p0 = struct("simTracks_phi", "simTracks_phi - friend.simTracks_phi", cut,
        "Difference in simTrk #phi", 50, -1e-6, 1e-6, col1, col1)
    p0.fillstyle = 0
    histos = doit([p0], imgdir, logy=True)

    cut = "simVertices_vtxId==0"

    p0 = struct("simVertices_size", "simVertices_size - friend.simVertices_size", "",
        "Difference in # simVertices", 50, -25, 25, col1, col1)
    p0.fillstyle = 0
    histos = doit([p0], imgdir, logy=True)

    p0 = struct("simVertices_vz", "simVertices_vz - friend.simVertices_vz", cut,
        "Difference in simVtx z_{0}", 50, -1e-6, 1e-6, col1, col1)
    p0.fillstyle = 0
    histos = doit([p0], imgdir, logy=True)

    cut = "trkParts_trkId==1"

    p0 = struct("trkParts_size", "trkParts_size - friend.trkParts_size", "",
        "Difference in # trackingParticles", 50, -25, 25, col1, col1)
    p0.fillstyle = 0
    histos = doit([p0], imgdir, logy=True)

    p0 = struct("trkParts_pt", "trkParts_pt - friend.trkParts_pt", cut,
        "Difference in trkPart p_{T} [GeV]", 50, -1e-6, 1e-6, col1, col1)
    p0.fillstyle = 0
    histos = doit([p0], imgdir, logy=True)

    p0 = struct("trkParts_eta", "trkParts_eta - friend.trkParts_eta", cut,
        "Difference in trkPart #eta", 50, -1e-6, 1e-6, col1, col1)
    p0.fillstyle = 0
    histos = doit([p0], imgdir, logy=True)

    p0 = struct("trkParts_phi", "trkParts_phi - friend.trkParts_phi", cut,
        "Difference in trkPart #phi", 50, -1e-6, 1e-6, col1, col1)
    p0.fillstyle = 0
    histos = doit([p0], imgdir, logy=True)

    cut = "trkVertices_vtxId==0"

    p0 = struct("trkVertices_size", "trkVertices_size - friend.trkVertices_size", "",
        "Difference in # trackingVertices", 50, -25, 25, col1, col1)
    p0.fillstyle = 0
    histos = doit([p0], imgdir, logy=True)

    p0 = struct("trkVertices_vz", "trkVertices_vz - friend.trkVertices_vz", cut,
        "Difference in trkVtx z_{0}", 50, -1e-6, 1e-6, col1, col1)
    p0.fillstyle = 0
    histos = doit([p0], imgdir, logy=True)


if sections["digis"]:

    def doit(x, imgdir=None, logy=False):
        histos = book(x)
        project(tree, histos, nentries=nentries)
        draw(histos, logy=logy)
        save(imgdir, x[0].name)
        return histos

    cut = "simPixelDigis_trkId==1"

    p0 = struct("simPixelDigis_size", "simPixelDigis_size - friend.simPixelDigis_size", "",
        "Difference in # simPixelDigis", 50, -25, 25, col1, col1)
    p0.fillstyle = 0
    histos = doit([p0], imgdir, logy=True)


    def doit(x, f, imgdir=None, logy=False):
        histos = book(x)
        f(tree, histos, nentries=nentries)
        for i, ix in enumerate(x):
            draw(histos[i:i+1], logy=logy)
            save(imgdir, ix.name)
        return histos

    def myProject(tree, histos, nentries=1000000000):

        for i_ievt, ievt in enumerate(tree):
            if (i_ievt == nentries):  break

            # Split the digis by layers
            r_b1, z_b1, phi_b1 = [], [], []
            r_b2, z_b2, phi_b2 = [], [], []
            r_b3, z_b3, phi_b3 = [], [], []
            r_b4, z_b4, phi_b4 = [], [], []
            r_b5, z_b5, phi_b5 = [], [], []
            r_b6, z_b6, phi_b6 = [], [], []

            for trkId, r, z, phi in izip(ievt.simPixelDigis_trkId, ievt.simPixelDigis_r, ievt.simPixelDigis_z, ievt.simPixelDigis_phi):

                if trkId==1:
                    if r < 25:
                        r_b1.append(r); z_b1.append(z), phi_b1.append(phi)
                    elif r < 38:
                        r_b2.append(r); z_b2.append(z), phi_b2.append(phi)
                    elif r < 53:
                        r_b3.append(r); z_b3.append(z), phi_b3.append(phi)
                    elif r < 71:
                        r_b4.append(r); z_b4.append(z), phi_b4.append(phi)
                    elif r < 92:
                        r_b5.append(r); z_b5.append(z), phi_b5.append(phi)
                    elif r < 110:
                        r_b6.append(r); z_b6.append(z), phi_b6.append(phi)

            f_r_b1, f_z_b1, f_phi_b1 = [], [], []
            f_r_b2, f_z_b2, f_phi_b2 = [], [], []
            f_r_b3, f_z_b3, f_phi_b3 = [], [], []
            f_r_b4, f_z_b4, f_phi_b4 = [], [], []
            f_r_b5, f_z_b5, f_phi_b5 = [], [], []
            f_r_b6, f_z_b6, f_phi_b6 = [], [], []

            f_ievt = ievt.GetFriend("friend")

            for trkId, r, z, phi in izip(f_ievt.simPixelDigis_trkId, f_ievt.simPixelDigis_r, f_ievt.simPixelDigis_z, f_ievt.simPixelDigis_phi):

                if trkId==1:
                    if r < 25:
                        f_r_b1.append(r); f_z_b1.append(z), f_phi_b1.append(phi)
                    elif r < 38:
                        f_r_b2.append(r); f_z_b2.append(z), f_phi_b2.append(phi)
                    elif r < 53:
                        f_r_b3.append(r); f_z_b3.append(z), f_phi_b3.append(phi)
                    elif r < 71:
                        f_r_b4.append(r); f_z_b4.append(z), f_phi_b4.append(phi)
                    elif r < 92:
                        f_r_b5.append(r); f_z_b5.append(z), f_phi_b5.append(phi)
                    elif r < 110:
                        f_r_b6.append(r); f_z_b6.append(z), f_phi_b6.append(phi)

            # Find which digi has smallest rho
            if not (len(r_b1) and len(r_b2) and len(r_b3) and len(r_b4) and len(r_b5) and len(r_b6)):
                continue
            i_b1 = min(enumerate(r_b1), key=itemgetter(1))[0]
            i_b2 = min(enumerate(r_b2), key=itemgetter(1))[0]
            i_b3 = min(enumerate(r_b3), key=itemgetter(1))[0]
            i_b4 = min(enumerate(r_b4), key=itemgetter(1))[0]
            i_b5 = min(enumerate(r_b5), key=itemgetter(1))[0]
            i_b6 = min(enumerate(r_b6), key=itemgetter(1))[0]

            if not (len(f_r_b1) and len(f_r_b2) and len(f_r_b3) and len(f_r_b4) and len(f_r_b5) and len(f_r_b6)):
                continue
            f_i_b1 = min(enumerate(f_r_b1), key=itemgetter(1))[0]
            f_i_b2 = min(enumerate(f_r_b2), key=itemgetter(1))[0]
            f_i_b3 = min(enumerate(f_r_b3), key=itemgetter(1))[0]
            f_i_b4 = min(enumerate(f_r_b4), key=itemgetter(1))[0]
            f_i_b5 = min(enumerate(f_r_b5), key=itemgetter(1))[0]
            f_i_b6 = min(enumerate(f_r_b6), key=itemgetter(1))[0]

            # Fill
            histos[0].h.Fill(len(r_b1) - len(f_r_b1))
            histos[1].h.Fill(r_b1[i_b1] - f_r_b1[f_i_b1])
            histos[2].h.Fill(z_b1[i_b1] - f_z_b1[f_i_b1])
            histos[3].h.Fill(phi_b1[i_b1] - f_phi_b1[f_i_b1])

            histos[4].h.Fill(len(r_b2) - len(f_r_b2))
            histos[5].h.Fill(r_b2[i_b2] - f_r_b2[f_i_b2])
            histos[6].h.Fill(z_b2[i_b2] - f_z_b2[f_i_b2])
            histos[7].h.Fill(phi_b2[i_b2] - f_phi_b2[f_i_b2])

            histos[8].h.Fill(len(r_b3) - len(f_r_b3))
            histos[9].h.Fill(r_b3[i_b3] - f_r_b3[f_i_b3])
            histos[10].h.Fill(z_b3[i_b3] - f_z_b3[f_i_b3])
            histos[11].h.Fill(phi_b3[i_b3] - f_phi_b3[f_i_b3])

            histos[12].h.Fill(len(r_b4) - len(f_r_b4))
            histos[13].h.Fill(r_b4[i_b4] - f_r_b4[f_i_b4])
            histos[14].h.Fill(z_b4[i_b4] - f_z_b4[f_i_b4])
            histos[15].h.Fill(phi_b4[i_b4] - f_phi_b4[f_i_b4])

            histos[16].h.Fill(len(r_b5) - len(f_r_b5))
            histos[17].h.Fill(r_b5[i_b5] - f_r_b5[f_i_b5])
            histos[18].h.Fill(z_b5[i_b5] - f_z_b5[f_i_b5])
            histos[19].h.Fill(phi_b5[i_b5] - f_phi_b5[f_i_b5])

            histos[20].h.Fill(len(r_b6) - len(f_r_b6))
            histos[21].h.Fill(r_b6[i_b6] - f_r_b6[f_i_b6])
            histos[22].h.Fill(z_b6[i_b6] - f_z_b6[f_i_b6])
            histos[23].h.Fill(phi_b6[i_b6] - f_phi_b6[f_i_b6])

    # NOTE: 0.002 bin width in phi corresponds to M_PI/1600 which corresponds to 400x1
    params = []
    for i in xrange(1,7):
        p0 = struct("simPixelDigis_b%i_size" % i, "", "", "Difference in # simPixelDigis_{Barrel %i}" % i, 50, -25, 25, col1, col1)
        p0.fillstyle = 0
        params.append(p0)

        p0 = struct("simPixelDigis_b%i_r" % i, "", "", "Difference in digi #rho_{Barrel %i} [cm]" % i, 50, -0.05, 0.05, col1, col1)
        p0.fillstyle = 0
        params.append(p0)

        p0 = struct("simPixelDigis_b%i_z" % i, "", "", "Difference in digi z_{Barrel %i} [cm]" % i, 50, -0.05, 0.05, col1, col1)
        p0.fillstyle = 0
        params.append(p0)

        p0 = struct("simPixelDigis_b%i_phi" % i, "", "", "Difference in digi #phi_{Barrel %i}" % i, 50, -0.05, 0.05, col1, col1)
        p0.fillstyle = 0
        params.append(p0)

    histos = doit(params, myProject, imgdir, logy=True)



if sections["stubs"]:

    def doit(x, imgdir=None, logy=False):
        histos = book(x)
        project(tree, histos, nentries=nentries)
        draw(histos, logy=logy)
        save(imgdir, x[0].name)
        return histos

    cut = "TTStubs_trkId==0"

    p0 = struct("TTStubs_size", "TTStubs_size - friend.TTStubs_size", "",
        "Difference in # TTStubs", 50, -25, 25, col1, col1)
    p0.fillstyle = 0
    histos = doit([p0], imgdir, logy=True)
