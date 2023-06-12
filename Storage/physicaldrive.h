
#ifndef PHYSICALDRIVE_H
#define PHYSICALDRIVE_H

#include <string>
#include <iostream>
#include <Windows.h>
#include <winioctl.h>
#include <vector>
#include <ntdddisk.h>
#include <ntddscsi.h>
#include <cmath>
#include <sstream>
#include <iomanip>
#include <locale>
#include <codecvt>
#include"partition.h"
#include "smartattribute.h"

using namespace std;

class PhysicalDrive
{
public:
    wstring getDriveName();
    int getTotalSpace();
    wstring getVolumeSerialNumber();
    vector<Partition> getPartitions();

    void setPartitions( vector<Partition>& partitions);
    void setDriveName(wstring name);
    void setTotalSpace(int size);
    void setVolumeSerialNumber(wstring serial);

private:
    wstring driveName;
    int totalSpace;
    wstring volumeSerialNumber;
    vector<Partition> partitions;
};

#endif // PHYSICALDRIVE_H
