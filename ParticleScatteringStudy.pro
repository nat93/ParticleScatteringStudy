TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
INCLUDEPATH += "/Users/andrii/root/obj/include"
INCLUDEPATH += "/Users/andrii/geant4.10/geant4-install/include/Geant4"
INCLUDEPATH += "/Users/andrii/geant4.10/clhep-install/include"
INCLUDEPATH += "include"

LIBS += -L/Users/andrii/root/obj/lib -lGui -lCore -lRIO -lNet -lHist -lGraf -lGraf3d -lGpad -lTree -lRint -lPostscript -lMatrix -lPhysics -lMathCore -lThread
LIBS += -L/Users/andrii/geant4.10/geant4-install/lib -lG4analysis -lG4digits_hits -lG4error_propagation -lG4event -lG4Tree -lG4FR -lG4GMocren -lG4visHepRep -lG4RayTracer -lG4VRML -lG4vis_management -lG4modeling -lG4interfaces -lG4persistency -lG4readout -lG4physicslists -lG4run -lG4tracking -lG4parmodels -lG4processes  -lG4track -lG4particles -lG4geometry -lG4materials -lG4graphics_reps -lG4intercoms -lG4global
LIBS += -L/Users/andrii/geant4.10/clhep-install/lib -lCLHEP -lCLHEP-2.4.1.3 -lCLHEP-Cast-2.4.1.3 -lCLHEP-Evaluator-2.4.1.3 -lCLHEP-Exceptions-2.4.1.3

SOURCES += \
    ana/AnalysisClass.cc \
    ana/ana.cc \
    main.cc \
    src/SteppingAction.cc \
    src/StackingMessenger.cc \
    src/StackingAction.cc \
    src/RunMessenger.cc \
    src/RunAction.cc \
    src/PrimaryMessenger.cc\
    src/PrimaryGeneratorAction.cc \
    src/EventAction.cc \
    src/DetectorMessenger.cc \
    src/DetectorConstruction.cc \

DISTFILES += \
    CMakeLists.txt \
    README.md \
    output/mergeFiles.bash \
    run.mac \
    runScreen.bash \
    vis.mac \
    runAll.bash

HEADERS += \
    ana/AnalysisClass.hh \
    include/*.hh \


