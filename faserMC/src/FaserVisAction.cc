#ifndef FASER_VISACTION_HH
#define FASER_VISACTION_HH 1

#include <vector>

#include "G4VisManager.hh"
#include "G4VSceneHandler.hh"
#include "G4Scene.hh"
#include "G4CallbackModel.hh"
#include "G4ModelingParameters.hh"
#include "G4Event.hh"
#include "G4Circle.hh"

#include "FaserVisAction.hh"
#include "FaserEvent.hh"
#include "FaserEventInformation.hh"

#include "TVector3.h"

void FaserVisAction::Draw()
{
  // Threading makes this incredibly painful
  G4VisManager* pVisManager = G4VisManager::GetInstance();
  if (pVisManager == nullptr) return;

  G4VSceneHandler* pVSceneHandler = pVisManager->GetCurrentSceneHandler();
  if (pVSceneHandler == nullptr) return;

  G4Scene* pScene = pVSceneHandler->GetScene();
  if (pScene == nullptr) return;

  const std::vector<G4Scene::Model>& EOEModelList =
    pScene -> GetEndOfEventModelList ();

  size_t nModels = EOEModelList.size();
  if (nModels == 0) return;

  const G4Event* g4Evt = nullptr;
  for (size_t i = 0; i < nModels; i++)
  {
      G4VModel* model = EOEModelList[i].fpModel;
      G4CallbackModel<G4VUserVisAction>* pCallback = dynamic_cast<G4CallbackModel<G4VUserVisAction>*>(model);
      if (pCallback != nullptr)
      {
          const G4ModelingParameters* pMP = pCallback->GetModelingParameters();
          if (pMP == nullptr) continue;
          g4Evt = pMP->GetEvent();
          if (g4Evt != nullptr) break;
      }
  }
  if (g4Evt == nullptr) return;

  FaserEventInformation* info = dynamic_cast<FaserEventInformation*>(g4Evt->GetUserInformation());
  if (info == nullptr) return;
  FaserEvent* event = info->GetFaserEvent(); // That was easy!
  if (event == nullptr) return;

  G4cout << "Found " << event->SpacePoints().size() << " space points to draw" << G4endl;
  for (const FaserSpacePoint * sp : event->SpacePoints()) {
    sp->Print();   
    TVector3 pos = sp->GlobalPosition();
    //G4cout << "Drawing space point at global position " << pos.X() << "," << pos.Y() << "," << pos.Z() << G4endl;
    G4Point3D point {pos.X(), pos.Y(), pos.Z()};
    G4Circle circle {point};
    circle.SetScreenDiameter(8.0);
    circle.SetFillStyle(G4Circle::filled);
    G4VisAttributes attribs {G4Colour{0.,0.,1.}};
    circle.SetVisAttributes(attribs);
    pVisManager->Draw(circle);
  }
  G4cout << "Finished drawing space points" << G4endl;
}

#endif