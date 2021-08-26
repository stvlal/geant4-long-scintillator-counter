#include "construction.hh"

MyDetectorConstruction::MyDetectorConstruction()
{

}

MyDetectorConstruction::~MyDetectorConstruction()
{

}

G4VPhysicalVolume *MyDetectorConstruction::Construct()
{
    // this pointer is used for accessing internal Geant4 database
    G4NistManager *nist = G4NistManager::Instance();

    G4Material *slabMat = new G4Material("slabMat", 1.023*g/cm3, 2);
    slabMat->AddElement(nist->FindOrBuildElement("H"), 10);
    slabMat->AddElement(nist->FindOrBuildElement("C"), 9);

    // set air as the world volume material
    G4Material *worldMat = nist->FindOrBuildMaterial("G4_AIR");


    // ------------ Generate & Add Material Properties Table ------------

    // wavelength obtained from the graph
    std::vector<G4double> wavelength = {
    398.346, 401.674, 403.006, 403.907, 404.730, 405.396, 406.415, 407.629, 408.335, 409.158, 410.294, 411.117, 412.097, 412.724, 413.469, 414.096, 414.645, 415.155, 415.822, 416.410, 417.037, 417.586, 418.253, 418.841, 419.429, 419.939, 420.566, 421.506, 422.368, 423.269, 424.131, 425.462, 427.224, 428.125, 429.064, 430.748, 432.940, 434.584, 435.954, 437.324, 438.968, 440.690, 443.117, 446.013, 448.009, 450.397, 452.081, 454.273, 456.230, 459.519, 462.142, 464.217, 466.135, 467.858, 469.659, 471.498, 473.260, 475.648, 478.232, 479.680, 481.520, 483.556, 486.571, 490.095, 493.188, 496.281, 499.649
    };

    // the vector of optical photon energies
    std::vector<G4double> photonEnergy;

    const G4double plankConst = 4.135667669e-15;
    const G4double lightSpeed = 299792458;

    // filling out the vector of optical photon energies
    for (G4int i = 0; i < wavelength.size(); ++i)
    {
        photonEnergy.push_back(plankConst * lightSpeed * 1e9 * eV / wavelength[i]);
    }

/*
    G4cout << "Points: " << wavelength.size() << G4endl;
    for (G4int i = 0; i < wavelength.size(); ++i)
    {
        G4cout << photonEnergy[i] / eV << "\t" << wavelength[i] << G4endl;
    }
*/

    // the vector down below are the normalized number of photons with a given energy in the fast scintillation component
    std::vector<G4double> scintilFast = {
    0.00959, 0.01637, 0.03138, 0.05050, 0.07098, 0.09352, 0.12493, 0.16318, 0.19528, 0.22533, 0.27177, 0.30866, 0.35647, 0.39063, 0.43503, 0.47465, 0.50812, 0.54569, 0.58736, 0.62493, 0.67002, 0.71442, 0.75404, 0.79708, 0.82850, 0.86676, 0.90365, 0.93984, 0.96921, 0.98355, 0.99036, 0.99376, 0.98758, 0.97936, 0.96705, 0.94242, 0.90548, 0.87744, 0.84804, 0.81795, 0.77146, 0.73453, 0.67709, 0.61828, 0.57998, 0.53962, 0.51705, 0.48899, 0.46641, 0.43356, 0.41370, 0.39111, 0.36580, 0.34254, 0.30903, 0.27620, 0.24952, 0.21941, 0.18999, 0.17698, 0.15918, 0.14070, 0.11673, 0.09822, 0.08040, 0.06737, 0.05433
    };


    // the refractive index for the slab material; it is assumed that there is no dispersion in the medium
    std::vector<G4double> rindexSlab;
    for (G4int i = 0; i < wavelength.size(); ++i)
    {
        rindexSlab.push_back(1.58);
    }


    // the absorption length for the slab material
    std::vector<G4double> absorptionSlab;
    for (G4int i = 0; i < wavelength.size(); ++i)
    {
        absorptionSlab.push_back(380 * cm);
    }

    // construct the material properties table for the slab
    G4MaterialPropertiesTable *mptSlab = new G4MaterialPropertiesTable();

    // add some properties to the slab material
    mptSlab->AddProperty("RINDEX", photonEnergy, rindexSlab)->SetSpline(true);
    mptSlab->AddProperty("ABSLENGTH", photonEnergy, absorptionSlab)->SetSpline(true);
    mptSlab->AddProperty("FASTCOMPONENT", photonEnergy, scintilFast)->SetSpline(true);

    // the number of photons that is emitted per amount of energy absorbed
    mptSlab->AddConstProperty("SCINTILLATIONYIELD", 10000. / MeV);

    // let's just there's RESOLUTIONSCALE = 1;
    mptSlab->AddConstProperty("RESOLUTIONSCALE", 1.0);

    // no one knows what it is
    mptSlab->AddConstProperty("FASTTIMECONSTANT", 1. * ns);

    // the relative strength of the fast component FASTCOMPONENT as a fraction of total scintillation yield
    mptSlab->AddConstProperty("YIELDRATIO", 1.0);

    // apply the properties to the slab material
    slabMat->SetMaterialPropertiesTable(mptSlab);

    // set the Birks constant for the scintillator
    slabMat->GetIonisation()->SetBirksConstant(0.154 * mm / MeV);


    // construct the material properties table for the world volume
    G4MaterialPropertiesTable *mptWorld = new G4MaterialPropertiesTable();

    // air refractive index
    std::vector<G4double> rindexWorld;
    for (G4int i = 0; i < wavelength.size(); ++i)
    {
        rindexWorld.push_back(1.0);
    }

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
