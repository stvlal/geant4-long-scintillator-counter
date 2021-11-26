#include "run.hh"

MyRunAction::MyRunAction()
{
}

MyRunAction::~MyRunAction()
{}

void MyRunAction::BeginOfRunAction(const G4Run *)
{
    // random seeds
    long seeds[2];
    time_t systime = time(NULL);
    seeds[0] = (long) systime;
    seeds[1] = (long) (systime*G4UniformRand());
    G4Random::setTheSeeds(seeds);

    // print on the screen the current seed
    G4cout << "Seed: " << G4Random::getTheSeed() << G4endl;

}

void MyRunAction::EndOfRunAction(const G4Run *)
{
    // write scorers output into the files
    G4UImanager *UImanager = G4UImanager::GetUIpointer();
    UImanager->ApplyCommand("/control/execute scoring_end.mac");
    delete UImanager;
}
