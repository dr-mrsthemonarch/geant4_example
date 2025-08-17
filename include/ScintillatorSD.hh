#ifndef ScintillatorSD_h
#define ScintillatorSD_h 1

#include "G4VSensitiveDetector.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4Threading.hh"

class ScintillatorSD : public G4VSensitiveDetector
{
public:
    ScintillatorSD(const G4String& name);
    virtual ~ScintillatorSD();

    // Methods from base class
    virtual void Initialize(G4HCofThisEvent* hitCollection) override;
    virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* history) override;
    virtual void EndOfEvent(G4HCofThisEvent* hitCollection) override;

private:
    // Thread-local storage for event data
    G4ThreadLocal static G4double fTotalEnergyDeposit;
    G4ThreadLocal static G4int fPhotonCount;
};

#endif
