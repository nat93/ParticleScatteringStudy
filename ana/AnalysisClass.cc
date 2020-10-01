#include "AnalysisClass.hh"
#include <TH1.h>
#include <TH2.h>
#include <TH3.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TVector3.h>
#include <TF1.h>
#include <THnSparse.h>

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
    f->cd();

    //----------------------------------------------- SETTINGS ---------------------------------------------//
    //______________________________________________________________________________________________________//
    // check with plot.cc
    const Int_t dim = 3;
    Double_t xx[dim];
    //======== LER W ========//
//    Int_t bins[dim]       = {2*60/1, 1600000, 80000};
//    Double_t xmin[dim]    = {0.0, 0.0, 0.0};
//    Double_t xmax[dim]    = {60e-3, TMath::PiOver2(), 8.0};
    //======== LER Ta ========//
//    Int_t bins[dim]       = {2*60/1, 1600000, 80000};
//    Double_t xmin[dim]    = {0.0, 0.0, 0.0};
//    Double_t xmax[dim]    = {60e-3, TMath::PiOver2(), 8.0};
    //======== LER Cu ========//
//    Int_t bins[dim]       = {2*220/4, 1600000, 80000};
//    Double_t xmin[dim]    = {0.0, 0.0, 0.0};
//    Double_t xmax[dim]    = {220e-3, TMath::PiOver2(), 8.0};
    //======== LER C ========//
//    Int_t bins[dim]       = {2*140/4, 1600000, 80000};
//    Double_t xmin[dim]    = {0.0, 0.0, 0.0};
//    Double_t xmax[dim]    = {140e-3, TMath::PiOver2(), 8.0};
    //======== HER W ========//
//    Int_t bins[dim]       = {2*60/1, 1600000, 80000};
//    Double_t xmin[dim]    = {0.0, 0.0, 0.0};
//    Double_t xmax[dim]    = {60e-3, TMath::PiOver2(), 8.0};
    //======== HER Ta ========//
//    Int_t bins[dim]       = {2*60/1, 1600000, 80000};
//    Double_t xmin[dim]    = {0.0, 0.0, 0.0};
//    Double_t xmax[dim]    = {60e-3, TMath::PiOver2(), 8.0};
    //======== HER Cu ========//
//    Int_t bins[dim]       = {2*220/4, 1600000, 80000};
//    Double_t xmin[dim]    = {0.0, 0.0, 0.0};
//    Double_t xmax[dim]    = {220e-3, TMath::PiOver2(), 8.0};
    //======== HER C ========//
    Int_t bins[dim]       = {2*140/4, 1600000, 80000};
    Double_t xmin[dim]    = {0.0, 0.0, 0.0};
    Double_t xmax[dim]    = {140e-3, TMath::PiOver2(), 8.0};
    //
    THnSparseF* h12 = new THnSparseF("h12","#DeltaMom vs #theta_{Scat.} vs target length",dim,bins,xmin,xmax);
    //______________________________________________________________________________________________________//

    TH1D* h0 = new TH1D("h0","D0 vs target length",bins[0],xmin[0],xmax[0]);
    TH1D* h1 = new TH1D("h1","D1 vs target length",bins[0],xmin[0],xmax[0]);

    TGraphErrors* grProb = new TGraphErrors(); grProb->SetName("grProb");

    TVector3 particleMom0;
    TVector3 particleMom1;

    cout<<endl;
    for (Long64_t jentry = 0; jentry < nEntries; jentry++)
    {
        fChain->GetEntry(jentry);

        if(jentry%1000 == 0)
        {
            printf("\r--> Progress: %.3f [%%] | targetL = %.3f [mm] ",100.0*jentry/nEntries,targetL*1000);
            fflush(stdout);
        }

        particleMom0.SetXYZ(MomX0,MomY0,MomZ0);
        particleMom1.SetXYZ(MomX1,MomY1,MomZ1);

        if(Det0 == 1)
        {
            h0->Fill(targetL);

            if(Det1 == 1)
            {
                h1->Fill(targetL);
                xx[0] = targetL; xx[1] = particleMom1.Theta()-particleMom0.Theta(); xx[2] = particleMom0.Mag()-particleMom1.Mag();
                h12->Fill(xx);
            }
        }
    }
    cout<<endl;

    GetProbability(h0,h1,grProb);

    h0->Write();
    h1->Write();
    grProb->Write();
    h12->Write();

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
