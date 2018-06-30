#pragma once

#include "TTree.h"
#include <map>
#include <memory>
#include <string>


//------------------------------------------------------------------------------

class FaserGeometry {

    std::map<std::string, int>    fIntProperties;
    std::map<std::string, double> fDoubleProperties;

public:

    // TODO: Read this also in from `faserGeo.mac` instead?
    static constexpr int ALL_PLANE_INDICES     [] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    static constexpr int FRONT_PLANE_INDICES   [] = {0, 1, 2};
    static constexpr int CENTRAL_PLANE_INDICES [] = {3, 4, 5, 6, 7, 8};
    static constexpr int END_PLANE_INDICES     [] = {9, 10, 11};

    FaserGeometry(const std::string & faserGeoFile);

    virtual ~FaserGeometry();

    template<typename T> T Get(const std::string & propertyName);
    template<typename T> T Get(const std::string & propertyName, T defaultValue);

    void DumpProperties();
    TTree * Tree();

private:

    TTree * fTree;

};

