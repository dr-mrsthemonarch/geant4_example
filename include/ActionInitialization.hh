//=============================================================================
// ActionInitializer.hh - Header file for action initialization
//=============================================================================

#ifndef ActionInitializer_h
#define ActionInitializer_h 1

#include "G4VUserActionInitialization.hh"
#include "globals.hh"

class ActionInitializer : public G4VUserActionInitialization
{
public:
    ActionInitializer();
    virtual ~ActionInitializer();

    virtual void BuildForMaster() const override;
    virtual void Build() const override;
};

#endif