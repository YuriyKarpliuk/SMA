
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "../Storage/LogicalDriveInfo.h"
#include "../Storage/PhysicalDriveInfo.h"
#include "../Storage/LogicalDrive.h"
#include "../Storage/DriveSmartInfo.h"
#include "../Storage/Partition.h"
#include "../Storage/ataattribute.h"
#include "../Storage/atainfo.h"
#include <QPixmap>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QPixmap pix(":/logo.png");

 ui->logo->setPixmap(pix.scaled(100,100,Qt::KeepAspectRatio));
    ui->picture_label->setPixmap(pix.scaled(150,150,Qt::KeepAspectRatio));
     ui->label_12->setPixmap(pix.scaled(150,150,Qt::KeepAspectRatio));
    ui->label_14->setPixmap(pix.scaled(150,150,Qt::KeepAspectRatio));
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::updateWindowTitle(const QString& pageTitle) {
    QString fullTitle = QString("Storage Management - %1").arg(pageTitle);


    setWindowTitle(fullTitle);
}


void MainWindow::on_logical_drive_clicked()
{
 ui->stackedWidget->setCurrentIndex(1);
    updateWindowTitle("Logical Drive");
 TableWidgetDisplay();

}
void MainWindow::on_physical_drive_clicked()
{
 ui->stackedWidget->setCurrentIndex(2);
 updateWindowTitle("Physical Drive");
 ListWidgetDisplay();
 TableWidget1Display(0);
}
void MainWindow::on_smart_clicked()
{
 ui->stackedWidget->setCurrentIndex(3);
 updateWindowTitle("Smart");
 ListWidget2Display();
 PhysicalDriveInfo* physicalDriveInfo = new PhysicalDriveInfo();
DriveSmartInfo* driveSmartInfo = new DriveSmartInfo();
 vector<PhysicalDrive> physicalDrives=physicalDriveInfo-> PhysicalDrivesInfo();
 for (int i = 0; i < physicalDrives.size();i++) {
     if(driveSmartInfo->supportsSMART(i)){
        TableWidget3Display(i);
         break;
     }

 }

}
void MainWindow::on_ata_clicked()
{
 ui->stackedWidget->setCurrentIndex(7);
 updateWindowTitle("ATA");
 ListWidget3Display();
 PhysicalDriveInfo* physicalDriveInfo = new PhysicalDriveInfo();
 ATAInfo* ataInfo = new ATAInfo();
 vector<PhysicalDrive> physicalDrives=physicalDriveInfo-> PhysicalDrivesInfo();
 for (int i = 0; i < physicalDrives.size();i++) {
     if(ataInfo->supportsATA(i)){
         TableWidget3Display(i);
         break;
     }

 }

}


void MainWindow::closeEvent(QCloseEvent* event)
{
    QMainWindow::closeEvent(event);
    qApp->quit();
}

void MainWindow::TableWidget4Display(int index)
{


    ui->tableWidget_4->setRowCount(0);
    ui->tableWidget_4->setColumnCount(2);

    QStringList hLabels;
    hLabels << "Name" << "Value";
    ui->tableWidget_4->setHorizontalHeaderLabels(hLabels);

    for (int i = 0; i < ui->tableWidget_4->columnCount(); i++) {
        ui->tableWidget_4->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Fixed);
        ui->tableWidget_4->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    }
 ATAInfo* ataInfo = new ATAInfo();

    QVector<ATAAttribute> attributes = ataInfo->getAtaAttributes(index);


    for (int i = 0; i < attributes.size(); i++) {
        ui->tableWidget_4->insertRow(i);
        QTableWidgetItem* attribute = new QTableWidgetItem;
        QTableWidgetItem* value = new QTableWidgetItem;


        attribute->setText(QString::fromStdString(attributes[i].getName()));
        value->setText(QString::fromStdString(attributes[i].getValue()));

        ui->tableWidget_4->setItem(i , 0, attribute);
        ui->tableWidget_4->setItem(i, 1, value);
    }

    ui->tableWidget_4->resizeRowsToContents();

    ui->tableWidget_4->resizeColumnToContents(0);
     ui->tableWidget_4->resizeColumnToContents(1);


}
void MainWindow::ListWidget3Display()
{
    ui->listWidget_3->clear();
      PhysicalDriveInfo* physicalDriveInfo = new PhysicalDriveInfo();
    ATAInfo* ataInfo = new ATAInfo();

    vector<PhysicalDrive> physicalDrives= physicalDriveInfo->PhysicalDrivesInfo();
    string driveTemplate="";
    for (int i = 0; i < physicalDrives.size();i++) {
        if(ataInfo->supportsATA(i)){
        wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
        string driveName = converter.to_bytes(physicalDrives[i].getDriveName());
        driveTemplate = driveName;
        ui->listWidget_3->addItem(QString::fromStdString(driveTemplate));
        }
    }

    connect(ui->listWidget_3, &QListWidget::itemSelectionChanged, this, &MainWindow::onList3ItemSelectionChanged);
}

void MainWindow::TableWidgetDisplay()
{

    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnCount(8);

    QStringList hLabels;
    hLabels<<"Drive"<<"Drive Type"<<"File System"<<"Total Size, MB"<<"Used Space, MB"<<"Free Space, MB"<<"Free, %"<<"Volume Serial";
    ui->tableWidget->setHorizontalHeaderLabels(hLabels);
    for (int i = 0; i < ui->tableWidget->columnCount(); i++) {
        ui->tableWidget->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Fixed);
        ui->tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    }
    LogicalDriveInfo* logicalDriveInfo = new LogicalDriveInfo();
    vector<LogicalDrive> lDrives = logicalDriveInfo->LogicalDrivesInfo();

    for (int i = 0; i < lDrives.size();i++) {
        ui->tableWidget->insertRow(i);
        QTableWidgetItem *driveName =new QTableWidgetItem;
        QTableWidgetItem *driveType =new QTableWidgetItem;
        QTableWidgetItem *fileSystem =new QTableWidgetItem;
        QTableWidgetItem *totalSize =new QTableWidgetItem;
        QTableWidgetItem *usedSpace =new QTableWidgetItem;
        QTableWidgetItem *freeSpace =new QTableWidgetItem;
        QTableWidgetItem *freePercentage =new QTableWidgetItem;
        QTableWidgetItem *volumeSerial =new QTableWidgetItem;
        driveName->setText(QString::fromWCharArray((lDrives[i].getDriveName()).c_str()));
        driveType->setText(QString::fromWCharArray((lDrives[i].getDriveType()).c_str()));
        fileSystem->setText(QString::fromWCharArray((lDrives[i].getFileSystemName()).c_str()));
        totalSize->setText(QString::fromStdString(to_string(lDrives[i].getTotalSpace())));
        usedSpace->setText(QString::fromStdString(to_string(lDrives[i].getUsedSpace())));
        freeSpace->setText(QString::fromStdString(to_string(lDrives[i].getFreeSpace())));
        freePercentage->setText(QString::fromStdString(to_string(lDrives[i].getFreeSpacePercentage())));
        volumeSerial->setText(QString::fromWCharArray((lDrives[i].getVolumeSerialNumber()).c_str()));

        ui->tableWidget->setItem(i,0,driveName);
        ui->tableWidget->setItem(i,1,driveType);
        ui->tableWidget->setItem(i,2,fileSystem);
        ui->tableWidget->setItem(i,3,totalSize);
        ui->tableWidget->setItem(i,4,usedSpace);
        ui->tableWidget->setItem(i,5,freeSpace);
        ui->tableWidget->setItem(i,6,freePercentage);
        ui->tableWidget->setItem(i,7,volumeSerial);


    }
}



void MainWindow::TableWidget1Display(int index)
{

    ui->tableWidget_2->setRowCount(0);
    ui->tableWidget_2->setColumnCount(5);


    for (int i = 0; i <5 ; ++i) {
        ui-> tableWidget_2->horizontalHeader()->setSectionResizeMode(i, QHeaderView::ResizeToContents);
    }

    QStringList hLabels;
    hLabels<<"Partition"<<"Partition Type"<<"Drive"<<"Start Offset, MB"<<"Partition Length, MB";
    ui->tableWidget_2->setHorizontalHeaderLabels(hLabels);
    for (int i = 0; i < ui->tableWidget_2->columnCount(); i++) {
        ui->tableWidget_2->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Fixed);
        ui->tableWidget_2->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    }

    PhysicalDriveInfo* physicalDriveInfo = new PhysicalDriveInfo();

    vector<Partition> partitions = physicalDriveInfo->PhysicalDrivesInfo()[index].getPartitions();




    for (int i = 0; i < partitions.size();i++) {
        ui->tableWidget_2->insertRow(i);
        QTableWidgetItem *number =new QTableWidgetItem;
        QTableWidgetItem *type =new QTableWidgetItem;
        QTableWidgetItem *drive =new QTableWidgetItem;
        QTableWidgetItem *startOffset =new QTableWidgetItem;
        QTableWidgetItem *length =new QTableWidgetItem;
        number->setText(QString::fromStdString("#"+to_string(partitions[i].getNumber())));
        type->setText(QString::fromStdString((partitions[i].getType())));
        drive->setText(QString::fromWCharArray((partitions[i].getDrive()).c_str()));
        startOffset->setText(QString::fromStdString(to_string(partitions[i].getStartOffset())));
        length->setText(QString::fromStdString(to_string(partitions[i].getSize())));

        ui->tableWidget_2->setItem(i,0,number);
        ui->tableWidget_2->setItem(i,1,type);
        ui->tableWidget_2->setItem(i,2,drive);
        ui->tableWidget_2->setItem(i,3,startOffset);
        ui->tableWidget_2->setItem(i,4,length);

    }

}

void MainWindow::TableWidget3Display(int index)
{
    ui->tableWidget_3->setRowCount(0);
    ui->tableWidget_3->setColumnCount(5);

    QStringList hLabels;
    hLabels << "Attribute Name" << "Value"<<"Worst"<<"Type"<<"Raw Value";
    ui->tableWidget_3->setHorizontalHeaderLabels(hLabels);
    for (int i = 0; i < ui->tableWidget_3->columnCount(); i++) {
        ui->tableWidget_3->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Fixed);
        ui->tableWidget_3->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    }

    DriveSmartInfo* driveSmartInfo = new DriveSmartInfo();
    QVector<SmartAttribute> smartAttributes =driveSmartInfo-> getSMARTAttributes(index);

    for (int i = 0; i < smartAttributes.size(); i++) {
        ui->tableWidget_3->insertRow(i);
        QTableWidgetItem* attribute = new QTableWidgetItem;
        QTableWidgetItem* value = new QTableWidgetItem;
        QTableWidgetItem* type = new QTableWidgetItem;
        QTableWidgetItem* worst = new QTableWidgetItem;
        QTableWidgetItem* rawValue = new QTableWidgetItem;


        attribute->setText(QString::fromStdString(smartAttributes[i].getName()));
        value->setText(QString::fromStdString(smartAttributes[i].getValue()));
 worst->setText(QString::fromStdString(smartAttributes[i].getWorstValue()));
         type->setText(QString::fromStdString(smartAttributes[i].getType()));
 rawValue->setText(QString::fromStdString(smartAttributes[i].getRawValue()));

        ui->tableWidget_3->setItem(i , 0, attribute);
        ui->tableWidget_3->setItem(i, 1, value);
        ui->tableWidget_3->setItem(i , 2, worst);
        ui->tableWidget_3->setItem(i, 3, type);
        ui->tableWidget_3->setItem(i , 4, rawValue);
    }

    ui->tableWidget_3->resizeRowsToContents();

    ui->tableWidget_3->resizeColumnToContents(0);

    ui->label_5->setText("");
    ui->label_5->setText(driveSmartInfo->getTestResult(index));
    ui->label_5->adjustSize();

    int labelWidth = ui->label_5->width();
    int labelHeight = ui->label_5->height();

    ui->frame_55->setFixedSize(labelWidth, labelHeight);

}






void MainWindow::ListWidgetDisplay(){
    ui->listWidget->clear();
    PhysicalDriveInfo* physicalDriveInfo = new PhysicalDriveInfo();
    vector<PhysicalDrive> physicalDrives= physicalDriveInfo->PhysicalDrivesInfo();
    string driveTemplate="";
    for (int i = 0; i < physicalDrives.size();i++) {
        wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
        string driveName = converter.to_bytes(physicalDrives[i].getDriveName());
        string volumeSerialNumber = converter.to_bytes(physicalDrives[i].getVolumeSerialNumber());
        string totalSpace = to_string(physicalDrives[i].getTotalSpace());
        driveTemplate = driveName + "(" + volumeSerialNumber + ")" + "[" + totalSpace + " GB]";
        ui->listWidget->addItem(QString::fromStdString(driveTemplate));
    }

    connect(ui->listWidget, &QListWidget::itemSelectionChanged, this, &MainWindow::onListItemSelectionChanged);

}


void MainWindow::ListWidget2Display(){
    ui->listWidget_2->clear();
    PhysicalDriveInfo* physicalDriveInfo = new PhysicalDriveInfo();
    DriveSmartInfo* driveSmartInfo = new DriveSmartInfo();


    vector<PhysicalDrive> physicalDrives= physicalDriveInfo->PhysicalDrivesInfo();
    string driveTemplate="";
    for (int i = 0; i < physicalDrives.size();i++) {
        if(driveSmartInfo->supportsSMART(i)){
            wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
            string driveName = converter.to_bytes(physicalDrives[i].getDriveName());
            driveTemplate = driveName;

            ui->listWidget_2->addItem(QString::fromStdString(driveTemplate));
        }

    }

    connect(ui->listWidget_2, &QListWidget::itemSelectionChanged, this, &MainWindow::onList2ItemSelectionChanged);

}
void MainWindow::onListItemSelectionChanged()
{
    int index=ui->listWidget->currentRow();
    TableWidget1Display(index);


}

void MainWindow::onList2ItemSelectionChanged()
{
    QVector<int> supportsIndexes;
      PhysicalDriveInfo* physicalDriveInfo = new PhysicalDriveInfo();
     DriveSmartInfo* driveSmartInfo = new DriveSmartInfo();
    vector<PhysicalDrive> physicalDrives=physicalDriveInfo-> PhysicalDrivesInfo();
    for (int i = 0; i < physicalDrives.size();i++) {
        if(driveSmartInfo->supportsSMART(i)){
           supportsIndexes.push_back(i);
        }

    }
    int index=ui->listWidget_2->currentRow();

    TableWidget3Display(supportsIndexes[index]);

}
void MainWindow::onList3ItemSelectionChanged()
{
    QVector<int> supportsIndexes;
    PhysicalDriveInfo* physicalDriveInfo = new PhysicalDriveInfo();
    DriveSmartInfo* driveSmartInfo = new DriveSmartInfo();

    vector<PhysicalDrive> physicalDrives= physicalDriveInfo->PhysicalDrivesInfo();
    for (int i = 0; i < physicalDrives.size();i++) {
        if(driveSmartInfo->supportsSMART(i)){
           supportsIndexes.push_back(i);
        }

    }
    int index=ui->listWidget_3->currentRow();
    TableWidget4Display(supportsIndexes[index]);

}



void MainWindow::on_back_from1_to_main_clicked()
{
        ui->stackedWidget->setCurrentIndex(0);
    updateWindowTitle("Main Page");
}


void MainWindow::on_pushButton_2_clicked()
{
     ui->stackedWidget->setCurrentIndex(0);
     updateWindowTitle("Main Page");
}


void MainWindow::on_pushButton_3_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
      updateWindowTitle("Main Page");
}







void MainWindow::on_pushButton_4_clicked()
{
      ui->stackedWidget->setCurrentIndex(0);
       updateWindowTitle("Main Page");
}





void MainWindow::on_pushButton_5_clicked()
{
       ui->stackedWidget->setCurrentIndex(0);
       updateWindowTitle("Main Page");
}


void MainWindow::on_pushButton_clicked()
{
       ui->stackedWidget->setCurrentIndex(4);
       updateWindowTitle("About");

}


void MainWindow::on_pushButton_6_clicked()
{
       ui->stackedWidget->setCurrentIndex(5);
       updateWindowTitle("Help");
}


void MainWindow::on_pushButton_9_clicked()
{
       ui->stackedWidget->setCurrentIndex(0);
       updateWindowTitle("Main Page");
}


void MainWindow::on_pushButton_7_clicked()
{
       ui->stackedWidget->setCurrentIndex(5);
       updateWindowTitle("Introduction to SMA");
}


void MainWindow::on_pushButton_12_clicked()
{
       ui->stackedWidget->setCurrentIndex(0);
       updateWindowTitle("Main Page");
}


void MainWindow::on_pushButton_8_clicked()
{
       ui->stackedWidget->setCurrentIndex(6);
       updateWindowTitle("User Interface");
}


void MainWindow::on_pushButton_10_clicked()
{

       ui->stackedWidget->setCurrentIndex(5);
       updateWindowTitle("Introduction to SMA");
}

