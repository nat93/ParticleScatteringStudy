ROOTCFLAGS    = $(shell $(ROOTSYS)/bin/root-config --cflags)
ROOTLIBS      = $(shell $(ROOTSYS)/bin/root-config --libs)
ROOTGLIBS     = $(shell $(ROOTSYS)/bin/root-config --glibs)

CPPFLAGS += $(shell $(ROOTSYS)/bin/root-config --cflags)
EXTRALIBS += $(shell $(ROOTSYS)/bin/root-config --libs)
LDLIBS += $(shell $(ROOTSYS)/bin/root-config --libs)

CXX           = g++
CXXFLAGS      = -g -Wall -fPIC -Wno-deprecated

NGLIBS         = -Wl, $(ROOTGLIBS) 
NGLIBS        += -lMinuit

CXXFLAGS      += $(ROOTCFLAGS)
CXX           += -I./
LIBS           = $(ROOTLIBS) 

GLIBS          = $(filter-out -lNew, $(NGLIBS))

CXX	      += -I./obj/
OUTLIB	      = ./obj/
.SUFFIXES: .cc,.C
.PREFIXES: ./obj/

#----------------------------------------------------#

all: ana plot

ana: ana.cc obj/AnalysisClass.o
	$(CXX) $(CXXFLAGS) -o ana $(OUTLIB)/*.o $(GLIBS) $<

obj/AnalysisClass.o: AnalysisClass.cc AnalysisClass.hh 
	$(CXX) $(CXXFLAGS) -c -I. -o $(OUTLIB)AnalysisClass.o $<

clean:
	rm -f *~
	rm -f ana
	rm -f plot
	rm -f $(OUTLIB)*.o
	rm -rf *.dSYM
