#include <memory>
#include <QHostAddress>
#include <QDebug>
#include <QTcpServer>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include "server.h"
#include "connection.h"

Server::Server()
    :server_( std::make_unique<QTcpServer>() )
{
    connect( server_.get(), &QTcpServer::newConnection, this, [this](){
        connectedClient_ = server_->nextPendingConnection();
        numbeOfClients_++;
        qDebug() << "Clients:"<< numbeOfClients_;

        connect( connectedClient_, &QTcpSocket::readyRead, this, [this](){
            QTcpSocket* readSocket = qobject_cast<QTcpSocket*>(sender());
            auto data = readSocket->readAll();
            process(data, readSocket);
            QString getIt = "GotIt";
            readSocket->write(getIt.toLatin1(), getIt.length());
        } );

        connect( connectedClient_, &QTcpSocket::disconnected, this, [this](){
            numbeOfClients_--;
            qDebug() << "Clients:"<< numbeOfClients_;
        });

    });



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
}

void Server::processLogin(QJsonObject object, QTcpSocket* readSocket)
{
    auto user = object.value(QString("Login")).toString();
    qDebug() << user << "was succesfully logged in";
    Connection connection(user, readSocket);
    connections_.push_back(connection);
}

void Server::processMessage(QJsonObject object)
{
    auto message = object.value(QString("Message")).toString();
    auto receiver = object.value(QString("Receiver")).toString();
    auto sender = object.value(QString("Sender")).toString();
    qDebug() << "Sender: " << sender << "Receiver: " << receiver << "Message: " << message;

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
