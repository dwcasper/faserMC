class FaserCluster
{
public:
  FaserCluster(std::vector<FaserDigi*>& digis);

  std::vector<FaserDigi*>& Digis() { return fDigis; }
  double Charge() const { return fCharge; }
  int LocalMaxima() const { return fMaxima; }

private:
  std::vector<FaserDigi*> fDigis;
  double fCharge;
  int fMaxima;
};

FaserCluster::FaserCluster(std::vector<FaserDigi*>& digis)
{
  fDigis = digis;
  fCharge = 0.0;
  fMaxima = 0;

  for (auto d : fDigis)
  {
    fCharge += d->Charge();
  }

  if (digis.size() < 3) 
  {
    fMaxima = 1;
    return;
  }

  for (int i = 0 ; i < digis.size(); i++)
  {
    if (i == 0) 
    {
      if (digis[i]->Charge() > digis[i+1]->Charge()) fMaxima++;
    }
    else if (i == digis.size() - 1)
    {
      if (digis[i]->Charge() > digis[i-1]->Charge()) fMaxima++;
    }
    else
    {
      if (digis[i]->Charge() > digis[i-1]->Charge() && digis[i]->Charge() > digis[i+1]->Charge()) fMaxima++;
    }
  }
}
