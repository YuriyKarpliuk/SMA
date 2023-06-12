#include "LogicalDriveInfo.h"

vector<LogicalDrive> LogicalDriveInfo::LogicalDrivesInfo()
{

    vector<LogicalDrive> logicalDrives;
    DWORD dwSize = GetLogicalDriveStrings(0, NULL);
    if (dwSize == 0) {
        cerr << "Error: GetLogicalDriveStrings failed (" << GetLastError() << ")" << endl;

    }

    wchar_t* buffer = new wchar_t[dwSize];
    if (GetLogicalDriveStrings(dwSize, buffer) == 0) {
        cerr << "Error: GetLogicalDriveStrings failed (" << GetLastError() << ")" << endl;
        delete[] buffer;

    }

    wchar_t* ptr = buffer;
    while (*ptr) {
        LogicalDrive logicalDrive;
        wstring driveName = wstring(ptr, 3);
        wstring driveType;

        switch (GetDriveType(driveName.c_str())) {
        case DRIVE_UNKNOWN:
            driveType = L"Unknown";
            break;
        case DRIVE_NO_ROOT_DIR:
            driveType = L"No root directory";
            break;
        case DRIVE_REMOVABLE:
            driveType = L"Removable";
            break;
        case DRIVE_FIXED:
            driveType = L"Local Disk";
            break;
        case DRIVE_REMOTE:
            driveType = L"Remote";
            break;
        case DRIVE_CDROM:
            driveType = L"CD/DVD";
            logicalDrive.setTotalSpace(0);
            logicalDrive.setUsedSpace(0);
            logicalDrive.setFreeSpace(0);
            logicalDrive.setFreeSpacePercentage(0);
            logicalDrive.setFileSystemName(L"");
            logicalDrive.setVolumeSerialNumber(L"");
            break;
        case DRIVE_RAMDISK:
            driveType = L"RAM disk";
            break;
        }

        logicalDrive.setDriveName(driveName);
        logicalDrive.setDriveType(driveType);



        if (driveType != L"CD/DVD") {
             ULARGE_INTEGER freeSpace, totalSpace;
            if (GetDiskFreeSpaceEx(driveName.c_str(), NULL, &totalSpace, &freeSpace) == 0) {
                cerr << "Error: GetDiskFreeSpaceEx failed (" << GetLastError() << ")" << endl;
                ptr += wcslen(ptr) + 1;
                continue;
            }

            DWORD volumeSerialNumber, maxComponentLength, fileSystemFlags;


            wchar_t fileSystemNameBuffer[MAX_PATH];
            if (GetVolumeInformation(driveName.c_str(), NULL, 0, &volumeSerialNumber, &maxComponentLength, &fileSystemFlags, fileSystemNameBuffer, MAX_PATH) == 0) {
                cerr << "Error: GetVolumeInformation failed (" << GetLastError() << ")" << endl;
                ptr += wcslen(ptr) + 1;
                continue;
            }

            wstringstream ss;
            ss << setfill(L'0') << std::setw(4) << std::hex << HIWORD(volumeSerialNumber) << L"-";
            ss << setfill(L'0') << std::setw(4) << std::hex << LOWORD(volumeSerialNumber);
            wstring volumeSerial = ss.str();
            logicalDrive.setTotalSpace(totalSpace.QuadPart / (1024 * 1024));
            logicalDrive.setUsedSpace(totalSpace.QuadPart / (1024 * 1024) - freeSpace.QuadPart / (1024 * 1024));
            logicalDrive.setFreeSpace(freeSpace.QuadPart / (1024 * 1024));
            logicalDrive.setFreeSpacePercentage(round(100 * ((double)(freeSpace.QuadPart / (1024 * 1024)) / (double)(totalSpace.QuadPart / (1024 * 1024)))));
            logicalDrive.setVolumeSerialNumber(volumeSerial);
            logicalDrive.setFileSystemName(fileSystemNameBuffer);

        }
        logicalDrives.push_back(logicalDrive);






        ptr += wcslen(ptr) + 1;
    }

    delete[] buffer;

    return logicalDrives;
}
