
#ifndef SMARTATTRIBUTE_H
#define SMARTATTRIBUTE_H

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unordered_set>
#include <algorithm>
#include <locale>
using namespace std;

class SmartAttribute
{
public:
    string getName();
    void setName(string name);
    string getType();
    void setType(string type);
    string getWorstValue();
    void setWorstValue(string worstValue);
    string getRawValue();
    void setRawValue(string rawValue);
    string getValue();
    void setValue(string value);
private:
    string name;
    string value;
    string worstValue;
    string type;
    string rawValue;
};

#endif // SMARTATTRIBUTE_H
