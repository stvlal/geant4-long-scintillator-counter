#ifndef PMT_HH
#define PMT_HH

#include "G4VSensitiveDetector.hh"

class MySensitiveDetector : public G4VSensitiveDetector
{
public:
    MySensitiveDetector(G4String);
    ~MySensitiveDetector();

private:
    G4Track *track;
    virtual G4bool ProcessHits(G4Step *, G4TouchableHistory *);
};

#endif
