#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TH1.h>
#include <TH2.h>
#include <TH3.h>
#include <TGraphErrors.h>
#include <TCanvas.h>
#include <TF1.h>
#include <TStyle.h>
#include <TLegend.h>
#include <THnSparse.h>

#include <ctime>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <fstream>
#include <string>
#include <time.h>

using namespace std;

void HistoToGraph(TH1D* hist, TGraphErrors* gr);
void NormGraph(TGraphErrors* gr);
TH2F *getXslice(TH3F *histIn, Int_t ix);
void createBinArrayByGroups(Int_t nBins, Int_t nGroups, Double_t *xMin, Double_t *xMax, Double_t *binArray);
void createBinArrayByGroups(Int_t nBins, Int_t nGroups, Float_t* xMin, Float_t* xMax, Float_t *binArray);

int main()
{
    time_t start_time, stop_time;
    start_time = time(NULL);

    //----------------------------------------------- SETTINGS ---------------------------------------------//
    //______________________________________________________________________________________________________//
    const Float_t minProb = 0.1/100.0; // 0.1% -- minimum counts to be written in the output scattering matrix file
    const Int_t nRebinX = 2;
    const Int_t nRebinY = 40000;
    const Int_t nRebinZ = 1000;
    Bool_t printSliceHistoTwoD = kFALSE;
    // check with AnalysisClass.cc
    const Int_t dim = 3;
    Int_t *iBins = new Int_t[dim];
    const Int_t nBinsPerGroup = 1000;
    const Int_t nGroups = 3;
    //======== LER W ========//
//    Int_t bins[dim]       = {2*60/1, 1600000, 80000};
//    Double_t xmin[dim]    = {0.0, 0.0, 0.0};
//    Double_t xmax[dim]    = {60e-3, TMath::PiOver2(), 8.0};
//    Double_t maxMom = 4.1; // [GeV/c]
//    TString filename = "output_ler_w.root_ana.root";
    //======== LER Ta ========//
//    Int_t bins[dim]       = {2*60/1, 1600000, 80000};
//    Double_t xmin[dim]    = {0.0, 0.0, 0.0};
//    Double_t xmax[dim]    = {60e-3, TMath::PiOver2(), 8.0};
//    Double_t maxMom = 4.1; // [GeV/c]
//    TString filename = "output_ler_ta.root_ana.root";
    //======== LER Cu ========//
//    Int_t bins[dim]       = {2*220/4, 1600000, 80000};
//    Double_t xmin[dim]    = {0.0, 0.0, 0.0};
//    Double_t xmax[dim]    = {220e-3, TMath::PiOver2(), 8.0};
//    Double_t maxMom = 4.1; // [GeV/c]
//    TString filename = "output_ler_cu.root_ana.root";
    //======== LER C ========//
//    Int_t bins[dim]       = {2*140/4, 1600000, 80000};
//    Double_t xmin[dim]    = {0.0, 0.0, 0.0};
//    Double_t xmax[dim]    = {140e-3, TMath::PiOver2(), 8.0};
//    Double_t maxMom = 4.1; // [GeV/c]
//    TString filename = "output_ler_c.root_ana.root";
    //======== HER W ========//
//    Int_t bins[dim]       = {2*60/1, 1600000, 80000};
//    Double_t xmin[dim]    = {0.0, 0.0, 0.0};
//    Double_t xmax[dim]    = {60e-3, TMath::PiOver2(), 8.0};
//    Double_t maxMom = 7.1; // [GeV/c]
//    TString filename = "output_her_w.root_ana.root";
    //======== HER Ta ========//
//    Int_t bins[dim]       = {2*60/1, 1600000, 80000};
//    Double_t xmin[dim]    = {0.0, 0.0, 0.0};
//    Double_t xmax[dim]    = {60e-3, TMath::PiOver2(), 8.0};
//    Double_t maxMom = 7.1; // [GeV/c]
//    TString filename = "output_her_ta.root_ana.root";
    //======== HER Cu ========//
//    Int_t bins[dim]       = {2*220/4, 1600000, 80000};
//    Double_t xmin[dim]    = {0.0, 0.0, 0.0};
//    Double_t xmax[dim]    = {220e-3, TMath::PiOver2(), 8.0};
//    Double_t maxMom = 7.1; // [GeV/c]
//    TString filename = "output_her_cu.root_ana.root";
    //======== HER C ========//
    Int_t bins[dim]       = {2*140/4, 1600000, 80000};
    Double_t xmin[dim]    = {0.0, 0.0, 0.0};
    Double_t xmax[dim]    = {140e-3, TMath::PiOver2(), 8.0};
    Double_t maxMom = 7.1; // [GeV/c]
    TString filename = "output_her_c.root_ana.root";
    //______________________________________________________________________________________________________//

    cout<<"--> Input file: "<<filename<<endl;

    cout<<"--> Open input file"<<endl;
    TFile *file = TFile::Open(filename.Data());
    cout<<"--> Read probability"<<endl;
    TGraphErrors* grProb = (TGraphErrors*)file->Get("grProb");
    cout<<"--> Extracting scattering data ..."<<endl;
    THnSparseF* h12 = (THnSparseF*)file->Get("h12");
    cout<<"--> Done"<<endl;
    cout<<"--> minProb = "<<minProb<<endl;

    TH2F* _h12proj[bins[0]];
    Float_t lengthArray[bins[0]];

    Double_t tMin[] = {xmin[1], 1e-3, 1};
    Double_t tMax[] = {1e-3, 1, xmax[1]};
    Double_t tArray[nBinsPerGroup*nGroups+1];
    createBinArrayByGroups(nBinsPerGroup,nGroups,tMin,tMax,tArray);

    Double_t mMin[] = {xmin[2], 1e-2, 1};
    Double_t mMax[] = {1e-2, 1, xmax[2]};
    Double_t mArray[nBinsPerGroup*nGroups+1];
    createBinArrayByGroups(nBinsPerGroup,nGroups,mMin,mMax,mArray);

    cout<<"--> Creating histograms ..."<<endl;
    for(Int_t i = 0; i < bins[0]; i++)
    {
        TString hName = "_h12proj_"; hName += i;
        _h12proj[i] = new TH2F(hName.Data(),"#DeltaMom vs #theta_{Scat.}",nBinsPerGroup*nGroups,tArray,nBinsPerGroup*nGroups,mArray);
        lengthArray[i] = -999;
    }

    TH3F* _h12 = new TH3F("_h12","#DeltaMom vs #theta_{Scat.} vs target length (raw)",
                          bins[0]/nRebinX,xmin[0],xmax[0],bins[1]/nRebinY,xmin[1],xmax[1],bins[2]/nRebinZ,xmin[2],xmax[2]);
    TH3F* _hh12 = new TH3F("_hh12","#DeltaMom vs #theta_{Scat.} vs target length (norm)",
                           bins[0]/nRebinX,xmin[0],xmax[0],bins[1]/nRebinY,xmin[1],xmax[1],bins[2]/nRebinZ,xmin[2],xmax[2]);
    TH2F* h12_10 = new TH2F("h12_10","#theta_{Scat.} vs target length",
                            bins[0]/nRebinX,xmin[0],xmax[0],bins[1]/nRebinY,xmin[1],xmax[1]);
    TH2F* h12_20 = new TH2F("h12_20","#DeltaMom vs target length",
                            bins[0]/nRebinX,xmin[0],xmax[0],bins[2]/nRebinZ,xmin[2],xmax[2]);
    TH2F* h12_21 = new TH2F("h12_21","#DeltaMom vs #theta_{Scat.}",
                            bins[1]/nRebinY,xmin[1],xmax[1],bins[2]/nRebinZ,xmin[2],xmax[2]);

    cout<<"--> Done"<<endl;

    Float_t _l, _p, _m, _t, _c;
    Int_t binx, biny, binz;

    cout<<"--> Fill 2D and 3D raw projections with scattering data ..."<<endl;
    Long64_t nBins = h12->GetNbins();
    for (Long64_t i = 0; i < nBins; i++)
    {
        _c = h12->GetBinContent(i,iBins);               // counts
        _l = h12->GetAxis(0)->GetBinCenter(iBins[0]);   // length
        _t = h12->GetAxis(1)->GetBinCenter(iBins[1]);   // theta
        _m = h12->GetAxis(2)->GetBinCenter(iBins[2]);   // dMom

        binx = _h12->GetXaxis()->FindBin(_l);
        biny = _h12->GetYaxis()->FindBin(_t);
        binz = _h12->GetZaxis()->FindBin(_m);
        // fill raw 3D
        _h12->SetBinContent(binx,biny,binz,_h12->GetBinContent(binx,biny,binz)+_c);
        // fill raw 2D
        h12_10->SetBinContent(binx,biny,h12_10->GetBinContent(binx,biny)+_c);
        h12_20->SetBinContent(binx,binz,h12_20->GetBinContent(binx,binz)+_c);
        h12_21->SetBinContent(biny,binz,h12_21->GetBinContent(biny,binz)+_c);

        if(iBins[0] >= 1 && iBins[0] <= bins[0])
        {
            _h12proj[iBins[0]-1]->Fill(_t,_m,_c); // length projection
            lengthArray[iBins[0]-1] = _l;
        }

        if(i%1000 == 0)
        {
            printf("\r--> Progress: %3.3f%% ",100.0*i/nBins);
            fflush(stdout);
        }
    }
    cout<<endl;
    cout<<"--> Done"<<endl;

    if(printSliceHistoTwoD)
    {
        cout<<"--> Print 2D raw histos by slices ..."<<endl;
        for(Int_t i = 0; i < bins[0]; i++)
        {
            TString cNameTmp = "cTmpRaw_L"; cNameTmp += (Int_t)(lengthArray[i]*10000);
            TCanvas* cTmp = new TCanvas(cNameTmp.Data(),cNameTmp.Data(),600,600);
            cTmp->cd();
            _h12proj[i]->Draw("colz");
            cNameTmp = "./png/" + cNameTmp + ".png";
            cTmp->SaveAs(cNameTmp.Data());
            delete cTmp;
        }
        cout<<"--> Done"<<endl;
    }

    cout<<"--> Delete raw scattering data"<<endl;
    delete h12;

    // Prepare a sample for SAD++
    TString outputFileName = filename + "_g4.dat";
    FILE * pFile = fopen(outputFileName.Data(),"w");
    fprintf(pFile, "(* Length dTheta dP Counts Prob *)\n");
    fprintf(pFile, "{\n");
    Long64_t nDataLines = 0;

    // loop for length
    cout<<"--> Write scattering matrix to a file ..."<<endl;
    Bool_t IsTheFirstLineForAgivenLength;
    for(Int_t ix = 0; ix < bins[0]; ix++)
    {
        if(ix%1 == 0)
        {
            printf("\r--> Progress: %3.3f%% ",100.0*ix/bins[0]);
            fflush(stdout);
        }

        // check if not empty
        if(_h12proj[ix]->GetMaximum() > 0)
        {
            // normalize each length slice
            _h12proj[ix]->Scale(1.0/_h12proj[ix]->GetMaximum());

            _l = lengthArray[ix];
            _p = grProb->Eval(_l);

            fprintf(pFile, "   \"L%d\"->{\n",ix+1);
            IsTheFirstLineForAgivenLength = kTRUE;

            // loop for theta
            Int_t nThetaBins = _h12proj[ix]->GetNbinsX();
            Int_t nMomBins = _h12proj[ix]->GetNbinsY();
            for(Int_t iy = 1; iy <= nThetaBins; iy++)
            {
                _t = _h12proj[ix]->GetXaxis()->GetBinCenter(iy);

                // loop for dMom
                for(Int_t iz = 1; iz <= nMomBins; iz++)
                {
                    _m = _h12proj[ix]->GetYaxis()->GetBinCenter(iz);
                    _c = _h12proj[ix]->GetBinContent(iy,iz);

                    if(_p < 0){_p = 0;}
                    if(_p > 1){_p = 1;}
                    if(_c <= minProb){continue;}

                    if(IsTheFirstLineForAgivenLength)
                    {
                        fprintf(pFile,"            { %.6f , %.6f , %.6f , %.6f , %.6f }",_l,_t,_m,_c,_p);
                        IsTheFirstLineForAgivenLength = kFALSE;
                    }
                    else
                    {
                        fprintf(pFile,",\n            { %.6f , %.6f , %.6f , %.6f , %.6f }",_l,_t,_m,_c,_p);
                    }

                    nDataLines++;
                }
            }
            fprintf(pFile, "\n   },\n");
        }
    }
    cout<<endl;
    cout<<"--> Done"<<endl;

    if(printSliceHistoTwoD)
    {
        cout<<"--> Print 2D norm. histos by slices ..."<<endl;
        for(Int_t i = 0; i < bins[0]; i++)
        {
            TString cNameTmp = "cTmpNorm_L"; cNameTmp += (Int_t)(lengthArray[i]*10000);
            TCanvas* cTmp = new TCanvas(cNameTmp.Data(),cNameTmp.Data(),600,600);
            cTmp->cd();
            _h12proj[i]->Draw("colz");
            cNameTmp = "./png/" + cNameTmp + ".png";
            cTmp->SaveAs(cNameTmp.Data());
            delete cTmp;
        }
        cout<<"--> Done"<<endl;
    }

    fprintf(pFile, "Null[]\n};\n");
    fclose (pFile);
    cout<<"--> Output file: "<<outputFileName<<endl;
    cout<<"--> nDataLines = "<<nDataLines<<endl;

    cout<<"--> Fill 3D norm. projection with scattering data ..."<<endl;
    for(Int_t ix = 1; ix <= _h12->GetNbinsX(); ix++)
    {
        TH2F* _h12projj = getXslice(_h12,ix);
        if(_h12projj->GetMaximum() <= 0)
        {
            delete _h12projj;
            continue;
        }

        _h12projj->Scale(1.0/_h12projj->GetMaximum());

        for(Int_t iy = 1; iy <= _h12->GetNbinsY(); iy++)
        {
            for(Int_t iz = 1; iz <= _h12->GetNbinsZ(); iz++)
            {
                _c = _h12projj->GetBinContent(iy,iz);

                _hh12->SetBinContent(ix,iy,iz,_c);
            }
        }

        delete _h12projj;

        if(ix%1 == 0)
        {
            printf("\r--> Progress: %3.3f%% ",100.0*ix/_h12->GetNbinsX());
            fflush(stdout);
        }
    }
    cout<<endl;
    cout<<"--> Done"<<endl;

    //=======================================================================================//

    cout<<"--> Plot 3D raw and norm. scattering data"<<endl;
    TCanvas* c3 = new TCanvas("c3","c3",450*3,450*1.5);
    c3->Divide(2,1);
    c3->cd(1);
    gPad->SetGrid();
    _h12->GetXaxis()->SetTitle("Target Length [m]");
    _h12->GetYaxis()->SetTitle("#theta_{Scat.} [rad]");
    _h12->GetZaxis()->SetTitle("#DeltaP [GeV/c]");
    _h12->GetXaxis()->CenterTitle();
    _h12->GetYaxis()->CenterTitle();
    _h12->GetZaxis()->CenterTitle();
    _h12->GetXaxis()->SetTitleOffset(1.4);
    _h12->GetYaxis()->SetTitleOffset(1.4);
    _h12->GetZaxis()->SetTitleOffset(1.4);
    _h12->Draw("BOX2 Z");
    c3->cd(2);
    gPad->SetGrid();
    _hh12->GetXaxis()->SetTitle("Target Length [m]");
    _hh12->GetYaxis()->SetTitle("#theta_{Scat.} [rad]");
    _hh12->GetZaxis()->SetTitle("#DeltaP [GeV/c]");
    _hh12->GetXaxis()->CenterTitle();
    _hh12->GetYaxis()->CenterTitle();
    _hh12->GetZaxis()->CenterTitle();
    _hh12->GetXaxis()->SetTitleOffset(1.4);
    _hh12->GetYaxis()->SetTitleOffset(1.4);
    _hh12->GetZaxis()->SetTitleOffset(1.4);
    _hh12->Draw("BOX2 Z");
    c3->SaveAs("./png/c3.png");

    //=======================================================================================//

    gStyle->SetOptStat(0);

    cout<<"--> Plot TH3D scattering data"<<endl;
    TCanvas* c1 = new TCanvas("c1","c1",450*1,450*1);
    c1->cd();
    gPad->SetGrid();
    grProb->GetXaxis()->SetTitle("Target Length [m]");
    grProb->GetYaxis()->SetTitle("Survival Probability");
    grProb->GetXaxis()->CenterTitle();
    grProb->GetYaxis()->CenterTitle();
    grProb->GetYaxis()->SetTitleOffset(1.2);
    grProb->SetMinimum(0.0);
    grProb->SetMaximum(1.1);
    grProb->Draw("APC");
    c1->SaveAs("./png/c1.png");

    //=======================================================================================//

    cout<<"--> Plot projections"<<endl;
    TCanvas* c2 = new TCanvas("c2","c2",450*3,450*2);
    c2->Divide(3,2);
    c2->cd(1);
    gPad->SetGrid();
    h12_10->Draw("colz");
    h12_10->GetXaxis()->SetTitle("Target Length [m]");
    h12_10->GetYaxis()->SetTitle("#theta_{Scat.} [rad]");
    h12_10->GetXaxis()->CenterTitle();
    h12_10->GetYaxis()->CenterTitle();
    h12_10->GetYaxis()->SetTitleOffset(1.2);
    gPad->SetLogz();

    c2->cd(2);
    gPad->SetGrid();
    h12_20->Draw("colz");
    h12_20->GetYaxis()->SetRangeUser(-0.1,maxMom);
    h12_20->GetXaxis()->SetTitle("Target Length [m]");
    h12_20->GetYaxis()->SetTitle("#DeltaP [GeV/c]");
    h12_20->GetXaxis()->CenterTitle();
    h12_20->GetYaxis()->CenterTitle();
    h12_20->GetYaxis()->SetTitleOffset(1.2);
    gPad->SetLogz();

    c2->cd(3);
    gPad->SetGrid();
    h12_21->Draw("colz");
    h12_21->GetYaxis()->SetRangeUser(-0.1,maxMom);
    h12_21->GetXaxis()->SetTitle("#theta_{Scat.} [rad]");
    h12_21->GetYaxis()->SetTitle("#DeltaP [GeV/c]");
    h12_21->GetXaxis()->CenterTitle();
    h12_21->GetYaxis()->CenterTitle();
    h12_21->GetYaxis()->SetTitleOffset(1.2);
    gPad->SetLogz();

    c2->cd(4);
    gPad->SetGrid();
    Bool_t firstPlot = true;
    Int_t nHist = 0;

    Double_t minThetaFit =  0.0e-3;
    Double_t maxThetaFit = 400.0e-3;

    TLegend* leg2 = new TLegend(0.5,0.2,0.8,0.5);

    for(Int_t binxi = 1; binxi <= h12_10->GetNbinsX(); binxi+=4)
    {
        Float_t _length = h12_10->GetXaxis()->GetBinCenter(binxi)*1e3; // length [mm]

        TString hName;
        hName.Form("projY_%d",binxi);
        TH1D* h12_10_ProjY = h12_10->ProjectionY(hName.Data(),binxi,binxi);
        h12_10_ProjY->GetXaxis()->SetRange(h12_10_ProjY->GetXaxis()->FindBin(minThetaFit),h12_10_ProjY->GetXaxis()->FindBin(maxThetaFit));
        h12_10_ProjY->SetMinimum(1);
        if(h12_10_ProjY->Integral() <= 0) continue;
        if(nHist >= 4)break;

        h12_10_ProjY->SetMarkerStyle(20+nHist);
        h12_10_ProjY->SetMarkerSize(0.5);
        h12_10_ProjY->SetMarkerColor(1+nHist);
        h12_10_ProjY->SetLineColor(1+nHist);

        if(firstPlot)
        {
            h12_10_ProjY->Draw("HIST & E1");
            firstPlot = false;
        }
        else
        {
            h12_10_ProjY->Draw("SAME & HIST & E1");
        }

        TString legStr;
        legStr.Form("Sim: \t L = %.1f [mm]",_length);
        leg2->AddEntry(h12_10_ProjY,legStr.Data(),"lpe");

        nHist++;
    }
    leg2->Draw();
    gPad->SetLogy();
    cout<<"--> nHist (h12_10) \t = "<<nHist<<endl;

    c2->cd(5);
    gPad->SetGrid();
    firstPlot = true;
    nHist = 0;

    TLegend* leg3 = new TLegend(0.3,0.55,0.6,0.85);

    for(Int_t binxi = 1; binxi <= h12_20->GetNbinsX(); binxi+=4)
    {
        TString hName;
        hName.Form("projYY_%d",binxi);
        TH1D* h12_20_ProjY = h12_20->ProjectionY(hName.Data(),binxi,binxi);
        h12_20_ProjY->SetMinimum(1);
        if(h12_20_ProjY->Integral() <= 0) continue;
        if(nHist >= 4)break;

        h12_20_ProjY->SetMarkerStyle(20+nHist);
        h12_20_ProjY->SetMarkerSize(0.5);
        h12_20_ProjY->SetMarkerColor(1+nHist);
        h12_20_ProjY->SetLineColor(1+nHist);

        if(firstPlot)
        {
            h12_20_ProjY->Draw("HIST & E1");
            firstPlot = false;
        }
        else
        {
            h12_20_ProjY->Draw("SAME & HIST & E1");
        }

        TString legStr;
        legStr.Form("Sim: \t L = %.1f [mm]",h12_20->GetXaxis()->GetBinCenter(binxi)*1e3);
        leg3->AddEntry(h12_20_ProjY,legStr.Data(),"lpe");

        nHist++;
    }
    leg3->Draw();
    gPad->SetLogy();
    cout<<"--> nHist (h12_20) \t = "<<nHist<<endl;

    c2->cd(6);
    gPad->SetGrid();
    firstPlot = true;
    nHist = 0;

    TLegend* leg4 = new TLegend(0.3,0.55,0.6,0.85);

    for(Int_t binxi = 1; binxi <= h12_21->GetNbinsX(); binxi+=1)
    {
        TString hName;
        hName.Form("projYYY_%d",binxi);
        TH1D* h12_21_ProjY = h12_21->ProjectionY(hName.Data(),binxi,binxi);
        h12_21_ProjY->SetMinimum(1);
        if(h12_21_ProjY->Integral() <= 0) continue;
        if(nHist >= 4)break;

        h12_21_ProjY->SetMarkerStyle(20+nHist);
        h12_21_ProjY->SetMarkerSize(0.5);
        h12_21_ProjY->SetMarkerColor(1+nHist);
        h12_21_ProjY->SetLineColor(1+nHist);

        if(firstPlot)
        {
            h12_21_ProjY->Draw("HIST & E1");
            firstPlot = false;
        }
        else
        {
            h12_21_ProjY->Draw("SAME & HIST & E1");
        }

        TString legStr;
        legStr.Form("Sim: \t #theta_{Scat.} = %.1f [mrad]",h12_21->GetXaxis()->GetBinCenter(binxi)*1e3);
        leg4->AddEntry(h12_21_ProjY,legStr.Data(),"lpe");

        nHist++;
    }
    leg4->Draw();
    gPad->SetLogy();
    cout<<"--> nHist (h12_21) \t = "<<nHist<<endl;
    c2->SaveAs("./png/c2.png");
/**/
    stop_time = time(NULL);
    cout<<"--> Running time is : "<<stop_time - start_time<<" [sec]"<<endl;

    return 0;
}

void HistoToGraph(TH1D* hist, TGraphErrors* gr)
{
    for(Int_t binxi = 1; binxi <= hist->GetNbinsX(); binxi++)
    {
        gr->SetPoint(gr->GetN(),hist->GetXaxis()->GetBinCenter(binxi),hist->GetBinContent(binxi));
        gr->SetPointError(gr->GetN()-1,hist->GetXaxis()->GetBinWidth(binxi)/TMath::Sqrt(12.0),hist->GetBinError(binxi));
    }
}

void NormGraph(TGraphErrors* gr)
{
    Double_t x, y, ex, ey;
    Double_t maxVal = -1.0, maxValerr;
    for(Int_t i = 0; i < gr->GetN(); i++)
    {
        gr->GetPoint(i,x,y);
        ey = gr->GetErrorY(i);

        if(y > maxVal)
        {
            maxVal = y;
            maxValerr = ey;
        }
    }
    for(Int_t i = 0; i < gr->GetN(); i++)
    {
        gr->GetPoint(i,x,y);
        ex = gr->GetErrorX(i);
        ey = gr->GetErrorY(i);

        gr->SetPoint(i,x,y/maxVal);
        gr->SetPointError(i,ex,TMath::Sqrt( TMath::Power(ey/maxVal,2) + TMath::Power(y*maxValerr/(maxVal*maxVal),2) ));
    }
}

TH2F* getXslice(TH3F* histIn, Int_t ix)
{
    TH2F* histOut = new TH2F("histOutXslice",histIn->GetTitle(),
                             histIn->GetNbinsY(),histIn->GetYaxis()->GetXmin(),histIn->GetYaxis()->GetXmax(),
                             histIn->GetNbinsZ(),histIn->GetZaxis()->GetXmin(),histIn->GetZaxis()->GetXmax());

    for(Int_t iy = 1; iy <= histIn->GetNbinsY(); iy++)
    {
        for(Int_t iz = 1; iz <= histIn->GetNbinsZ(); iz++)
        {
            histOut->SetBinContent(iy,iz,histIn->GetBinContent(ix,iy,iz));
        }
    }

    return histOut;
}

void createBinArrayByGroups(Int_t nBins, Int_t nGroups, Double_t* xMin, Double_t* xMax, Double_t *binArray)
{
    cout<<endl<<"|--> createBinArrayByGroups <--"<<endl;
    cout<<"| nBins = "<<nBins<<endl;
    cout<<"| nGroups = "<<nGroups<<endl;
    Double_t dxBinGroup[nGroups];
    for(Int_t i = 0; i < nGroups; i++)
    {
        dxBinGroup[i] = (xMax[i] - xMin[i])/nBins;
        cout<<"| xGroup["<<i<<"] = \t"<<xMin[i]<<"\t -- \t"<<xMax[i]<<"\t : dx = \t"<<dxBinGroup[i]<<endl;
    }

    Int_t iBinCom = 0;

    for(Int_t iGroup = 0; iGroup < nGroups; iGroup++)
    {
        for(Int_t iBin = 0; iBin < nBins; iBin++)
        {
            binArray[iBinCom] = xMin[iGroup] + iBin*dxBinGroup[iGroup];
            iBinCom++;
        }
    }
    binArray[iBinCom] = xMax[nGroups-1];
    cout<<endl;

//    for(Int_t i = 0; i <= iBinCom; i++){cout<<i<<" "<<binArray[i]<<endl;}

    return;
}

void createBinArrayByGroups(Int_t nBins, Int_t nGroups, Float_t* xMin, Float_t* xMax, Float_t *binArray)
{
    cout<<endl<<"|--> createBinArrayByGroups <--"<<endl;
    cout<<"| nBins = "<<nBins<<endl;
    cout<<"| nGroups = "<<nGroups<<endl;
    Float_t dxBinGroup[nGroups];
    for(Int_t i = 0; i < nGroups; i++)
    {
        dxBinGroup[i] = (xMax[i] - xMin[i])/nBins;
        cout<<"| xGroup["<<i<<"] = \t"<<xMin[i]<<"\t -- \t"<<xMax[i]<<"\t : dx = \t"<<dxBinGroup[i]<<endl;
    }

    Int_t iBinCom = 0;

    for(Int_t iGroup = 0; iGroup < nGroups; iGroup++)
    {
        for(Int_t iBin = 0; iBin < nBins; iBin++)
        {
            binArray[iBinCom] = xMin[iGroup] + iBin*dxBinGroup[iGroup];
            iBinCom++;
        }
    }
    binArray[iBinCom] = xMax[nGroups-1];
    cout<<endl;

//    for(Int_t i = 0; i <= iBinCom; i++){cout<<iBinCom<<" "<<binArray[i]<<endl;}

    return;
}
