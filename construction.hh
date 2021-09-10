#ifndef CONSTRUCTION_HH
#define CONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Box.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"
#include "G4LogicalVolume.hh"
#include "G4SystemOfUnits.hh"
#include "detector.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "CADMesh.hh"
#include <string>
#include "G4PhysicalConstants.hh"

class MyDetectorConstruction: public G4VUserDetectorConstruction
{
public:
    MyDetectorConstruction();
    ~MyDetectorConstruction();

    virtual G4VPhysicalVolume *Construct();

    void SetCADFilename(std::string name)
    {
        filename = name;
    };

private:
    G4LogicalVolume *logicDetector;
    G4LogicalVolume *logicInnerDetector;
    virtual void ConstructSDandField();
    std::string filename;
};
#endif
