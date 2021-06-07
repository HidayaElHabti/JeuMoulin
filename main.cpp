#include "Plateau.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Plateau p;
    p.show();
    return a.exec();
}
