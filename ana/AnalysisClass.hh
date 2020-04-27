//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon Apr 27 16:58:54 2020 by ROOT version 6.19/01
// from TTree tree/A Tree with Data of the primary particle
// found on file: output_all.root
//////////////////////////////////////////////////////////

#ifndef AnalysisClass_hh
#define AnalysisClass_hh

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

class AnalysisClass {

public:
   AnalysisClass();
   void InitTree(TString);

   virtual ~AnalysisClass();
   virtual Int_t    Cut(Long64_t);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);

   void SetInputFileName(TString name){_input_filename = name;}
   void SetOutputFileName(TString name){_output_filename = name;}

   TString GetInputFileName(){return _input_filename;}
   TString GetOutputFileName(){return _output_filename;}

private:
    TString _input_filename;
    TString _output_filename;

    TTree          *fChain;   //!pointer to the analyzed TTree or TChain
    Int_t           fCurrent; //!current Tree number in a TChain

 // Fixed size dimensions of array or collections stored in the TTree if any.

    // Declaration of leaf types
    Int_t           EventID;
    Int_t           PDG;
    Double_t        Q;
    Double_t        Mass;
    Int_t           Det0;
    Double_t        X0;
    Double_t        Y0;
    Double_t        Z0;
    Double_t        MomX0;
    Double_t        MomY0;
    Double_t        MomZ0;
    Double_t        Ekin0;
    Int_t           Det1;
    Double_t        X1;
    Double_t        Y1;
    Double_t        Z1;
    Double_t        MomX1;
    Double_t        MomY1;
    Double_t        MomZ1;
    Double_t        Ekin1;
    Double_t        targetL;
    Double_t        targetW;
    Double_t        targetH;

    // List of branches
    TBranch        *b_EventID;   //!
    TBranch        *b_PDG;   //!
    TBranch        *b_Q;   //!
    TBranch        *b_Mass;   //!
    TBranch        *b_Det0;   //!
    TBranch        *b_X0;   //!
    TBranch        *b_Y0;   //!
    TBranch        *b_Z0;   //!
    TBranch        *b_MomX0;   //!
    TBranch        *b_MomY0;   //!
    TBranch        *b_MomZ0;   //!
    TBranch        *b_Ekin0;   //!
    TBranch        *b_Det1;   //!
    TBranch        *b_X1;   //!
    TBranch        *b_Y1;   //!
    TBranch        *b_Z1;   //!
    TBranch        *b_MomX1;   //!
    TBranch        *b_MomY1;   //!
    TBranch        *b_MomZ1;   //!
    TBranch        *b_Ekin1;   //!
    TBranch        *b_targetL;   //!
    TBranch        *b_targetW;   //!
    TBranch        *b_targetH;   //!


};

#endif
