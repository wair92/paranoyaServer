#include <QHostAddress>

#include "serverapplication.h"
#include "server.h"

ServerApplication::ServerApplication(int& argc, char** argv)
    :app_(argc, argv), server_()
{
    QHostAddress ip;
    ip.setAddress("127.0.0.1");
    server_.setServer(ip,9008);
    server_.listen();
}

ServerApplication::~ServerApplication()
{

}

int ServerApplication::run()
{
    return app_.exec();
}
