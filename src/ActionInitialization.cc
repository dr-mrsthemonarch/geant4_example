
#include "ActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"
#include "EventAction.hh"       // Add this
#include "SteppingAction.hh"
#include "G4RunManager.hh"

ActionInitialization::ActionInitialization()
 : G4VUserActionInitialization()
{}

ActionInitialization::~ActionInitialization()
{}

void ActionInitialization::BuildForMaster() const
{
    // Master thread actions (for data merging if needed)
    // SetUserAction(new RunAction);
}

void ActionInitialization::Build() const
{
    // Set primary generator
    SetUserAction(new PrimaryGeneratorAction);

    // Set event action for per-event data collection
    EventAction* eventAction = new EventAction;
    SetUserAction(eventAction);

    // Set stepping action (pass event action for data accumulation)
    // SetUserAction(new SteppingAction(eventAction));

    // Add other user actions here as needed
    // SetUserAction(new RunAction);
}