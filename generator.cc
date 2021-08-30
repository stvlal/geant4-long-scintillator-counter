#include "generator.hh"

MyPrimaryGenerator::MyPrimaryGenerator()
{
    // set the number of the initial particles in 1 run
    fParticleGun = new G4ParticleGun(1);            // for e-
    // fParticleGun = new G4ParticleGun(3e6);       // for gamma

    // get the particle for particle gun
    G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
    G4String particleName = "e-";
    //G4String particleName = "gamma";
    G4ParticleDefinition *particle = particleTable->FindParticle(particleName);

    // the initial position of the particle and the direction of its momentum
    G4ThreeVector pos(0.*cm, 0.*cm, 8*cm);          // for e-
    //G4ThreeVector pos(0.*cm, 0.*cm, 1.24*cm);       // for gamma
    G4ThreeVector mom(0., 0., -1.);

    // tune the particle gun
    fParticleGun->SetParticlePosition(pos);
    fParticleGun->SetParticleMomentumDirection(mom);
    fParticleGun->SetParticleMomentum(3*GeV);           // for e-
    //fParticleGun->SetParticleMomentum(3.5*eV);        // for gamma
    fParticleGun->SetParticleDefinition(particle);


}

MyPrimaryGenerator::~MyPrimaryGenerator()
{
    delete fParticleGun;
}

void MyPrimaryGenerator::GeneratePrimaries(G4Event *anEvent)
{
    fParticleGun->GeneratePrimaryVertex(anEvent);
}
