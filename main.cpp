
#include "GUI/mainwindow.h"

#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    QIcon icon(":/logo.png");
    w.setWindowIcon(icon);

    return a.exec();
}
