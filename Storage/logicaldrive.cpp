
#include "logicaldrive.h"


void LogicalDrive::setDriveName(wstring driveName){
    this->driveName=driveName;
}
void LogicalDrive::setDriveType(wstring driveType){
    this->driveType=driveType;
}
void LogicalDrive::setFreeSpace(int freeSpace){
    this->freeSpace=freeSpace;
}
void LogicalDrive::setFreeSpacePercentage(int freeSpacePercentage){
    this->freeSpacePercentage=freeSpacePercentage;
}
void LogicalDrive::setFileSystemName(wstring fileSystemName){
    this->fileSystemName=fileSystemName;
}
void LogicalDrive::setUsedSpace(int usedSpace){
    this->usedSpace=usedSpace;
}
void LogicalDrive::setTotalSpace(int totalSpace){
    this->totalSpace=totalSpace;
}
void LogicalDrive::setVolumeSerialNumber(wstring volumeSerialNumber){
    this->volumeSerialNumber=volumeSerialNumber;
}

wstring LogicalDrive::getDriveName(){
    return driveName;
}

wstring LogicalDrive::getDriveType(){
    return driveType;
}
int LogicalDrive::getFreeSpace(){
    return freeSpace;
}
int LogicalDrive::getFreeSpacePercentage(){
    return freeSpacePercentage;
}
int LogicalDrive::getUsedSpace(){
    return usedSpace;
}
int LogicalDrive::getTotalSpace(){
    return totalSpace;
}
wstring LogicalDrive::getVolumeSerialNumber(){
    return volumeSerialNumber;
}
wstring LogicalDrive::getFileSystemName(){
    return fileSystemName;
}


