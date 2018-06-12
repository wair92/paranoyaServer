#ifndef CONNECTION_H
#define CONNECTION_H
#include <QString>
#include <QTcpSocket>

class Connection
{
public:
    Connection(QString username_, QTcpSocket* socket);
    QString getName() const { return username_;}
    QTcpSocket* getSocket() const { return openedSocket_;}
private:
    QString username_;
    QTcpSocket* openedSocket_;
};

#endif // CONNECTION_H
