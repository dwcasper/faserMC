class FaserSamplerCluster
{
public:
  FaserSamplerCluster(int index, std::vector<FaserDigi*>& digis);
  FaserSamplerCluster(const FaserSamplerCluster& right);
  virtual ~FaserSamplerCluster();

  std::vector<FaserDigi*>& Digis() { return fDigis; }

private:

  int fIndex;
  std::vector<FaserDigi*> fDigis;
};

FaserSamplerCluster::FaserSamplerCluster(const FaserSamplerCluster& right)
  : fDigis(right.fDigis)
{
  fIndex = right.fIndex;
}


FaserSamplerCluster::FaserSamplerCluster(int index, std::vector<FaserDigi*>& digis)
  : fIndex(index), fDigis(digis)
{
  
}

FaserSamplerCluster::~FaserSamplerCluster()
{ }

