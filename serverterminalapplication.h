#ifndef SERVERAPPLICATION_H
#define SERVERAPPLICATION_H

#include <QCoreApplication>
#include "server.h"

class ServerTerminalApplication
{
public:
    ServerTerminalApplication(int& argc, char** argv);
    virtual ~ServerTerminalApplication();
    int run();
private:
    QCoreApplication app_;
    Server server_;
};

#endif // SERVERAPPLICATION_H
