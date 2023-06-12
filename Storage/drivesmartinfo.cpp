#include "drivesmartinfo.h"


QString DriveSmartInfo:: trim(const QString& str) {
    int first = str.indexOf(QRegularExpression("[^\\s]"));
    if (first == -1) {
        return "";
    }
    int last = str.lastIndexOf(QRegularExpression("[^\\s]"));
    return str.mid(first, (last - first + 1));
}


QMap<int, QString> DriveSmartInfo:: getDriveTypes()
{
    QMap<int, QString> driveTypes;

    QProcess process;
    process.setProgram("smartctl");
    process.setArguments(QStringList() << "--scan");
    process.start();

    if (!process.waitForStarted()) {
        qCritical() << "Error executing smartctl command.";
        return QMap<int, QString>();
    }
    if (!process.waitForFinished()) {
        qCritical() << "Error reading smartctl output.";
        return QMap<int, QString>();
    }

    QString output = process.readAllStandardOutput();

    QStringList lines = output.split("\n");

    int index = 0;
    foreach (QString line, lines) {
        if (line.startsWith("/dev/")) {
            QString driveType = line.section("#", 1).trimmed();
            driveType = driveType.split(',').last().trimmed(); // Extract the last part after comma
            driveTypes.insert(index, driveType);
            ++index;
        }
    }

    return driveTypes;
}

bool  DriveSmartInfo:: supportsSMART(int index){
    QString command = "smartctl.exe -A /dev/sd" + QString(QChar('a' + index));

    QProcess process;
    process.setProgram("smartctl.exe");
    process.setArguments(QStringList() << "-A" << "/dev/sd" + QString(QChar('a' + index)));
    process.start();

    if (!process.waitForStarted()) {
        qCritical() << "Error executing smartctl.exe command.";
        return false;
    }
    if (!process.waitForFinished()) {
        qCritical() << "Error reading smartctl.exe output.";
        return false;
    }

    QString output = process.readAllStandardOutput();


    bool containsString = output.contains("=== START OF SMART DATA SECTION ===") || output.contains("=== START OF READ SMART DATA SECTION ===");


    return containsString;
}

QString  DriveSmartInfo::getTestResult(int index) {
    QString command = "smartctl.exe -H /dev/sd" + QString(QChar('a' + index));

    QProcess process;
    process.setProgram("smartctl.exe");
    process.setArguments(QStringList() << "-H" << "/dev/sd" + QString(QChar('a' + index)));
    process.start();

    if (!process.waitForStarted()) {
        qCritical() << "Error executing smartctl.exe command.";
        return "";
    }
    if (!process.waitForFinished()) {
        qCritical() << "Error reading smartctl.exe output.";
        return "";
    }

    QString output = process.readAllStandardOutput();

    QStringList lines = output.split("\n");
    QString resultValue;
    bool startParsing = false;
    foreach (const QString& line, lines) {
        QString trimmedLine = line.trimmed();

        if (startParsing) {
            resultValue += line + "\n";
        }

        if (trimmedLine.contains("=== START OF SMART DATA SECTION ===") || trimmedLine.contains("=== START OF READ SMART DATA SECTION ===")) {
            startParsing = true;
        }
    }

    return resultValue;
}


QVector<SmartAttribute>  DriveSmartInfo:: getSMARTAttributes(int index) {
    QVector<SmartAttribute> attributes;
    QString command = "smartctl.exe -A /dev/sd" + QString(QChar('a' + index));

    QProcess process;
    process.setProgram("smartctl.exe");
    process.setArguments(QStringList() << "-A" << "/dev/sd" + QString(QChar('a' + index)));
    process.start();

    if (!process.waitForStarted()) {
        qCritical() << "Error executing smartctl.exe command.";
        return QVector<SmartAttribute>();
    }
    if (!process.waitForFinished()) {
        qCritical() << "Error reading smartctl.exe output.";
        return QVector<SmartAttribute>();
    }

    QString output = process.readAllStandardOutput();
    // Rest of the code to process the output

    QStringList lines = output.split("\n");
    bool startParsing = false;
    QString attributeName; // Store the current attribute name
    QString attributeValue; // Store the current attribute value
      QString attributeWorst;
      QString attributeType;
        QString attributeRawValue;
      if(getDriveTypes().value(index)=="NVMe device"){
        QString empty="";
        foreach (const QString& line, lines) {
            QString trimmedLine = line.trimmed();

            if (startParsing) {
                trimmedLine.remove(QChar(0xA0));

                // Extract attribute name and value
                int colonPos = trimmedLine.indexOf(":");
                if (colonPos != -1) {
                    attributeName = trimmedLine.left(colonPos);
                    attributeValue = trimmedLine.mid(colonPos + 1);
                    // Remove leading/trailing spaces from attribute name and value
                    attributeName = trim(attributeName);
                    attributeValue = trim(attributeValue);
                    attributeValue.replace(QChar(0xFFFD), ' ');

                    // Create and add the attribute to the current physical drive
                    SmartAttribute attribute;
                    attribute.setName(attributeName.toStdString());
                    attribute.setValue(attributeValue.toStdString());
                    attribute.setWorstValue(empty.toStdString());
                    attribute.setType(empty.toStdString());
                    attribute.setRawValue(empty.toStdString());
                    attributes.append(attribute);
                }
            }
            else if (trimmedLine.contains("=== START OF SMART DATA SECTION ===")) {
                startParsing = true;
            }
        }
      }
      else{
        QStringList lines = output.split("\n");
        bool startParsing = false;

        foreach (const QString& line, lines) {
            QString trimmedLine = line.trimmed();

            if (startParsing) {
                QStringList columns = trimmedLine.split(" ", Qt::SkipEmptyParts);
                if (columns.size() >= 10) {
                    QString attributeName = columns[1];
                    QString attributeValue = columns[2];
                    QString attributeWorst = columns[3];
                    QString attributeType = columns[5];
                    QString attributeRawValue = columns[9];

                    SmartAttribute attribute;
                    attribute.setName(attributeName.toStdString());
                    attribute.setValue(attributeValue.toStdString());
                    attribute.setWorstValue(attributeWorst.toStdString());
                    attribute.setType(attributeType.toStdString());
                    attribute.setRawValue(attributeRawValue.toStdString());

                    attributes.append(attribute);
                }
            } else if (trimmedLine.contains("ID# ATTRIBUTE_NAME          FLAG     VALUE WORST THRESH TYPE      UPDATED  WHEN_FAILED RAW_VALUE")) {
                startParsing = true;
            }
        }


      }



    return attributes;
}




