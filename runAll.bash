#!/bin/bash

threadID=$1;

echo "Bash version ${BASH_VERSION}..." > log_${threadID}.txt;

# GEANT4 env. variables
cd ~/geant4.10/geant4-build/; source geant4make.sh; cd -;

# ROOT env. variables
source ~/root/obj/bin/thisroot.sh;

chmod u+x exe;
chmod a+x exe;

runID=1;
incr=1;
nParticles=10000;

# LER
mom=4.000000000; # [GeV/c]
parType="e+";

# HER
#mom=7.007290000; # [GeV/c]
#parType="e-";

for length in $(seq 1.0 1.0 30.0)
do
    for deltaP in $(seq -0.01 0.001 0.01)
    do
        echo "##-----------------------------------------##"                >  ./macFiles/run_${threadID}_${runID}.mac;
        echo "/run/verbose 1"                                               >> ./macFiles/run_${threadID}_${runID}.mac;
        echo "/event/verbose 0"                                             >> ./macFiles/run_${threadID}_${runID}.mac;
        echo "/tracking/verbose 0"                                          >> ./macFiles/run_${threadID}_${runID}.mac;
        echo "##-----------------------------------------##"                >> ./macFiles/run_${threadID}_${runID}.mac;
        echo "/target/length ${length} mm"                                  >> ./macFiles/run_${threadID}_${runID}.mac;
        echo "/target/width 80.0 mm"                                        >> ./macFiles/run_${threadID}_${runID}.mac;
        echo "/target/height 80.0 mm"                                       >> ./macFiles/run_${threadID}_${runID}.mac;
        echo "/output/filename ../output/output_${threadID}_${runID}.root"  >> ./macFiles/run_${threadID}_${runID}.mac;
        echo "/stack/killsecondaries 1"                                     >> ./macFiles/run_${threadID}_${runID}.mac;
        echo "/beam/deltaP ${deltaP}"                                       >> ./macFiles/run_${threadID}_${runID}.mac;
        echo "/beam/P ${mom} GeV"                                           >> ./macFiles/run_${threadID}_${runID}.mac;
        echo "/beam/pName ${parType}"                                       >> ./macFiles/run_${threadID}_${runID}.mac;
        echo "/beam/PdirX 0.0"                                              >> ./macFiles/run_${threadID}_${runID}.mac;
        echo "/beam/PdirY 0.0"                                              >> ./macFiles/run_${threadID}_${runID}.mac;
        echo "/beam/PdirZ 1.0"                                              >> ./macFiles/run_${threadID}_${runID}.mac;
        echo ""                                                             >> ./macFiles/run_${threadID}_${runID}.mac;
        echo "/run/initialize"                                              >> ./macFiles/run_${threadID}_${runID}.mac;
        echo "##-----------------------------------------##"                >> ./macFiles/run_${threadID}_${runID}.mac;
        echo "/run/beamOn ${nParticles}"                                    >> ./macFiles/run_${threadID}_${runID}.mac;
        echo "##-----------------------------------------##"                >> ./macFiles/run_${threadID}_${runID}.mac;

        cat ./macFiles/run_${threadID}_${runID}.mac >> log_${threadID}.txt;
        ./exe ./macFiles/run_${threadID}_${runID}.mac >> log_${threadID}.txt;
        runID=$(($runID+$incr));
    done
done

