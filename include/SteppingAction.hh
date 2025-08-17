
#ifndef SteppingAction_h
#define SteppingAction_h 1


#include "G4UserSteppingAction.hh"
#include "G4Step.hh"
#include "G4Threading.hh"
#include "globals.hh"

class SteppingAction : public G4UserSteppingAction
{
public:
    SteppingAction();
    virtual ~SteppingAction();

    virtual void UserSteppingAction(const G4Step* step) override;

private:
    // Thread-local storage for tracking data
    G4ThreadLocal static G4int fEventPhotonCount;
    G4ThreadLocal static G4int fLastEventID;
};

#endif
