struct ClusterFit
{
public:
  ClusterFit(double chi2, double pos, double err, double wid, double amp, int n);
  double chiSquared;
  double stripPosition;
  double positionError;
  double width;
  double amplitude;
  int nDoF;
};

ClusterFit::ClusterFit(double chi2, double pos, double err, double wid, double amp, int n)
  : chiSquared(chi2), stripPosition(pos), positionError(err), width(wid), amplitude(amp), nDoF(n)
{ }

class FaserCluster
{
public:
  FaserCluster(int index, std::vector<FaserDigi*>& digis);
  FaserCluster(const FaserCluster& right);
  virtual ~FaserCluster();

  std::vector<FaserDigi*>& Digis() { return fDigis; }
  double Charge() const { return fCharge; }
  std::vector<int> LocalMaxima() const { return fMaxima; }
  void Draw() { fH.Draw(); Fit(); }
  std::vector<ClusterFit> Fit();
  std::vector<ClusterFit> WeightedAverage();

private:
  static TF1* threeParGauss;
  static TF1* fivePar2Gauss;
  static TF1* sixPar2Gauss;

  int fIndex;
  std::vector<FaserDigi*> fDigis;
  double fCharge;
  int fNMaxima;
  std::vector<int> fMaxima;
  TH1F fH;
  TString fFitName;
  std::vector<ClusterFit> fFitResults;
};

FaserCluster::FaserCluster(const FaserCluster& right)
  : fH(right.fH), fFitResults(right.fFitResults), fMaxima(right.fMaxima), fDigis(right.fDigis)
{
  fIndex = right.fIndex;
  fCharge = right.fCharge;
  fNMaxima = right.fNMaxima;
  fFitName = right.fFitName;
}

TF1* FaserCluster::threeParGauss = new TF1("g3", "gaus");
TF1* FaserCluster::fivePar2Gauss = new TF1("g5", "[0]*exp(-0.5*((x-[1])/[2])^2)+[3]*exp(-0.5*((x-[4])/[2])^2)");
TF1* FaserCluster::sixPar2Gauss = new TF1("g6", "[0]*exp(-0.5*((x-[1])/[2])^2)+[3]*exp(-0.5*((x-[4])/[5])^2)");

FaserCluster::FaserCluster(int index, std::vector<FaserDigi*>& digis)
  : fIndex(index), fDigis(digis), fCharge(0.0), fNMaxima(0),
    fH(TString("cluster_"+std::to_string(index)),
       TString("cluster_"+std::to_string(index)),digis.size(),-0.5, digis.size() - 0.5),
    fFitName("none")
{

  
  for (int i = 0; i < digis.size(); i++)
  {
    double q = digis[i]->Charge();
    fCharge += q;
  }
  double qAve = fCharge/digis.size();
  for (int i = 0; i < digis.size(); i++)
  {
    double q = digis[i]->Charge();
    fH.SetBinContent(i+1, q);
    fH.SetBinError(i+1, 0.05*sqrt(q*50000.0)); // total guess
  }
  fH.SetMinimum(0.0);

  if (digis.size() == 1)
  {
    fMaxima.push_back(0);
    fNMaxima = 1;
    return;
  }
  if (digis.size() == 2) 
  {
    if (fDigis[0]->Charge() > fDigis[1]->Charge())
    {
      fMaxima.push_back(0);
    }
    else
    {
      fMaxima.push_back(1);
    }
    fNMaxima = 1;
    return;
  }

  for (int i = 0 ; i < digis.size(); i++)
  {
    if (i == 0) 
    {
      if (digis[i]->Charge() > digis[i+1]->Charge()) 
      {
	fNMaxima++;
	fMaxima.push_back(i);
      }
    }
    else if (i == digis.size() - 1)
    {
      if (digis[i]->Charge() > digis[i-1]->Charge()) 
      {
	fNMaxima++;
	fMaxima.push_back(i);
      }
    }
    else
    {
      if (digis[i]->Charge() > digis[i-1]->Charge() && digis[i]->Charge() > digis[i+1]->Charge()) 
      {
	fNMaxima++;
	fMaxima.push_back(i);
      }
    }
  }
}

FaserCluster::~FaserCluster()
{ }

std::vector<ClusterFit> FaserCluster::WeightedAverage()
{
  // bail out on 1 hit cluster
  if (fDigis.size() == 1)
  {
    fFitResults.push_back(ClusterFit(0.0, fDigis[0]->Strip(), 
				     0.5, 0.5,
				     fDigis[0]->Charge(), 1));
    return fFitResults;
  }
  // find highest local maximum
  double maxQ = 0.0;
  int iLoc = 0;
  for (auto i : fMaxima)
  {
    if (fDigis[i]->Charge() > maxQ)
    {
      maxQ = fDigis[i]->Charge();
      iLoc = i;
    }
  }

  int iMin = max(0, iLoc - 1);
  int iMax = min((int)fDigis.size() - 1, iLoc + 1);
  double sumX = 0.0;
  double sumQ = 0.0;
  double sumXQ = 0.0;
  double sumX2Q = 0.0;
  for (int i = iMin; i <= iMax; i++)
  {
    double q = fH.GetBinContent(i+1);
    double sq = fH.GetBinError(i+1);
    sumQ += q;
    sumX += i * i * sq * sq;
    sumXQ += q * i;
    sumX2Q += q * i * i;
  }
  double wtdX = sumXQ/sumQ;
  double errX = sqrt(sumX)/sumQ;
  double widX = sqrt(sumX2Q/sumQ - wtdX*wtdX);
  threeParGauss->FixParameter(1, wtdX);
  threeParGauss->FixParameter(2, widX);
  threeParGauss->SetParameter(0, maxQ);
  threeParGauss->SetParLimits(0, maxQ/3.0, maxQ*3.0 );
  fFitName = "g3";
  fH.Fit(fFitName, "QIEMB");
  TF1* fit = fH.GetFunction(fFitName);
  fH.SetMaximum(max(fH.GetMaximum(),1.05*fit->GetParameter(0)));
  fFitResults.push_back(ClusterFit(fit->GetChisquare(), wtdX + fDigis[0]->Strip(), 
				     errX, widX,
				     fit->GetParameter(0), fDigis.size() - 1));
  return fFitResults;
}

std::vector<ClusterFit> FaserCluster::Fit()
{
  if (fFitName != "none") return fFitResults;
  if (fNMaxima == 1 && fDigis.size()>3)
  {
    fFitName = "g3";
    threeParGauss->ReleaseParameter(0);
    threeParGauss->ReleaseParameter(1);
    threeParGauss->ReleaseParameter(2);
    threeParGauss->SetParameters(fDigis[fMaxima[0]]->Charge(),fMaxima[0],1);
    threeParGauss->SetParLimits(0, fDigis[fMaxima[0]]->Charge()/3.0, fDigis[fMaxima[0]]->Charge()*3.0 );
    threeParGauss->SetParLimits(1, 0, fDigis.size());
    threeParGauss->SetParLimits(2, 0.25, 2.5);
    fH.Fit(fFitName,"QIEMB");
    // sanity check
    TF1* fit = fH.GetFunction(fFitName);
    for (int i = 0; i <= 2; i++)
    {
      double pLow, pHigh;
      threeParGauss->GetParLimits(i, pLow, pHigh);
      if (fit->GetParameter(i) < pLow || fit->GetParameter(i) > pHigh)
      {
	cout << "Sanity check on 1gauss fit failed" << endl;
      }
    }
    fH.SetMaximum(max(fH.GetMaximum(),1.05*fit->GetParameter(0)));
    fFitResults.push_back(ClusterFit(fit->GetChisquare(), fit->GetParameter(1) + fDigis[0]->Strip(), 
				     fit->GetParError(1), fit->GetParameter(2),
				     fit->GetParameter(0), fDigis.size() - 3));
    return fFitResults;
  }

  if (fDigis.size()>=5 && fNMaxima == 2)
  {
    //double qAve = fCharge/fDigis.size();
    //cout << "maxima : " << fDigis[fMaxima[0]]->Charge() << " at " << fMaxima[0] << " / " <<
    //  fDigis[fMaxima[1]]->Charge() << " at " << fMaxima[1] << endl;
    TF1* theFunc = nullptr;
    if (fDigis.size() == 5)
    {
      fivePar2Gauss->SetParameters(fDigis[fMaxima[0]]->Charge(),fMaxima[0],1,
				   fDigis[fMaxima[1]]->Charge(),fMaxima[1]);
      fivePar2Gauss->SetParLimits(0, fDigis[fMaxima[0]]->Charge()/3.0, fDigis[fMaxima[0]]->Charge()*3.0 );
      fivePar2Gauss->SetParLimits(3, fDigis[fMaxima[1]]->Charge()/3.0, fDigis[fMaxima[1]]->Charge()*3.0 );
      fivePar2Gauss->SetParLimits(1, max(0.0, fMaxima[0]-1.0), (fMaxima[0]+fMaxima[1])/2.0);
      fivePar2Gauss->SetParLimits(4, (fMaxima[0]+fMaxima[1])/2.0, min(fDigis.size()-1.0, fMaxima[1]+1.0));
      fivePar2Gauss->SetParLimits(2, 0.25, 2.5);
      fFitName = "g5";
      theFunc = fivePar2Gauss;
    }
    else
    {
      sixPar2Gauss->SetParameters(fDigis[fMaxima[0]]->Charge(),fMaxima[0],1,
				  fDigis[fMaxima[1]]->Charge(),fMaxima[1],1);
      sixPar2Gauss->SetParLimits(0, fDigis[fMaxima[0]]->Charge()/3.0, fDigis[fMaxima[0]]->Charge()*3.0 );
      sixPar2Gauss->SetParLimits(3, fDigis[fMaxima[1]]->Charge()/3.0, fDigis[fMaxima[1]]->Charge()*3.0 );
      sixPar2Gauss->SetParLimits(1, max(0.0, fMaxima[0]-1.0), (fMaxima[0]+fMaxima[1])/2.0);
      sixPar2Gauss->SetParLimits(4, (fMaxima[0]+fMaxima[1])/2.0, min(fDigis.size()-1.0, fMaxima[1]+1.0));
      sixPar2Gauss->SetParLimits(2, 0.25, 2.5);
      sixPar2Gauss->SetParLimits(5, 0.25, 2.5);
      fFitName = "g6";
      theFunc = sixPar2Gauss;
    }
    fH.Fit(fFitName, "QIEM");
    // sanity check
    TF1* fit = fH.GetFunction(fFitName);
    for (int i = 0; i <= (fFitName == "g5" ? 4 : 5); i++)
    {
      double pLow, pHigh;
      theFunc->GetParLimits(i, pLow, pHigh);
      if (fit->GetParameter(i) < pLow || fit->GetParameter(i) > pHigh)
      {
	cout << "Sanity check on 2gauss fit failed" << endl;
      }
    }
    double y1 = fit->GetParameter(0);
    double y2 = fit->GetParameter(3);
    fH.SetMaximum(max(fH.GetMaximum(),1.05*max(y1,y2)));
    fFitResults.push_back(ClusterFit(fit->GetChisquare(), fit->GetParameter(1) + fDigis[0]->Strip(),
				     fit->GetParError(1), fit->GetParameter(2), fit->GetParameter(0),
				     fDigis.size() - (fFitName == "g5" ? 5 : 6)));
    fFitResults.push_back(ClusterFit(fit->GetChisquare(), fit->GetParameter(4) + fDigis[0]->Strip(),
				     fit->GetParError(4), fit->GetParameter( (fFitName == "g5" ? 2 : 5) ),
				     fit->GetParameter(3), fDigis.size() - ( fFitName == "g5" ? 5 : 6 ) ));
    return fFitResults;
  }

  return WeightedAverage();
}

