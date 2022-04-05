#include "pmt.hh"

MySensitiveDetector::MySensitiveDetector(G4String name) :
    G4VSensitiveDetector(name)
{

}

MySensitiveDetector::~MySensitiveDetector()
{

}

G4bool MySensitiveDetector::ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist)
{
    track = aStep->GetTrack();
    //track->SetTrackStatus(fStopAndKill);

    G4double time = aStep->GetPreStepPoint()->GetLocalTime();
    //G4double time = aStep->GetPreStepPoint()->GetGlobalTime();
    //G4double time = aStep->GetPreStepPoint()->GetProperTime();

    G4cout << "Time: " << time << G4endl;

    return true;
}
