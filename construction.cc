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


    //G4Material *pmtMat = G4Material::GetMaterial("Glass");
    G4Material *pmtMat = nist->FindOrBuildMaterial("G4_GLASS_PLATE");

    //G4Material *photocathMat = G4Material::GetMaterial("Al");
    // compose the photocathode material ( 2.7 is the density of aluminum)
    G4Material *photocathMat = new G4Material("photocathMat", 2.7*g/cm3, 3);
    photocathMat->AddElement(nist->FindOrBuildElement("K"), 1);
    photocathMat->AddElement(nist->FindOrBuildElement("Cs"), 1);
    photocathMat->AddElement(nist->FindOrBuildElement("Sb"), 1);


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

    solidPMT = new G4Tubs("solidPMT", innerRadius_pmt, outerRadius_pmt,
                                height_pmt, startAngle_pmt, spanningAngle_pmt);

    solidPhotocath = new G4Tubs("solidPhotocath", innerRadius_pmt,
      outerRadius_pmt,height_pmt / 2., startAngle_pmt, spanningAngle_pmt);


    logicPMT_1 = new G4LogicalVolume(solidPMT,
                                         pmtMat, "logicPMT_1");

    logicPMT_2 = new G4LogicalVolume(solidPMT,
                                         pmtMat, "logicPMT_2");

    logicPMT_1->SetVisAttributes(pmtVisAtt);
    logicPMT_2->SetVisAttributes(pmtVisAtt);

    logicPhotocath = new G4LogicalVolume(solidPhotocath,
                        photocathMat, "logicPhotocath");


    physPhotocath_1 = new G4PVPlacement(0,
      G4ThreeVector(0., 0., height_pmt / 2.),logicPhotocath,"physPhotocath_1",
                                                   logicPMT_1, false, 0, true);

    physPhotocath_2 = new G4PVPlacement(0,
      G4ThreeVector(0., 0.,  -height_pmt / 2.),logicPhotocath,"physPhotocath_2",
                                                   logicPMT_2, false, 0, true);


    physPMT_1 = new G4PVPlacement(rotPMT,
      G4ThreeVector(-88.5*cm,0.,0.), logicPMT_1, "physPMT_1",
                                 logicWorld, false, 0, true);
    physPMT_2 = new G4PVPlacement(rotPMT,
      G4ThreeVector(88.5*cm,0.,0.),logicPMT_2, "physPMT_2",
                               logicWorld, false, 0, true);


    // Photocathode surface properties

    std::vector<G4double> photocath_EFF = {
        0.014930,0.000765,0.020564,0.027417,0.036249,0.043102,0.052087,
        0.062594,0.074320,0.083915,0.095336,0.107214,0.118789,0.131886,
        0.148485,0.161887,0.175593,0.187471,0.192801,0.202852,0.213208,
        0.222497,0.234527,0.247776,0.260110,0.270921,0.280970,0.289953,
        0.298328,0.310509,0.309442,0.308374,0.306696,0.302581,0.295725,
        0.287497,0.278965,0.268452,0.257177,0.250473,0.242856,0.239351,
        0.225184,0.215890,0.202484,0.189230,0.177651,0.171252,0.158303,
        0.145050,0.129970,0.111386,0.093563,0.076807,0.060812,0.050451,
         0.038414,0.030186,0.020738,0.011900,0.007630,0.004732,0.001071
    };
    std::vector<G4double> wavelength_EFF = {
        279.580,280.098,281.241,283.159,285.203,287.248,289.292,292.231,
        294.146,295.806,297.721,300.276,302.063,303.849,306.018,308.188,
        309.846,312.272,314.318,315.850,318.405,320.833,324.028,327.479,
        332.338,337.581,343.849,351.014,354.595,360.222,363.295,367.264,
        373.153,380.196,386.344,394.541,403.378,413.240,423.614,430.274,
        436.935,440.777,448.464,456.278,465.629,474.852,484.075,490.351,
        498.421,505.212,510.980,519.054,527.383,535.328,543.400,553.262,
                 564.789,574.394,586.432,598.086,610.505,622.028,635.983
    };

    std::vector<G4double> photocath_ReR = {
        1.92808,1.99665,2.08162,2.14274,2.19938,2.25752,2.32311,2.39466,
        2.48261,2.53776,2.59441,2.62720,2.66296,2.70022,2.74940,2.81052,
        2.89250,2.95808,2.99236,3.00725,2.99977,2.99080,2.99674,3.02356,
        3.05932,3.10553,3.16366,3.21434,3.25906,3.26202,3.22174,3.17998,
        3.14716,3.10538,3.06808,3.03375,3.01286,2.98897,2.96805,2.95309,
                                                 2.94706,2.95447,2.95741
    };
    std::vector<G4double> wavelength_ReR = {
        380.414,384.560,388.903,392.950,397.194,401.735,406.375,411.606,
        416.838,420.391,423.846,428.288,434.015,439.938,445.664,450.600,
        457.312,464.223,469.850,476.169,483.871,490.882,498.485,506.482,
        512.011,516.552,520.796,524.942,529.385,535.013,542.616,548.048,
        554.664,563.947,572.735,582.906,590.016,599.989,615.195,630.105,
                                                 650.544,666.935,679.376
    };

    std::vector<G4double> photocath_ImR = {
        1.69256,1.68005,1.67927,1.69334,1.71289,1.71445,1.68630,1.65033,
        1.61593,1.57213,1.52756,1.50958,1.50254,1.49941,1.49550,1.47830,
        1.46422,1.43607,1.41809,1.39463,1.35631,1.32816,1.27498,1.18975,
        1.13657,1.09747,1.07167,1.05994,1.05994,1.06541,1.06385,1.05134,
        1.02084,0.98800,0.94343,0.89025,0.83395,0.78703,0.73933,0.69476,
        0.65175,0.61969,0.57902,0.55165,0.52585,0.50004,0.47424,0.45234,
        0.42888,0.40621,0.39370,0.37649,0.37180,0.36007,0.34599,0.33896,
                                         0.33661,0.33661,0.33192,0.33114
    };
    std::vector<G4double> wavelength_ImR = {
        380.455,385.870,392.092,397.506,402.919,407.960,412.503,415.615,
        418.354,421.964,426.508,429.931,433.478,438.892,442.937,446.857,
        451.214,455.633,460.051,464.283,468.515,470.881,474.989,480.093,
        483.392,487.251,490.923,495.031,500.756,505.361,511.459,515.380,
        519.052,522.102,525.338,528.886,532.061,535.049,538.037,540.340,
        543.701,546.440,550.921,556.274,561.315,567.040,572.268,577.807,
        586.519,595.107,600.272,608.487,617.012,626.471,634.188,642.215,
                                         657.337,666.111,672.583,679.491
    };


    G4MaterialPropertiesTable *mptPhotocath = new G4MaterialPropertiesTable();
    mptPhotocath->AddProperty("EFFICIENCY", wavelength_EFF, photocath_EFF);
    mptPhotocath->AddProperty("REALRINDEX", wavelength_ReR, photocath_ReR);
    mptPhotocath->AddProperty("IMAGINARYRINDEX", wavelength_ImR, photocath_ImR);

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
    //opSlabAirSurface->SetFinish(polishedfrontpainted);


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
    //opLgAirSurface->SetFinish(polishedfrontpainted);


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

void MyDetectorConstruction::ConstructSDandField()
{
    MySensitiveDetector *pmt = new MySensitiveDetector("pmt");

    logicPhotocath->SetSensitiveDetector(pmt);
}
