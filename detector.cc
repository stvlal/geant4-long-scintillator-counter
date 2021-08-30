#include "detector.hh"

MySensitiveDetector::MySensitiveDetector(G4String name): G4VSensitiveDetector(name)
{

}

MySensitiveDetector::~MySensitiveDetector()
{

}

G4bool MySensitiveDetector::ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist)
{
    // access the particle track
    G4Track *track = aStep->GetTrack();

    // make the particles not go through the counters
    track->SetTrackStatus(fStopAndKill);

    // the pointer for accessing the particle detector touch
    const G4VTouchable *touchable = aStep->GetPreStepPoint()->GetTouchable();

    // counterNo is 0 or 1 depending on what counter worked
    G4int counterNo = touchable->GetCopyNumber();

    // get event ID for event counting
    G4int event = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();

    // construct analysis manager and fill ROOT tree with event and the number of times when counters register particles
    G4AnalysisManager *man = G4AnalysisManager::Instance();
    man->FillNtupleIColumn(0, event);
    man->FillNtupleDColumn(1, counterNo);
    man->AddNtupleRow(0);

    return true;
}
