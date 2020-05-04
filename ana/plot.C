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

int plot()
{
    time_t start_time, stop_time;
    start_time = time(NULL);

    TString filename = "output_all.root_ana.root";

    TFile *file = TFile::Open(filename.Data());

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

    static Double_t nominalMom = /*e+*/4.00000; // [GeV/c]

    for(Int_t i = 0; i < nP; i++)
    {
        Double_t deltaP = -0.02 + i*0.001;
        Pmin[i] = nominalMom*(1.0 + deltaP - 0.001/2.0);
        Pmax[i] = nominalMom*(1.0 + deltaP + 0.001/2.0);

        TString grName;

        grName.Form("grProb_%d",i);
        grProb[i] = (TGraphErrors*)file->Get(grName.Data());

        grName.Form("grTheta_%d",i);
        grTheta[i] = (TGraphErrors*)file->Get(grName.Data());

        grName.Form("hh0_%d",i);
        hh0[i] = (TH1D*)file->Get(grName.Data());

        grName.Form("hh1_%d",i);
        hh1[i] = (TH1D*)file->Get(grName.Data());

        grName.Form("hh6_%d",i);
        hh6[i] = (TH2D*)file->Get(grName.Data());

        grName.Form("hh10_%d",i);
        hh10[i] = (TH2D*)file->Get(grName.Data());

        grName.Form("hh11_%d",i);
        hh11[i] = (TH2D*)file->Get(grName.Data());
    }

    TGraphErrors* gr2 = (TGraphErrors*)file->Get("gr2");
    TH2D* h6 = (TH2D*)file->Get("h6");
    TH2D* h10 = (TH2D*)file->Get("h10");
    TH2D* h11 = (TH2D*)file->Get("h11");
    TH3D* h12 = (TH3D*)file->Get("h12");

    //====================================//

    TString hTitle;
    hTitle.Form("P = %.1f #pm %.2f %% [GeV/c]",nominalMom,0.05);
    const Int_t refID = 20;
    gStyle->SetOptStat(0);

    TCanvas* c1 = new TCanvas("c1","c1",450*1,450*1);
    c1->cd();
    gPad->SetGrid();
    grProb[refID]->SetTitle(hTitle.Data());
    grProb[refID]->GetXaxis()->SetTitle("Target Length [m]");
    grProb[refID]->GetYaxis()->SetTitle("Survival Probability");
    grProb[refID]->GetXaxis()->CenterTitle();
    grProb[refID]->GetYaxis()->CenterTitle();
    grProb[refID]->GetYaxis()->SetTitleOffset(1.2);
    grProb[refID]->Draw("APC");

    //=======================================================================================//

    TCanvas* c2 = new TCanvas("c2","c2",450*3,450*2);
    c2->Divide(3,2);
    c2->cd(1);
    gPad->SetGrid();
    hh6[refID]->Draw("colz");

    hh6[refID]->SetTitle(hTitle.Data());
    hh6[refID]->GetXaxis()->SetTitle("Target Length [m]");
    hh6[refID]->GetYaxis()->SetTitle("#theta_{Scat.} [rad]");
    hh6[refID]->GetXaxis()->CenterTitle();
    hh6[refID]->GetYaxis()->CenterTitle();
    hh6[refID]->GetYaxis()->SetTitleOffset(1.2);

    c2->cd(2);
    gPad->SetGrid();
    hh10[refID]->Draw("colz");
    hh10[refID]->GetYaxis()->SetRangeUser(-0.1,4.1);
    hh10[refID]->SetTitle(hTitle.Data());
    hh10[refID]->GetXaxis()->SetTitle("Target Length [m]");
    hh10[refID]->GetYaxis()->SetTitle("#DeltaE_{kin.} [GeV]");
    hh10[refID]->GetXaxis()->CenterTitle();
    hh10[refID]->GetYaxis()->CenterTitle();
    hh10[refID]->GetYaxis()->SetTitleOffset(1.2);

    c2->cd(3);
    gPad->SetGrid();
    hh11[refID]->Draw("colz");
    hh11[refID]->GetYaxis()->SetRangeUser(-0.1,4.1);
    hh11[refID]->SetTitle(hTitle.Data());
    hh11[refID]->GetXaxis()->SetTitle("#theta_{Scat.} [rad]");
    hh11[refID]->GetYaxis()->SetTitle("#DeltaE_{kin.} [GeV]");
    hh11[refID]->GetXaxis()->CenterTitle();
    hh11[refID]->GetYaxis()->CenterTitle();
    hh11[refID]->GetYaxis()->SetTitleOffset(1.2);

    c2->cd(4);
    gPad->SetGrid();
    Bool_t firstPlot = true;
    Int_t nHist = 0;

    Double_t minThetaFit =  0.0e-3;
    Double_t maxThetaFit = 20.0e-3;

    TLegend* leg2 = new TLegend(0.5,0.5,0.8,0.8);

    for(Int_t binxi = 1; binxi <= hh6[refID]->GetNbinsX(); binxi++)
    {
        TString hName;
        hName.Form("projY_%d",binxi);
        TH1D* hh6_ProjY = hh6[refID]->ProjectionY(hName.Data(),binxi,binxi);
        hh6_ProjY->GetXaxis()->SetRange(hh6_ProjY->GetXaxis()->FindBin(minThetaFit),hh6_ProjY->GetXaxis()->FindBin(maxThetaFit));
        if(hh6_ProjY->Integral() <= 0) continue;
        if(nHist >= 4)break;

        hh6_ProjY->SetMarkerStyle(20+nHist);
        hh6_ProjY->SetMarkerSize(0.5);
        hh6_ProjY->SetMarkerColor(1+nHist);

        if(firstPlot)
        {
            hh6_ProjY->Draw("HIST & E1");
            firstPlot = false;
        }
        else
        {
            hh6_ProjY->Draw("SAME & HIST & E1");
        }

        TString legStr;
        legStr.Form("Sim: \t L = %.1f [mm]",hh6[refID]->GetXaxis()->GetBinCenter(binxi)*1e3);
        leg2->AddEntry(hh6_ProjY,legStr.Data(),"lpe");

        nHist++;
    }
    leg2->Draw();
    cout<<"--> nHist (hh6) \t = "<<nHist<<endl;

    c2->cd(5);
    gPad->SetGrid();
    firstPlot = true;
    nHist = 0;

    Double_t minEkin = 0.0;
    Double_t maxEkin = 0.2;

    TLegend* leg3 = new TLegend(0.5,0.5,0.8,0.8);

    for(Int_t binxi = 1; binxi <= hh10[refID]->GetNbinsX(); binxi++)
    {
        TString hName;
        hName.Form("projYY_%d",binxi);
        TH1D* hh10_ProjY = hh10[refID]->ProjectionY(hName.Data(),binxi,binxi);
        hh10_ProjY->GetXaxis()->SetRange(hh10_ProjY->GetXaxis()->FindBin(minEkin),hh10_ProjY->GetXaxis()->FindBin(maxEkin));
        if(hh10_ProjY->Integral() <= 0) continue;
        if(nHist >= 4)break;

        hh10_ProjY->SetMarkerStyle(20+nHist);
        hh10_ProjY->SetMarkerSize(0.5);
        hh10_ProjY->SetMarkerColor(1+nHist);

        if(firstPlot)
        {
            hh10_ProjY->Draw("HIST & E1");
            firstPlot = false;
        }
        else
        {
            hh10_ProjY->Draw("SAME & HIST & E1");
        }

        TString legStr;
        legStr.Form("Sim: \t L = %.1f [mm]",hh10[refID]->GetXaxis()->GetBinCenter(binxi)*1e3);
        leg3->AddEntry(hh10_ProjY,legStr.Data(),"lpe");

        nHist++;
    }
    leg3->Draw();
    gPad->SetLogy();
    cout<<"--> nHist (hh10) \t = "<<nHist<<endl;

    c2->cd(6);
    gPad->SetGrid();
    firstPlot = true;
    nHist = 0;

    TLegend* leg4 = new TLegend(0.2,0.2,0.5,0.5);

    for(Int_t binxi = 1; binxi <= hh11[refID]->GetNbinsX(); binxi++)
    {
        TString hName;
        hName.Form("projYYY_%d",binxi);
        TH1D* hh11_ProjY = hh11[refID]->ProjectionY(hName.Data(),binxi,binxi);
        if(hh11_ProjY->Integral() <= 0) continue;
        if(nHist >= 4)break;

        hh11_ProjY->SetMarkerStyle(20+nHist);
        hh11_ProjY->SetMarkerSize(0.5);
        hh11_ProjY->SetMarkerColor(1+nHist);

        if(firstPlot)
        {
            hh11_ProjY->Draw("HIST & E1");
            firstPlot = false;
        }
        else
        {
            hh11_ProjY->Draw("SAME & HIST & E1");
        }

        TString legStr;
        legStr.Form("Sim: \t #theta_{Scat.} = %.1f [mrad]",hh11[refID]->GetXaxis()->GetBinCenter(binxi)*1e3);
        leg4->AddEntry(hh11_ProjY,legStr.Data(),"lpe");

        nHist++;
    }
    leg4->Draw();
    gPad->SetLogy();
    cout<<"--> nHist (hh11) \t = "<<nHist<<endl;

    hTitle.Form("P = %.1f #pm %.1f %% [GeV/c]",nominalMom,2.0);

    TCanvas* c3 = new TCanvas("c3","c3",450*2,450*2);
    c3->cd();
    gPad->SetGrid();
    h12->Rebin3D(8);
    h12->SetTitle(hTitle.Data());
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

    //=======================================================================================//

    TCanvas* c4 = new TCanvas("c4","c4",450*3,450*2);
    c4->Divide(7,6);

    for(Int_t i = 0; i < nP; i++)
    {
        c4->cd(i+1);
        gPad->SetGrid();
        hh6[i]->Draw("colz");

        hTitle.Form("D0: P = [%.3f | %.3f] GeV/c",Pmin[i],Pmax[i]);
        hh6[i]->SetTitle(hTitle.Data());
        hh6[i]->GetXaxis()->SetTitle("Target Length [m]");
        hh6[i]->GetYaxis()->SetTitle("#theta_{Scat.} [rad]");
        hh6[i]->GetXaxis()->CenterTitle();
        hh6[i]->GetYaxis()->CenterTitle();
        hh6[i]->GetYaxis()->SetTitleOffset(1.2);
    }

    TCanvas* c5 = new TCanvas("c5","c5",450*3,450*2);
    c5->Divide(10,7);
    gPad->SetGrid();
    minThetaFit =  0.0e-3;
    maxThetaFit = 20.0e-3;


    for(Int_t i = 0; i < nP; i++)
    {
        hTitle.Form("D0: P = [%.3f | %.3f] GeV/c",Pmin[i],Pmax[i]);

        Bool_t firstPlot = true;
        nHist = 0;

        for(Int_t binxi = 1; binxi <= hh6[i]->GetNbinsX(); binxi++)
        {
            c5->cd(nHist+1);
            gPad->SetGrid();

            TString hName;
            hName.Form("projY_%d_%d",i,binxi);
            TH1D* hh6_ProjY = hh6[i]->ProjectionY(hName.Data(),binxi,binxi);
            hh6_ProjY->GetXaxis()->SetRange(hh6_ProjY->GetXaxis()->FindBin(minThetaFit),hh6_ProjY->GetXaxis()->FindBin(maxThetaFit));
            if(hh6_ProjY->Integral() <= 0) continue;

            if(firstPlot)
            {
                hh6_ProjY->Draw("HIST & E1");
                firstPlot = false;
            }
            else
            {
                hh6_ProjY->Draw("SAME & HIST & E1");
            }
            nHist++;
        }
    }
    //=======================================================================================//

    hTitle.Form("P = %.1f #pm %.1f %% [GeV/c]",nominalMom,2.0);

    TCanvas* cc1 = new TCanvas("cc1","cc1",450*1,450*1);
    cc1->cd();
    gPad->SetGrid();
    gr2->SetTitle(hTitle.Data());
    gr2->GetXaxis()->SetTitle("Target Length [m]");
    gr2->GetYaxis()->SetTitle("Survival Probability");
    gr2->GetXaxis()->CenterTitle();
    gr2->GetYaxis()->CenterTitle();
    gr2->GetYaxis()->SetTitleOffset(1.2);
    gr2->Draw("APC");

    //=======================================================================================//

    TCanvas* cc2 = new TCanvas("cc2","cc2",450*3,450*2);
    cc2->Divide(3,2);
    cc2->cd(1);
    gPad->SetGrid();
    h6->Draw("colz");

    h6->SetTitle(hTitle.Data());
    h6->GetXaxis()->SetTitle("Target Length [m]");
    h6->GetYaxis()->SetTitle("#theta_{Scat.} [rad]");
    h6->GetXaxis()->CenterTitle();
    h6->GetYaxis()->CenterTitle();
    h6->GetYaxis()->SetTitleOffset(1.2);

    cc2->cd(2);
    gPad->SetGrid();
    h10->Draw("colz");

    h10->GetYaxis()->SetRangeUser(-0.1,4.1);
    h10->SetTitle(hTitle.Data());
    h10->GetXaxis()->SetTitle("Target Length [m]");
    h10->GetYaxis()->SetTitle("#DeltaE_{kin.} [GeV]");
    h10->GetXaxis()->CenterTitle();
    h10->GetYaxis()->CenterTitle();
    h10->GetYaxis()->SetTitleOffset(1.2);

    cc2->cd(3);
    gPad->SetGrid();
    h11->Draw("colz");

    h11->GetYaxis()->SetRangeUser(-0.1,4.1);
    h11->SetTitle(hTitle.Data());
    h11->GetXaxis()->SetTitle("#theta_{Scat.} [rad]");
    h11->GetYaxis()->SetTitle("#DeltaE_{kin.} [GeV]");
    h11->GetXaxis()->CenterTitle();
    h11->GetYaxis()->CenterTitle();
    h11->GetYaxis()->SetTitleOffset(1.2);

    cc2->cd(4);
    gPad->SetGrid();
    firstPlot = true;
    nHist = 0;

    minThetaFit =  0.0e-3;
    maxThetaFit = 20.0e-3;

    TLegend* legg2 = new TLegend(0.5,0.5,0.8,0.8);

    for(Int_t binxi = 1; binxi <= h6->GetNbinsX(); binxi++)
    {
        TString hName;
        hName.Form("projjY_%d",binxi);
        TH1D* h6_ProjY = h6->ProjectionY(hName.Data(),binxi,binxi);
        h6_ProjY->GetXaxis()->SetRange(h6_ProjY->GetXaxis()->FindBin(minThetaFit),h6_ProjY->GetXaxis()->FindBin(maxThetaFit));
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
        legg2->AddEntry(h6_ProjY,legStr.Data(),"lpe");

        nHist++;
    }
    legg2->Draw();
    cout<<"--> nHist (h6) \t = "<<nHist<<endl;

    cc2->cd(5);
    gPad->SetGrid();
    firstPlot = true;
    nHist = 0;

    minEkin = 0.0;
    maxEkin = 0.2;

    TLegend* legg3 = new TLegend(0.5,0.5,0.8,0.8);

    for(Int_t binxi = 1; binxi <= h10->GetNbinsX(); binxi++)
    {
        TString hName;
        hName.Form("projjYY_%d",binxi);
        TH1D* h10_ProjY = h10->ProjectionY(hName.Data(),binxi,binxi);
        h10_ProjY->GetXaxis()->SetRange(h10_ProjY->GetXaxis()->FindBin(minEkin),h10_ProjY->GetXaxis()->FindBin(maxEkin));
        h10_ProjY->SetMinimum(1e3);
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
        legg3->AddEntry(h10_ProjY,legStr.Data(),"lpe");

        nHist++;
    }
    legg3->Draw();
    gPad->SetLogy();
    cout<<"--> nHist (h10) \t = "<<nHist<<endl;

    cc2->cd(6);
    gPad->SetGrid();
    firstPlot = true;
    nHist = 0;

    TLegend* legg4 = new TLegend(0.2,0.2,0.5,0.5);

    for(Int_t binxi = 1; binxi <= h11->GetNbinsX(); binxi++)
    {
        TString hName;
        hName.Form("projjYYY_%d",binxi);
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
        legg4->AddEntry(h11_ProjY,legStr.Data(),"lpe");

        nHist++;
    }
    legg4->Draw();
    gPad->SetLogy();
    cout<<"--> nHist (h11) \t = "<<nHist<<endl;

    cc2->cd(1);
    gPad->SetLogz();
    h6->RebinX(10);
    h6->RebinY(10);

    cc2->cd(2);
    gPad->SetLogz();
    h10->RebinX(10);
    h10->RebinY(10);
    h10->GetYaxis()->SetRangeUser(-0.1,4.1);

    cc2->cd(3);
    gPad->SetLogz();
    //=======================================================================================//

    stop_time = time(NULL);
    cout<<"--> Running time is : "<<stop_time - start_time<<" [sec]"<<endl;

    return 0;
}

