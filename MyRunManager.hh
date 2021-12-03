#ifndef GLASS_SRC_MYRUNMANAGER_HH
#define GLASS_SRC_MYRUNMANAGER_HH

#include "G4RunManager.hh"
#include "action.hh"
#include "G4ScoringManager.hh"
#include "G4TransportationManager.hh"
#include "G4ParallelWorldScoringProcess.hh"

class MyRunManager : public G4RunManager
{
public:
    MyRunManager();
    virtual ~MyRunManager(){};
    void ConstructScoringWorlds() override;
private:
    G4String mySingleParallelWorldName;
};

#endif
