
#ifndef ATAINFO_H
#define ATAINFO_H


#include "ataattribute.h"
#include <string>
#include <iostream>
#include <vector>
#include "smartattribute.h"
#include <cstdlib>
#include <QVector>
#include <QString>
#include <QProcess>
#include <QTextStream>
#include <QDebug>
#include <QRegularExpression>
class ATAInfo
{
public:
    QVector<ATAAttribute> getAtaAttributes(int index);
    bool supportsATA(int index);

};

#endif // ATAINFO_H
