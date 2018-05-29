Int_t digiID(FaserDigi* d)
{
  return 0;
}

Int_t rowID(FaserDigi* d)
{
    return d->Row() + 2 * ( d->Sensor() + 4 * ( d->Module() + 2 * d->Plane() ) );
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
  }

  cout << "CutFlow summary: " << endl;
  for (auto stat : cutFlow)
  {
    cout << "Cut " << stat.first << ": " << stat.second << endl;
  }
}

