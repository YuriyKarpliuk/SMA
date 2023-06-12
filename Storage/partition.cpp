
#include "partition.h"

string Partition:: getType(){
    return type;
}
int Partition:: getNumber(){
    return number;
}
wstring Partition:: getDrive(){
    return drive;
}
int Partition:: getSize(){
    return size;
}
int Partition:: getStartOffset(){
    return startOffset;
}
void Partition::setType(string type){
    this->type=type;
}
void Partition::setStartOffset(int startOffset){
    this->startOffset=startOffset;
}
void Partition::setSize(int size){
    this->size=size;
}
void Partition::setNumber(int number){
    this->number=number;
}
void Partition::setDrive(wstring drive){
    this->drive=drive;
}
