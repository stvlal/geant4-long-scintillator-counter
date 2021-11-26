#include "physics.hh"

MyPhysicsList::MyPhysicsList()
{
    // processes registered
    RegisterPhysics (new G4EmStandardPhysics());
    RegisterPhysics (new G4OpticalPhysics());
}

MyPhysicsList::~MyPhysicsList()
{

}
