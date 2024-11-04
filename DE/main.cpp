#include "bc.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    BC w;
    w.show();
    return a.exec();
}
