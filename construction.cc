#include "construction.hh"

MyDetectorConstruction::MyDetectorConstruction()
{
    filename = "";
}
MyDetectorConstruction::~MyDetectorConstruction()
{
}

G4VPhysicalVolume *MyDetectorConstruction::Construct()
{
    // ------------ Materials ------------

    // this pointer is used for accessing internal Geant4 database
    nist = G4NistManager::Instance();

    // compose the slab material
    slabMat = new G4Material("slabMat", 1.023*g/cm3, 2);
    slabMat->AddElement(nist->FindOrBuildElement("H"), 10);
    slabMat->AddElement(nist->FindOrBuildElement("C"), 9);

    // set air as the world volume material
    worldMat = nist->FindOrBuildMaterial("G4_AIR");

    // set plexiglass as the light guide volume material
    lgMat = nist->FindOrBuildMaterial("G4_PLEXIGLASS");

    G4Material *pmtMat = nist->FindOrBuildMaterial("G4_GLASS_PLATE");


    G4Material *photocathMat = new G4Material("photocathMat", 2.6989*g/cm3, 1);
    photocathMat->AddElement(nist->FindOrBuildElement("Al"), 1);


    // ------------ Generate & Add Material Properties Table ------------

    //////// slab material ////////

    // wavelength obtained from the graph
    std::vector<G4double> wavelength = {
    398.346, 401.674, 403.006, 403.907, 404.730, 405.396, 406.415, 407.629,
    408.335, 409.158, 410.294, 411.117, 412.097, 412.724, 413.469, 414.096,
    414.645, 415.155, 415.822, 416.410, 417.037, 417.586, 418.253, 418.841,
    419.429, 419.939, 420.566, 421.506, 422.368, 423.269, 424.131, 425.462,
    427.224, 428.125, 429.064, 430.748, 432.940, 434.584, 435.954, 437.324,
    438.968, 440.690, 443.117, 446.013, 448.009, 450.397, 452.081, 454.273,
    456.230, 459.519, 462.142, 464.217, 466.135, 467.858, 469.659, 471.498,
    473.260, 475.648, 478.232, 479.680, 481.520, 483.556, 486.571, 490.095,
                                                 493.188, 496.281, 499.649
    };

    // the vector of optical photon energies
    std::vector<G4double> photonEnergy;

    const G4double plankConst = 4.135667669e-15;
    const G4double lightSpeed = 299792458;

    // filling out the vector of optical photon energies
    for (size_t i = 0; i < wavelength.size(); ++i)
    {
        photonEnergy.push_back(plankConst * lightSpeed * 1e9 * eV /
                                                            wavelength[i]);
    }


    // the vector down below is the normalized number of photons with
    // a given energy in the fast scintillation component
    std::vector<G4double> scintilFast = {
    0.00959, 0.01637, 0.03138, 0.05050, 0.07098, 0.09352, 0.12493, 0.16318,
    0.19528, 0.22533, 0.27177, 0.30866, 0.35647, 0.39063, 0.43503, 0.47465,
    0.50812, 0.54569, 0.58736, 0.62493, 0.67002, 0.71442, 0.75404, 0.79708,
    0.82850, 0.86676, 0.90365, 0.93984, 0.96921, 0.98355, 0.99036, 0.99376,
    0.98758, 0.97936, 0.96705, 0.94242, 0.90548, 0.87744, 0.84804, 0.81795,
    0.77146, 0.73453, 0.67709, 0.61828, 0.57998, 0.53962, 0.51705, 0.48899,
    0.46641, 0.43356, 0.41370, 0.39111, 0.36580, 0.34254, 0.30903, 0.27620,
    0.24952, 0.21941, 0.18999, 0.17698, 0.15918, 0.14070, 0.11673, 0.09822,
                                                 0.08040, 0.06737, 0.05433
    };


    // the refractive index for the slab material;
    //it is assumed that there is no dispersion in the medium
    std::vector<G4double> rindexSlab;
    for (size_t i = 0; i < wavelength.size(); ++i)
    {
        rindexSlab.push_back(1.58);
    }


    // the absorption length for the slab material
    std::vector<G4double> absorptionSlab;
    for (size_t i = 0; i < wavelength.size(); ++i)
    {
        absorptionSlab.push_back(380 * cm);
    }

    // construct the material properties table for the slab
    mptSlab = new G4MaterialPropertiesTable();

    // add some properties to the slab material
    mptSlab->AddProperty("RINDEX", photonEnergy, rindexSlab)
                                        ->SetSpline(true);
    mptSlab->AddProperty("ABSLENGTH", photonEnergy, absorptionSlab)
                                        ->SetSpline(true);
    mptSlab->AddProperty("FASTCOMPONENT", photonEnergy, scintilFast)
                                        ->SetSpline(true);

    // the number of photons that is emitted per amount of energy absorbed
    mptSlab->AddConstProperty("SCINTILLATIONYIELD", 10000. / MeV);

    // let's just there's RESOLUTIONSCALE = 1;
    mptSlab->AddConstProperty("RESOLUTIONSCALE", 1.0);

    // no one knows what it is
    mptSlab->AddConstProperty("FASTTIMECONSTANT", 1. * ns);

    // the relative strength of the fast component FASTCOMPONENT
    // as a fraction of total scintillation yield
    mptSlab->AddConstProperty("YIELDRATIO", 1.0);

    // apply the properties to the slab material
    slabMat->SetMaterialPropertiesTable(mptSlab);

    // set the Birks constant for the scintillator
    slabMat->GetIonisation()->SetBirksConstant(0.154 * mm / MeV);


    //////// world material ////////

    // construct the material properties table for the world volume
    mptWorld = new G4MaterialPropertiesTable();

    // air refractive index
    std::vector<G4double> rindexWorld;
    for (size_t i = 0; i < wavelength.size(); ++i)
    {
        rindexWorld.push_back(1.0);
    }

    // set the refractive index for the air
    mptWorld->AddProperty("RINDEX", photonEnergy, rindexWorld);

    // apply the properties to the world material (air)
    worldMat->SetMaterialPropertiesTable(mptWorld);


    //////// light guide material ////////

    // construct the material properties table for the light guide volume
    mptLG = new G4MaterialPropertiesTable();

    // plexiglass refractive index
    std::vector<G4double> rindexLG;
    for (size_t i = 0; i < wavelength.size(); ++i)
    {
        rindexLG.push_back(1.49);
    }

    // the absorption length for the light guide material
    std::vector<G4double> absorptionLG;
    for (size_t i = 0; i < wavelength.size(); ++i)
    {
        absorptionLG.push_back(380 * cm);
    }

    // add some properties to the light guide material
    mptLG->AddProperty("RINDEX", photonEnergy, rindexLG);
    mptLG->AddProperty("ABSLENGTH", photonEnergy, absorptionLG);

    // apply the properties to the light guide material (plexiglass)
    lgMat->SetMaterialPropertiesTable(mptLG);


    // ------------- Volumes --------------

    // set the sizes of the world volume
    solidWorld = new G4Box("solidWorld", 150*cm, 30*cm, 25*cm);

    // create a logical world volume
    logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");
    logicWorld->SetVisAttributes(G4VisAttributes::Invisible);

    // create a physical world volume
    physWorld = new G4PVPlacement(0, G4ThreeVector(0.,0.,0.),
                 logicWorld, "physWorld", 0, false, 0, true);


    // color for the slab (blue)
    slabVisAtt = new G4VisAttributes(G4Colour(0.3,0.3,0.3));
    slabVisAtt->SetForceSolid(true);
    slabVisAtt->SetVisibility(true);

    // set the sizes of the slab volume
    solidSlab = new G4Box("slab", 70*cm, 5*cm, 1.25*cm);

    // create a logical slab volume
    logicSlab = new G4LogicalVolume(solidSlab, slabMat, "logicSlab");
    logicSlab->SetVisAttributes(slabVisAtt);

    // create a physical slab volume
    physSlab = new G4PVPlacement(0, G4ThreeVector(0.,0.,0.),
         logicSlab, "physSlab", logicWorld, false, 0, true);


    // color for the light guides (brown)
    lgVisAtt = new G4VisAttributes(G4Colour(0.5,0.3,0.2));
    lgVisAtt->SetForceSolid(true);
    lgVisAtt->SetVisibility(true);

    // getting the CAD model (some_filename.stl)
    mesh = new CADMesh((char*) filename.c_str());
    mesh->SetScale(mm);
    mesh->SetReverse(false);

    // CAD model 1 rotation
    rot1 = new G4RotationMatrix();
    rot1->rotateZ(90*deg);

    // CAD model 2 rotation
    rot2 = new G4RotationMatrix();
    rot2->rotateZ(-90*deg);

    // creating the first light guide
    solidLG_1 = mesh->TessellatedMesh();
    logicLG_1 = new G4LogicalVolume(solidLG_1, lgMat, "logicLG_1", 0, 0, 0);
    logicLG_1->SetVisAttributes(lgVisAtt);
    physLG_1 = new G4PVPlacement(rot1, G4ThreeVector(-70*cm, 0.,0.),
                 logicLG_1, "physLG_1", logicWorld, false, 0, true);

    // creating the second light guide
    solidLG_2 = mesh->TessellatedMesh();
    logicLG_2 = new G4LogicalVolume(solidLG_2, lgMat, "logicLG_2", 0, 0, 0);
    logicLG_2->SetVisAttributes(lgVisAtt);
    physLG_2 = new G4PVPlacement(rot2, G4ThreeVector(70*cm, 0.,0.),
                logicLG_2, "physLG_2", logicWorld, false, 0, true);


    // ------------- PMTs --------------

    // color for the PMTs ()
    G4VisAttributes *pmtVisAtt = new G4VisAttributes(G4Colour(0.7,0.8,0.7));
    pmtVisAtt->SetForceSolid(true);
    pmtVisAtt->SetVisibility(true);

    // creating PMTs form
    G4double innerRadius_pmt = 0.;
    G4double outerRadius_pmt = 23. * mm;
    G4double height_pmt = 5. * mm;
    G4double startAngle_pmt = 0.;
    G4double spanningAngle_pmt = 360. * deg;

    G4RotationMatrix *rotPMT = new G4RotationMatrix();
    rotPMT->rotateY(90*deg);

    G4Tubs *solidPMT = new G4Tubs("solidPMT", innerRadius_pmt, outerRadius_pmt,
                                height_pmt, startAngle_pmt, spanningAngle_pmt);

    G4Tubs *solidPhotocath = new G4Tubs("solidPhotocath", innerRadius_pmt,
      outerRadius_pmt,height_pmt / 2., startAngle_pmt, spanningAngle_pmt);


    G4LogicalVolume *logicPMT_1 = new G4LogicalVolume(solidPMT,
                                         pmtMat, "logicPMT_1");

    G4LogicalVolume *logicPMT_2 = new G4LogicalVolume(solidPMT,
                                         pmtMat, "logicPMT_2");

    logicPMT_1->SetVisAttributes(pmtVisAtt);
    logicPMT_2->SetVisAttributes(pmtVisAtt);

    G4LogicalVolume *logicPhotocath = new G4LogicalVolume(solidPhotocath,
                                         photocathMat, "logicPhotocath");


    G4VPhysicalVolume *physPhotocath_1 = new G4PVPlacement(0,
      G4ThreeVector(0., 0., height_pmt / 2.),logicPhotocath,"physPhotocath_1",
                                                   logicPMT_1, false, 0, true);

    G4VPhysicalVolume *physPhotocath_2 = new G4PVPlacement(0,
      G4ThreeVector(0., 0.,  -height_pmt / 2.),logicPhotocath,"physPhotocath_2",
                                                   logicPMT_2, false, 0, true);


    G4VPhysicalVolume *physPMT_1 = new G4PVPlacement(rotPMT,
      G4ThreeVector(-88.5*cm,0.,0.), logicPMT_1, "physPMT_1",
                                 logicWorld, false, 0, true);
    G4VPhysicalVolume *physPMT_2 = new G4PVPlacement(rotPMT,
      G4ThreeVector(88.5*cm,0.,0.),logicPMT_2, "physPMT_2",
                               logicWorld, false, 0, true);


    // Photocathode surface properties

    std::vector<G4double> photocath_EFF;
    for (size_t i = 0; i < wavelength.size(); ++i)
    {
        photocath_EFF.push_back(1.);
    }

    std::vector<G4double> photocath_ReR;
    for (size_t i = 0; i < wavelength.size(); ++i)
    {
        photocath_ReR.push_back(1.92);
    }

    std::vector<G4double> photocath_ImR;
    for (size_t i = 0; i < wavelength.size(); ++i)
    {
        photocath_ImR.push_back(1.69);
    }

    G4MaterialPropertiesTable *mptPhotocath = new G4MaterialPropertiesTable();
    mptPhotocath->AddProperty("EFFICIENCY", photonEnergy, photocath_EFF);
    mptPhotocath->AddProperty("REALRINDEX", photonEnergy, photocath_ReR);
    mptPhotocath->AddProperty("IMAGINARYRINDEX", photonEnergy, photocath_ImR);

    G4OpticalSurface *opPhotocathSurface = new G4OpticalSurface(
                    "opPhotocathSurface", glisur, polished, dielectric_metal);
    opPhotocathSurface->SetMaterialPropertiesTable(mptPhotocath);


    // Create logical skin surface
    new G4LogicalSkinSurface("photocathSurface", logicPhotocath,
                                            opPhotocathSurface);


    // ------------- Surfaces --------------

    //////// slab - air surface ////////

    // construct slab - air optical surface
    opSlabAirSurface = new G4OpticalSurface("opSlabAirSurface");
    // set the type of interface
    opSlabAirSurface->SetType(dielectric_dielectric);
    // set the simulation model used by the boundary process
    opSlabAirSurface->SetModel(glisur);
    // set the surface finish
    opSlabAirSurface->SetFinish(polished);


    // create a logical border surface
    slabAirSurface = new G4LogicalBorderSurface("slabAirSurface",
                          physSlab, physWorld, opSlabAirSurface);


    //////// light guides - air surface ////////

    // construct light guide - air optical surface
    opLgAirSurface = new G4OpticalSurface("opLgAirSurface");
    // set the type of interface
    opLgAirSurface->SetType(dielectric_dielectric);
    // set the simulation model used by the boundary process
    opLgAirSurface->SetModel(glisur);
    // set the surface finish
    opLgAirSurface->SetFinish(polished);


    // create a logical border surface
    lgAirSurface_1 = new G4LogicalBorderSurface("lgAirSurface_1",
                            physLG_1, physWorld, opLgAirSurface);
    lgAirSurface_2 = new G4LogicalBorderSurface("lgAirSurface_2",
                            physLG_2, physWorld, opLgAirSurface);


    //////// slab - light guides surface ////////

    // construct slab - light guides optical surface
    opSlabLgSurface = new G4OpticalSurface("opSlabLgSurface");
    // set the type of interface
    opSlabLgSurface->SetType(dielectric_dielectric);
    // set the simulation model used by the boundary process
    opSlabLgSurface->SetModel(glisur);
    // set the surface finish
    opSlabLgSurface->SetFinish(polished);


    // create a logical border surface
    slabLgSurface_1 = new G4LogicalBorderSurface("slabLgSurface_1",
                              physSlab, physLG_1, opSlabLgSurface);
    slabLgSurface_2 = new G4LogicalBorderSurface("slabLgSurface_2",
                              physSlab, physLG_2, opSlabLgSurface);


/*    // Generate & Add Material Properties Table attached to the optical surfaces

    //////// slab surface ////////

    // optical photon energies
    //std::vector<G4double> ephoton = { 2.034 * eV, 4.136 * eV };

    std::vector<G4double> reflectivity;
    std::vector<G4double> efficiency;
    for (G4int i = 0; i < wavelength.size(); ++i)
    {
        reflectivity.push_back(0.94);
        efficiency.push_back(0.95);
    }

    // optical slab surface properties
    //std::vector<G4double> reflectivity = { 0.3, 0.5 };   // how to get this parameter for EJ-200
    //std::vector<G4double> efficiency   = { 0.8, 1.0 };   // how to get this parameter for EJ-200

    // construct the material properties table for the optical surface
    G4MaterialPropertiesTable *mptslabAirSurface = new G4MaterialPropertiesTable();

    // add some properties to the optical surface
    mptslabAirSurface->AddProperty("REFLECTIVITY", photonEnergy, reflectivity);
    mptslabAirSurface->AddProperty("EFFICIENCY", photonEnergy, efficiency);
    mptslabAirSurface->AddProperty("RINDEX", photonEnergy, rindexSlab);

    // apply the properties to the optical surface
    opSlabAirSurface->SetMaterialPropertiesTable(mptslabAirSurface);


    //////// light guide surface ////////

    // optical light guide surface properties
    //std::vector<G4double> reflectivityLG = { 0.3, 0.5 };   // how to get this parameter for plexiglass
    //std::vector<G4double> efficiencyLG   = { 0.8, 1.0 };   // how to get this parameter for plexiglass

    // construct the material properties table for the optical surface
    G4MaterialPropertiesTable *mptLgSurface = new G4MaterialPropertiesTable();

    // add some properties to the optical surface
    mptLgSurface->AddProperty("REFLECTIVITY", photonEnergy, reflectivity);
    mptLgSurface->AddProperty("EFFICIENCY", photonEnergy, efficiency);
    mptLgSurface->AddProperty("RINDEX", photonEnergy, rindexLG);

    // apply the properties to the optical surface
    opLgAirSurface->SetMaterialPropertiesTable(mptLgSurface);

*/

    return physWorld;
}
