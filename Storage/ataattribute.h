
#ifndef ATAATTRIBUTE_H
#define ATAATTRIBUTE_H


#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unordered_set>
#include <algorithm>
#include <locale>
using namespace std;

class ATAAttribute
{
public:
    string getName();
    void setName(string name);

    string getValue();
    void setValue(string value);
private:
    string name;
    string value;

};
#endif // ATAATTRIBUTE_H
