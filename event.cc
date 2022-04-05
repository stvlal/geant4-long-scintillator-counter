#include "event.hh"

MyEventAction::MyEventAction(MyRunAction*)
{
}

MyEventAction::~MyEventAction()
{}

void MyEventAction::BeginOfEventAction(const G4Event*)
{
    G4SDManager *SDman = G4SDManager::GetSDMpointer();
}

void MyEventAction::EndOfEventAction(const G4Event*)
{

}
