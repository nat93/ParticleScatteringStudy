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

    TString filename = "output_her_cu.root";
    AnalysisClass* _ana = new AnalysisClass();
    _ana->SetInputFileName(filename);
    _ana->InitTree("tree");
    cout<<"--> Input file name: "<<_ana->GetInputFileName()<<endl;
    filename += "_ana.root";
    _ana->SetOutputFileName(filename);
    cout<<"--> Output file name: "<<_ana->GetOutputFileName()<<endl;

    _ana->Loop();
    _ana->~AnalysisClass();

    stop_time = time(NULL);
    cout<<"--> Running time is : "<<stop_time - start_time<<" [sec]"<<endl;

    return 0;
}
