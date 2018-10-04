//#include "FaserTrackerGeometry.hh"
//#include "TFile.h"
//#include "TTree.h"
//#include <iostream>
//
//using std::cout;
//
////------------------------------------------------------------------------------
//
//void FaserTrackerGeometry::WriteToFile(const std::string & fileName)
//{
//  if (nStrips       == -10000  ||
//      stripPitch    == -10000. ||
//      stereoAngle   == -10000. ||
//      moduleOffsetX == -10000. ||
//      sensorOffsetY == -10000. ||
//      rowOffsetY    == -10000. ||
//      planeIndices_front.size() < 1   ||
//      planeIndices_central.size() < 1 ||
//      planeIndices_end.size() < 1     ||
//      planeZ.size() < 1)
//  {
//      cout << "ERROR  FaserTrackerGeometry:\n"
//           << "       Invalid geometry:\n"
//           << "          nStrips       = " << nStrips << '\n'
//           << "          stripPitch    = " << stripPitch << '\n'
//           << "          stereoAngle   = " << stereoAngle << '\n'
//           << "          moduleOffsetX = " << moduleOffsetX << '\n'
//           << "          sensorOffsetY = " << sensorOffsetY << '\n'
//           << "          rowOffsetY    = " << rowOffsetY << '\n'
//           << "          planeZ.size   = " << planeZ.size() << '\n'
//           << "          planeIndices_front.size   = " << planeIndices_front.size() << '\n'
//           << "          planeIndices_central.size = " << planeIndices_central.size() << '\n'
//           << "          planeIndices_end.size     = " << planeIndices_end.size() << '\n'
//           << '\n'
//           << "       Not writing to file!\n"
//           << '\n';
//      return;
//  }
//
//  TFile outputFile {fileName.c_str(), "recreate"};
//  outputFile.cd();
//  TTree outputTree {"faserGeo", "faserGeo"};
//
//  outputTree.Branch("nStrips"       , &nStrips       , "nStrips/I");
//  outputTree.Branch("stripPitch"    , &stripPitch    , "stripPitch/D");
//  outputTree.Branch("stereoAngle"   , &stereoAngle   , "stereoAngle/D");
//  outputTree.Branch("moduleOffsetX" , &moduleOffsetX , "moduleOffsetX/D");
//  outputTree.Branch("sensorOffsetY" , &sensorOffsetY , "sensorOffsetY/D");
//  outputTree.Branch("rowOffsetY"    , &rowOffsetY    , "rowOffsetY/D");
//  outputTree.Branch("planeIndices_front", "std::vector<int>", &p_planeIndices_front);
//  outputTree.Branch("planeIndices_central", "std::vector<int>", &p_planeIndices_central);
//  outputTree.Branch("planeIndices_end", "std::vector<int>", &p_planeIndices_end);
//  outputTree.Branch("planeZ", "std::vector<double>", &p_planeZ);
//
//  outputTree.Fill();
//  outputTree.Write();
//  outputFile.Close();
//}
//
