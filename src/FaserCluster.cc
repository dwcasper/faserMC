#include "FaserCluster.hh"

#include "globals.hh"

using std::vector;
using std::shared_ptr;
using std::make_shared;
using std::cout;
using std::min;
using std::max;


//------------------------------------------------------------------------------

FaserCluster::FaserCluster() 
    : fIndex {0} 
    , fPosition {0.0}
    , fCharge {0.0} { }

FaserCluster::FaserCluster(const FaserCluster& right)
    : fIndex(right.fIndex)
    , fCharge(right.fCharge)
    , fDigis(right.fDigis)
    , fMaxima(right.fMaxima)
    , fPosition(right.fPosition) { }


FaserCluster& FaserCluster::operator= (const FaserCluster& right)
{
    if (this == &right) return *this;
    fIndex = right.fIndex;
    fDigis = right.fDigis;
    fMaxima = right.fMaxima;
    fCharge = right.fCharge;
    fPosition = right.fPosition;
}

FaserCluster::FaserCluster(int index, vector<FaserDigi*>& digis)
    : fIndex(index)
    , fCharge(0.0)
    , fDigis(digis)
    , fPosition(0.0)
{
    for (size_t i = 0; i < digis.size(); i++) 
    {
        double q = digis[i]->Charge();
        fCharge += q;
    }

    if (digis.size() == 1) 
    {
        fMaxima.push_back(0);
    } 
    else if (digis.size() == 2)  
    {
        if (fDigis[0]->Charge() > fDigis[1]->Charge()) 
        {
            fMaxima.push_back(0);
        } 
        else 
        {
            fMaxima.push_back(1);
        }
    }
    else
    {
        for (size_t i = 0 ; i < digis.size(); i++)
        {
            if (i == 0) 
            {
                if (digis[i]->Charge() > digis[i+1]->Charge()) 
                {
                    fMaxima.push_back(i);
                }
            } 
            else if (i == digis.size() - 1) 
            {
                if (digis[i]->Charge() > digis[i-1]->Charge()) 
                {
                    fMaxima.push_back(i);
                }
            } 
            else 
            {
                if (digis[i]->Charge() > digis[i-1]->Charge() && digis[i]->Charge() > digis[i+1]->Charge()) 
                {
                    fMaxima.push_back(i);
                }
            }
        }
    }
    WeightedAverage();
}

FaserCluster::~FaserCluster()
{ }

//------------------------------------------------------------------------------

void FaserCluster::WeightedAverage() {
    // bail out on 1 hit cluster
    if (fDigis.size() == 1) 
    {
        fPosition = G4double(fDigis[0]->Strip());
        return;
    }

    // find highest local maximum
    G4double maxQ = 0.0;
    G4int iLoc = 0;
    for (auto i : fMaxima) {
        if (fDigis[i]->Charge() > maxQ) {
            maxQ = fDigis[i]->Charge();
            iLoc = i;
        }
    }

    G4int iMin = max(0, iLoc - 1);
    G4int iMax = min((int)fDigis.size() - 1, iLoc + 1);
    G4double sumX = 0.0;
    G4double sumQ = 0.0;
    G4double sumXQ = 0.0;
    for (G4int i = iMin; i <= iMax; i++) {
        G4double q = fDigis[i]->Charge();
        sumQ += q;
        sumXQ += q * i;
    }
    fPosition = sumXQ/sumQ;
    return;

}
