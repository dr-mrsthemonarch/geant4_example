//=============================================================================
// PhysicsList.hh - Header file for physics processes
//=============================================================================

#ifndef PhysicsList_h
#define PhysicsList_h 1

#include "G4VUserPhysicsList.hh"
#include "globals.hh"

class PhysicsList: public G4VUserPhysicsList
{
public:
    PhysicsList();
    virtual ~PhysicsList();

protected:
    // These methods are pure virtual in G4VUserPhysicsList
    virtual void ConstructParticle() override;
    virtual void ConstructProcess() override;
    virtual void SetCuts() override;
};

#endif