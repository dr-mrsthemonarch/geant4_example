#include "DetectorConstruction.hh"
#include "ScintillatorSD.hh"
#include "G4SDManager.hh""
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

    // Define world material (air) with optical properties
    G4Material *world_mat = nist->FindOrBuildMaterial("G4_AIR");

    // Add optical properties to air for optical photon transport
    G4MaterialPropertiesTable* airMPT = new G4MaterialPropertiesTable();

    const G4int NUM = 10;
    G4double photonEnergy[NUM] = {
        1.5*eV, 2.0*eV, 2.2*eV, 2.4*eV, 2.6*eV,
        2.8*eV, 3.0*eV, 3.2*eV, 3.4*eV, 3.6*eV
    };

    G4double rindexAir[NUM];
    G4double absorptionAir[NUM];

    for(int i = 0; i < NUM; i++) {
        rindexAir[i] = 1.000293;     // Refractive index of air at STP
        absorptionAir[i] = 10.*m;    // Long absorption length in air
    }

    airMPT->AddProperty("RINDEX", photonEnergy, rindexAir, NUM);
    airMPT->AddProperty("ABSLENGTH", photonEnergy, absorptionAir, NUM);

    world_mat->SetMaterialPropertiesTable(airMPT);


    // Define scintillator material (NaI with Tl doping)
    G4Material *scint_mat = nist->FindOrBuildMaterial("G4_SODIUM_IODIDE");
    G4MaterialPropertiesTable* scintMPT = new G4MaterialPropertiesTable();

    G4double rindexNaI[NUM]       = {1.85, 1.85};
    G4double absorption[NUM]      = {0.5*m, 0.5*m};
    G4double scintillation[NUM];

    for(int i = 0; i < NUM; i++) {
        rindexNaI[i] = 1.85;        // Constant refractive index
        absorption[i] = 50.*cm;      // Absorption length
        scintillation[i] = 1.0;      // Relative scintillation intensity
    }
    // Peak emission around 415 nm (3.0 eV) for NaI:Tl
    scintillation[6] = 1.0;  // Peak at 3.0 eV



    scintMPT->AddProperty("RINDEX", photonEnergy, rindexNaI, NUM);
    scintMPT->AddProperty("ABSLENGTH", photonEnergy, absorption, NUM);
    scintMPT->AddProperty("SCINTILLATIONCOMPONENT1", photonEnergy, scintillation, NUM);

    // Light yield ~38k photons/MeV for NaI:Tl
    scintMPT->AddConstProperty("SCINTILLATIONYIELD", 50./MeV);
    scintMPT->AddConstProperty("RESOLUTIONSCALE", 1.0);
    scintMPT->AddConstProperty("SCINTILLATIONTIMECONSTANT1", 230.*ns);
    // scintMPT->AddConstProperty("SCINTILLATIONYIELD1", 1.0/MeV);

    scint_mat->SetMaterialPropertiesTable(scintMPT);


    // World dimensions
    G4double world_size = 2.0 * m;

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
    // Register sensitive detector
    G4SDManager* SDManager = G4SDManager::GetSDMpointer();
    ScintillatorSD* scintSD = new ScintillatorSD("ScintillatorSD");
    SDManager->AddNewDetector(scintSD);
    logicScintillator->SetSensitiveDetector(scintSD);

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
