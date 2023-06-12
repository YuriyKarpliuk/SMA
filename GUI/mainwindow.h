
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QVector>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow

{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent* event) override;

private slots:

    void on_logical_drive_clicked();

    void on_physical_drive_clicked();

    void on_smart_clicked();

    void on_back_from1_to_main_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();
    void updateWindowTitle(const QString& pageTitle);

    void on_pushButton_4_clicked();

    void on_ata_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_10_clicked();

private:
    Ui::MainWindow *ui;
     void TableWidgetDisplay();
     void ListWidget2Display();
    void ListWidgetDisplay();
     void TableWidget1Display(int index);
      void TableWidget3Display(int index);
    void onListItemSelectionChanged();
      void onList2ItemSelectionChanged();
    void onList3ItemSelectionChanged();
    void TableWidget4Display(int index);
    void ListWidget3Display();

};

#endif // MAINWINDOW_H
