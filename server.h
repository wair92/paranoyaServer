#ifndef SERVER_H
#define SERVER_H
#include <memory>
#include <QTcpServer>
#include <QObject>

class Server: public QObject
{
    Q_OBJECT
public:
    Server();
    virtual ~Server();
    void setServer(QHostAddress address, quint16 port);
    void listen();
private:
    std::unique_ptr<QTcpServer> server_;
    QHostAddress address_;
    quint16 port_;
    QTcpSocket* connectedClient_ = {nullptr};
    int numbeOfClients_ = {0};
};

#endif // SERVER_H
