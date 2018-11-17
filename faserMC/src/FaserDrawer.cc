#include "FaserDrawer.hh"
#include "FaserTruthParticle.hh"
#include "G4Polyline.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "G4VVisManager.hh"
#include "TVector3.h"
#include "TMath.h"

#include <iostream>
using std::cout;

//------------------------------------------------------------------------------

void FaserDrawer::DrawSpacePoints(FaserEvent * event) {

  if (!event) return;

  G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
  if (!pVVisManager) return;

  for (const FaserSpacePoint * sp : event->SpacePoints()) {
    TVector3 pos = sp->GlobalPos();
    G4Point3D point {pos.X(), pos.Y(), pos.Z()};
    G4Circle circle {point};
    circle.SetScreenDiameter(8.0);
    circle.SetFillStyle(G4Circle::filled);
    G4VisAttributes attribs {G4Colour{0.,0.,1.}};
    circle.SetVisAttributes(attribs);
    pVVisManager->Draw(circle);
  }

}

//------------------------------------------------------------------------------
void FaserDrawer::DrawPropagatedTrajectory(FaserEvent * event) {

  if (!event) return;

  G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
  if (!pVVisManager) return;

  // Loop through particles, looking for decay products of the dark photon.
  vector<FaserTruthParticle*> particles;
  for (FaserTruthParticle * tp : event->Particles()) {
    if (tp && tp->ParentID()==1) particles.push_back(tp);
    //if (tp && tp->ParentID()<100) particles.push_back(tp);
  }

  G4Polyline propagatedTrajectory;

  for (FaserTruthParticle * particle : particles) {

    // First obtain the track parameters (x0, y0, R, phi0, lambda).
    HelixTrajectory tr;

    tr.h = 1;
    // Set to reverse orientation if particle charge is positive instead of negative.
    if (particle->PdgCode() < 0) tr.h = -1;

    G4ThreeVector vertex = particle->Vertex();
    G4ThreeVector p = particle->Momentum();

    double p_perp = TMath::Sqrt(p.x()*p.x() + p.z()*p.z());
    tr.R = p_perp / (0.3*bField.Mag()) * 1000.; // mm
    tr.x0 = vertex.x();
    tr.y0 = vertex.y();
    tr.z0 = vertex.z();
    if (tr.z0 > 0.) tr.phi0 = TMath::ATan(tr.x0/tr.z0);
    else if (tr.z0 < 0.) tr.phi0 = TMath::Pi() - TMath::ATan(tr.x0/tr.z0);
    else if (tr.x0 > 0.) tr.phi0 = 0.5*TMath::Pi();
    else if (tr.x0 < 0.) tr.phi0 = -0.5*TMath::Pi();
    else tr.phi0 = 0.;  // undefined, since at origin in this case
    tr.lambda = p.y() / p.mag();

    G4Point3D startPoint {tr.x0, tr.y0, tr.z0};

    //TODO: Get rid of this hard-coding.
    double zMax = 3000.; // mm
    //double xMin = -100.; // mm
    //double xMax =  100.; // mm
    //double yMin =  -80.; // mm
    //double yMax =   80.; // mm
    G4Point3D curPos {tr.x0, tr.y0, tr.z0};
    double sArc = 0.;
    int maxIter = 1000000;
    int iter = -1;
    while (true) { // 10-mm increments
      ++iter;
      if (iter > maxIter) break;

      curPos = TrackPosAtS(tr, sArc);
      sArc += 100.;

      if (
        curPos.z() > zMax)
        //curPos.z() > zMax ||
        //curPos.x() < xMin ||
        //curPos.x() > xMax ||
        //curPos.y() < yMin ||
        //curPos.y() > yMax)
      {
        break;
      }
      propagatedTrajectory.push_back(curPos);
    }
  }

  G4VisAttributes att {G4Colour{1.,0.,0.}};
  propagatedTrajectory.SetVisAttributes(att);

  if (pVVisManager) pVVisManager->Draw(propagatedTrajectory);

}

//------------------------------------------------------------------------------

G4Point3D FaserDrawer::TrackPosAtS(const HelixTrajectory & tr, double sArc) {

  double x = tr.x0 + tr.R*(TMath::Sin(tr.phi0 + tr.h*sArc*TMath::Cos(tr.lambda)/tr.R) - TMath::Sin(tr.phi0));
  double y = tr.y0 + sArc*TMath::Sin(tr.lambda);
  double z = tr.z0 + tr.R*(TMath::Cos(tr.phi0 + tr.h*sArc*TMath::Cos(tr.lambda)/tr.R) - TMath::Cos(tr.phi0));

  return {x, y, z};
}

//------------------------------------------------------------------------------

G4Point3D FaserDrawer::TrackPosAtZ(const HelixTrajectory & tr, double z) {

  // TODO: The calculation of `s` here needs to be account for the case of
  //       `(z - z0)/R + cos(phi0)` being outside the range of [0, pi].
  double sArc = tr.R/(tr.h * TMath::Cos(tr.lambda)) *
               (TMath::ACos((z - tr.z0)/tr.R + TMath::Cos(tr.phi0)) - tr.phi0);

  double x = tr.x0 + tr.R*(TMath::Sin(tr.phi0 + tr.h*sArc*TMath::Cos(tr.lambda)/tr.R) - TMath::Sin(tr.phi0));
  double y = tr.y0 + sArc*TMath::Sin(tr.lambda);

  return {x, y, z};
}
