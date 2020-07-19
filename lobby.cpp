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
    int tcpPort = port+1;
    tcpServer = new QTcpServer(this);
    tcpSocket = new QTcpSocket(this);

    connect(tcpSocket, SIGNAL(readyRead()), this,SLOT(readTCPDatagram()));

    // whenever a user connects, it will emit signal
    connect(tcpServer, SIGNAL(newConnection()),
            this, SLOT(newTCPConnection()));

    while(tcpPort<port+10&&!tcpServer->listen(QHostAddress::Any, tcpPort))
        tcpPort++;

    udpSocket = new QUdpSocket(this);
    udpSocket->bind(port,QUdpSocket::ShareAddress);
    connect(&timer, SIGNAL(timeout()), this, SLOT(broadcastDatagram()));
    connect(udpSocket, SIGNAL(readyRead()), this,SLOT(processDatagram()));

    QJsonObject obj;
    obj["organizationName"] = QCoreApplication::organizationName();
    obj["organizationDomain"] = QCoreApplication::organizationDomain();
    obj["applicationName"] = QCoreApplication::applicationName();
    obj["applicationPid"] = QCoreApplication::applicationPid();
    obj["localHostName"] = QHostInfo::localHostName();
    obj["tcpPort"] = tcpPort;
    obj["ipList"] = "";

    for(QHostAddress h: QNetworkInterface::allAddresses()){
      if(h.protocol()==QAbstractSocket::IPv4Protocol)
        obj["ipList"] = obj["ipList"].toString()
                +(obj["ipList"].toString().isEmpty()?"":",")
                +h.toString();
    }

    QJsonDocument doc(obj);
    datagram = doc.toJson(QJsonDocument::Compact);
}

void Lobby::connectApp(const QString &app)
{
    QJsonDocument doc = QJsonDocument::fromJson(app.toUtf8());
    QJsonObject obj = doc.object();

    for(const QString &ip:obj["ipList"].toString().split(","))
    {
        tcpSocket->connectToHost(ip,obj["tcpPort"].toInt());
        if (tcpSocket->waitForConnected(1000)){
            //tcpSocketList.append(tcpSocket);
            emit connectedToServer(app);
            return;
        }
    }
    qDebug()<<"not connected";
}

void Lobby::sendMsg(const QString &msg)
{
    tcpSocket->write(msg.toStdString().data());
    tcpSocket->flush();

    tcpSocket->waitForBytesWritten(3000);
}

void Lobby::broadcastDatagram()
{
    udpSocket->writeDatagram(datagram, QHostAddress::Broadcast, port);
}

void Lobby::processDatagram()
{
    while (udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(datagram.data(), datagram.size());

        if(datagram==this->datagram)
            continue;

        if(!apps.count(datagram)){
            apps[datagram]=QVariant(true);
            emit appsChanged();
        }

    }
}

void Lobby::newTCPConnection()
{
    QTcpSocket *socket = tcpServer->nextPendingConnection();
    tcpSocket=socket;
    connect(tcpSocket, SIGNAL(readyRead()), this,SLOT(readTCPDatagram()));
}

void Lobby::readTCPDatagram()
{
    emit msgReceived(tcpSocket->readAll());
}
