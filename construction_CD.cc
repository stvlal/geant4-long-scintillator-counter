#include "construction.hh"

MyDetectorConstruction::MyDetectorConstruction()
{
    filename = "";
}

MyDetectorConstruction::~MyDetectorConstruction()
{
    delete nist;

    delete slabMat;
    delete worldMat;
    delete lgMat;

    delete mptSlab;
    delete mptWorld;
    delete mptLG;

    delete solidWorld;
    delete logicWorld;
    delete physWorld;

    delete solidSlab;
    delete logicSlab;
    delete physSlab;

    delete mesh;
    delete rot1;
    delete rot2;

    delete solidLG_1;
    delete logicLG_1;
    delete physLG_1;

    delete solidLG_2;
    delete logicLG_2;
    delete physLG_2;

    delete opSlabAirSurface;
    delete slabAirSurface;

    delete opLgAirSurface;
    delete lgAirSurface_1;
    delete lgAirSurface_2;

    delete opSlabLgSurface;
    delete slabLgSurface_1;
    delete slabLgSurface_2;
}
