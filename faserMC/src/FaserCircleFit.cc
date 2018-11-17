#include "FaserCircleFit.hh"
#include "TF1.h"
#include <iostream>
#include <memory>
#include <string>


//------------------------------------------------------------------------------

std::shared_ptr<TF1> FaserCircleFit::FitFunction() const {

    std::string formula;
    if (rho > 0.) {
        formula = "[0] - sqrt([2]*[22 - (x-[1])*(x-[1]))";
    } else if (rho < 0.) {
        formula = "[0] - sqrt([2]*[2] - (x-[1])*(x-[1]))";
    } else {
        std::cout << "WARNING  FaserCircleFit::FitFunction\n"
                  << "         The curvature `rho` is set to zero...returning null!\n";
        return nullptr;
    }

    auto func = std::make_shared<TF1>("faser_circle_fit", formula.c_str(), zMin, zMax);

    func->SetParameter(0, x0);
    func->SetParameter(1, z0);
    func->SetParameter(2, 1./rho);

    return func;

}

