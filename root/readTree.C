#include "FaserCluster.C"

Int_t rowID(FaserDigi* d)
{
    return d->Row() + 2 * ( d->Sensor() + 4 * ( d->Module() + 2 * d->Plane() ) );
}

std::vector<std::vector<FaserDigi*> > clusterOneRow(std::vector<FaserDigi*> digits)
{
  std::vector<std::vector<FaserDigi*> > clusters;
  std::vector<FaserDigi*> cluster;
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

void sortDigits(std::vector<FaserDigi*>& v)
{
  std::sort(v.begin(), v.end(), [](FaserDigi* const& l, FaserDigi* const& r) { return l->Strip() < r->Strip(); });
}

std::map<int, std::vector<FaserDigi*> > mapDigitsByPlane(FaserEvent* e)
{
  std::map<int, std::vector<FaserDigi*> > planeMap;
  for (FaserDigi* d : e->Digis())
  {
    planeMap[d->Plane()].push_back( d );
  }
  return planeMap;
}

std::map<int, std::vector<FaserDigi*> > mapDigitsByRow( std::vector<FaserDigi*> v)
{
  std::map<int, std::vector<FaserDigi*> > rowMap;
  
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

void readTree(TString fileName)
{

  TFile* f = TFile::Open(fileName,"READ");
  TTree* t = (TTree*) f->Get("faser");

  FaserEvent* e = new FaserEvent();
  TBranch* b = t->GetBranch("event");
  b->SetAddress(&e);

  Int_t n = t->GetEntries();
  cout << "File contains " << n << " event records" << endl;

  // Histograms
  TH1F* qClust = new TH1F("qClust", "Qclust", 100, 0., 500000.);
  TH1F* nClust = new TH1F("nClust", "Nclust", 21, -0.5, 20.5);
  TH1F* nMaxima = new TH1F("nMaxima", "Nmaxima", 21, -0.5, 20.5);
  TH1F* qClustFail = new TH1F("qClustFail", "QclustFail", 100, 0., 500000.);
  TH1F* nClustFail = new TH1F("nClustFail", "NclustFail", 21, -0.5, 20.5);
  TH1F* nMaximaFail = new TH1F("nMaximaFail", "NmaximaFail", 21, -0.5, 20.5);

  std::map<Int_t, Int_t> cutFlow;
  for (Int_t i = 0 ; i < n; i++)
  {
    Int_t cut = 0;
    cutFlow[cut++]++;

    b->GetEntry(i);
    // if there are no digits, read next
    if (e->Digis().size() == 0) continue;
    cout << "Event has " << e->Digis().size() << " digits" << endl;
    cutFlow[cut++]++;

    // collect the digits by planes of the detector
    std::map<int, std::vector<FaserDigi*> > planeMap = mapDigitsByPlane(e);
    cout << "Event has activity in " << planeMap.size() << " planes" << endl;
    // require activity in at least three planes
    if ( planeMap.size() < 3 ) continue;
    cutFlow[cut++]++;

    // process each plane in turn
    std::map<int, std::map<int, std::vector<FaserDigi*> > > rowMap;
    Int_t nGoodPlanes = 0;
    for ( auto& plane : planeMap )
    {
      rowMap[plane.first] = mapDigitsByRow(plane.second);
      cout << "Plane " << plane.first << " has " << rowMap[plane.first].size() << " active rows" << endl;
      if (rowMap[plane.first].size() < 2) continue;
      nGoodPlanes++;
    }
    if (nGoodPlanes < 3) continue;
    cutFlow[cut++]++;

    // cluster the digits in each row of a plane
    std::map<int, std::vector<FaserCluster> > clusterMap;
    for ( auto& plane : rowMap )
    {
      for ( auto& row : plane.second )
      {
	std::vector<std::vector<FaserDigi*> > clusters = clusterOneRow(row.second);
	for ( auto& c : clusters )
	{
	  clusterMap[plane.first].push_back(FaserCluster(c));
	}
      }
      cout << "Plane " << plane.first << " has " << clusterMap[plane.first].size() << " clusters" << endl;
    }

    // Require at least 4 clusters in first plane
    if (clusterMap[0].size() < 4 ) 
    {
      for ( auto& c : clusterMap[0] )
      {
	qClustFail->Fill(c.Charge());
	nClustFail->Fill(c.Digis().size());
	nMaximaFail->Fill(c.LocalMaxima());
      }
      continue;
    }
    
    cutFlow[cut++]++;

    for ( auto& plane : clusterMap )
    {
      for ( auto& c : plane.second )
      {
	qClust->Fill(c.Charge());
	nClust->Fill(c.Digis().size());
	nMaxima->Fill(c.LocalMaxima());
      }
    }
  }

  cout << "CutFlow summary: " << endl;
  for (auto stat : cutFlow)
  {
    cout << "Cut " << stat.first << ": " << stat.second << endl;
  }
}

