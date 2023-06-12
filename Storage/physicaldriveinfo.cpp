
#include "physicaldriveinfo.h"
static const GUID PARTITION_BASIC_DATA_GUID = { 0xEBD0A0A2, 0xB9E5, 0x4433, { 0x87, 0xC0, 0x68, 0xB6, 0xB7, 0x26, 0x99, 0xC7 } };
static const GUID PARTITION_ENTRY_UNUSED_GUID = { 0x00000000, 0x0000, 0x0000, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } };
static const GUID PARTITION_SYSTEM_GUID = { 0xC12A7328, 0xF81F, 0x11D2, { 0xBA, 0x4B, 0x00, 0xA0, 0xC9, 0x3E, 0xC9, 0x3B } };
static const GUID PARTITION_MSFT_RESERVED_GUID = { 0xE3C9E316, 0x0B5C, 0x4DB8, { 0x81, 0x7D, 0xF9, 0x2D, 0xF0, 0x02, 0x15, 0xAE } };
static const GUID PARTITION_LDM_METADATA_GUID = { 0x5808C8AA, 0x7E8F, 0x42E0, { 0x85, 0xD2, 0xE1, 0xE9, 0x04, 0x34, 0xCF, 0xB3 } };
static const GUID PARTITION_LDM_DATA_GUID = { 0xAF9B60A0, 0x1431, 0x4F62, { 0xBC, 0x68, 0x33, 0x11, 0x71, 0x4A, 0x69, 0xAD } };
static const GUID PARTITION_MSFT_RECOVERY_GUID = { 0xDE94BBA4, 0x06D1, 0x4D40, { 0xA1, 0x6A, 0xBF, 0xD5, 0x01, 0x79, 0xD6, 0xAC } };


string PhysicalDriveInfo:: GUIDToString(const GUID& guid) {
    char buffer[39];
    sprintf_s(buffer,
              sizeof(buffer),
              "%08lx-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",
              guid.Data1, guid.Data2, guid.Data3,
              guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3],
              guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]);
    return buffer;
}

vector<PhysicalDrive>  PhysicalDriveInfo:: PhysicalDrivesInfo(){


   vector<Partition> partitions;
    // Get a list of all physical drives
    std::vector<std::wstring> physicalDrives;
    for (int i = 0; i < 256; i++) {
        std::wstring drivePath = L"\\\\.\\PhysicalDrive" + std::to_wstring(i);
        HANDLE hDrive = CreateFile(drivePath.c_str(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, 0, nullptr);
        if (hDrive != INVALID_HANDLE_VALUE) {
            physicalDrives.push_back(drivePath);
            CloseHandle(hDrive);
        }
    }
    vector<PhysicalDrive> physicalDrivesInfo(physicalDrives.size());

    int j=0;
    // Loop through each physical drive and retrieve its partition information
    for (const auto& drivePath : physicalDrives) {

        if(partitions.size()!=0){
            partitions.clear();
        }
        HANDLE hDrive = CreateFile(drivePath.c_str(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, 0, nullptr);
        if (hDrive == INVALID_HANDLE_VALUE) {
            std::wcerr << L"Error: could not open physical drive " << drivePath << std::endl;
            continue;
        }




        // Get the partition information for the drive
        DWORD bytesReturned = 0;
        GET_LENGTH_INFORMATION lengthInfo = { 0 };
        if (!DeviceIoControl(hDrive, IOCTL_DISK_GET_LENGTH_INFO, nullptr, 0, &lengthInfo, sizeof(lengthInfo), &bytesReturned, nullptr)) {
            std::wcerr << L"Error: could not get length information for " << drivePath << std::endl;
            CloseHandle(hDrive);
            continue;
        }

        DRIVE_LAYOUT_INFORMATION_EX* driveLayout = nullptr;
        DWORD bufferSize = sizeof(DRIVE_LAYOUT_INFORMATION_EX) + sizeof(PARTITION_INFORMATION_EX) * 128;
        driveLayout = (DRIVE_LAYOUT_INFORMATION_EX*)malloc(bufferSize);
        if (!DeviceIoControl(hDrive, IOCTL_DISK_GET_DRIVE_LAYOUT_EX, nullptr, 0, driveLayout, bufferSize, &bytesReturned, nullptr)) {
            std::wcerr << L"Error: could not get partition information for " << drivePath << std::endl;
            free(driveLayout);
            CloseHandle(hDrive);
            continue;
        }

        STORAGE_PROPERTY_QUERY query = { StorageDeviceProperty, PropertyStandardQuery };
        char buffer[1024] = { 0 };
        if (!DeviceIoControl(hDrive, IOCTL_STORAGE_QUERY_PROPERTY, &query, sizeof(query), buffer, sizeof(buffer), nullptr, nullptr)) {
            std::wcerr << L"Error: could not query storage device property for " << drivePath << std::endl;
            CloseHandle(hDrive);
            continue;
        }

        DISK_GEOMETRY_EX diskGeometry = { 0 };
        if (!DeviceIoControl(hDrive, IOCTL_DISK_GET_DRIVE_GEOMETRY_EX, nullptr, 0, &diskGeometry, sizeof(diskGeometry), &bytesReturned, nullptr)) {
            std::wcerr << L"Error: could not get disk geometry for " << drivePath << std::endl;
            CloseHandle(hDrive);
            continue;
        }
        ULONGLONG diskSize = diskGeometry.DiskSize.QuadPart / (1024 * 1024 * 1024);

        STORAGE_DEVICE_DESCRIPTOR* descriptor = reinterpret_cast<STORAGE_DEVICE_DESCRIPTOR*>(buffer);
        std::string productName(descriptor->ProductIdOffset + buffer);

        char* serialNumber = buffer + descriptor->SerialNumberOffset;
        std::string serialNumberStr(serialNumber);

        wstring_convert<codecvt_utf8_utf16<wchar_t>> converter;
        wstring productNameStr = converter.from_bytes(productName);

        wstringstream ws;
        ws << serialNumber;
       wstring serialNumberWstr = ws.str();

        physicalDrivesInfo[j].setDriveName(productNameStr);
        physicalDrivesInfo[j].setTotalSpace(diskSize);
        physicalDrivesInfo[j].setVolumeSerialNumber(serialNumberWstr);

        for (DWORD i = 0; i < driveLayout->PartitionCount; i++) {

            const PARTITION_INFORMATION_EX& partitionInfo = driveLayout->PartitionEntry[i];
            Partition partition;


            if (partitionInfo.PartitionStyle == PARTITION_STYLE_MBR) {
                BYTE systemId = partitionInfo.Mbr.PartitionType;

                switch (systemId) {
                case 0x00:
                       partition.setType("Unused");
                    break;
                case 0x01:
                      partition.setType("FAT12");
                    break;
                case 0x04:
                      partition.setType("FAT16");
                    break;
                case 0x05:
                    partition.setType("Extended");
                    break;
                case 0x07:
                     partition.setType("NTFS");
                    break;
                case 0x0B:
                    partition.setType("FAT32");
                    break;
                case 0xC0:
                    partition.setType("Valid NTFT");
                    break;
                case 0x80:
                    partition.setType("NTFT");
                    break;
                case 0x0F:
                    partition.setType("Extended LBA");
                    break;
                case 0x42:
                    partition.setType("Logical Dynamic Disk");
                    break;
                default:
                    partition.setType("Unknown");
                    break;
                }
                 if (partition.getType() != "Unused" && partition.getSize()!=0) {
                partition.setNumber(i+1);
                partition.setStartOffset(partitionInfo.StartingOffset.QuadPart / (1024 * 1024));
                partition.setSize(partitionInfo.PartitionLength.QuadPart / (1024 * 1024));
                partitions.push_back(partition);
                 }
            }
            else if (partitionInfo.PartitionStyle == PARTITION_STYLE_GPT) {

                GUID partitionType = partitionInfo.Gpt.PartitionType;

                if (partitionType == PARTITION_BASIC_DATA_GUID) {
                    partition.setType("Basic Data");

                    WCHAR volumeName[MAX_PATH] = { 0 };
                    string guidString = GUIDToString(partitionInfo.Gpt.PartitionId);
                    wstring volumeNamePrefix = L"\\\\?\\Volume{";
                    wstring volumeNameSuffix = L"}\\";
                    wstring volumeNameString = volumeNamePrefix + std::wstring(guidString.begin(), guidString.end()) + volumeNameSuffix;
                    lstrcpyW(volumeName, volumeNameString.c_str());

                    DWORD requiredLength = 0;
                    if (!GetVolumePathNamesForVolumeNameW(volumeName, nullptr, 0, &requiredLength)) {
                        if (GetLastError() != ERROR_MORE_DATA) {
                            std::cerr << "Failed to get volume path names. Error code: " << GetLastError() << std::endl;
                        }
                    }

                    std::vector<wchar_t> buffer(requiredLength);
                    if (!GetVolumePathNamesForVolumeNameW(volumeName, buffer.data(), buffer.size(), &requiredLength)) {
                        std::cerr << "Failed to get volume path names. Error code: " << GetLastError() << std::endl;
                    }

                    for (const wchar_t* path = buffer.data(); *path; path += wcslen(path) + 1) {
                        partition.setDrive(path);
                    }

                }
                else if (partitionType == PARTITION_ENTRY_UNUSED_GUID) {
                     partition.setType("Unused");
                }
                else if (partitionType == PARTITION_SYSTEM_GUID) {
                      partition.setType("EFI System");
                }
                else if (partitionType == PARTITION_MSFT_RESERVED_GUID) {
                     partition.setType("Microsoft Reserved");
                }
                else if (partitionType == PARTITION_LDM_METADATA_GUID) {
                     partition.setType("LDM Metadata");
                }
                else if (partitionType == PARTITION_LDM_DATA_GUID) {
                     partition.setType("LDM data");
                }
                else if (partitionType == PARTITION_MSFT_RECOVERY_GUID) {
                     partition.setType("MS Recovery");
                }
                else {
                     partition.setType("Unknown");
                }

                partition.setNumber(i+1);
                partition.setStartOffset(partitionInfo.StartingOffset.QuadPart / (1024 * 1024));
                partition.setSize(partitionInfo.PartitionLength.QuadPart / (1024 * 1024));
                partitions.push_back(partition);

            }
           physicalDrivesInfo[j].setPartitions(partitions);


        }
        free(driveLayout);
        CloseHandle(hDrive);
        j++;
    }
    return physicalDrivesInfo;
}
