#ifndef SERVER_H
#define SERVER_H
#include <memory>
#include <QTcpServer>
#include <QObject>
#include <QTimer>
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
    void processLogout(QJsonObject object );
    void processHeartBeat(QJsonObject object);

    bool isLogin(const QJsonObject& obj) const;
    bool isMessage(const QJsonObject& obj) const;
    bool isLogout(const QJsonObject& obj) const;
    bool isHeartBeat(const QJsonObject& obj) const;

    void sendMessageToReceiver(const QString& receiver, QJsonObject object);
    void sendLoginConfirm(const QString& user);

    void setInactiveClientTimer();
    void connectNewConnection();
    void connectNewData();
    void connectDisconection();

    QTcpSocket *findReceiver(const QString& receiver);
    void removeInactiveClients();

    std::unique_ptr<QTcpServer> server_;
    QHostAddress address_;
    quint16 port_;
    QTcpSocket* connectedClient_ = {nullptr};
    int numbeOfClients_ = {0};
    std::vector<Connection> connections_;
    QTimer inactiveClientChecker_;
};

#endif // SERVER_H
