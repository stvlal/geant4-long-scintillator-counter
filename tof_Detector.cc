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

    // detect interactive mode (if no arguments) and define UI session
    G4UIExecutive *ui = 0;
    if (argc == 1)
    {
        ui = new G4UIExecutive(argc, argv);
    }

    // construct the visualization manager and initialize it
    G4VisManager *visManager = new G4VisExecutive();
    visManager->Initialize();

    // get the pointer to the User Interface manager
    G4UImanager *UImanager = G4UImanager::GetUIpointer();

    if(ui)
    {
        // interactive mode
        // the commands in the vis.mac file in the . folder can be used to visualize the detector geometry and particle tracks
        // apply the commands written in the vis.mac file
        UImanager->ApplyCommand("/control/execute vis.mac");

        // start User Interface session
        ui->SessionStart();
        delete ui;
    }
    else
    {
        // batch mode
        G4String command = "/control/execute ";
        G4String fileName = argv[1];

        UImanager->ApplyCommand(command + fileName);
    }

    delete visManager;
    delete runManager;

    return 0;
}
