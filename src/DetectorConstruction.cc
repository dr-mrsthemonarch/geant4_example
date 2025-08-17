//=============================================================================
// DetectorConstruction.cc - Implementation of detector geometry
//=============================================================================

#include "DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"

DetectorConstruction::DetectorConstruction()
    : G4VUserDetectorConstruction() {
}

DetectorConstruction::~DetectorConstruction() {
}

G4VPhysicalVolume *DetectorConstruction::Construct() {
    // Get materials from NIST database
    G4NistManager *nist = G4NistManager::Instance();

    // Define world material (air)
    G4Material *world_mat = nist->FindOrBuildMaterial("G4_AIR");

    // Define scintillator material (NaI with Tl doping)
    G4Material *scint_mat = nist->FindOrBuildMaterial("G4_SODIUM_IODIDE");
    G4MaterialPropertiesTable* scintMPT = new G4MaterialPropertiesTable();

    // Example photon energy range (2–3.5 eV ~ 350–620 nm)
    const G4int NUM = 2;
    G4double photonEnergy[NUM]    = {2.0*eV, 3.5*eV};
    G4double rindexNaI[NUM]       = {1.85, 1.85};
    G4double absorption[NUM]      = {0.5*m, 0.5*m};

    scintMPT->AddProperty("RINDEX", photonEnergy, rindexNaI, NUM);
    scintMPT->AddProperty("ABSLENGTH", photonEnergy, absorption, NUM);
    // scintMPT->AddConstProperty("SCINTILLATIONYIELD", 100./MeV);


    // Light yield ~38k photons/MeV for NaI:Tl
    scintMPT->AddConstProperty("SCINTILLATIONYIELD", 38000./MeV);
    scintMPT->AddConstProperty("RESOLUTIONSCALE", 1.0);
    scintMPT->AddConstProperty("SCINTILLATIONTIMECONSTANT1", 230.*ns);
    scintMPT->AddConstProperty("SCINTILLATIONYIELD1", 1.0);

    scint_mat->SetMaterialPropertiesTable(scintMPT);

    // World dimensions
    G4double world_size = 1.0 * m;

    // Scintillator dimensions
    G4double scint_sizeX = 5.0 * cm;
    G4double scint_sizeY = 5.0 * cm;
    G4double scint_sizeZ = 10.0 * cm;

    // Create world volume
    G4Box *solidWorld = new G4Box("World",
                                  0.5 * world_size,
                                  0.5 * world_size,
                                  0.5 * world_size);

    G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld,world_mat,"World");

    G4VPhysicalVolume *physWorld = new G4PVPlacement(0, // no rotation
                                                     G4ThreeVector(), // at (0,0,0)
                                                     logicWorld, // logical volume
                                                     "World", // name
                                                     0, // mother volume
                                                     false, // no boolean operation
                                                     0, // copy number
                                                     true); // overlaps checking

    // Create scintillator detector
    G4Box *solidScintillator = new G4Box("Scintillator",
                                         0.5 * scint_sizeX,
                                         0.5 * scint_sizeY,
                                         0.5 * scint_sizeZ);

    G4LogicalVolume *logicScintillator = new G4LogicalVolume(solidScintillator,
                                                             scint_mat,
                                                             "Scintillator");

    // Place scintillator at the center of the world
    new G4PVPlacement(0, // no rotation
                      G4ThreeVector(0, 0, 0), // at center
                      logicScintillator, // logical volume
                      "Scintillator", // name
                      logicWorld, // mother volume
                      false, // no boolean operation
                      0, // copy number
                      true); // overlaps checking

    // Set visualization attributes
    G4VisAttributes *worldVisAtt = new G4VisAttributes(G4Colour(0, 0, 0, 0.1)); // white, transparent
    worldVisAtt->SetVisibility(false); // Make world invisible
    logicWorld->SetVisAttributes(worldVisAtt);

    G4VisAttributes *scintVisAtt = new G4VisAttributes(G4Colour(0.0, 1.0, 1.0, 0.8)); // cyan, semi-transparent
    scintVisAtt->SetVisibility(true);
    scintVisAtt->SetForceSolid(true);
    logicScintillator->SetVisAttributes(scintVisAtt);

    return physWorld;
}
