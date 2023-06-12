
#ifndef LOGICALDRIVE_H
#define LOGICALDRIVE_H
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


using namespace std;

class LogicalDrive
{
public:
    wstring getDriveName();
    wstring getDriveType();
    int getTotalSpace();
    int getUsedSpace();
    int getFreeSpace();
    int getFreeSpacePercentage();
    wstring getVolumeSerialNumber();
    wstring getFileSystemName();


    void setDriveName(wstring name);
    void setDriveType(wstring type);
    void setTotalSpace(int size);
    void setUsedSpace(int size);
    void setFreeSpace(int size);
    void setFreeSpacePercentage(int percentage);
    void setVolumeSerialNumber(wstring serial);
    void setFileSystemName(wstring name);

private:
        wstring driveName;
        wstring driveType;
        int totalSpace;
        int usedSpace;
        int freeSpace;
        int freeSpacePercentage;
        wstring volumeSerialNumber;
        wstring fileSystemName;

};

#endif // LOGICALDRIVE_H
