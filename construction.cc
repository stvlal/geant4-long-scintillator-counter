#include "construction.hh"

MyDetectorConstruction::MyDetectorConstruction()
{

}

MyDetectorConstruction::~MyDetectorConstruction()
{

}

G4VPhysicalVolume *MyDetectorConstruction::Construct()
{
    // define the density of the material according to the datasheet
    G4double density;
    density = 1.023*g/cm3;

    // this pointer is used for accessing internal Geant4 database
    G4NistManager *nist = G4NistManager::Instance();

    // set PolyVinylToluene as polymer base with the datasheet density; it is scintillator material
    G4Material *slabMat = nist->BuildMaterialWithNewDensity("PolyVinylToluene","G4_PLASTIC_SC_VINYLTOLUENE",density);

    // set air as the world volume material
    G4Material *worldMat = nist->FindOrBuildMaterial("G4_AIR");


    // ------------ Generate & Add Material Properties Table ------------

    // the vector of optical photon energies (were taken from GEAT4 example: OpNovice)
    std::vector<G4double> photonEnergy = {
    2.034 * eV, 2.068 * eV, 2.103 * eV, 2.139 * eV, 2.177 * eV, 2.216 * eV,
    2.256 * eV, 2.298 * eV, 2.341 * eV, 2.386 * eV, 2.433 * eV, 2.481 * eV,
    2.532 * eV, 2.585 * eV, 2.640 * eV, 2.697 * eV, 2.757 * eV, 2.820 * eV,
    2.885 * eV, 2.954 * eV, 3.026 * eV, 3.102 * eV, 3.181 * eV, 3.265 * eV,
    3.353 * eV, 3.446 * eV, 3.545 * eV, 3.649 * eV, 3.760 * eV, 3.877 * eV,
    4.002 * eV, 4.136 * eV
    };

    // 2 vectors down below are the normalized number of photons with a given energy in the fast and slow scintillation components, respectively; (were taken from GEAT4 example: OpNovice; not clear how to get them for EJ-200)
    std::vector<G4double> scintilFast = {
    1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
    1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
    1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00
    }; // water

    std::vector<G4double> scintilSlow = {
    0.01, 1.00, 2.00, 3.00, 4.00, 5.00, 6.00, 7.00, 8.00, 9.00, 8.00,
    7.00, 6.00, 4.00, 3.00, 2.00, 1.00, 0.01, 1.00, 2.00, 3.00, 4.00,
    5.00, 6.00, 7.00, 8.00, 9.00, 8.00, 7.00, 6.00, 5.00, 4.00
    }; // water

    // the refractive index for the slab material; it is assumed that there is no dispersion in the medium
    std::vector<G4double> rindexSlab = {
    1.58, 1.58, 1.58, 1.58, 1.58, 1.58, 1.58, 1.58, 1.58, 1.58, 1.58,
    1.58, 1.58, 1.58, 1.58, 1.58, 1.58, 1.58, 1.58, 1.58, 1.58, 1.58,
    1.58, 1.58, 1.58, 1.58, 1.58, 1.58, 1.58, 1.58, 1.58, 1.58
    }; // EJ-200

    // the absorption length for the slab material
    std::vector<G4double> absorptionSlab = {
    380 * cm, 380 * cm, 380 * cm, 380 * cm, 380 * cm, 380 * cm,
    380 * cm, 380 * cm, 380 * cm, 380 * cm, 380 * cm, 380 * cm,
    380 * cm, 380 * cm, 380 * cm, 380 * cm, 380 * cm, 380 * cm,
    380 * cm, 380 * cm, 380 * cm, 380 * cm, 380 * cm, 380 * cm,
    380 * cm, 380 * cm, 380 * cm, 380 * cm, 380 * cm, 380 * cm,
    380 * cm, 380 * cm
    }; // EJ-200

    // construct the material properties table for the slab
    G4MaterialPropertiesTable *mptSlab = new G4MaterialPropertiesTable();

    // add some properties to the slab material
    mptSlab->AddProperty("RINDEX", photonEnergy, rindexSlab)->SetSpline(true);
    mptSlab->AddProperty("ABSLENGTH", photonEnergy, absorptionSlab)->SetSpline(true);
    mptSlab->AddProperty("FASTCOMPONENT", photonEnergy, scintilFast)->SetSpline(true);
    mptSlab->AddProperty("SLOWCOMPONENT", photonEnergy, scintilSlow)->SetSpline(true);

    // the number of photons that is emitted per amount of energy absorbed
    mptSlab->AddConstProperty("SCINTILLATIONYIELD", 10000 / MeV);

    // not clear how to calculate RESOLUTIONSCALE; let's just there's RESOLUTIONSCALE = 1;
    // some information about it can be found at https://opengate.readthedocs.io/en/latest/generating_and_tracking_optical_photons.html#scintillation
    mptSlab->AddConstProperty("RESOLUTIONSCALE", 1.0);

    // raise time
    mptSlab->AddConstProperty("FASTTIMECONSTANT", 0.9 * ns);

    // decay time
    mptSlab->AddConstProperty("SLOWTIMECONSTANT", 2.1 * ns);

    // the relative strength of the fast component FASTCOMPONENT as a fraction of total scintillation yield
    // not clear how to calculate
    mptSlab->AddConstProperty("YIELDRATIO", 0.8);

    // apply the properties to the slab material
    slabMat->SetMaterialPropertiesTable(mptSlab);


    // construct the material properties table for the world volume
    G4MaterialPropertiesTable *mptWorld = new G4MaterialPropertiesTable();

    // air refractive index
    std::vector<G4double> rindexWorld = {
    1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
    1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
    1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00
    };

    // set the refractive index for the air
    mptWorld->AddProperty("RINDEX", photonEnergy, rindexWorld);

    // apply the properties to the world material (air)
    worldMat->SetMaterialPropertiesTable(mptWorld);


    // ------------- Volumes --------------

    // set the sizes of the world volume
    G4Box *solidWorld = new G4Box("solidWorld", 80*cm, 15*cm, 11.25*cm);

    // create a logical world volume
    G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");

    // create a physical world volume
    G4VPhysicalVolume *physWorld = new G4PVPlacement(0, G4ThreeVector(0.,0.,0.), logicWorld, "physWorld", 0, false, 0, true);


    // set the sizes of the slab volume
    G4Box *solidSlab = new G4Box("slab", 70*cm, 5*cm, 1.25*cm);

    // create a logical slab volume
    G4LogicalVolume *logicSlab = new G4LogicalVolume(solidSlab, slabMat, "logicSlab");

    // create a physical slab volume
    G4VPhysicalVolume *physSlab = new G4PVPlacement(0, G4ThreeVector(0.,0.,0.), logicSlab, "physSlab", logicWorld, false, 0, true);


    // ------------- Surfaces --------------

    // construct optical slab surface
    G4OpticalSurface *opSlabSurface = new G4OpticalSurface("opSlabSurface");
    opSlabSurface->SetType(dielectric_dielectric);       // set the type of interface
    opSlabSurface->SetFinish(ground);                    // set the surface finish
    opSlabSurface->SetModel(glisur);                     // set the simulation model used by the boundary process

    // create a logical border surface
    G4LogicalBorderSurface *slabSurface = new G4LogicalBorderSurface("slabSurface", physSlab, physWorld, opSlabSurface);


    // Generate & Add Material Properties Table attached to the optical surfaces

    // optical photon energies
    std::vector<G4double> ephoton = { 2.034 * eV, 4.136 * eV };

    // optical slab surface properties
    std::vector<G4double> reflectivity = { 0.3, 0.5 };   // how to get this parameter for EJ-200
    std::vector<G4double> efficiency   = { 0.8, 1.0 };   // how to get this parameter for EJ-200

    // construct the material properties table for the optical surface
    G4MaterialPropertiesTable *mptSlabSurface = new G4MaterialPropertiesTable();

    // add some properties to the optical surface
    mptSlabSurface->AddProperty("REFLECTIVITY", ephoton, reflectivity);
    mptSlabSurface->AddProperty("EFFICIENCY", ephoton, efficiency);

    // apply the properties to the optical surface
    opSlabSurface->SetMaterialPropertiesTable(mptSlabSurface);


    // ------------- Counters at the ends of the slab --------------
    // hereafter counter = a sensitive detector at the end of the slab (something like PMT)

    // set the sizes of the counter volume
    G4Box *solidDetector = new G4Box("solidDetector", 0.5*cm, 5*cm, 1.25*cm);

    // create a logical counter volume
    logicDetector = new G4LogicalVolume(solidDetector, worldMat, "logicDetector");

    // create a physical counters volumes; one of them has index '0', another one has index '1'
    for (G4int i = 0; i < 2; i++)
    {
        G4VPhysicalVolume *physDetector = new G4PVPlacement(0, G4ThreeVector(70.6*cm - 2*i*70.6*cm, 0., 0.), logicDetector, "physDetector", logicWorld, false, i, true);
    }

    return physWorld;
}

// the function for sensitive detector construction
void MyDetectorConstruction::ConstructSDandField()
{
    MySensitiveDetector *sensDet = new MySensitiveDetector("sensitiveDetector");

    logicDetector->SetSensitiveDetector(sensDet);
}
