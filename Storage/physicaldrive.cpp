
#include "physicaldrive.h"

void PhysicalDrive::setDriveName(wstring driveName){
    this->driveName=driveName;
}
void PhysicalDrive::setTotalSpace(int totalSpace){
    this->totalSpace=totalSpace;
}
void PhysicalDrive::setVolumeSerialNumber(wstring volumeSerialNumber){
    this->volumeSerialNumber=volumeSerialNumber;
}

wstring PhysicalDrive::getDriveName(){
    return driveName;
}


int PhysicalDrive::getTotalSpace(){
    return totalSpace;
}
wstring PhysicalDrive::getVolumeSerialNumber(){
    return volumeSerialNumber;
}

vector<Partition>PhysicalDrive:: getPartitions() {
    return partitions;
}



void PhysicalDrive:: setPartitions(vector<Partition>& partitions) {
    this->partitions = partitions;
}




