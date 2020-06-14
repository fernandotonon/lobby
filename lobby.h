#ifndef LOBBY_H
#define LOBBY_H

#include <QObject>
#include <QQuickItem>
#include <QtNetwork>

class Lobby : public QQuickItem
{
    Q_OBJECT
public:
    Lobby();
    void start();

signals:

private:
    QUdpSocket *udpSocket = nullptr;
    QTimer timer;
    QThread *thread;
    int interval = 1000;
    int port = 3333;

private slots:
    void broadcastDatagram();
    void processDatagram();

};

#endif // LOBBY_H
