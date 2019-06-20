#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow* w = new MainWindow();
    w->setAttribute(Qt::WA_DeleteOnClose, true);

    w->setWindowTitle("GUI FOR IR CAMERA");
    w->show();

    return a.exec();
}
