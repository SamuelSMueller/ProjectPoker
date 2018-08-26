#include "serverStuff.h"

ServerStuff::ServerStuff(QObject *pwgt, QString rName) : QObject(pwgt), m_nNextBlockSize(0)
{
    tcpServer = new QTcpServer(this);
    roomname = rName;
}

QList<QTcpSocket *> ServerStuff::getClients()
{
    return clients;
}

void ServerStuff::newConnection()
{

    QTcpSocket *clientSocket = tcpServer->nextPendingConnection();
    connect(clientSocket, &QTcpSocket::disconnected, clientSocket, &QTcpSocket::deleteLater);
    connect(clientSocket, &QTcpSocket::readyRead, this, &ServerStuff::readClient);
    connect(clientSocket, &QTcpSocket::disconnected, this, &ServerStuff::gotDisconnection);

    clients << clientSocket;
}

void ServerStuff::readClient()
{
    QTcpSocket *clientSocket = (QTcpSocket*)sender(); //c-style cast, should be qobject_cast<QTcpSocket*>(sender());

    QDataStream in(clientSocket);
    for (;;)
    {
        if (!m_nNextBlockSize) {
                if (clientSocket->bytesAvailable() < sizeof(quint16)) { break; }
            in >> m_nNextBlockSize;
        }

        if (clientSocket->bytesAvailable() < m_nNextBlockSize) { break; }
        QString str;
        in >> str;

        emit gotNewMesssage(str, clientSocket);

        if(!str.startsWith("/USER/") && !str.startsWith("/DCON/"))
        {
            foreach(QTcpSocket *clientSocketA, clients)
            {
                sendToClient(clientSocketA, str);
            }
        }
        m_nNextBlockSize = 0;

        if (QString().arg(str) == -1)
        {
            qDebug() << "Some error occured";
        }

    }

}

void ServerStuff::gotDisconnection()
{
    clients.removeAt(clients.indexOf((QTcpSocket*)sender()));
    emit smbDisconnected();
}

qint64 ServerStuff::sendToClient(QTcpSocket* socket, const QString& str)
{
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out << quint16(0) << str;

    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));

    return socket->write(arrBlock);
}
