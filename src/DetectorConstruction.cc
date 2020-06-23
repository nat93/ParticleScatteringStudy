#include "DetectorConstruction.hh"
#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4RotationMatrix.hh"
#include "G4NistManager.hh"
#include "G4VisAttributes.hh"
#include "G4SDManager.hh"
#include "G4PhysicalConstants.hh"
#include "G4RunManager.hh"
#include "G4ThreeVector.hh"
#include "Constants.hh"
#include "G4SystemOfUnits.hh"
#include "globals.hh"

DetectorConstruction::DetectorConstruction(RunAction* runAct) : G4VUserDetectorConstruction(),
    fDetectorMessenger(0),runAction(runAct)
{
    // default values
    _target_length  = 1.0*mm;
    _target_width   = 1.0*mm;
    _target_height  = 1.0*mm;

    fDetectorMessenger = new DetectorMessenger(this);
}

DetectorConstruction::~DetectorConstruction()
{
    delete fDetectorMessenger;
}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
    G4NistManager* nistManager = G4NistManager::Instance();

//    G4Material* target_material = nistManager->FindOrBuildMaterial("G4_W");
//    G4Material* target_material = nistManager->FindOrBuildMaterial("G4_Cu");
//    G4Material* target_material = nistManager->FindOrBuildMaterial("G4_GRAPHITE");
    G4Material* target_material = nistManager->FindOrBuildMaterial("G4_Ta");

    G4Material* VACUUM      = nistManager->FindOrBuildMaterial("G4_Galactic");

    // WORLD
    G4Box* world_box = new G4Box("world",
                                 Constants::world_dim_x/2,
                                 Constants::world_dim_y/2,
                                 Constants::world_dim_z/2);
    G4LogicalVolume* world_log = new G4LogicalVolume(world_box, VACUUM, "world");
    G4VPhysicalVolume* world_phys = new G4PVPlacement(0, G4ThreeVector(0.,0.,0.),world_log,"world",0,false,0);

    // TARGET
    if(_target_length > 0)
    {
        G4Box* target_box = new G4Box("target_box",
                                      _target_width/2,
                                      _target_height/2,
                                      _target_length/2);

        runAction->_targetL = target_box->GetZHalfLength()*2.0/m;
        runAction->_targetW = target_box->GetXHalfLength()*2.0/m;
        runAction->_targetH = target_box->GetYHalfLength()*2.0/m;

        G4LogicalVolume* target_log = new G4LogicalVolume(target_box,target_material,"target_box");
        new G4PVPlacement(0,G4ThreeVector(Constants::target_pos_x,
                                          Constants::target_pos_y,
                                          Constants::target_pos_z),target_log,"target_box",world_log,false,0);

        target_log->SetVisAttributes(G4VisAttributes(G4Color::Magenta()));
    }

    // PHANTOM SENSITIVE PLANE
    G4Box* phantom_box = new G4Box("phantom",
                                   Constants::world_dim_x/2,
                                   Constants::world_dim_y/2,
                                   Constants::phantom_dim_z/2);

    G4LogicalVolume* phantom_log_0 = new G4LogicalVolume(phantom_box,VACUUM,"phantom_0");
    G4LogicalVolume* phantom_log_1 = new G4LogicalVolume(phantom_box,VACUUM,"phantom_1");
    new G4PVPlacement(0,G4ThreeVector(0,0, Constants::phantom_0_pos_z),phantom_log_0,"phantom_0",world_log,false,0);
    new G4PVPlacement(0,G4ThreeVector(0,0, Constants::phantom_1_pos_z),phantom_log_1,"phantom_1",world_log,false,0);

    phantom_log_0->SetVisAttributes(G4VisAttributes(G4Color::Gray()));
    phantom_log_1->SetVisAttributes(G4VisAttributes(G4Color::Gray()));

    return world_phys;
}
