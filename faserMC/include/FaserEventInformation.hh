#ifndef FASER_EVENTINFORMATION_HH
#define FASER_EVENTINFORMATION_HH 1

#include "G4VUserEventInformation.hh"

class FaserEvent;

class FaserEventInformation : public G4VUserEventInformation
{
  public:
    FaserEventInformation(FaserEvent* event);
    virtual ~FaserEventInformation();
    FaserEvent* GetFaserEvent() const;
    virtual void Print() const;
  private:
    FaserEvent* fEvent;
};

#endif