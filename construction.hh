#ifndef CONSTRUCTION_HH
#define CONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Box.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"
#include "G4LogicalVolume.hh"
#include "G4SystemOfUnits.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "CADMesh.hh"
#include <string>
#include "G4PhysicalConstants.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4Tubs.hh"
#include "G4LogicalSkinSurface.hh"

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
    std::string filename;

    G4NistManager *nist;

    G4Material *slabMat;
    G4Material *worldMat;
    G4Material *lgMat;

    G4MaterialPropertiesTable *mptSlab;
    G4MaterialPropertiesTable *mptWorld;
    G4MaterialPropertiesTable *mptLG;

    G4Box *solidWorld;
    G4LogicalVolume *logicWorld;
    G4VPhysicalVolume *physWorld;

    G4VisAttributes *slabVisAtt;
    G4Box *solidSlab;
    G4LogicalVolume *logicSlab;
    G4VPhysicalVolume *physSlab;

    G4VisAttributes *lgVisAtt;
    CADMesh *mesh;
    G4RotationMatrix *rot1;
    G4RotationMatrix *rot2;

    G4VSolid *solidLG_1;
    G4LogicalVolume *logicLG_1;
    G4VPhysicalVolume *physLG_1;

    G4VSolid *solidLG_2;
    G4LogicalVolume *logicLG_2;
    G4VPhysicalVolume *physLG_2;



    G4OpticalSurface *opSlabAirSurface;
    G4LogicalBorderSurface *slabAirSurface;

    G4OpticalSurface *opLgAirSurface;
    G4LogicalBorderSurface *lgAirSurface_1;
    G4LogicalBorderSurface *lgAirSurface_2;

    G4OpticalSurface *opSlabLgSurface;
    G4LogicalBorderSurface *slabLgSurface_1;
    G4LogicalBorderSurface *slabLgSurface_2;

};
#endif
