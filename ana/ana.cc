#include "AnalysisClass.hh"

#include <ctime>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <fstream>
#include <string>
#include <time.h>

using namespace std;

int main()
{
    time_t start_time, stop_time;
    start_time = time(NULL);

    //----------------------------------------------- SETTINGS ---------------------------------------------//
    //______________________________________________________________________________________________________//
    TString mainInDir   = "./"; // "/Volumes/HDD_BACKUP/SCATTERING_MATRIX/";
    TString mainOutDir  = "./";
    //======== LER W ========//
//    TString filename = "output_ler_w.root";
    //======== LER Ta ========//
//    TString filename = "output_ler_ta.root";
    //======== LER Cu ========//
//    TString filename = "output_ler_cu.root";
    //======== LER C ========//
//    TString filename = "output_ler_c.root";
    //======== HER W ========//
//    TString filename = "output_her_w.root";
    //======== HER Ta ========//
//    TString filename = "output_her_ta.root";
    //======== HER Cu ========//
//    TString filename = "output_her_cu.root";
    //======== HER C ========//
    TString filename = "output_her_c.root";
    //______________________________________________________________________________________________________//

    AnalysisClass* _ana = new AnalysisClass();
    _ana->SetInputFileName(mainInDir+filename);
    _ana->InitTree("tree");
    cout<<"--> Input file name: "<<_ana->GetInputFileName()<<endl;
    filename += "_ana.root";
    _ana->SetOutputFileName(mainOutDir+filename);
    cout<<"--> Output file name: "<<_ana->GetOutputFileName()<<endl;

    _ana->Loop();
    _ana->~AnalysisClass();

    stop_time = time(NULL);
    cout<<"--> Running time is : "<<stop_time - start_time<<" [sec]"<<endl;

    return 0;
}
