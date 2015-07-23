#include <QCoreApplication>
#include "netter.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    new Netter(&a);

    return a.exec();
}
