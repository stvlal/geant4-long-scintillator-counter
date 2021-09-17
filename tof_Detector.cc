#include <iostream>
#include "G4RunManager.hh"
#include "G4UIExecutive.hh"
#include "G4VisManager.hh"
#include "G4VisExecutive.hh"
#include "G4UImanager.hh"
#include "construction.hh"
#include "physics.hh"
#include "action.hh"
#include "G4ScoringManager.hh"


int main(int argc, char **argv)
{
    // construct the default run manager
    G4RunManager *runManager = new G4RunManager();

    // Activate UI-command base scorer
    G4ScoringManager * scManager = G4ScoringManager::GetScoringManager();
    scManager->SetVerboseLevel(1);

    MyDetectorConstruction *myDetectorConstruction = new MyDetectorConstruction();

    // detect interactive mode and define UI session
    G4UIExecutive *ui = 0;
    if (argc == 1)
    {
        G4cout << "Usage: " << G4endl;
        G4cout << "./tof_Detector run.mac light_guide.stl, or" << G4endl;
        G4cout << "./tof_Detector light_guide.stl" << G4endl;
        return 0;
    }
    else if (argc == 2)
    {
        ui = new G4UIExecutive(argc, argv);
        myDetectorConstruction->SetCADFilename(argv[1]);
    }

    // set mandatory initialization classes
    runManager->SetUserInitialization(myDetectorConstruction);
    runManager->SetUserInitialization(new MyPhysicsList());
    runManager->SetUserInitialization(new MyActionInitialization());

    // initialize G4 kernel
    runManager->Initialize();


    // random seeds
    G4Random::setTheEngine(new CLHEP::RanecuEngine());
    G4long seed = time(NULL);
    G4Random::setTheSeed(seed);


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
        myDetectorConstruction->SetCADFilename(argv[2]);
        G4String command = "/control/execute ";
        G4String fileName = argv[1];

        UImanager->ApplyCommand(command + fileName);
    }

    delete visManager;
    delete runManager;

    return 0;
}
