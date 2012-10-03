#include <QApplication>
#include "Navigateur.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Navigateur w;

    w.show();
    return a.exec();
}
