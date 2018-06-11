#ifndef SERVERAPPLICATION_H
#define SERVERAPPLICATION_H

#include <QCoreApplication>
#include "server.h"

class ServerApplication
{
public:
    ServerApplication(int& argc, char** argv);
    virtual ~ServerApplication();
    int run();
private:
    QCoreApplication app_;
    Server server_;
};

#endif // SERVERAPPLICATION_H
