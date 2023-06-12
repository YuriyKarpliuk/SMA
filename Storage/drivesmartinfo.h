
#ifndef DRIVESMARTINFO_H
#define DRIVESMARTINFO_H

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
class DriveSmartInfo
{
public:
    QVector<SmartAttribute> getSMARTAttributes(int index);
    QString getTestResult(int index);
    QMap<int, QString> getDriveTypes();
    QString trim(const QString& str);
    bool supportsSMART(int index);
};

#endif // DRIVESMARTINFO_H
