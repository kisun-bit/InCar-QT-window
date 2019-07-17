#include <QApplication>
#include "mainmeun.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainMeun m;
    m.show();

    return a.exec();
}
