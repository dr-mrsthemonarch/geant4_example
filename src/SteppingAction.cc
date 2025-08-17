#include "SteppingAction.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4ParticleDefinition.hh"
#include "G4OpticalPhoton.hh"
#include "G4VProcess.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4SystemOfUnits.hh"
#include "G4AutoLock.hh"

// Define thread-local storage
G4ThreadLocal G4int SteppingAction::fEventPhotonCount = 0;
G4ThreadLocal G4int SteppingAction::fLastEventID = -1;

// Mutex for thread-safe output (only if needed)
namespace {
  G4Mutex steppingMutex = G4MUTEX_INITIALIZER;
}

SteppingAction::SteppingAction()
 : G4UserSteppingAction()
{
}

SteppingAction::~SteppingAction()
{
}

void SteppingAction::UserSteppingAction(const G4Step* step)
{
    G4Track* track = step->GetTrack();
    G4int currentEventID = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();

    // Reset counter for new events (thread-local)
    if (currentEventID != fLastEventID) {
        // Minimal output in MT mode - avoid printing per event
        fEventPhotonCount = 0;
        fLastEventID = currentEventID;
    }

    G4ParticleDefinition* particle = track->GetDefinition();

    // Track optical photons
    if (particle == G4OpticalPhoton::OpticalPhotonDefinition()) {
        // Count optical photons created by scintillation
        if (track->GetCurrentStepNumber() == 1) { // First step of the track
            const G4VProcess* creatorProcess = track->GetCreatorProcess();
            if (creatorProcess && creatorProcess->GetProcessName() == "Scintillation") {
                fEventPhotonCount++;

                // In MT mode, avoid detailed output per photon
                // Only enable for debugging specific events
                /*
                if (fEventPhotonCount <= 5 && currentEventID < 10) {
                    G4AutoLock lock(&steppingMutex);
                    G4ThreeVector position = track->GetPosition();
                    G4double energy = track->GetKineticEnergy();
                    G4cout << "Thread " << G4Threading::G4GetThreadId()
                           << " Event " << currentEventID
                           << " Photon #" << fEventPhotonCount
                           << " at (" << position.x()/mm << ", "
                           << position.y()/mm << ", " << position.z()/mm
                           << ") mm, E = " << energy/eV << " eV" << G4endl;
                }
                */
            }
        }
    }
}