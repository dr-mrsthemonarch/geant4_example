#include "../include/EventAction.hh"

#include "G4Event.hh"
#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"
#include "G4AutoLock.hh"

// Mutex for thread-safe output
namespace {
    G4Mutex eventMutex = G4MUTEX_INITIALIZER;
}

EventAction::EventAction()
 : G4UserEventAction(),
   fEnergyDeposit(0.),
   fScintPhotonCount(0)
{
}

EventAction::~EventAction()
{
}

void EventAction::BeginOfEventAction(const G4Event* event)
{
    // Reset per-event counters
    fEnergyDeposit = 0.;
    fScintPhotonCount = 0;
}

void EventAction::EndOfEventAction(const G4Event* event)
{
    // Print summary only occasionally in MT mode
    G4int eventID = event->GetEventID();

    // Only print every 1000th event or events with significant activity
    if (eventID % 1000 == 0 || (fEnergyDeposit > 100.*keV && fScintPhotonCount > 1000)) {
        G4AutoLock lock(&eventMutex);
        G4cout << "Event " << eventID
               << ": Energy = " << fEnergyDeposit/keV << " keV"
               << ", Scint photons = " << fScintPhotonCount
               << G4endl;
    }
}