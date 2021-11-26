#ifndef TOF_DETECTOR
#define TOF_DETECTOR

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

G4RunManager *runManager;
G4ScoringManager *scManager;
MyDetectorConstruction *myDetectorConstruction;
G4UIExecutive *ui;
G4VisManager *visManager;
G4UImanager *UImanager;

#endif
