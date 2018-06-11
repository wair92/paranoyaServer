#include <memory>
#include <QHostAddress>
#include <QDebug>
#include <QTcpServer>
#include <QTcpSocket>
#include "server.h"

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
            qDebug() << data;
        } );

        connect( connectedClient_, &QTcpSocket::disconnected, this, [this](){
            numbeOfClients_--;
            qDebug() << "Clients:"<< numbeOfClients_;
        });

    });



}

Server::~Server()
{

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
