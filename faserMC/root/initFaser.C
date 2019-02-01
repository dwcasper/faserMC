void initFaser()
{
  if (!TClassTable::GetDict("FaserEvent"))
  {
    string libDir = string(getenv("FASERMC_DIR")) + "/faserMC_run/lib64/";
    string libFaserRootClasses = libDir + "libFaserRootClassesDict.so";

    gSystem->Load(libFaserRootClasses.c_str());
  }
}

