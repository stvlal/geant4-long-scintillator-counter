#include <iostream>
#include "G4RunManager.hh"
#include "G4UIExecutive.hh"
#include "G4VisManager.hh"
#include "G4VisExecutive.hh"
#include "G4UImanager.hh"
#include "construction.hh"
#include "physics.hh"
#include "action.hh"


int main(int argc, char **argv)
{
    // construct the default run manager
    G4RunManager *runManager = new G4RunManager();

    // set mandatory initialization classes
    runManager->SetUserInitialization(new MyDetectorConstruction());
    runManager->SetUserInitialization(new MyPhysicsList());
    runManager->SetUserInitialization(new MyActionInitialization());

    // initialize G4 kernel
    runManager->Initialize();

    // construct the object of the G4UIExecutive class
    G4UIExecutive *ui = new G4UIExecutive(argc, argv);

    // construct the visualization manager and initialize it
    G4VisManager *visManager = new G4VisExecutive();
    visManager->Initialize();

    // construct User Interface manager and set verbosities
    G4UImanager *UImanager = G4UImanager::GetUIpointer();

    // the commands in the /* */ down below can be used to visualize the detector geometry and particle tracks
    /*
    UImanager->ApplyCommand("/vis/open OGL");
    UImanager->ApplyCommand("/vis/viewer/set/viewpointVector 0.22 0.2 0.5");
    UImanager->ApplyCommand("/vis/viewer/zoom 2.6");
    UImanager->ApplyCommand("/vis/drawVolume");
    UImanager->ApplyCommand("/vis/viewer/set/autoRefresh true");
    UImanager->ApplyCommand("/vis/scene/add/trajectories smooth");
    */

    // run 1 event (if you use GUI you can execute this command there by hitting corresponding button)
    UImanager->ApplyCommand("/run/beamOn 1");

    // start User Interface session
    /* ui->SessionStart(); */

    return 0;
}
