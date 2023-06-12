
#ifndef PHYSICALDRIVEINFO_H
#define PHYSICALDRIVEINFO_H
#include "physicaldrive.h"

class PhysicalDriveInfo
{
public:
  vector<PhysicalDrive> PhysicalDrivesInfo();
    string GUIDToString(const GUID& guid);
};


#endif // PHYSICALDRIVEINFO_H
