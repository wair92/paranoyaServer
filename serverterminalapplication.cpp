#include <QHostAddress>

#include "serverterminalapplication.h"
#include "server.h"

ServerTerminalApplication::ServerTerminalApplication(int& argc, char** argv)
    :app_(argc, argv), server_()
{
    QHostAddress ip;
    server_.listen();
}

ServerTerminalApplication::~ServerTerminalApplication()
{

}

int ServerTerminalApplication::run()
{
    return app_.exec();
}
