//=============================================================================
// ActionInitializer.cc - Implementation of action initialization
//=============================================================================

#include "ActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"

// Include other action classes here when they are added:
// #include "RunAction.hh"
// #include "EventAction.hh"
// #include "SteppingAction.hh"

ActionInitializer::ActionInitializer()
: G4VUserActionInitialization()
{
}

ActionInitializer::~ActionInitializer()
{
}

void ActionInitializer::BuildForMaster() const
{
    // Actions for master thread (for multi-threaded mode)
    // Only RunAction should be instantiated for the master thread
    // SetUserAction(new RunAction(true)); // true indicates master thread

    // For now, we don't have RunAction implemented yet
    // This will be added when we implement hit collection and analysis
}

void ActionInitializer::Build() const
{
    // Actions for worker threads (and sequential mode)

    // Primary generator is mandatory
    SetUserAction(new PrimaryGeneratorAction);

    // Add other actions here as needed:
    // SetUserAction(new RunAction(false)); // false indicates worker thread
    // SetUserAction(new EventAction);
    // SetUserAction(new SteppingAction);
    // SetUserAction(new StackingAction);
    // SetUserAction(new TrackingAction);

    // Note: In multithreaded mode, each worker thread gets its own instance
    // of these actions. Thread-local data should be used for accumulation.
}