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
    QML_ELEMENT
public:
    explicit Lobby(QObject *parent = nullptr);
    void start();
    QVariantMap getApps(){return QVariantMap(apps);}
    Q_INVOKABLE void connectApp(const QString &app);
    Q_INVOKABLE void sendMsg(const QString &msg, const QString &UUID = "");
    const QString appUUID = QUuid::createUuid().toString();

signals:
    void appsChanged();
    void msgReceived(const QString &msg);
    void connectedTo(const QString &UUID);

private:
    QUdpSocket *udpSocket = nullptr;
    QTcpSocket *tcpSocket = nullptr;
    QTcpServer *tcpServer = nullptr;
    QMap<QString,QTcpSocket*> tcpSocketMap;
    QList<QTcpSocket*> tcpSocketUnnamedList;
    QTimer timer;
    QThread *thread;
    int interval = 1000;
    int port = 3333;
    QByteArray datagram;
    QMap<QString,QVariant> apps;
    void writeTCPMsg(QTcpSocket* socket, const QString &msg);

private slots:
    void broadcastDatagram();
    void processDatagram();
    void newTCPConnection();
    void readTCPDatagram();

};

#endif // LOBBY_H
