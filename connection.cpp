#include "connection.h"

Connection::Connection(QString username, QTcpSocket* socket)
{
    username_ = username;
    openedSocket_ = socket;
    qDebug() << "Created new connection: "<< username;
}
