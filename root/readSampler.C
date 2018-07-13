#include "FaserSamplerCluster.C"

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
  for (FaserDigi* d : e->SamplerDigis())
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
  TH1F* nClust0 = new TH1F("nClust0", "Nclust0", 41, -0.5, 40.5);
  TH1F* nClust1 = new TH1F("nClust1", "Nclust1", 41, -0.5, 40.5);
  TH1F* nClust00 = new TH1F("nClust00", "Nclust00", 41, -0.5, 40.5);
  TH1F* nClust01 = new TH1F("nClust01", "Nclust01", 41, -0.5, 40.5);
  TH1F* nClust10 = new TH1F("nClust10", "Nclust10", 41, -0.5, 40.5);
  TH1F* nClust11 = new TH1F("nClust11", "Nclust11", 41, -0.5, 40.5);
  TH1F* nClust = new TH1F("nClust", "Nclust", 41, -0.5, 40.5);

  std::map<Int_t, Int_t> cutFlow;
  for (Int_t i = 0 ; i < n; i++)
  {
    Int_t cut = 0;
    cutFlow[cut++]++;

    b->GetEntry(i);

    // if there are no digits, read next
    if (e->SamplerDigis().size() == 0) continue;
    cout << "Event has " << e->SamplerDigis().size() << " sampler digits" << endl;
    cutFlow[cut++]++;

    // collect the digits by planes of the detector
    std::map<int, std::vector<FaserDigi*> > planeMap = mapDigitsByPlane(e);
    cout << "Event has activity in " << planeMap.size() << " planes" << endl;
    // require activity in at least two planes
    if ( planeMap.size() < 2 ) continue;
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
    if (nGoodPlanes < 2) continue;
    cutFlow[cut++]++;

    int clusterNumber = 0;
    // cluster the digits in each row of a plane
    std::map<int, std::vector<FaserSamplerCluster> > clusterMap;
    for ( auto& plane : rowMap )
    {
      for ( auto& row : plane.second )
      {
        std::vector<std::vector<FaserDigi*> > clusters = clusterOneRow(row.second);
        for ( auto& c : clusters )
        {
          clusterMap[plane.first].push_back(FaserSamplerCluster(clusterNumber++, c));
        }
      }
      cout << "Plane " << plane.first << " has " << clusterMap[plane.first].size() << " clusters" << endl;
    }

    cutFlow[cut++]++;

    int max00 = 0;
    int max01 = 0;
    int max10 = 0;
    int max11 = 0;
    for ( auto& plane : clusterMap )
    {
      for ( auto& c : plane.second )
      {
        int cSize = c.Digis().size();
        if (plane.first == 0)
        {
          if (c.Digis()[0]->Sensor()%2 == 0)
          {
            max00 = std::max(max00, cSize);
          }
          else
          {
            max01 = std::max(max01, cSize);
          }
        }
        else
        {
          if (c.Digis()[0]->Sensor()%2 == 0)
          {
            max10 = std::max(max10, cSize);
          }
          else
          {
            max11 = std::max(max11, cSize);
          }
        }
      }
    }
    if (max00 > 0) 
    {
      nClust->Fill(max00);
      nClust0->Fill(max00);
      nClust00->Fill(max00);
    }
    if (max01 > 0) 
    {
      nClust->Fill(max01);
      nClust0->Fill(max01);
      nClust01->Fill(max01);
    }
    if (max10 > 0) 
    {
      nClust->Fill(max10);
      nClust1->Fill(max10);
      nClust10->Fill(max10);
    }
    if (max11 > 0) 
    {
      nClust->Fill(max11);
      nClust1->Fill(max11);
      nClust11->Fill(max11);
    }
  }

  cout << "CutFlow summary: " << endl;
  for (auto stat : cutFlow)
  {
    cout << "Cut " << stat.first << ": " << stat.second << endl;
  }
}

