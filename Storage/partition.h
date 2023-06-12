
#ifndef PARTITION_H
#define PARTITION_H

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



class Partition
{
public:
    string getType();
    int getNumber();
    wstring getDrive();
    int getSize();
    int getStartOffset();


    void setType(string type);
    void setDrive(wstring drive);
    void setNumber(int number);
    void setSize(int size);
    void setStartOffset(int startOffset);
private:
    string type;
    int number;
    wstring drive;
    int startOffset;
    int size;

};

#endif // PARTITION_H
