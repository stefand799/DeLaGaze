#include "delagazeclient.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DeLaGazeClient w;
    w.show();
    return a.exec();
}
