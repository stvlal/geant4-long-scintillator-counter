#include "MyRunManager.hh"

namespace MeshNames
{

  inline static const G4String BASE_NAME = "lg_end";

  inline static G4String getMeshName(G4int i)
  {
    return BASE_NAME + std::to_string(i+1);
  }
};


MyRunManager::MyRunManager() : G4RunManager()
{
    mySingleParallelWorldName = "mySingleParallelWorld";
}

void MyRunManager::ConstructScoringWorlds() {

  G4ScoringManager* ScM = G4ScoringManager::GetScoringManagerIfExist();
  if(!ScM) return;
  G4int nPar = ScM->GetNumberOfMesh();
  if(nPar<1) return;

  G4ParticleTable::G4PTblDicIterator* theParticleIterator
   = G4ParticleTable::GetParticleTable()->GetIterator();
  for(G4int iw=0;iw<nPar;iw++)
  {

    G4VScoringMesh* mesh = ScM->GetMesh(iw);

    G4String worldName =
      (mesh->GetWorldName().find(MeshNames::BASE_NAME) !=
        std::string::npos) ? mySingleParallelWorldName :
          ScM->GetWorldName(iw);

    G4VPhysicalVolume* pWorld
       = G4TransportationManager::GetTransportationManager()
         ->IsWorldExisting(worldName);


    if(!pWorld)
    {
      pWorld = G4TransportationManager::GetTransportationManager()
         ->GetParallelWorld(worldName);
      pWorld->SetName(worldName);

      G4ParallelWorldScoringProcess* theParallelWorldScoringProcess
        = new G4ParallelWorldScoringProcess(worldName);
      theParallelWorldScoringProcess->SetParallelWorld(worldName);

      theParticleIterator->reset();
      while( (*theParticleIterator)() ){
        G4ParticleDefinition* particle = theParticleIterator->value();
        G4ProcessManager* pmanager = particle->GetProcessManager();
        if(pmanager)
        {
          pmanager->AddProcess(theParallelWorldScoringProcess);
          if(theParallelWorldScoringProcess->IsAtRestRequired(particle))
          { pmanager->SetProcessOrdering(theParallelWorldScoringProcess, idxAtRest, 9999); }
          pmanager->SetProcessOrderingToSecond(theParallelWorldScoringProcess, idxAlongStep);
          pmanager->SetProcessOrdering(theParallelWorldScoringProcess, idxPostStep, 9999);
        }
      }
    }
    mesh->Construct(pWorld);
  }
  GeometryHasBeenModified();

}
