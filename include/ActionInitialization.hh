//=============================================================================
// ActionInitializer.hh - Header file for action initialization
//=============================================================================

#ifndef ActionInitializer_h
#define ActionInitializer_h 1

#include "G4VUserActionInitialization.hh"
#include "globals.hh"

class ActionInitialization : public G4VUserActionInitialization
{
public:
    ActionInitialization();
    virtual ~ActionInitialization();

    virtual void BuildForMaster() const override;
    virtual void Build() const override;
};

#endif