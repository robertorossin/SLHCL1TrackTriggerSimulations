#!/usr/bin/env python

from rootdrawing import *
from parser import *
import math
import csv

# Configurations
col  = TColor.GetColor("#1f78b4")  # mu0
fcol = TColor.GetColor("#a6cee3")  # mu0


# ______________________________________________________________________________
mPtFactor = 0.3*3.8*1e-2/2.0

def calcIdealPhi(phi, invPt, r):
    #return phi - mPtFactor * r * invPt
    return phi - asin(mPtFactor * r * invPt)

def calcIdealZ(eta, vz, invPt, r):
    cotTheta = sinh(eta)
    #return vz + r * cotTheta
    return vz + (1.0 / (mPtFactor * invPt) * asin(mPtFactor * r * invPt)) * cotTheta

def calcIdealXY(eta, z0, invPt, phi0, z):
    c=3E10
    m=0.105
    Pt = -1.0/invPt
    Pz = abs(Pt) * math.sinh(eta)
    P  = abs(Pt) * math.cosh(eta)
    R = 1E2*abs(Pt)/(0.3*3.8)
    gamma = P/m # gamma = E/m ~ p/m
#     omega = c/R
    omega = Pt/P*c/R
    vz = c*Pz/P
    Deltaz = z-z0
    Deltat = Deltaz/vz
    DeltaAlpha = omega*Deltat
    rho = 2*abs(R)*math.sin(abs(DeltaAlpha)/2)
    phi = phi0 + DeltaAlpha/2
    X = rho*math.cos(phi)
    Y = rho*math.sin(phi)
#     print " # ", X, Y, X*X+Y*Y, omega, R, Deltat, phi0, DeltaAlpha, invPt
    return [X,Y]

def isint(value):
  try:
    int(value)
    return True
  except:
    return False

def getCI(h, threshold=0.95):
    nbinsx = h.GetNbinsX()
    integral = h.Integral(1,nbinsx)
    if integral==0: return -1
    integral_i = 0.
    x = h.GetBinCenter(1)
    for i in xrange(1,nbinsx+1):
        integral_i += h.GetBinContent(i)
        if (integral_i / integral) <= threshold:
            x = h.GetBinCenter(i)
    assert(integral_i == integral)
    return x

def loadModules():

    moduleCoords = {}
    nLine=0
    with open('/home/rossin/Dropbox/TT/CMSSW_6_2_0_SLHC25_patch3/src/SLHCL1TrackTriggerSimulations/AMSimulation/data/module_vertices.csv', 'rb') as csvfile:
        moduleList = csv.reader(csvfile, delimiter=',')
        for row in moduleList:
            nLine+=1
            if (nLine==1): continue 
            nMod = -1
            if (isint(row[0])):
                nMod=int(row[0])
                floatrow=[]
                for i in row:
                    floatrow.append(float(i))
                phi = math.atan2(floatrow[2]+floatrow[5]+floatrow[8]+floatrow[11],floatrow[1]+floatrow[4]+floatrow[7]+floatrow[10])
                floatrow.append(phi)
                moduleCoords[nMod]=floatrow

    return moduleCoords

# Calculate constants
rMeans = b_rcoord_cmssw  # cm
pitches = [100., 100., 100., 90., 90., 90.]  # um
thicknesses = [0.26, 0.16, 0.16, 0.18, 0.18, 0.18]  # cm
# drCorrs = []
D = ["PS","2S"]
D = range(15)
var = ['dx','dy','dr','dphi','drphi','dparallel','dperpendicular']
CLstring = ['RMS','CL90']
# for i in xrange(6):
#     drCorr = pitches[i] / thicknesses[i] / rMeans[i] * 1e-4
#     drCorrs.append(drCorr)


# ______________________________________________________________________________
def drawer_book():
    histos = {}

    for v in var:
        for CL in CLstring:
            hname  = "trackResidual_2D_%s_%s" % (v,CL)
            htitle = ";Disk Layer;Disk annulus;%s %s" % (v,CL)
            nbinsx, xmin, xmax, nbinsy, ymin, ymax = 5, 10.5, 15.5, 15, -0.5, 14.5
            histos[hname] = TH2F(hname, htitle, nbinsx, xmin, xmax, nbinsy, ymin, ymax)
            
    # TH1F
    for i in xrange(11,16):
#         hname = "stub_ds_%i" % i
#         nbinsx, xmin, xmax = modify_binning(40, -10., 10.)
#         histos[hname] = TH1F(hname, "; stub #Delta s_{L%i}" % i, nbinsx, xmin, xmax)

#         hname = "stub_dphi_%i" % i
#         nbinsx, xmin, xmax = modify_binning(1000, -0.01, 0.01)
#         histos[hname] = TH1F(hname, "; stub #delta #phi_{L%i} [rad]" % i, nbinsx, xmin, xmax)

#         hname = "stub_dphi_corr_%i" % i
#         histos[hname] = TH1F(hname, "; stub #delta #phi_{L%i} (corr) [rad]" % i, nbinsx, xmin, xmax)
# 
#         hname = "stub_dphi_four_%i" % i
#         histos[hname] = TH1F(hname, "; stub #delta #phi_{L%i} (@stub r) [rad]" % i, nbinsx, xmin, xmax)
# 
#         hname = "stub_absdphi_%i" % i
#         nbinsx, xmin, xmax = modify_binning(1000, -0.01, 0.01)
#         histos[hname] = TH1F(hname, "; stub |#delta #phi|_{L%i} [rad]" % i, nbinsx, xmin, xmax)
# 
#         hname = "stub_absdphi_corr_%i" % i
#         histos[hname] = TH1F(hname, "; stub |#delta #phi|_{L%i} (corr) [rad]" % i, nbinsx, xmin, xmax)
# 
#         hname = "stub_absdphi_four_%i" % i
#         histos[hname] = TH1F(hname, "; stub |#delta #phi|_{L%i} (@stub r) [rad]" % i, nbinsx, xmin, xmax)
        
        for d in D:
            hname = "trackResidual_dx_L%i_D%02i" % (i,d)
            nbinsx, xmin, xmax = modify_binning(1000, -1., 1.) if (d<9) else modify_binning(1000, -5., 5.)
            histos[hname] = TH1F(hname, "; #delta x_{L%i-D%02i} [cm]" % (i,d), nbinsx, xmin, xmax)
    
            hname = "trackResidual_dy_L%i_D%02i" % (i,d)
            nbinsx, xmin, xmax = modify_binning(1000, -1., 1.)  if (d<9) else modify_binning(1000, -5., 5.)
            histos[hname] = TH1F(hname, "; #delta y_{L%i-D%02i} [cm]" % (i,d), nbinsx, xmin, xmax)
    
            hname = "trackResidual_dr_L%i_D%02i" % (i,d)
            nbinsx, xmin, xmax = modify_binning(1000, -1., 1.)  if (d<9) else modify_binning(1000, -5., 5.)
            histos[hname] = TH1F(hname, "; #delta r_{L%i-D%02i} [cm]" % (i,d), nbinsx, xmin, xmax)
    
            hname = "trackResidual_dphi_L%i_D%02i" % (i,d)
            nbinsx, xmin, xmax = modify_binning(1000, -0.025, 0.025) 
            histos[hname] = TH1F(hname, "; #delta #phi_{L%i-D%02i}" % (i,d), nbinsx, xmin, xmax)
    
            hname = "trackResidual_drphi_L%i_D%02i" % (i,d)
            nbinsx, xmin, xmax = modify_binning(1000, -1., 1.)  if (d<9) else modify_binning(1000, -2., 2.)
            histos[hname] = TH1F(hname, "; #delta r#phi_{L%i-D%02i} [cm]" % (i,d), nbinsx, xmin, xmax)

            hname = "trackResidual_dparallel_L%i_D%02i" % (i,d)
            nbinsx, xmin, xmax = modify_binning(1000, -1., 1.)  if (d<9) else modify_binning(1000, -5., 5.)
            histos[hname] = TH1F(hname, "; #delta x#parallel_{L%i-D%02i} [cm]" % (i,d), nbinsx, xmin, xmax)

            hname = "trackResidual_dperpendicular_L%i_D%02i" % (i,d)
            nbinsx, xmin, xmax = modify_binning(1000, -1., 1.)
            histos[hname] = TH1F(hname, "; #delta x#perp_{L%i-D%02i} [cm]" % (i,d), nbinsx, xmin, xmax)

    # TH2F
#     barrel_ds_cuts = [2.5, 2.5, 3.0, 4.5, 5.5, 6.5]
    for i in xrange(11,16):
        for d in D:
            hname = "trackResidual_dy_vs_dx_L%i_D%02i" % (i,d)
            nbinsx, xmin, xmax = modify_binning(1000, -1., 1.) if (d<9) else modify_binning(1000, -5., 5.)
            histos[hname] = TH2F(hname, " ; #delta y_{L%i_D%02i} [cm]; #delta x_{L%i_D%02i} [cm]" % (i,d,i,d), nbinsx, xmin, xmax, nbinsx, xmin, xmax)
    
            hname = "trackResidual_dx_vs_invPt_L%i_D%02i" % (i,d)
            nbinsx, xmin, xmax = 60, -0.6, 0.6
            nbinsy, ymin, ymax = modify_binning(1000, -1., -1.) if (d<9) else modify_binning(1000, -5., 5.)
            histos[hname] = TH2F(hname, "; signed 1/p_{T} [1/GeV]; #delta x_{L%i_D%02i}" % (i,d), nbinsx, xmin, xmax, nbinsy, ymin, ymax)
    
            hname = "trackResidual_dy_vs_invPt_L%i_D%02i" % (i,d)
            nbinsx, xmin, xmax = 60, -0.6, 0.6
            nbinsy, ymin, ymax = modify_binning(1000, -1., -1.) if (d<9) else modify_binning(1000, -5., 5.)
            histos[hname] = TH2F(hname, "; signed 1/p_{T} [1/GeV]; #delta y_{L%i_D%02i}" % (i,d), nbinsx, xmin, xmax, nbinsy, ymin, ymax)
    
            hname = "trackResidual_dparallel_vs_invPt_L%i_D%02i" % (i,d)
            nbinsx, xmin, xmax = 60, -0.6, 0.6
            nbinsy, ymin, ymax = modify_binning(1000, -1., -1.) if (d<9) else modify_binning(1000, -5., 5.)
            histos[hname] = TH2F(hname, "; signed 1/p_{T} [1/GeV]; #delta x#parallel_{L%i_D%02i}" % (i,d), nbinsx, xmin, xmax, nbinsy, ymin, ymax)
    
            hname = "trackResidual_dperpendicular_vs_invPt_L%i_D%02i" % (i,d)
            nbinsx, xmin, xmax = 60, -0.6, 0.6
            nbinsy, ymin, ymax = modify_binning(1000, -1., -1.) if (d<9) else modify_binning(1000, -5., 5.)
            histos[hname] = TH2F(hname, "; signed 1/p_{T} [1/GeV]; #delta x#perp_{L%i_D%02i}" % (i,d), nbinsx, xmin, xmax, nbinsy, ymin, ymax)
    
    # Style
    for hname, h in histos.iteritems():
        if "vs_" not in hname:
            h.SetLineWidth(2); h.SetMarkerSize(0)
            h.SetLineColor(col); h.SetFillColor(fcol)
        else:
            h.SetMarkerStyle(1)
    donotdelete.append(histos)
    return histos

def drawer_project(tree, histos, options):
    
    moduleCoords=loadModules()
    print "Loaded" , len(moduleCoords.keys()) , "modules" 


    fileIn = ROOT.TFile("/data/rossin/EOS/SingleMuon_Disks_20160602/SingleMuon_tt35_tt43_ntuple_9_1_Lfv.root", "open")
#     fileIn = ROOT.TFile("/home/rossin/data/EOS/SingleMuon_tt35_20150918/SingleMuon_tt35_ntuple_1000_tt43_ntuple_999.root", "open")
    tree = fileIn.Get("ntupler/tree")
    entries = tree.GetEntriesFast()
#     tree.Print()
#     tree = TChain("ntupler/tree", "")
#     tree.AddFileInfoList()
    tree.SetBranchStatus("*", 0)
    tree.SetBranchStatus("genParts_pt"           , 1)
    tree.SetBranchStatus("genParts_eta"          , 1)
    tree.SetBranchStatus("genParts_phi"          , 1)
    tree.SetBranchStatus("genParts_vx"           , 1)
    tree.SetBranchStatus("genParts_vy"           , 1)
    tree.SetBranchStatus("genParts_vz"           , 1)
    tree.SetBranchStatus("genParts_charge"       , 1)
    tree.SetBranchStatus("TTStubs_modId"         , 1)
    tree.SetBranchStatus("TTStubs_r"             , 1)
    tree.SetBranchStatus("TTStubs_phi"           , 1)
    tree.SetBranchStatus("TTStubs_x"             , 1)
    tree.SetBranchStatus("TTStubs_y"             , 1)
    tree.SetBranchStatus("TTStubs_z"             , 1)
    tree.SetBranchStatus("TTStubs_trigBend"      , 1)

    # Loop over events
    for ievt, evt in enumerate(tree):
        if (ievt == options.nentries):  break

        if (ievt % 1000 == 0):  print "Processing event: %i" % ievt

        # Only tracks with at least 5 stubs
        if evt.TTStubs_modId.size() < 5:
            continue

        # All the last 5 stubs must be in the disks
        layers = []
        for moduleId in evt.TTStubs_modId:
            lay = decodeLayer(moduleId)
            layers.append(lay)
        countLayers=[0,0,0,0,0]
        for l in layers:
            countLayers[0] += (l==11)
            countLayers[1] += (l==12)
            countLayers[2] += (l==13)
            countLayers[3] += (l==14)
        for l in countLayers:
            if (l==0 or l>3): continue;
#         if (layers[-5:] != [11,12,13,14,15] and layers[-4:] != [11,12,13,14] and layers[-3:] != [11,12,13]):
#         if (layers[-5:] != [11,12,13,14,15]):
            continue

        # Track info
        gen_pt     = evt.genParts_pt[0]
        if gen_pt<3: continue
        gen_eta    = evt.genParts_eta[0]
        gen_phi    = evt.genParts_phi[0]
        gen_vx     = evt.genParts_vx[0]
        gen_vy     = evt.genParts_vy[0]
        gen_vz     = evt.genParts_vz[0]
        gen_charge = evt.genParts_charge[0]
        gen_invPt  = float(gen_charge) / gen_pt

        for j in xrange(len(layers)):
            # Stub info
            stub_modId = evt.TTStubs_modId[j]
            if (stub_modId<110000): continue;
            i_layer = int (stub_modId)/10000 - 11
            i_disk  = int ((stub_modId/100)%100)

            stub_phi   = evt.TTStubs_phi     [j]
            stub_r     = evt.TTStubs_r       [j]
            stub_x     = evt.TTStubs_x       [j]
            stub_y     = evt.TTStubs_y       [j]
            stub_z     = evt.TTStubs_z       [j]
            stub_ds    = evt.TTStubs_trigBend[j]
#             stub_dr    = rMeans[i] - stub_r

#             print j, stub_x, stub_y, stub_r*stub_r, stub_z, stub_modId, gen_vz#, " | ", 100*calcIdealXY(gen_eta, gen_vz, gen_invPt, gen_phi, stub_z)[0],100*calcIdealXY(gen_eta, gen_vz, gen_invPt, gen_phi, stub_z)[1]
            X, Y = calcIdealXY(gen_eta, gen_vz, gen_invPt, gen_phi, stub_z)
            X += gen_vx
            Y += gen_vy
            R   = math.sqrt(X*X+Y*Y)
            Phi = math.atan2(Y,X)
            module=moduleCoords[stub_modId]
            moduleStripPhi = module[-1]
            Sx = math.cos(moduleStripPhi)
            Sy = math.sin(moduleStripPhi)
            Dx = X-stub_x
            Dy = Y-stub_y
            Dparallel = Sx*Dx+Sy*Dy
            DphiResidualToStrip = math.acos(Dparallel/math.sqrt(Dx*Dx+Dy*Dy))
            Dperpendicular = math.sqrt(Dx*Dx+Dy*Dy)*math.sin(DphiResidualToStrip)
            if ievt%2: Dperpendicular =-Dperpendicular

#             i = j-(len(layers)-5)
            i = i_layer
            d = i_disk
#             if stub_r > 59.9: d = 1
            histos["trackResidual_dx_L%i_D%02i"              % (i+11,(D[d]))].Fill(Dx)
            histos["trackResidual_dy_L%i_D%02i"              % (i+11,(D[d]))].Fill(Dy)
            histos["trackResidual_dr_L%i_D%02i"              % (i+11,(D[d]))].Fill(R-stub_r)
            histos["trackResidual_dphi_L%i_D%02i"            % (i+11,(D[d]))].Fill(Phi-stub_phi)
            histos["trackResidual_drphi_L%i_D%02i"           % (i+11,(D[d]))].Fill(stub_r*(Phi-stub_phi))
            histos["trackResidual_dperpendicular_L%i_D%02i"  % (i+11,(D[d]))].Fill(Dperpendicular)
            histos["trackResidual_dparallel_L%i_D%02i"       % (i+11,(D[d]))].Fill(Dparallel)
            
            histos["trackResidual_dy_vs_dx_L%i_D%02i"        % (i+11,(D[d]))].Fill(X-stub_x ,Y-stub_y)
            histos["trackResidual_dx_vs_invPt_L%i_D%02i"     % (i+11,(D[d]))].Fill(gen_invPt,X-stub_x)
            histos["trackResidual_dy_vs_invPt_L%i_D%02i"     % (i+11,(D[d]))].Fill(gen_invPt,Y-stub_y)
            histos["trackResidual_dperpendicular_vs_invPt_L%i_D%02i"     % (i+11,(D[d]))].Fill(Dperpendicular,Y-stub_y)
            histos["trackResidual_dparallel_vs_invPt_L%i_D%02i"     % (i+11,(D[d]))].Fill(Dparallel,Y-stub_y)

#             histos["stub_ds_vs_invPt_%i" % i].Fill(gen_invPt, stub_ds)
# 
#             ideal_phi_at_stub_r = calcIdealPhi(gen_phi, gen_invPt, stub_r)
#             ideal_phi_at_mean_r = calcIdealPhi(gen_phi, gen_invPt, rMeans[i])
#             stub_phi_corr = stub_phi - drCorrs[i] * stub_ds * stub_dr
#             ideal_xy_at_stub_z  = calcIdealXY(gen_eta, gen_vz, gen_invPt, gen_phi, stub_z)
#             
# 
#             histos["stub_dphi_%i" % i].Fill(stub_phi - ideal_phi_at_mean_r)
#             histos["stub_dphi_corr_%i" % i].Fill(stub_phi_corr - ideal_phi_at_mean_r)
#             histos["stub_dphi_four_%i" % i].Fill(stub_phi - ideal_phi_at_stub_r)
# 
#             histos["stub_absdphi_%i" % i].Fill(abs(stub_phi - ideal_phi_at_mean_r))
#             histos["stub_absdphi_corr_%i" % i].Fill(abs(stub_phi_corr - ideal_phi_at_mean_r))
#             histos["stub_absdphi_four_%i" % i].Fill(abs(stub_phi - ideal_phi_at_stub_r))
# 
#             ideal_z_at_stub_r = calcIdealZ(gen_eta, gen_vz, gen_invPt, stub_r)
#             ideal_z_at_mean_r = calcIdealZ(gen_eta, gen_vz, gen_invPt, rMeans[i])
#             stub_z_corr = 0
# 
#             histos["stub_dz_%i" % i].Fill(stub_z - ideal_z_at_mean_r)
#             #histos["stub_dz_corr_%i" % i].Fill(stub_z_corr - ideal_z_at_mean_r)
#             histos["stub_dz_four_%i" % i].Fill(stub_z - ideal_z_at_stub_r)
# 
#             histos["stub_absdz_%i" % i].Fill(abs(stub_z - ideal_z_at_mean_r))
#             #histos["stub_absdz_corr_%i" % i].Fill(abs(stub_z_corr - ideal_z_at_mean_r))
#             histos["stub_absdz_four_%i" % i].Fill(abs(stub_z - ideal_z_at_stub_r))

    for v in var:
        for i in xrange(11,16):
            for CL in CLstring:
                h2=histos["trackResidual_2D_%s_%s" % (v,CL)]
                for d in range(len(D)):
                    h=histos["trackResidual_%s_L%i_D%02i"              % (v,i,D[d])]
                    if (CL=='RMS'): h2.SetBinContent(i-11+1,d+1,h.GetRMS())
                    else          : h2.SetBinContent(i-11+1,d+1,getCI(h,0.9))
    options.nentries = ievt
    tree.SetBranchStatus("*", 1)
    return

def drawer_draw(histos, options):
    gStyle.SetPaintTextFormat("2.4f")

    for hname, h in histos.iteritems():
        if h.ClassName() == "TH1F":
            # TH1F
            h.SetMaximum(h.GetMaximum() * 1.4); h.SetMinimum(0)
            h.SetStats(1); h.Draw("hist")
            CMS_label()
            save(options.outdir, hname, dot_root=False, dot_pdf=False)
        else:
            # TH2F
            if hname.find("_vs_")>=0:
                pf = h.ProfileX("pf_"+hname, 1, -1, "s")
                pf.SetLineColor(2)
                pf.SetLineWidth(1)
                h.SetMinimum(1); h.SetMaximum(options.nentries/1e2); h.Draw("COLZ")
                gPad.SetLogz(1)
                gPad.SetRightMargin(0.12)
                gPad.Modified(); gPad.Update()
#                 movePalette(h)
#                 gPad.Modified(); gPad.Update()
#                 pf.Fit("pol1", "N")
                pf.Draw("same")
            else:
                h.SetStats(0)
                h.SetMinimum(0);
                h.SetMarkerSize(1.5)
                h.Draw("COLZ")
                h.Draw("text same")
                gPad.SetLogz(0)
                gPad.SetRightMargin(0.22)
                gPad.Modified(); gPad.Update()

# 
#                 tline.DrawLine(h.GetXaxis().GetXmin(), h.ds_cut+0.25, h.GetXaxis().GetXmax(), h.ds_cut+0.25)
#                 tline.DrawLine(h.GetXaxis().GetXmin(), -h.ds_cut-0.25, h.GetXaxis().GetXmax(), -h.ds_cut-0.25)

#             CMS_label()
            save(options.outdir, hname, dot_root=False, dot_pdf=False)

            if hname.startswith("stub_ds_vs_invPt"):
                gPad.SetLogz(0)
                gPad.SetRightMargin(0.05)

def drawer_sitrep(histos, options):
    # Get one-sided confidence interval

    print "--- SITREP ---------------------------------------------------------"
#     print "--- Using rMeans : ", [round(x,2) for x in rMeans]
#     print "--- Using drCorrs: ", [round(x,6) for x in drCorrs]

    values1, values2 = [], []
    for i in xrange(5):
        for d in D:
            h = histos["trackResidual_dparallel_L%i_D%02i"       % (i+11,d)]
            values1.append(h.GetRMS())
            values2.append(getCI(h,0.9))
    print "90% CI parallel width       :", [round(x,6) for x in values2]

    values1, values2 = [], []
    for i in xrange(5):
        for d in D:
            h = histos["trackResidual_dperpendicular_L%i_D%02i"       % (i+11,d)]
            values1.append(h.GetRMS())
            values2.append(getCI(h,0.9))
    print "90% CI perpendicular width       :", [round(x,6) for x in values2]



# ______________________________________________________________________________
# Main function
def main(options):

    # Init
    drawerInit = DrawerInit()
    tchain = TChain("ntupler/tree", "")
    tchain.AddFileInfoList(options.tfilecoll.GetList())

    # Process
    histos = drawer_book()
    drawer_project(tchain, histos, options)
    drawer_draw(histos, options)
    drawer_sitrep(histos, options)


# ______________________________________________________________________________
if __name__ == '__main__':

    # Setup argument parser
    parser = argparse.ArgumentParser()

    # Add default arguments
    add_drawer_arguments(parser)

    # Add more arguments
    #parser.add_argument("ss", help="short name of superstrip definition (e.g. ss256)")
    #parser.add_argument("npatterns", type=int, help="number of patterns to reach the desired coverage")

    # Parse default arguments
    options = parser.parse_args()
    parse_drawer_options(options)

    # Call the main function
    main(options)
