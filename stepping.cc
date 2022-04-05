#include "stepping.hh"

MySteppingAction::MySteppingAction(MyEventAction *eventAction)
{
    fEventAction = eventAction;
}

MySteppingAction::~MySteppingAction()
{}

void MySteppingAction::UserSteppingAction(G4Step *step)
{
    // get the current volume for the given step
    G4LogicalVolume *volume = step->GetPreStepPoint()
    ->GetTouchableHandle()->GetVolume()->GetLogicalVolume();

    // get a pointer to my own detector construction
    const MyDetectorConstruction *detectorConstruction =
    static_cast<const MyDetectorConstruction*>
    (G4RunManager::GetRunManager()->GetUserDetectorConstruction());

    // get the actual detector volume (logical photocathode)
    G4LogicalVolume *fScoringVolume = detectorConstruction->GetScoringVolume();

    G4cout << "HEY, WHATS UP? 0_0" << G4endl;

    // do nothing if the photon is not in the photocathode volume
    if(volume != fScoringVolume)
        return;

    // execute ProcessHits method otherwise
    G4SDManager *SDman = G4SDManager::GetSDMpointer();
    MySensitiveDetector *pmtSD = (MySensitiveDetector*) SDman->FindSensitiveDetector("pmt");
    G4cout << "HEY, WHATS UP?" << G4endl;
    pmtSD->ProcessHits(step, nullptr);
}
