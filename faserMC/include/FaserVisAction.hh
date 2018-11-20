#ifndef FaserVisAction_hh
#define FaserVisAction_hh

#include "G4VUserVisAction.hh"

class FaserVisAction : public G4VUserVisAction
{
  public:
    FaserVisAction() { }
    void Draw();
  private:
    FaserVisAction(const FaserVisAction&);
    FaserVisAction& operator=(const FaserVisAction&);
};

#endif