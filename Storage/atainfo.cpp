
#include "atainfo.h"
#include "ataattribute.h"
#include"drivesmartinfo.h"




bool ATAInfo:: supportsATA(int index){
    QString command = "smartctl.exe -i /dev/sd" + QString(QChar('a' + index));

    QProcess process;
    process.setProgram("smartctl.exe");
    process.setArguments(QStringList() << "-i" << "/dev/sd" + QString(QChar('a' + index)));
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


    bool containsString = output.contains("=== START OF INFORMATION SECTION ===");

    return containsString;
}

QVector<ATAAttribute> ATAInfo:: getAtaAttributes(int index) {
    DriveSmartInfo* driveSmartInfo = new DriveSmartInfo();

    QVector<ATAAttribute> attributes;
    QString command = "smartctl.exe -x /dev/sd" + QString(QChar('a' + index));

    QProcess process;
    process.setProgram("smartctl.exe");
    process.setArguments(QStringList() << "-x" << "/dev/sd" + QString(QChar('a' + index)));
    process.start();

    if (!process.waitForStarted()) {
        qCritical() << "Error executing smartctl.exe command.";
        return QVector<ATAAttribute>();
    }
    if (!process.waitForFinished()) {
        qCritical() << "Error reading smartctl.exe output.";
        return QVector<ATAAttribute>();
    }

    QString output = process.readAllStandardOutput();
    // Rest of the code to process the output

    QStringList lines = output.split("\n");
    bool startParsing = false;
    foreach (const QString& line, lines) {
        QString trimmedLine = line.trimmed();

        if (startParsing) {
            // Check if the line is empty
            if (trimmedLine.isEmpty()) {
                break; // Exit the loop if the line is empty
            }

            // Extract attribute name and value
            int colonPos = trimmedLine.indexOf(":");
            if (colonPos != -1) {
                QString attributeName = trimmedLine.left(colonPos).trimmed();
                QString attributeValue = trimmedLine.mid(colonPos + 1).trimmed();
                attributeName = driveSmartInfo->trim(attributeName);
                attributeValue = driveSmartInfo->trim(attributeValue);
                attributeValue.replace(QChar(0xFFFD), ' ');

                // Create and add the attribute to the current physical drive
                ATAAttribute attribute;
                attribute.setName(attributeName.toStdString());
                attribute.setValue(attributeValue.toStdString());

                attributes.append(attribute);
            }
        }
        else if (trimmedLine.contains("=== START OF INFORMATION SECTION ===")) {
            startParsing = true;
        }
    }

    return attributes;
}
