void initFaser()
{
  if (!TClassTable::GetDict("FaserEvent"))
  {
    gSystem->Load("/home/dcasper/faser/run/lib/libFaserRootClassesDict.so");
  }
}

