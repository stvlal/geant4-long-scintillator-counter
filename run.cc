#include "run.hh"

MyRunAction::MyRunAction()
{
    // construct analysis manager
    G4AnalysisManager *man = G4AnalysisManager::Instance();

    // create ROOT tree
    man->CreateNtuple("counts", "counts");
    man->CreateNtupleIColumn("fEvent");         // event number; now we have only 1 event with different number of particles

    man->CreateNtupleDColumn("fCounter");       // the number of times when counter registeres particles;
                                                // making it clear: left and right counters have their own ID number (0 and 1);
                                                // when a particle enters the left counter '0' appears and the same thing with
                                                // right counter (but '1' appears); afterwards 0+0+0+..+0 = n*0, where n is the
                                                // number of photons registered by the left counter (1+1+1+..+1 = m*1 for the right
                                                // one). See .root-file for clarification.
    man->FinishNtuple(0);
}

MyRunAction::~MyRunAction()
{}

void MyRunAction::BeginOfRunAction(const G4Run* run)
{
    // random seeds
    long seeds[2];
    time_t systime = time(NULL);
    seeds[0] = (long) systime;
    seeds[1] = (long) (systime*G4UniformRand());
    G4Random::setTheSeeds(seeds);

    // print on the screen the current seed
    G4cout << "Seed: " << G4Random::getTheSeed() << G4endl;

    // construct analysis manager
    G4AnalysisManager *man = G4AnalysisManager::Instance();

    // getting the ID of current run
    G4int runID = run->GetRunID();

    // convert runNumber (integer) to string
    std::stringstream strRunID;
    strRunID << runID;

    // open output file; it will be recreated every time the program run
    man->OpenFile("output_" + strRunID.str() + "_.root");
}

void MyRunAction::EndOfRunAction(const G4Run*)
{
    G4AnalysisManager *man = G4AnalysisManager::Instance();

    man->Write();
    man->CloseFile();
}
