#ifndef LOBBY_H
#define LOBBY_H

#include <QObject>
#include <qqml.h>
#include <QtNetwork>
#include <QMap>
#include <QVariant>

class Lobby : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantMap apps READ getApps NOTIFY appsChanged)
    Q_INVOKABLE void startLobby(){
            start();
        }
    QML_ELEMENT
public:
    explicit Lobby(QObject *parent = nullptr);
    void start();
    QVariantMap getApps(){return QVariantMap(apps);}

signals:
    void appsChanged();

private:
    QUdpSocket *udpSocket = nullptr;
    QTimer timer;
    QThread *thread;
    int interval = 1000;
    int port = 3333;

    QMap<QString,QVariant> apps;

private slots:
    void broadcastDatagram();
    void processDatagram();

};

#endif // LOBBY_H
