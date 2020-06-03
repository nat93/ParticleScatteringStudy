#ifndef Constants_h
#define Constants_h

#include "globals.hh"
#include "G4SystemOfUnits.hh"

namespace Constants
{
    const G4double world_dim_x = 5000.0*mm;
    const G4double world_dim_y = 5000.0*mm;
    const G4double world_dim_z = 1000.0*mm;

    const G4double target_pos_x =  0.0*mm;
    const G4double target_pos_y =  0.0*mm;
    const G4double target_pos_z =  0.0*mm;

    const G4double phantom_dim_z    =  1.0*mm;
    const G4double phantom_0_pos_z  = -400.0*mm;
    const G4double phantom_1_pos_z  =  400.0*mm;

    const G4double beam_pos_x = 0.0*mm;
    const G4double beam_pos_y = 0.0*mm;
    const G4double beam_pos_z = -450.0*mm;
}

#endif
