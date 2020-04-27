#include "AnalysisClass.hh"

#include <ctime>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <fstream>
#include <string>

using namespace std;

int main()
{
    TString filename = "output_all.root";
    AnalysisClass* _ana = new AnalysisClass();
    _ana->SetInputFileName(filename);
    _ana->InitTree("tree");
    cout<<"--> Input file name: "<<_ana->GetInputFileName()<<endl;
    filename += "_ana.root";
    _ana->SetOutputFileName(filename);
    cout<<"--> Output file name: "<<_ana->GetOutputFileName()<<endl;

    _ana->Loop();
    return 0;
}
