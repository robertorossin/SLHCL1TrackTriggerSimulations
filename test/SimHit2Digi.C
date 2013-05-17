/*
  Small ROOT macro showing an example of extracted data analysis.

  RECIPE 2: Matching simhits and digis


  Use:

  root[1]-> .L SimHit2Digi.C
  root[2]->  simhit2digi(filename) // Link simhits to digis 

  where filename is the name of the extracted ROOT file

  Information about this macro may be found in the following page:

  http://sviret.web.cern.ch/sviret/Welcome.php?n=CMS.HLLHCTuto (STEP III)

  Author: viret@in2p3_dot_fr
  Date: 16/05/2013

*/

void simhit2digi(std::string filename)
{
  gROOT->ProcessLine("#include <vector>");
  

  // First get the data
  // by merging all the available files

  TChain *Pix              = new TChain("Pixels");   
  TChain *MC               = new TChain("MC");   

  Pix->Add(filename.c_str());
  MC->Add(filename.c_str());

//
//1/ Pixels TREE content:
//
// http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/HL_LHC/Extractors/RecoExtractor/interface/PixelExtractor.h?view=markup
//

  int                 m_pix; 

  std::vector<float>  *m_pix_x;  
  std::vector<float>  *m_pix_y;  
  std::vector<float>  *m_pix_z;  
  std::vector<float>  *m_pix_e;  
  std::vector<int>    *m_pix_row;   
  std::vector<int>    *m_pix_column;
  std::vector<int>    *m_pix_simhit;
  std::vector< std::vector<int> >  *m_pix_simhitID;
  std::vector<int>    *m_pix_layer; 
  std::vector<int>    *m_pix_module;
  std::vector<int>    *m_pix_ladder;

  Pix->SetBranchAddress("PIX_n",         &m_pix);
  Pix->SetBranchAddress("PIX_x",         &m_pix_x);
  Pix->SetBranchAddress("PIX_y",         &m_pix_y);
  Pix->SetBranchAddress("PIX_z",         &m_pix_z);
  Pix->SetBranchAddress("PIX_charge",    &m_pix_e);
  Pix->SetBranchAddress("PIX_row",       &m_pix_row);
  Pix->SetBranchAddress("PIX_column",    &m_pix_column);
  Pix->SetBranchAddress("PIX_simhit",    &m_pix_simhit);
  Pix->SetBranchAddress("PIX_simhitID",  &m_pix_simhitID);
  Pix->SetBranchAddress("PIX_layer",     &m_pix_layer);
  Pix->SetBranchAddress("PIX_module",    &m_pix_module);
  Pix->SetBranchAddress("PIX_ladder",    &m_pix_ladder);

//
//2/ MC TREE content:
//
// http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/HL_LHC/Extractors/RecoExtractor/interface/MCExtractor.h?view=markup
//

  int    		m_gen_n;    
  int    		m_part_n;    
  int                   m_part_nstracks;  
  int    		m_part_nhit;
 
  std::vector<float>    *m_gen_x;    
  std::vector<float>    *m_gen_y;    
  std::vector<float>    *m_gen_z;    
  std::vector<float> 	*m_gen_px;   
  std::vector<float> 	*m_gen_py;   
  std::vector<float>    *m_gen_pz;   
  std::vector<int>      *m_gen_pdg;  
  std::vector<int>      *m_part_pdgId;
  std::vector<float> 	*m_part_px;   
  std::vector<float> 	*m_part_py;   
  std::vector<float>	*m_part_pz;   
  std::vector<float>    *m_part_eta;  
  std::vector<float> 	*m_part_phi;  
  std::vector<float>    *m_part_x;    
  std::vector<float>    *m_part_y;    
  std::vector<float>    *m_part_z;    
  std::vector<int>      *m_st;        
  std::vector<int>      *m_hits;      
  std::vector<int>      *m_st_id;  
  std::vector<float>    *m_hits_x;    
  std::vector<float>    *m_hits_y;    
  std::vector<float>    *m_hits_z;    
  std::vector<float>    *m_hits_e;    
  std::vector<float>    *m_hits_tof;  
  std::vector<int>      *m_hits_proc; 
  std::vector<int>      *m_hits_id;   
  std::vector<int>      *m_hits_pdgId;
  std::vector<int>      *m_hits_layer;
  std::vector<int>      *m_hits_ladder;
  std::vector<int>      *m_hits_module;

  MC->SetBranchAddress("gen_n",            &m_gen_n);
  MC->SetBranchAddress("gen_pdg",          &m_gen_pdg);
  MC->SetBranchAddress("gen_px",           &m_gen_px);
  MC->SetBranchAddress("gen_py",           &m_gen_py);
  MC->SetBranchAddress("gen_pz",           &m_gen_pz);
  MC->SetBranchAddress("gen_x",            &m_gen_x);
  MC->SetBranchAddress("gen_y",            &m_gen_y);
  MC->SetBranchAddress("gen_z",            &m_gen_z);
  MC->SetBranchAddress("subpart_n",        &m_part_n);
  MC->SetBranchAddress("subpart_hits",     &m_hits);
  MC->SetBranchAddress("subpart_st",       &m_st);
  MC->SetBranchAddress("subpart_pdgId",    &m_part_pdgId);
  MC->SetBranchAddress("subpart_px",       &m_part_px);
  MC->SetBranchAddress("subpart_py",       &m_part_py);
  MC->SetBranchAddress("subpart_pz",       &m_part_pz);
  MC->SetBranchAddress("subpart_eta",      &m_part_eta);
  MC->SetBranchAddress("subpart_phi",      &m_part_phi);
  MC->SetBranchAddress("subpart_x",        &m_part_x);
  MC->SetBranchAddress("subpart_y",        &m_part_y);
  MC->SetBranchAddress("subpart_z",        &m_part_z);
  MC->SetBranchAddress("subpart_nhit",     &m_part_nhit);
  MC->SetBranchAddress("subpart_hits_x",   &m_hits_x);
  MC->SetBranchAddress("subpart_hits_y",   &m_hits_y);
  MC->SetBranchAddress("subpart_hits_z",   &m_hits_z);
  MC->SetBranchAddress("subpart_hits_e",   &m_hits_e);
  MC->SetBranchAddress("subpart_hits_tof", &m_hits_tof);
  MC->SetBranchAddress("subpart_hits_proc",&m_hits_proc);
  MC->SetBranchAddress("subpart_hits_id",  &m_hits_id);
  MC->SetBranchAddress("subpart_hits_pdgId",  &m_hits_pdgId);
  MC->SetBranchAddress("subpart_hits_layer",  &m_hits_layer);
  MC->SetBranchAddress("subpart_hits_ladder", &m_hits_ladder);
  MC->SetBranchAddress("subpart_hits_module", &m_hits_module);
  MC->SetBranchAddress("subpart_nsimtracks",  &m_part_nstracks);
  MC->SetBranchAddress("subpart_st_ids",      &m_st_id);


  TH1F* x_match = new TH1F("x_match","",200,-3.,3.);
  TH1F* y_match = new TH1F("y_match","",200,-3.,3.);
  TH1F* z_match = new TH1F("z_match","",200,-3.,3.);

  TH2F* x_vs_y_match = new TH2F("x_vs_y_match","",200,-3.,3.,200,-3.,3.);

  int n_entries = MC->GetEntries();

  double PTGEN   = 0.;
  double D0GEN   = 0.;
  //int    nh_tot  = 0;
  //int    nst_tot = 0;

  int  n_matched   = 0;
  int  n_unmatched = 0;

  double dphi_min = 0.;
  double dphi     = 0.;
  double dr       = 0.;
  double dz       = 0.;

  int disk      = 0;
  int lad_cor   = 0;

  int lay;
  int lad;
  int mod;

  int i_st;
  int i_match;

  for (int j=0;j<n_entries;++j)
  {
    MC->GetEntry(j);
    Pix->GetEntry(j);

    int nh_tot=0;
    int nst_tot=0;

    if (j==0)
    {
      cout <<endl;
      cout << "Analyzing event " << j <<endl;
      cout << "where " << m_gen_n << " particle(s) where produced" <<endl;
    }

    // Loop over tracking particles
    for (int k=0;k<m_part_n;++k)
    {
      D0GEN = sqrt(m_part_x->at(k)*m_part_x->at(k)+m_part_y->at(k)*m_part_y->at(k));
      PTGEN = sqrt(m_part_px->at(k)*m_part_px->at(k)+m_part_py->at(k)*m_part_py->at(k));
 
      if (j==0)
      {
	cout << "-------------------------------------------------"  <<endl;
	cout << "Tracking particle " << k << " properties:"  <<endl;
	cout << "PDG code      : " << m_part_pdgId->at(k) << endl;
	cout << "pT (in GeV/c) : " << PTGEN << endl;
	cout << "d0 (in cm)    : " << D0GEN << endl;
	cout << "z0 (in cm)    : " << m_part_z->at(k) << endl;
	cout << "N_simhits     : " << m_hits->at(k) << endl;
	cout << "N_simtracks   : " << m_st->at(k) << endl;
      }

      // Looping on the TP simhits
      
      for (int l=nh_tot;l<m_hits->at(k)+nh_tot;++l)
      {
	if (m_hits_layer->at(l)<5) continue; // Don't care about pixel hits

	lay = m_hits_layer->at(l);
	mod = m_hits_module->at(l);
	lad = m_hits_ladder->at(l)-1;

	disk=0;
	if (lay>10 && lay<=17) disk=int(lay-10)%8;
	if (lay>17 && lay<=24) disk=-int(lay-17)%8;
	if (abs(disk)>=5)      lad = lad+int(abs(disk)-4)%4;

	if (j==0)
	{
	  if (disk==0)
	  {
	    cout << "    SimHit " << l << " in barrel layer " << lay <<endl;
	    cout << "    X/Y/Z (in cm) : " << m_hits_x->at(l) 
		 << "/" << m_hits_y->at(l) 
		 << "/" << m_hits_z->at(l) << endl; 
	  }
	  else
	  {
	    cout << "    SimHit " << l << " in endcap disk " << disk <<endl;
	    cout << "    X/Y/Z (in cm) : " << m_hits_x->at(l) 
		 << "/" << m_hits_y->at(l) 
		 << "/" << m_hits_z->at(l) << endl; 
	  }
	}

	// Then try to match the simhit with a digi

	dphi_min = 100.;
	dphi = dphi_min;
	i_match = -1;

	for (int i=0;i<m_pix;++i)
	{
	  // First of all few basic info
	  if (m_pix_layer->at(i)!=lay) continue;
	  if (m_pix_module->at(i)!=mod) continue;	      
	  if (m_pix_ladder->at(i)!= m_hits_ladder->at(l)) continue;

	  // Then go for a more precise matchin
	  
	  for (int ik=0;ik<m_pix_simhit->at(i);++ik) // Loop over simhit (matching)
	  {
	    i_st = m_pix_simhitID->at(i).at(ik);

	    for (int il=nst_tot;il<m_st->at(k)+nst_tot;++il)
	    {
	      if (i_st==m_st_id->at(il))
	      {
		dz = fabs(m_pix_z->at(i)-m_hits_z->at(l));
		dr = fabs(sqrt(m_pix_x->at(i)*m_pix_x->at(i)+m_pix_y->at(i)*m_pix_y->at(i))
			  -sqrt(m_hits_x->at(l)*m_hits_x->at(l)+m_hits_y->at(l)*m_hits_y->at(l)));

		dphi = fabs(atan2(m_pix_y->at(i),m_pix_x->at(i))
			    -atan2(m_hits_y->at(l),m_hits_x->at(l)));

		if (dphi<dphi_min)
		{
		  dphi_min = dphi;
		  i_match = i;
		}
	      }
	    }
	  }

	  if (i_match!=-1)
	  {
	    if (j==0)
	    {
	      cout << "    matched with Digi " << i_match << endl;
	      cout << "    X/Y/Z (in cm) : " << m_pix_x->at(i) << "/" 
		   << m_pix_y->at(i) << "/"
		   << m_pix_z->at(i) << endl;
	    }
	    break;
	  }
	}

	if (i_match!=-1)
	{
	  n_matched++;

	  x_match->Fill(m_pix_x->at(i_match)-m_hits_x->at(l));
	  y_match->Fill(m_pix_y->at(i_match)-m_hits_y->at(l));
	  z_match->Fill(m_pix_z->at(i_match)-m_hits_z->at(l));

	  x_vs_y_match->Fill(m_pix_x->at(i_match)-m_hits_x->at(l),
			     m_pix_y->at(i_match)-m_hits_y->at(l));
	}
	else
	{
	  n_unmatched++;
	}
      }
      
      nst_tot+=m_st->at(k);
      nh_tot+=m_hits->at(k);

    }
  }

  cout << "We have looped over " << n_unmatched+n_matched << " simhits" << endl; 
  cout << n_matched << " were matched" << endl; 

  const Int_t NRGBs = 5;
  const Int_t NCont = 255;
  
  Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
  Double_t red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
  Double_t green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
  Double_t blue[NRGBs]  = { 0.51, 1.00, 0.12, 0.00, 0.00 };
  TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
  gStyle->SetNumberContours(NCont);
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);

  c2 = new TCanvas("c2","Tracker SimHit matching II",201,107,977,913);

  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
  c2->Range(-3.8,-3.756374,4.308333,3.747875);
  c2->SetFillColor(0);
  c2->SetBorderMode(0);
  c2->SetBorderSize(2);
  c2->SetRightMargin(0.1613566);
  c2->SetFrameBorderMode(0);
  c2->SetFrameBorderMode(0);

  x_vs_y_match->GetXaxis()->SetTitle("x_{Digi}-x_{SimHit} (in cm)");
  x_vs_y_match->GetYaxis()->SetTitle("y_{Digi}-y_{SimHit} (in cm)");
  x_vs_y_match->Draw("colz");

  c2->Update();

  c1 = new TCanvas("c1","Tracker SimHit matching",201,107,1488,879);
  c1->SetFillColor(0);
  c1->SetBorderMode(0);
  c1->Divide(3,1);
  c1->Range(-149,-3.9,149.,3.6);
  c1->SetFillColor(0);
  c1->SetBorderMode(0);
  c1->SetBorderSize(2);
  c1->SetRightMargin(0.066);
  c1->SetTopMargin(0.066);
  c1->SetFrameBorderMode(0);
  c1->SetFrameBorderMode(0);

  c1->cd(1);
  x_match->GetXaxis()->SetTitle("x_{Digi}-x_{SimHit} (in cm)");
  x_match->Draw();

  c1->cd(2);
  y_match->GetXaxis()->SetTitle("y_{Digi}-y_{SimHit} (in cm)");
  y_match->Draw();

  c1->cd(3);
  z_match->GetXaxis()->SetTitle("z_{Digi}-z_{SimHit} (in cm)");
  z_match->Draw();

  c1->Update();


}
