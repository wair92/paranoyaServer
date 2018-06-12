#ifndef SERVER_H
#define SERVER_H
#include <memory>
#include <QTcpServer>
#include <QObject>
#include "connection.h"

class Server: public QObject
{
    Q_OBJECT
public:
    Server();
    virtual ~Server();
    void setServer(QHostAddress address, quint16 port);
    void listen();
private:
    void process( QByteArray data, QTcpSocket* readSocket );
    void processLogin( QJsonObject object, QTcpSocket* readSocket );
    void processMessage( QJsonObject object );
    bool isLogin(const QJsonObject& obj) const;
    bool isMessage(const QJsonObject& obj) const;

    std::unique_ptr<QTcpServer> server_;
    QHostAddress address_;
    quint16 port_;
    QTcpSocket* connectedClient_ = {nullptr};
    int numbeOfClients_ = {0};
    std::vector<Connection> connections_;
};

#endif // SERVER_H
