
#include "smartattribute.h"

string SmartAttribute:: getName(){
    return name;
}
string SmartAttribute:: getValue(){
    return value;
}
void SmartAttribute::setName(string name){
    this->name=name;
}
void SmartAttribute::setValue(string value){
    this->value=value;
}
string SmartAttribute:: getWorstValue(){
    return worstValue;
}
void SmartAttribute::setWorstValue(string worstValue){
    this->worstValue=worstValue;
}
string SmartAttribute:: getRawValue(){
    return rawValue;
}
void SmartAttribute::setRawValue(string rawValue){
    this->rawValue=rawValue;
}
string SmartAttribute:: getType(){
    return type;
}
void SmartAttribute::setType(string type){
    this->type=type;
}

