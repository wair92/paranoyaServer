#include <memory>
#include <QHostAddress>
#include <QDebug>
#include <QTcpServer>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDateTime>
#include <QTimer>
#include "server.h"
#include "connection.h"

Server::Server()
    :server_( std::make_unique<QTcpServer>() ),
    config_( "config.json" )
{
    QHostAddress ip;
    ip.setAddress(config_.getIp());
    setServer(ip, config_.getPort());
    connectNewConnection();
    setInactiveClientTimer();
}

Server::~Server()
{
    if(server_->isListening()){
        qDebug() << "Closing server";
        server_->close();
    }
}

void Server::setServer(QHostAddress address, quint16 port)
{
    address_ = address;
    port_  = port;
}

void Server::listen()
{
    qDebug() << "Server is listening on adderss" << address_ << "and port: " << port_;
    server_->listen( address_, port_ );
}

void Server::process(QByteArray data, QTcpSocket* readSocket)
{
    auto document = QJsonDocument::fromJson(data);
    QJsonObject object = document.object();
    if(isLogin(object)){
        processLogin(object, readSocket);
    }
    if(isMessage(object)){
        processMessage(object);
    }
    if(isLogout(object)){
        processLogout(object);
    }
    if(isHeartBeat(object)){
        processHeartBeat(object);
    }
    if(isUserListRequest(object)){
        processUserListRequest(object);
    }
}

void Server::processLogin(QJsonObject object, QTcpSocket* readSocket)
{
    auto user = object.value(QString("Login")).toString();
    qDebug() << user << "was succesfully logged in";
    Connection connection(user, readSocket);
    connections_.push_back(connection);
    sendLoginConfirm( user );
}

void Server::processMessage(QJsonObject object)
{
    auto message = object.value(QString("Message")).toString();
    auto receiver = object.value(QString("Receiver")).toString();
    auto sender = object.value(QString("Sender")).toString();
    qDebug() << "Sender: " << sender << "Receiver: " << receiver << "Message: " << message;
    sendMessageToReceiver(receiver, object);
}

void Server::processLogout(QJsonObject object)
{
    auto user = object.value(QString("Logout")).toString();
    qDebug() << "Logging out: " << user;
    int index = 0;
    for(int i = 0; i< connections_.size(); i++){
        if(connections_[i].getName() == user){
            index = i;
            connections_.erase( connections_.begin() + index);
            break;
        }
    }

}

void Server::processHeartBeat(QJsonObject object)
{
    auto user = object.value(QString("HeartBeat")).toString();
    //qDebug() << QDateTime::currentDateTime() << "user: " << user << "is alive";

    for(auto it = connections_.begin(); it != connections_.end(); it++ ){
        if(it->getName() == user){
            it->setActive();
        }
    }
    sendHeartBeatBack(user);
}

void Server::processUserListRequest(QJsonObject object)
{
    auto user = object.value(QString("Username")).toString();
    sendUserList(user);
}

bool Server::isLogin( const QJsonObject& obj ) const
{
    auto login = obj.value(QString("Id"));
    if(login.toString() == "Login")
        return true;
    else
        return false;
}

bool Server::isMessage( const QJsonObject& obj ) const
{
    auto message = obj.value(QString("Id"));
    if(message.toString() == "Message")
        return true;
    else
        return false;
}

bool Server::isLogout(const QJsonObject &obj) const
{
    auto login = obj.value(QString("Id"));
    if(login.toString() == "Logout")
        return true;
    else
        return false;
}

bool Server::isHeartBeat(const QJsonObject &obj) const
{
    auto login = obj.value(QString("Id"));
    if(login.toString() == "HeartBeat")
        return true;
    else
        return false;
}

bool Server::isUserListRequest(const QJsonObject &obj) const
{
    auto login = obj.value(QString("Id"));
    if(login.toString() == "UserList")
        return true;
    else
        return false;
}

void Server::sendMessageToReceiver(const QString &receiver, QJsonObject object)
{
    QTcpSocket* foundReceiver = findReceiver(receiver);
    if(foundReceiver)
    {
        QJsonDocument doc( object );
        auto dataToSend = doc.toJson(QJsonDocument::Compact);
        foundReceiver->write(dataToSend);
    }
}

void Server::sendLoginConfirm(const QString& receiver)
{
    QTcpSocket* foundReceiver = findReceiver(receiver);
    if(foundReceiver)
    {
        QJsonObject confirmObject;
        confirmObject.insert("Id", QJsonValue::fromVariant("LoginConfirm"));
        confirmObject.insert("Username", QJsonValue::fromVariant( receiver ));
        confirmObject.insert("HeartBeatTimer", QJsonValue::fromVariant( config_.getHearbeatTime() ));

        QJsonDocument doc( confirmObject );
        auto dataToSend = doc.toJson(QJsonDocument::Compact);
        foundReceiver->write(dataToSend);
    }
}

void Server::sendHeartBeatBack(const QString &user)
{
    QTcpSocket* foundReceiver = findReceiver(user);
    if(foundReceiver){
        QJsonObject confirmObject;
        confirmObject.insert("Id", QJsonValue::fromVariant("HeartBeatBack"));
        confirmObject.insert("Username", QJsonValue::fromVariant( user ));

        QJsonDocument doc( confirmObject );
        auto dataToSend = doc.toJson(QJsonDocument::Compact);
        foundReceiver->write(dataToSend);
    }
}

void Server::sendUserList(const QString &user)
{
    QTcpSocket* foundReceiver = findReceiver(user);
    if(foundReceiver){
        QJsonObject message;
        message.insert("Id", QJsonValue::fromVariant("UserListResponse"));
        message.insert("Username", QJsonValue::fromVariant( user ));
        QJsonArray users;
        for( const Connection& i: connections_){
            if(i.getName() != user ){
                users.push_back( QJsonValue::fromVariant( i.getName() ));
            }
        }
        message.insert("Users", users);

        QJsonDocument doc( message );
        auto dataToSend = doc.toJson(QJsonDocument::Compact);
        foundReceiver->write(dataToSend);
        qDebug() << "Sent userList to" << user;
    }
}

void Server::setInactiveClientTimer()
{
    inactiveClientChecker_.setSingleShot(false);
    inactiveClientChecker_.setInterval(config_.getHearbeatTime() * 2 + 2000);
    inactiveClientChecker_.start();
    connect(&inactiveClientChecker_, &QTimer::timeout, this, [this](){
        removeInactiveClients();
    });
}

void Server::connectNewConnection()
{
    connect( server_.get(), &QTcpServer::newConnection, this, [this](){
        connectedClient_ = server_->nextPendingConnection();
        numbeOfClients_++;
        qDebug() << "Clients:"<< numbeOfClients_;
        connectNewData();
        connectDisconection();

    });
}

void Server::connectNewData()
{
    connect( connectedClient_, &QTcpSocket::readyRead, this, [this](){
        QTcpSocket* readSocket = qobject_cast<QTcpSocket*>(sender());
        auto data = readSocket->readAll();
        process(data, readSocket);
    } );
}

void Server::connectDisconection()
{
    connect( connectedClient_, &QTcpSocket::disconnected, this, [this](){
        numbeOfClients_--;
        qDebug() << "Clients:"<< numbeOfClients_;
    });
}

QTcpSocket *Server::findReceiver(const QString &receiver)
{
    for (const Connection& i : connections_){
            if(i.getName() == receiver)
                return i.getSocket();
    }
    qDebug() << "Client" << receiver << "doesnt exist";
    return nullptr;
}

void Server::removeInactiveClients()
{
    connections_.erase(
                std::remove_if(connections_.begin(), connections_.end(), [](Connection& i) {
                           if(i.markedForRemoving() == true){
                               qDebug() << "Deleting: " << i.getName();
                                i.getSocket()->close();
                                return true;
                           }
                           else{
                               i.markAsPossiblyInactive();
                               return false;
                           }

                       }), connections_.end());
}
