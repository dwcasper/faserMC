#include "FaserEvent.hh"

#include "G4ParticleTable.hh"
#include "G4VTrajectoryPoint.hh"

////////////////////////////////////////////////////////////////////////////////


FaserEvent::FaserEvent(G4int eventNumber)
  : fEventNumber(eventNumber)
{}

FaserEvent::FaserEvent()
  : FaserEvent(0)
{ }

FaserEvent::~FaserEvent()
{ 
  for (auto p : fParticles) if (p) delete p;
  fParticles.clear();

  for (auto c : fClusters) if (c) delete c;
  fClusters.clear();
}

void FaserEvent::SetParticles(G4TrajectoryContainer* particles)
{
  if (particles == nullptr) return;
  for (size_t i = 0; i < particles->size(); i++)
  {
    G4VTrajectory& t = *((*particles)[i]);
    G4int pdgCode = t.GetPDGEncoding();
    G4double mass = G4ParticleTable::GetParticleTable()->FindParticle(pdgCode)->GetPDGMass();
    G4ThreeVector momentum = t.GetInitialMomentum();
    G4double energy = sqrt(momentum.mag2() + mass*mass);
    FaserTruthParticle* p = new FaserTruthParticle(t.GetTrackID(), t.GetParentID(), pdgCode,
						   (*(t.GetPoint(0))).GetPosition(), momentum, energy); 
    fParticles.push_back(p);
  }
}

void FaserEvent::SetClusters()
{
  fClusters.clear();

  // If there are no digits, return empty `fClusters`
  if (fTrackerDigis.size() == 0) return;

  // Collect the digits by planes of the detector
  map<int, vector<FaserDigi*>> planeMap = mapDigitsByPlane(*this);

  // Process each plane in turn
  map<int, map<int, vector<FaserDigi*>>> rowMap;
  for (auto & plane : planeMap) 
  {
    rowMap[plane.first] = mapDigitsByRow(plane.second);
  }

  int clusterNumber = 0;
  // cluster the digits in each row of a plane
  map<int, vector<FaserCluster*>> clusterMap;
  for ( auto& plane : rowMap ) {
    for ( auto& row : plane.second ) {
      vector<vector<FaserDigi*>> clusters = clusterOneRow(row.second);
      for ( auto& c : clusters ) {
        clusterMap[plane.first].push_back(new FaserCluster(clusterNumber++, c));
      }
    }
  }

  for ( auto& plane : clusterMap ) {
    for ( auto c : plane.second ) {
      fClusters.push_back( c );
    }
  }

}


////////////////////////////////////////////////////////////////////////////////
// Utility methods for cluster finding                                        //
////////////////////////////////////////////////////////////////////////////////

int FaserEvent::rowID(FaserDigi* d)
{
  return d->Row() + 2 * ( d->Sensor() + 4 * ( d->Module() + 2 * d->Plane() ) );
}

//------------------------------------------------------------------------------

vector<vector<FaserDigi*>> FaserEvent::clusterOneRow(vector<FaserDigi*> digits)
{
  vector<vector<FaserDigi*>> clusters;
  vector<FaserDigi*> cluster;
  int previousStrip = 1 << 30;
  for (auto d : digits)
  {
    if (cluster.size() > 0 && d->Strip() > previousStrip + 1)
    {
        clusters.push_back(cluster);
        cluster.clear();
    }
    cluster.push_back(d);
    previousStrip = d->Strip();
  }
  if (cluster.size() > 0) clusters.push_back(cluster);
  return clusters;
}

//------------------------------------------------------------------------------

void FaserEvent::sortDigits(vector<FaserDigi*>& v)
{
    std::sort(v.begin(), v.end(), [](FaserDigi* const& l, FaserDigi* const& r) { return l->Strip() < r->Strip(); });
}

//------------------------------------------------------------------------------

map<int, vector<FaserDigi*>> FaserEvent::mapDigitsByPlane(FaserEvent & evt)
{
  map<int, vector<FaserDigi*>> planeMap;
  for (FaserDigi* d : evt.TrackerDigis())
  {
    planeMap[d->Plane()].push_back( d );
  }
  return planeMap;
}

//------------------------------------------------------------------------------

map<int, vector<FaserDigi*>> FaserEvent::mapDigitsByRow(vector<FaserDigi*> v)
{
  map<int, vector<FaserDigi*>> rowMap;

  for (FaserDigi* d : v)
  {
    rowMap[rowID(d)].push_back( d );
  }
  for (auto& row : rowMap)
  {
    sortDigits(row.second);
  }
  return rowMap;
}
