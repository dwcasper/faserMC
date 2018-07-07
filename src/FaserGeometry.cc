#include "FaserGeometry.hh"
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>

using std::cout;
using std::getline;
using std::runtime_error;
using std::string;

//------------------------------------------------------------------------------

FaserGeometry::FaserGeometry(const string & faserGeoFile)
    : fTree {nullptr}
{

    std::ifstream in;
    in.open(faserGeoFile);
    if (!in.is_open()) {
        cout << "ERROR  FaserGeometry::FaserGeometry\n"
             << "       Unable to open " << faserGeoFile << "\n"
             << "       Exiting.\n";
        exit(1);
    }

    string line;
    while (getline(in, line)) {
        if (line.find("/faser/geo/") != 0) continue;
        std::stringstream pieces {line};
        string piece;
        std::vector<string> tokens;
        while (getline(pieces, piece, ' ')) {
            tokens.push_back(piece);
        }

        // A bit sloppy, but does the job:
        string propertyName = tokens[0].substr(tokens[0].rfind('/')+1, tokens[0].find(' ')-tokens[0].rfind('/'));

        if (tokens.size() == 2) {  // integer property
            fIntProperties[propertyName] = std::stoi(tokens[1]);
        } else if (tokens.size() == 3) {
            double value = std::stod(tokens[1]);

            // Store all distances in millimeters.
            if      (tokens[2] == "m" ) fDoubleProperties[propertyName] = value * 1e3;
            else if (tokens[2] == "cm") fDoubleProperties[propertyName] = value * 10.;
            else if (tokens[2] == "mm") fDoubleProperties[propertyName] = value * 1.;
            else if (tokens[2] == "um") fDoubleProperties[propertyName] = value * 1e-3;

            // Store all angles in radians.
            else if (tokens[2] == "rad")  fDoubleProperties[propertyName] = value * 1.;
            else if (tokens[2] == "mrad") fDoubleProperties[propertyName] = value * 1e-3;
            else {
                cout << "WARNING  FaserGeometry::FaserGeometry\n"
                     << "         Unrecognized unit for property `" << propertyName << "`\n"
                     << "         Skipping!\n";
            }
        }
    }

}

//------------------------------------------------------------------------------

FaserGeometry::~FaserGeometry() {
}

//------------------------------------------------------------------------------

template<typename T> T FaserGeometry::Get(const string & propertyName) {

    if (typeid(T) == typeid(int)) {
        if (fIntProperties.count(propertyName) == 0) {
            cout << "ERROR  FaserGeometry::Get\n"
                 << "       No such `int` property found: " << propertyName << "\n";
            throw runtime_error {"property not found"};
        }
        return fIntProperties[propertyName];
    }

    if (typeid(T) == typeid(double)) {
        if (fDoubleProperties.count(propertyName) == 0) {
            cout << "ERROR  FaserGeometry::Get\n"
                 << "       No such `double` property found: " << propertyName << "\n";
            throw runtime_error {"property not found"};
        }
        return fDoubleProperties[propertyName];
    }

    else {
        cout << "ERROR  FaserGeometry::Get\n"
             << "       Invalid type requested. Only `int` and `double` are supported.\n";
        throw runtime_error {"invalid type"};
    }

}

//------------------------------------------------------------------------------

template<typename T> T FaserGeometry::Get(const string & propertyName, T defaultValue) {
    if (typeid(T) == typeid(int)) {
        if (fIntProperties.count(propertyName) == 0) {
            return defaultValue;
        }
        return fIntProperties[propertyName];
    }

    if (typeid(T) == typeid(double)) {
        if (fDoubleProperties.count(propertyName) == 0) {
            return defaultValue;
        }
        return fDoubleProperties[propertyName];
    }

}

//------------------------------------------------------------------------------

void FaserGeometry::DumpProperties() {

    cout << "INFO  FaserGeometry::DumpProperties\n"
         << "      Dumping properties:\n"
         << "\n";

    for (const auto & it : fIntProperties) {
        cout << "   " << it.first << " = " << it.second << "\n";
    }

    for (const auto & it : fDoubleProperties) {
        cout << "   " << it.first << " = " << it.second << "\n";
    }

    cout << "\n";

}

//------------------------------------------------------------------------------

TTree * FaserGeometry::Tree() {

    if (fTree) return fTree;

    cout << "INFO  FaserGeometry::Tree\n"
         << "      Creating `TTree` with FASER geometry settings.\n"
         << "\n";

    fTree = new TTree {"faserGeo", "faserGeo"};

    for (auto & it : fIntProperties) {
        string name = it.first;
        int & value = it.second;
        fTree->Branch(name.c_str(), &value, (name+"/I").c_str());
    }

    for (auto & it : fDoubleProperties) {
        string name = it.first;
        double & value = it.second;
        fTree->Branch(name.c_str(), &value, (name+"/D").c_str());
    }

    fTree->Fill();
    return fTree;

}


