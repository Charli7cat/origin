#include <QCoreApplication>
#include "tcpserver.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    TCPserver server(nullptr, 12345);

    return a.exec();
}