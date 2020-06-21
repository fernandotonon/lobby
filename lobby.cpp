#include "lobby.h"
#include <QDebug>

Lobby::Lobby(QObject *parent) :
    QObject(parent)
{
    thread = new QThread(this);
    timer.setInterval(interval);
    timer.moveToThread(thread);
    thread->start();
    connect(thread,SIGNAL(started()),&timer,SLOT(start()));

    start();
}

void Lobby::start()
{
    udpSocket = new QUdpSocket(this);
    udpSocket->bind(port,QUdpSocket::ShareAddress);
    connect(&timer, SIGNAL(timeout()), this, SLOT(broadcastDatagram()));
    connect(udpSocket, SIGNAL(readyRead()), this,SLOT(processDatagram()));
}

void Lobby::broadcastDatagram()
{
    QJsonObject obj;
    obj["organizationName"] = QCoreApplication::organizationName();
    obj["organizationDomain"] = QCoreApplication::organizationDomain();
    obj["applicationName"] = QCoreApplication::applicationName();
    obj["applicationPid"] = QCoreApplication::applicationPid();
    obj["localHostName"] = QHostInfo::localHostName();
    obj["localPort"] = udpSocket->localPort();
    QJsonDocument doc(obj);
    QByteArray datagram = doc.toJson(QJsonDocument::Compact);
    udpSocket->writeDatagram(datagram, QHostAddress::Broadcast, port);
}

void Lobby::processDatagram()
{
    while (udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(datagram.data(), datagram.size());

        if(!apps.count(datagram)){
            apps[datagram]=QVariant(true);
            emit appsChanged();
        }else
            apps[datagram]=QVariant(true);
    }
}

