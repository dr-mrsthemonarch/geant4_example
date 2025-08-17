//=============================================================================
// main.cpp - Main simulation program for scintillator light output testing
//=============================================================================

#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

// Include your custom classes
#include "DetectorConstruction.hh"
#include "ActionInitialization.hh"
#include "PhysicsList.hh"

int main(int argc, char** argv)
{
    // Construct the default run manager
    G4RunManager* runManager = new G4RunManager;
    
    // Set mandatory initialization classes
    runManager->SetUserInitialization(new DetectorConstruction);
    runManager->SetUserInitialization(new PhysicsList);
    runManager->SetUserInitialization(new ActionInitialization);
    
    // Initialize G4 kernel
    runManager->Initialize();
    
    // Initialize visualization
    G4VisManager* visManager = new G4VisExecutive;
    visManager->Initialize();
    
    // Get the pointer to the User Interface manager
    G4UImanager* UImanager = G4UImanager::GetUIpointer();
    
    if (argc != 1) {
        // Batch mode - execute macro file
        G4String command = "/control/execute ";
        G4String fileName = argv[1];
        UImanager->ApplyCommand(command + fileName);
    } else {
        // Interactive mode
        G4UIExecutive* ui = new G4UIExecutive(argc, argv);
        
        // Execute initialization macro if it exists
        UImanager->ApplyCommand("/control/execute init_vis.mac");
        
        ui->SessionStart();
        delete ui;
    }
    
    // Job termination - clean up
    delete visManager;
    delete runManager;
    
    return 0;
}
