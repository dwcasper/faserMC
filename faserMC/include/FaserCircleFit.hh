#pragma once

#include <memory>
#include "TF1.h"


struct FaserCircleFit {

    bool    isValid;

    double  x0;
    double  z0;
    double  rho;

    double  xMin;
    double  xMax;
    double  zMin;
    double  zMax;


    FaserCircleFit(bool isValid_, double x0_=0., double z0_=0., double rho_=0.)
        : isValid {isValid_}
        , x0 {x0_}
        , z0 {z0_}
        , rho {rho_}
    {
    }

    virtual ~FaserCircleFit() {
    }

    std::shared_ptr<TF1> FitFunction() const;

};

