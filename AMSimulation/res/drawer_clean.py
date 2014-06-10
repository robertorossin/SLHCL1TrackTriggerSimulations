#!/usr/bin/env python

from rootdrawing import *

# ______________________________________________________________________________
# Configurations

sections = {}
sections["clean"] = True

drawerInit = DrawerInit()
infiles = get_infiles("input_val_20140601.txt", fast=True)

chain = TChain("ntupler/tree", "")
for f in infiles:
    chain.Add(f)
tree = chain
print tree.GetEntries()

tree_friend = TChain("ntupler/tree", "")
tree_friend.Add("stubs.root")

imgdir = "figures_clean/"
if not imgdir.endswith("/"):  imgdir += "/"
if gSystem.AccessPathName(imgdir):
    gSystem.mkdir(imgdir)

# ______________________________________________________________________________
def project2(tree1, tree2, histos, normalize=-1, drawOverflow=True, drawUnderflow=True):
    for i, h in enumerate(histos):
        if i % 2 == 0:
            tree1.Project(h.randname, h.var, h.cut, "goff")
        else:
            tree2.Project(h.randname, h.var, h.cut, "goff")
        if normalize > 0:  h.Scale(normalize / h.GetSumOfWeights())
        if drawOverflow :  h.fixOverflow()
        if drawUnderflow:  h.fixUnderflow()
    return

# ______________________________________________________________________________
if sections["clean"]:

    col = sBase03

    fcol0 = ssBase4
    fcol0_sel = sMagenta
    fcol1 = ssBase6
    fcol1_sel = sGreen

    cut0 = "(TTStubs_trkId==1 && genParts_pt[0]>2 && abs(genParts_eta[0])<1.0)"
    cut1 = "(TTStubs_trkId==1 && genParts_pt[0]>2 && abs(genParts_eta[0])<2.2 && abs(genParts_eta[0])>1.0)"
    lab0 = " {|#eta|<1.0}"
    lab1 = " {1.0<|#eta|<2.2}"

    var = "atan2(TTStubs_r,TTStubs_z)-atan2(genParts_pt[0],genParts_pt[0]*sinh(genParts_eta[0]))"
    p0 = struct("filtering_deltaTheta_etabin0", var, cut0,
        "#Delta#theta(stub,gen muon)"+lab0, 40, -0.8, 0.8, col, fcol0)
    p1 = struct("filtering_deltaTheta_etabin0_sel", var, cut0,
        "#Delta#theta(stub,gen muon)"+lab0, 40, -0.8, 0.8, col, fcol0_sel)
    histos = book([p0,p1]); project2(tree, tree_friend, histos); draws(histos, logy=True); save(imgdir, p0.name)

    p0 = struct("filtering_deltaTheta_etabin1", var, cut1,
        "#Delta#theta(stub,gen muon)"+lab1, 40, -0.8, 0.8, col, fcol1)
    p1 = struct("filtering_deltaTheta_etabin1_sel", var, cut1,
        "#Delta#theta(stub,gen muon)"+lab1, 40, -0.8, 0.8, col, fcol1_sel)
    histos = book([p0,p1]); project2(tree, tree_friend, histos); draws(histos, logy=True); save(imgdir, p0.name)

    var = "deltaPhi(atan2(TTStubs_y,TTStubs_x),genParts_phi[0])"
    p0 = struct("filtering_deltaPhi_etabin0", var, cut0,
        "#Delta#phi(stub,gen muon)"+lab0, 40, -0.8, 0.8, col, fcol0)
    p1 = struct("filtering_deltaPhi_etabin0_sel", var, cut0,
        "#Delta#phi(stub,gen muon)"+lab0, 40, -0.8, 0.8, col, fcol0_sel)
    histos = book([p0,p1]); project2(tree, tree_friend, histos); draws(histos, logy=True); save(imgdir, p0.name)

    p0 = struct("filtering_deltaPhi_etabin1", var, cut1,
        "#Delta#phi(stub,gen muon)"+lab1, 40, -0.8, 0.8, col, fcol1)
    p1 = struct("filtering_deltaPhi_etabin1_sel", var, cut1,
        "#Delta#phi(stub,gen muon)"+lab1, 40, -0.8, 0.8, col, fcol1_sel)
    histos = book([p0,p1]); project2(tree, tree_friend, histos); draws(histos, logy=True); save(imgdir, p0.name)

    cut0 = "(genParts_pt[0]>2 && abs(genParts_eta[0])<1.0)"
    cut1 = "(genParts_pt[0]>2 && abs(genParts_eta[0])<2.2 && abs(genParts_eta[0])>1.0)"
    lab0 = " {|#eta|<1.0}"
    lab1 = " {1.0<|#eta|<2.2}"

    var = "Sum$(TTStubs_trkId==1)"
    p0 = struct("filtering_nstubs_etabin0", var, cut0,
        "# stubs"+lab0, 25, 0, 25, col, fcol0)
    p1 = struct("filtering_nstubs_etabin0_sel", var, cut0,
        "# stubs"+lab0, 25, 0, 25, col, fcol0_sel)
    histos = book([p0,p1]); project2(tree, tree_friend, histos); draws(histos, logy=True); save(imgdir, p0.name)

    p0 = struct("filtering_nstubs_etabin1", var, cut1,
        "# stubs"+lab1, 25, 0, 25, col, fcol1)
    p1 = struct("filtering_nstubs_etabin1_sel", var, cut1,
        "# stubs"+lab1, 25, 0, 25, col, fcol1_sel)
    histos = book([p0,p1]); project2(tree, tree_friend, histos); draws(histos, logy=True); save(imgdir, p0.name)

    var_tmp = "(Sum$(TTStubs_trkId==1 && int(TTStubs_modId/10000)==%i)>0)"
    var = "+".join([(var_tmp % i) for i in (5,6,7,8,9,10,11,12,13,14,15,18,19,20,21,22)])
    p0 = struct("filtering_nlayers_etabin0", var, cut0,
        "# layers with stubs"+lab0, 25, 0, 25, col, fcol0)
    p1 = struct("filtering_nlayers_etabin0_sel", var, cut0,
        "# layers with stubs"+lab0, 25, 0, 25, col, fcol0_sel)
    histos = book([p0,p1]); project2(tree, tree_friend, histos); draws(histos, logy=True); save(imgdir, p0.name)

    p0 = struct("filtering_nlayers_etabin1", var, cut1,
        "# layers with stubs"+lab1, 25, 0, 25, col, fcol1)
    p1 = struct("filtering_nlayers_etabin1_sel", var, cut1,
        "# layers with stubs"+lab1, 25, 0, 25, col, fcol1_sel)
    histos = book([p0,p1]); project2(tree, tree_friend, histos); draws(histos, logy=True); save(imgdir, p0.name)

    #var = "trkParts_ntkhits[0]"
    #p0 = struct("filtering_tp_ntkhits_etabin0", var, cut0,
    #    "trkParticle # hits"+lab0, 50, 0, 50, col, fcol0)
    #p1 = struct("filtering_tp_ntkhits_etabin0_sel", var, cut0,
    #    "trkParticle # hits"+lab0, 50, 0, 50, col, fcol0_sel)
    #histos = book([p0,p1]); project2(tree, tree_friend, histos); draws(histos, logy=True); save(imgdir, p0.name)

    #p0 = struct("filtering_tp_ntkhits_etabin1", var, cut1,
    #    "trkParticle # hits"+lab1, 50, 0, 50, col, fcol1)
    #p1 = struct("filtering_tp_ntkhits_etabin1_sel", var, cut1,
    #    "trkParticle # hits"+lab1, 50, 0, 50, col, fcol1_sel)
    #histos = book([p0,p1]); project2(tree, tree_friend, histos); draws(histos, logy=True); save(imgdir, p0.name)

    #var = "trkParts_ntklayers[0]"
    #p0 = struct("filtering_tp_ntklayers_etabin0", var, cut0,
    #    "trkParticle # layers"+lab0, 50, 0, 50, col, fcol0)
    #p1 = struct("filtering_tp_ntklayers_etabin0_sel", var, cut0,
    #    "trkParticle # layers"+lab0, 50, 0, 50, col, fcol0_sel)
    #histos = book([p0,p1]); project2(tree, tree_friend, histos); draws(histos, logy=True); save(imgdir, p0.name)

    #p0 = struct("filtering_tp_ntklayers_etabin1", var, cut1,
    #    "trkParticle # layers"+lab1, 50, 0, 50, col, fcol1)
    #p1 = struct("filtering_tp_ntklayers_etabin1_sel", var, cut1,
    #    "trkParticle # layers"+lab1, 50, 0, 50, col, fcol1_sel)
    #histos = book([p0,p1]); project2(tree, tree_friend, histos); draws(histos, logy=True); save(imgdir, p0.name)

