#include "AnalysisClass.hh"
#include <TH1.h>
#include <TH2.h>
#include <TH3.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TVector3.h>
#include <TF1.h>

#include <ctime>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <fstream>
#include <string>

using namespace std;

AnalysisClass::AnalysisClass(){}

AnalysisClass::~AnalysisClass()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t AnalysisClass::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t AnalysisClass::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void AnalysisClass::InitTree(TString treeName="tree")
{
    TTree *tree;
    TFile *f = new TFile(_input_filename.Data());
    if (f->IsOpen())
    {
        f->GetObject(treeName.Data(),tree);
    }
    else
    {
        printf("ERROR:: Cannot of the file!");
        assert(0);
    }


    // The Init() function is called when the selector needs to initialize
    // a new tree or chain. Typically here the branch addresses and branch
    // pointers of the tree will be set.
    // It is normally not necessary to make changes to the generated
    // code, but the routine can be extended by the user if needed.
    // Init() will be called many times when running on PROOF
    // (once per file to be processed).

    // Set branch addresses and branch pointers
    if (!tree) return;
    fChain = tree;
    fCurrent = -1;
    fChain->SetMakeClass(1);

    fChain->SetBranchAddress("EventID", &EventID, &b_EventID);
    fChain->SetBranchAddress("PDG", &PDG, &b_PDG);
    fChain->SetBranchAddress("Q", &Q, &b_Q);
    fChain->SetBranchAddress("Mass", &Mass, &b_Mass);
    fChain->SetBranchAddress("Det0", &Det0, &b_Det0);
    fChain->SetBranchAddress("X0", &X0, &b_X0);
    fChain->SetBranchAddress("Y0", &Y0, &b_Y0);
    fChain->SetBranchAddress("Z0", &Z0, &b_Z0);
    fChain->SetBranchAddress("MomX0", &MomX0, &b_MomX0);
    fChain->SetBranchAddress("MomY0", &MomY0, &b_MomY0);
    fChain->SetBranchAddress("MomZ0", &MomZ0, &b_MomZ0);
    fChain->SetBranchAddress("Ekin0", &Ekin0, &b_Ekin0);
    fChain->SetBranchAddress("Det1", &Det1, &b_Det1);
    fChain->SetBranchAddress("X1", &X1, &b_X1);
    fChain->SetBranchAddress("Y1", &Y1, &b_Y1);
    fChain->SetBranchAddress("Z1", &Z1, &b_Z1);
    fChain->SetBranchAddress("MomX1", &MomX1, &b_MomX1);
    fChain->SetBranchAddress("MomY1", &MomY1, &b_MomY1);
    fChain->SetBranchAddress("MomZ1", &MomZ1, &b_MomZ1);
    fChain->SetBranchAddress("Ekin1", &Ekin1, &b_Ekin1);
    fChain->SetBranchAddress("targetL", &targetL, &b_targetL);
    fChain->SetBranchAddress("targetW", &targetW, &b_targetW);
    fChain->SetBranchAddress("targetH", &targetH, &b_targetH);
    Notify();
}

Bool_t AnalysisClass::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void AnalysisClass::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t AnalysisClass::Cut(Long64_t)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}

void AnalysisClass::Loop()
{
    if (fChain == 0) return;

    Long64_t nEntries = fChain->GetEntriesFast();
    cout<<"--> nEntries = "<<nEntries<<endl;

    TFile *f = new TFile(_output_filename.Data(),"RECREATE");
    TH1D* h0 = new TH1D("h0","D0 vs target length",400,0,40e-3);
    TH1D* h1 = new TH1D("h1","D1 vs target length",400,0,40e-3);
    TGraphErrors* gr2 = new TGraphErrors(); gr2->SetName("gr2");
    TH1D* h3 = new TH1D("h3","#theta_{Det0}",1000000,-TMath::Pi(),TMath::Pi());
    TH1D* h4 = new TH1D("h4","#theta_{Det1}",1000000,-TMath::Pi(),TMath::Pi());
    TH1D* h5 = new TH1D("h5","#theta_{Scat.}",1000000,0,TMath::PiOver2());
    TH2D* h6 = new TH2D("h6","#theta_{Scat.} vs target length",400,0,40e-3,10000,0,TMath::PiOver2());
    TH1D* h7 = new TH1D("h7","E_{kin,Det0}",1000000,0,10);
    TH1D* h8 = new TH1D("h8","E_{kin,Det1}",1000000,0,10);
    TH1D* h9 = new TH1D("h9","#DeltaE_{kin}",1000000,-10,10);
    TH2D* h10 = new TH2D("h10","#DeltaE_{kin} vs target length",400,0,40e-3,10000,-10,10);
    TH2D* h11 = new TH2D("h11","#DeltaE_{kin} vs #theta_{Scat.}",1000,0,TMath::PiOver2(),1000,-10,10);
    TH3D* h12 = new TH3D("h12","#DeltaE_{kin} vs #theta_{Scat.} vs target length",80,0,40e-3,100,0,TMath::PiOver2(),100,0,4);

    TVector3 particleMom0;
    TVector3 particleMom1;

    const Int_t nP = 41;
    Double_t Pmin[nP];
    Double_t Pmax[nP];
    TH1D* hh0[nP];
    TH1D* hh1[nP];
    TH2D* hh6[nP];
    TH2D* hh10[nP];
    TH2D* hh11[nP];
    TGraphErrors* grProb[nP];
    TGraphErrors* grTheta[nP];

    fChain->GetEntry(0);
    static Double_t nominalMom = (PDG > 0)? /*e-*/7.00729 : /*e+*/4.00000; // [GeV/c]

    for(Int_t i = 0; i < nP; i++)
    {
        Double_t deltaP = -0.02 + i*0.001;
        Pmin[i] = nominalMom*(1.0 + deltaP - 0.001/2.0);
        Pmax[i] = nominalMom*(1.0 + deltaP + 0.001/2.0);

        TString grTitle, grName;

        grTitle.Form("P = [%.3f | %.3f] GeV/c",Pmin[i],Pmax[i]);

        grName.Form("grProb_%d",i);
        grProb[i] = new TGraphErrors();
        grProb[i]->SetName(grName.Data());
        grProb[i]->SetTitle(grTitle.Data());

        grName.Form("grTheta_%d",i);
        grTheta[i] = new TGraphErrors();
        grTheta[i]->SetName(grName.Data());
        grTheta[i]->SetTitle(grTitle.Data());

        grName.Form("hh0_%d",i);
        grTitle.Form("D0: P = [%.3f | %.3f] GeV/c",Pmin[i],Pmax[i]);
        hh0[i] = new TH1D(grName.Data(),grTitle.Data(),400,0,40e-3);

        grName.Form("hh1_%d",i);
        grTitle.Form("D1: P = [%.3f | %.3f] GeV/c",Pmin[i],Pmax[i]);
        hh1[i] = new TH1D(grName.Data(),grTitle.Data(),400,0,40e-3);

        grName.Form("hh6_%d",i);
        grTitle.Form("#theta_{Scat.}: P = [%.3f | %.3f] GeV/c",Pmin[i],Pmax[i]);
        hh6[i] = new TH2D(grName.Data(),grTitle.Data(),400,0,40e-3,10000,0,TMath::PiOver2());

        grName.Form("hh10_%d",i);
        grTitle.Form("#DeltaE_{kin}: P = [%.3f | %.3f] GeV/c",Pmin[i],Pmax[i]);
        hh10[i] = new TH2D(grName.Data(),grTitle.Data(),400,0,40e-3,10000,-10,10);

        grName.Form("hh11_%d",i);
        grTitle.Form("#DeltaE_{kin} vs #theta_{Scat.}: P = [%.3f | %.3f] GeV/c",Pmin[i],Pmax[i]);
        hh11[i] = new TH2D(grName.Data(),grTitle.Data(),1000,0,TMath::PiOver2(),1000,-10,10);
    }

    cout<<endl;
    for (Long64_t jentry = 0; jentry < nEntries; jentry++)
    {
        if(jentry%1000 == 0)
        {
            printf("\r--> Progress: %.1f %%",100.0*jentry/nEntries);
            fflush(stdout);
        }

        fChain->GetEntry(jentry);

        particleMom0.SetXYZ(MomX0,MomY0,MomZ0);
        particleMom1.SetXYZ(MomX1,MomY1,MomZ1);

        if(Det0 == 1)
        {
            h0->Fill(targetL);
            h3->Fill(particleMom0.Theta());
            h7->Fill(Ekin0);

            for(Int_t i = 0; i < nP; i++)
            {
                if(particleMom0.Mag() < Pmax[i] && particleMom0.Mag() > Pmin[i])
                {
                    hh0[i]->Fill(targetL);
                }
            }

            if(Det1 == 1)
            {
                h1->Fill(targetL);
                h4->Fill(particleMom1.Theta());
                h5->Fill(particleMom1.Theta()-particleMom0.Theta());
                h6->Fill(targetL,particleMom1.Theta()-particleMom0.Theta());
                h8->Fill(Ekin1);
                h9->Fill(Ekin0-Ekin1);
                h10->Fill(targetL,Ekin0-Ekin1);
                h11->Fill(particleMom1.Theta()-particleMom0.Theta(),Ekin0-Ekin1);
                h12->Fill(targetL,particleMom1.Theta()-particleMom0.Theta(),Ekin0-Ekin1);

                for(Int_t i = 0; i < nP; i++)
                {
                    if(particleMom0.Mag() < Pmax[i] && particleMom0.Mag() > Pmin[i])
                    {
                        hh1[i]->Fill(targetL);
                        hh6[i]->Fill(targetL,particleMom1.Theta()-particleMom0.Theta());
                        hh10[i]->Fill(targetL,Ekin0-Ekin1);
                        hh11[i]->Fill(particleMom1.Theta()-particleMom0.Theta(),Ekin0-Ekin1);
                    }
                }
            }
        }
    }
    cout<<endl;

    GetProbability(h0,h1,gr2);
    for(Int_t i = 0; i < nP; i++)
    {
        GetProbability(hh0[i],hh1[i],grProb[i]);
        grProb[i]->Write();
    }

    h0->Write();
    h1->Write();
    gr2->Write();
    h3->Write();
    h4->Write();
    h5->Write();
    h6->Write();
    h7->Write();
    h8->Write();
    h9->Write();
    h10->Write();
    h11->Write();
    h12->Write();

    for(Int_t i = 0; i < nP; i++)
    {
        hh0[i]->Write();
        hh1[i]->Write();
        hh6[i]->Write();
        hh10[i]->Write();
        hh11[i]->Write();
    }

    f->Close();
}

void AnalysisClass::GetProbability(TH1D* h0, TH1D* h1, TGraphErrors* gr)
{
    Int_t nBins = 0;
    if(h0->GetNbinsX() != h1->GetNbinsX())
    {
        cout<<endl<<"ERROR:: Different number of bins in X-axis!"<<endl<<endl;
        assert(0);
    }
    else
    {
        nBins = h0->GetNbinsX();

        for(Int_t binxi = 1; binxi <= nBins; binxi++)
        {
            Double_t xx = h0->GetBinCenter(binxi);
            Double_t ex = h0->GetBinWidth(1)/TMath::Sqrt(12.0);
            Double_t v0 = h0->GetBinContent(binxi);
            Double_t e0 = h0->GetBinError(binxi);
            Double_t v1 = h1->GetBinContent(binxi);
            Double_t e1 = h1->GetBinError(binxi);
            Double_t e2 = TMath::Sqrt(TMath::Power(e1/v0,2) + TMath::Power(v1*e0/(v0*v0),2));
            Double_t v2 = v1/v0;

            if(v0 != 0)
            {
                gr->SetPoint(gr->GetN(),xx,v2);
                gr->SetPointError(gr->GetN()-1,ex,e2);
            }
        }
    }
}













