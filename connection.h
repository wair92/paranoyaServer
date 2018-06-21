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
    void markAsPossiblyInactive() { checkForDeletion_ = true;}
    void setActive() { checkForDeletion_ = false;}
    bool markedForRemoving() {return checkForDeletion_;}
private:
    QString username_;
    QTcpSocket* openedSocket_;
    bool checkForDeletion_ {false};
};

#endif // CONNECTION_H
