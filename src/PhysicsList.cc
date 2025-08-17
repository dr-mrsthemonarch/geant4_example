//=============================================================================
// PhysicsList.cc - Implementation of physics processes
//=============================================================================

#include "PhysicsList.hh"

#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"
#include "G4ProcessManager.hh"

// Particles
#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4OpticalPhoton.hh"

// Electromagnetic processes
#include "G4PhotoElectricEffect.hh"
#include "G4ComptonScattering.hh"
#include "G4GammaConversion.hh"
#include "G4eIonisation.hh"
#include "G4eBremsstrahlung.hh"
#include "G4eplusAnnihilation.hh"

// Optical processes
#include "G4Scintillation.hh"
#include "G4OpAbsorption.hh"
#include "G4OpRayleigh.hh"
#include "G4OpBoundaryProcess.hh"
#include "G4Cerenkov.hh" // optional if you want Cherenkov too

PhysicsList::PhysicsList()
: G4VUserPhysicsList()
{
    defaultCutValue = 0.1*CLHEP::mm;
}

PhysicsList::~PhysicsList()
{
}

void PhysicsList::ConstructParticle()
{
    // Gamma, electron, positron
    G4Gamma::GammaDefinition();
    G4Electron::ElectronDefinition();
    G4Positron::PositronDefinition();

    // Optical photon
    G4OpticalPhoton::OpticalPhotonDefinition();
}

void PhysicsList::ConstructProcess()
{
    AddTransportation();

    // ---- Electromagnetic processes ----
    // Gamma
    G4ProcessManager* pManager = G4Gamma::Gamma()->GetProcessManager();
    pManager->AddDiscreteProcess(new G4PhotoElectricEffect);
    pManager->AddDiscreteProcess(new G4ComptonScattering);
    pManager->AddDiscreteProcess(new G4GammaConversion);

    // Electron
    pManager = G4Electron::Electron()->GetProcessManager();
    pManager->AddProcess(new G4eIonisation,      -1, 2, 2);
    pManager->AddProcess(new G4eBremsstrahlung, -1, 3, 3);

    // Positron
    pManager = G4Positron::Positron()->GetProcessManager();
    pManager->AddProcess(new G4eIonisation,      -1, 2, 2);
    pManager->AddProcess(new G4eBremsstrahlung, -1, 3, 3);
    pManager->AddProcess(new G4eplusAnnihilation, 0,-1, 4);

    // ---- Optical processes ----
    G4Scintillation* scint = new G4Scintillation("Scintillation");
    scint->SetTrackSecondariesFirst(true);


    G4OpAbsorption* absorption = new G4OpAbsorption();
    G4OpRayleigh*   rayleigh   = new G4OpRayleigh();
    G4OpBoundaryProcess* boundary = new G4OpBoundaryProcess();

    // Attach optical processes to optical photons
    pManager = G4OpticalPhoton::OpticalPhoton()->GetProcessManager();
    pManager->AddDiscreteProcess(absorption);
    pManager->AddDiscreteProcess(rayleigh);
    pManager->AddDiscreteProcess(boundary);

    // Register scintillation for all charged particles
    auto particleIterator=GetParticleIterator();
    particleIterator->reset();
    while( (*particleIterator)() ){
        G4ParticleDefinition* particle = particleIterator->value();
        G4ProcessManager* pmanager = particle->GetProcessManager();
        if (scint->IsApplicable(*particle)) {
            pmanager->AddProcess(scint);
            pmanager->SetProcessOrderingToLast(scint, idxAtRest);
            pmanager->SetProcessOrderingToLast(scint, idxPostStep);
        }
    }
}

void PhysicsList::SetCuts()
{
    SetCutsWithDefault();
}
