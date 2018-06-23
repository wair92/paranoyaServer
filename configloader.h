#ifndef CONFIGLOADER_H
#define CONFIGLOADER_H
#include <QString>

class ConfigLoader
{
public:
    ConfigLoader(QString configPath);
    virtual ~ConfigLoader();
    QString getIp() {return ip_;}
    quint16 getPort() {return port_;}
    quint32 getHearbeatTime() {return heartBeatTime_;}
private:
    void setDefaultValules();
    QString ip_;
    quint16 port_;
    quint32 heartBeatTime_;
};

#endif // CONFIGLOADER_H
