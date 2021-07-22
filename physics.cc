#include "physics.hh"

MyPhysicsList::MyPhysicsList()
{
    // register only electromagnetic and optical processes 
    RegisterPhysics (new G4EmStandardPhysics());
    RegisterPhysics (new G4OpticalPhysics());
}

MyPhysicsList::~MyPhysicsList()
{

}
