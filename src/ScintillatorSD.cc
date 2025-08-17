#include "ScintillatorSD.hh"

#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4StepPoint.hh"
#include "G4VProcess.hh"
#include "G4ParticleDefinition.hh"
#include "G4OpticalPhoton.hh"
#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4AutoLock.hh"

// Define thread-local storage
G4ThreadLocal G4double ScintillatorSD::fTotalEnergyDeposit = 0.;
G4ThreadLocal G4int ScintillatorSD::fPhotonCount = 0;

// Mutex for thread-safe output (only if you want synchronized output)
namespace {
    G4Mutex scoringMutex = G4MUTEX_INITIALIZER;
}

ScintillatorSD::ScintillatorSD(const G4String& name)
 : G4VSensitiveDetector(name)
{
}

ScintillatorSD::~ScintillatorSD()
{
}

void ScintillatorSD::Initialize(G4HCofThisEvent* hitCollection)
{
    // Reset thread-local counters for each event
    fTotalEnergyDeposit = 0.;
    fPhotonCount = 0;
}


G4bool ScintillatorSD::ProcessHits(G4Step* step, G4TouchableHistory* history)
{
    G4Track* track = step->GetTrack();
    G4ParticleDefinition* particle = track->GetDefinition();
    
    // Count optical photons
    if (particle == G4OpticalPhoton::OpticalPhotonDefinition()) {
        // Count scintillation photons (created by scintillation process)
        const G4VProcess* creatorProcess = track->GetCreatorProcess();
        if (creatorProcess && creatorProcess->GetProcessName() == "Scintillation") {
            fPhotonCount++;
        }
    } else {
        // For other particles, collect energy deposit
        G4double edep = step->GetTotalEnergyDeposit();
        if (edep > 0.) {
            fTotalEnergyDeposit += edep;
        }
    }
    
    return true;
}


void ScintillatorSD::EndOfEvent(G4HCofThisEvent* hitCollection)
{
    // Only print if there's something to report and we want output
    // In MT mode, minimize output to avoid thread conflicts
    if (fTotalEnergyDeposit > 0. || fPhotonCount > 0) {
        // Thread-safe output (optional - can be commented out for performance)
        /*
        G4AutoLock lock(&scoringMutex);
        G4int eventID = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
        G4cout << "Event " << eventID
               << ": Energy = " << fTotalEnergyDeposit/keV << " keV"
               << ", Photons = " << fPhotonCount
               << G4endl;
        */
    }
}
