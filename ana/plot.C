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
TH2D* getXslice(TH3D* histIn, Int_t ix);

int plot()
{
    time_t start_time, stop_time;
    start_time = time(NULL);

    TString filename = "output_all.root_ana.root";
    cout<<"--> Input file: "<<filename<<endl;

    TFile *file = TFile::Open(filename.Data());

    TGraphErrors* grProb = (TGraphErrors*)file->Get("grProb");
    TH2D* h6 = (TH2D*)file->Get("h6");
    TH2D* h10 = (TH2D*)file->Get("h10");
    TH2D* h11 = (TH2D*)file->Get("h11");
    TH3D* h12 = (TH3D*)file->Get("h12");

    TH3D* hh12 = new TH3D("hh12",h12->GetTitle(),
                             h12->GetNbinsX(),h12->GetXaxis()->GetXmin(),h12->GetXaxis()->GetXmax(),
                             h12->GetNbinsY(),h12->GetYaxis()->GetXmin(),h12->GetYaxis()->GetXmax(),
                             h12->GetNbinsZ(),h12->GetZaxis()->GetXmin(),h12->GetZaxis()->GetXmax());
    //====================================//
    // Prepare a sample for SAD++
    TString outputFileName = "G4SAMPLE_LENGTH_ANGLE_ENERGY_C_PROB.dat";
    FILE * pFile = fopen(outputFileName.Data(),"w");
    Float_t _l, _t, _e, _c, _p;
    cout<<endl;
    fprintf (pFile, "{\n");
    Long64_t nDataLines = 0;
    cout<<"Read a 3D histogram ..."<<endl;
    for(Int_t ix = 1; ix <= h12->GetNbinsX(); ix++)
    {
        TH2D* h12proj = getXslice(h12,ix);
        if(h12proj->GetMaximum() <= 0)
        {
            delete h12proj;
            continue;
        }

        h12proj->Scale(1.0/h12proj->GetMaximum());

        _l = h12->GetXaxis()->GetBinCenter(ix);
        _p = grProb->Eval(_l);
        for(Int_t iy = 1; iy <= h12->GetNbinsY(); iy++)
        {
            _t = h12->GetYaxis()->GetBinCenter(iy);
            for(Int_t iz = 1; iz <= h12->GetNbinsZ(); iz++)
            {
                _e = h12->GetZaxis()->GetBinCenter(iz);
                _c = h12proj->GetBinContent(iy,iz);

                hh12->SetBinContent(ix,iy,iz,_c);

                if(_p < 0){_p = 0;}
                if(_p > 1){_p = 1;}
                if(_c < 0){_c = 0;}

                fprintf (pFile, "   { %.6f , %.6f , %.6f , %.6f , %.6f } ,\n",_l,_t,_e,_c,_p);
                nDataLines++;
            }
        }
        delete h12proj;

        printf("\r--> Progress: %d/%d (l = %.1f [mm])",ix,h12->GetNbinsX(),_l*1e3);
        fflush(stdout);
    }
    fprintf (pFile, "Null[]\n};\n");
    cout<<endl;
    fclose (pFile);
    cout<<"--> Output file: "<<outputFileName<<endl;
    cout<<"--> nDataLines = "<<nDataLines<<endl;
    //====================================//

    TCanvas* c3 = new TCanvas("c3","c3",450*3,450*1.5);
    c3->Divide(2,1);
    c3->cd(1);
    gPad->SetGrid();
    h12->GetXaxis()->SetTitle("Target Length [m]");
    h12->GetYaxis()->SetTitle("#theta_{Scat.} [rad]");
    h12->GetZaxis()->SetTitle("#DeltaE_{kin.} [GeV]");
    h12->GetXaxis()->CenterTitle();
    h12->GetYaxis()->CenterTitle();
    h12->GetZaxis()->CenterTitle();
    h12->GetXaxis()->SetTitleOffset(1.4);
    h12->GetYaxis()->SetTitleOffset(1.4);
    h12->GetZaxis()->SetTitleOffset(1.4);
    h12->Draw("BOX2 Z");
    c3->cd(2);
    gPad->SetGrid();
    hh12->GetXaxis()->SetTitle("Target Length [m]");
    hh12->GetYaxis()->SetTitle("#theta_{Scat.} [rad]");
    hh12->GetZaxis()->SetTitle("#DeltaE_{kin.} [GeV]");
    hh12->GetXaxis()->CenterTitle();
    hh12->GetYaxis()->CenterTitle();
    hh12->GetZaxis()->CenterTitle();
    hh12->GetXaxis()->SetTitleOffset(1.4);
    hh12->GetYaxis()->SetTitleOffset(1.4);
    hh12->GetZaxis()->SetTitleOffset(1.4);
    hh12->Draw("BOX2 Z");
    //=======================================================================================//

    gStyle->SetOptStat(0);

    TCanvas* c1 = new TCanvas("c1","c1",450*1,450*1);
    c1->cd();
    gPad->SetGrid();
    grProb->GetXaxis()->SetTitle("Target Length [m]");
    grProb->GetYaxis()->SetTitle("Survival Probability");
    grProb->GetXaxis()->CenterTitle();
    grProb->GetYaxis()->CenterTitle();
    grProb->GetYaxis()->SetTitleOffset(1.2);
    grProb->Draw("APC");

    //=======================================================================================//

    TCanvas* c2 = new TCanvas("c2","c2",450*3,450*2);
    c2->Divide(3,2);
    c2->cd(1);
    gPad->SetGrid();
//    h6->Rebin2D(10);
    h6->Draw("colz");
    h6->GetXaxis()->SetTitle("Target Length [m]");
    h6->GetYaxis()->SetTitle("#theta_{Scat.} [rad]");
    h6->GetXaxis()->CenterTitle();
    h6->GetYaxis()->CenterTitle();
    h6->GetYaxis()->SetTitleOffset(1.2);

    c2->cd(2);
    gPad->SetGrid();
//    h10->Rebin2D(10);
    h10->Draw("colz");
    h10->GetYaxis()->SetRangeUser(-0.1,4.1);
    h10->GetXaxis()->SetTitle("Target Length [m]");
    h10->GetYaxis()->SetTitle("#DeltaE_{kin.} [GeV]");
    h10->GetXaxis()->CenterTitle();
    h10->GetYaxis()->CenterTitle();
    h10->GetYaxis()->SetTitleOffset(1.2);

    c2->cd(3);
    gPad->SetGrid();
//    h11->Rebin2D(10);
    h11->Draw("colz");
    h11->GetYaxis()->SetRangeUser(-0.1,4.1);
    h11->GetXaxis()->SetTitle("#theta_{Scat.} [rad]");
    h11->GetYaxis()->SetTitle("#DeltaE_{kin.} [GeV]");
    h11->GetXaxis()->CenterTitle();
    h11->GetYaxis()->CenterTitle();
    h11->GetYaxis()->SetTitleOffset(1.2);

    c2->cd(4);
    gPad->SetGrid();
    Bool_t firstPlot = true;
    Int_t nHist = 0;

    Double_t minThetaFit =  0.0e-3;
    Double_t maxThetaFit = 40.0e-3;

    TLegend* leg2 = new TLegend(0.5,0.2,0.8,0.5);

    for(Int_t binxi = 1; binxi <= h6->GetNbinsX(); binxi++)
    {
        TString hName;
        hName.Form("projY_%d",binxi);
        TH1D* h6_ProjY = h6->ProjectionY(hName.Data(),binxi,binxi);
        h6_ProjY->GetXaxis()->SetRange(h6_ProjY->GetXaxis()->FindBin(minThetaFit),h6_ProjY->GetXaxis()->FindBin(maxThetaFit));
        h6_ProjY->SetMinimum(1);
        if(h6_ProjY->Integral() <= 0) continue;
        if(nHist >= 4)break;

        h6_ProjY->SetMarkerStyle(20+nHist);
        h6_ProjY->SetMarkerSize(0.5);
        h6_ProjY->SetMarkerColor(1+nHist);

        if(firstPlot)
        {
            h6_ProjY->Draw("HIST & E1");
            firstPlot = false;
        }
        else
        {
            h6_ProjY->Draw("SAME & HIST & E1");
        }

        TString legStr;
        legStr.Form("Sim: \t L = %.1f [mm]",h6->GetXaxis()->GetBinCenter(binxi)*1e3);
        leg2->AddEntry(h6_ProjY,legStr.Data(),"lpe");

        nHist++;
    }
    leg2->Draw();
    gPad->SetLogy();
    cout<<"--> nHist (h6 ) \t = "<<nHist<<endl;

    c2->cd(5);
    gPad->SetGrid();
    firstPlot = true;
    nHist = 0;

    Double_t minEkin = 0.0;
    Double_t maxEkin = 10;

    TLegend* leg3 = new TLegend(0.3,0.55,0.6,0.85);

    for(Int_t binxi = 1; binxi <= h10->GetNbinsX(); binxi++)
    {
        TString hName;
        hName.Form("projYY_%d",binxi);
        TH1D* h10_ProjY = h10->ProjectionY(hName.Data(),binxi,binxi);
        h10_ProjY->GetXaxis()->SetRange(h10_ProjY->GetXaxis()->FindBin(minEkin),h10_ProjY->GetXaxis()->FindBin(maxEkin));
        h10_ProjY->SetMinimum(1);
        if(h10_ProjY->Integral() <= 0) continue;
        if(nHist >= 4)break;

        h10_ProjY->SetMarkerStyle(20+nHist);
        h10_ProjY->SetMarkerSize(0.5);
        h10_ProjY->SetMarkerColor(1+nHist);

        if(firstPlot)
        {
            h10_ProjY->Draw("HIST & E1");
            firstPlot = false;
        }
        else
        {
            h10_ProjY->Draw("SAME & HIST & E1");
        }

        TString legStr;
        legStr.Form("Sim: \t L = %.1f [mm]",h10->GetXaxis()->GetBinCenter(binxi)*1e3);
        leg3->AddEntry(h10_ProjY,legStr.Data(),"lpe");

        nHist++;
    }
    leg3->Draw();
    gPad->SetLogy();
    cout<<"--> nHist (h10) \t = "<<nHist<<endl;

    c2->cd(6);
    gPad->SetGrid();
    firstPlot = true;
    nHist = 0;

    TLegend* leg4 = new TLegend(0.3,0.55,0.6,0.85);

    for(Int_t binxi = 1; binxi <= h11->GetNbinsX(); binxi++)
    {
        TString hName;
        hName.Form("projYYY_%d",binxi);
        TH1D* h11_ProjY = h11->ProjectionY(hName.Data(),binxi,binxi);
        if(h11_ProjY->Integral() <= 0) continue;
        if(nHist >= 4)break;

        h11_ProjY->SetMarkerStyle(20+nHist);
        h11_ProjY->SetMarkerSize(0.5);
        h11_ProjY->SetMarkerColor(1+nHist);

        if(firstPlot)
        {
            h11_ProjY->Draw("HIST & E1");
            firstPlot = false;
        }
        else
        {
            h11_ProjY->Draw("SAME & HIST & E1");
        }

        TString legStr;
        legStr.Form("Sim: \t #theta_{Scat.} = %.1f [mrad]",h11->GetXaxis()->GetBinCenter(binxi)*1e3);
        leg4->AddEntry(h11_ProjY,legStr.Data(),"lpe");

        nHist++;
    }
    leg4->Draw();
    gPad->SetLogy();
    cout<<"--> nHist (h11) \t = "<<nHist<<endl;

    //=======================================================================================//

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

TH2D* getXslice(TH3D* histIn, Int_t ix)
{
    TH2D* histOut = new TH2D("histOutXslice",histIn->GetTitle(),
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

