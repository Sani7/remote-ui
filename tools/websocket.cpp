#include "websocket.hpp"
#include "QtWebSockets/qwebsocketserver.h"
#include "QtWebSockets/qwebsocket.h"
#include <QtCore/QDebug>

Websocket::Websocket(uint16_t port, QObject *parent) :
    QObject(parent),
    m_pWebSocketServer(new QWebSocketServer(QStringLiteral("sim_us Server"),
                                              QWebSocketServer::NonSecureMode, this))
{
    if (m_pWebSocketServer->listen(QHostAddress::Any, port)) {
        qDebug() << "Echoserver listening on port" << port;
        connect(m_pWebSocketServer, &QWebSocketServer::newConnection,
                this, &Websocket::onNewConnection);
        connect(m_pWebSocketServer, &QWebSocketServer::closed, this, &Websocket::closed);
    }
}

Websocket::~Websocket()
{
    m_pWebSocketServer->close();
    qDeleteAll(m_clients.begin(), m_clients.end());
}

void Websocket::broadcast(QString message)
{
    foreach (QWebSocket* conn, m_clients) {
        conn->sendTextMessage(message);
    }
}

void Websocket::send(QWebSocket* conn, QString message)
{
    conn->sendTextMessage(message);
}

void Websocket::onNewConnection()
{
    QWebSocket *pSocket = m_pWebSocketServer->nextPendingConnection();

    connect(pSocket, &QWebSocket::textMessageReceived, this, &Websocket::processTextMessage);
    connect(pSocket, &QWebSocket::binaryMessageReceived, this, &Websocket::processBinaryMessage);
    connect(pSocket, &QWebSocket::disconnected, this, &Websocket::socketDisconnected);

    m_clients << pSocket;
}

void Websocket::processTextMessage(QString message)
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    qDebug() << "Message received:" << message;
    emit on_message(pClient, message);
}

void Websocket::processBinaryMessage(QByteArray message)
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    qDebug() << "Binary Message received:" << message;
}

void Websocket::socketDisconnected()
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    qDebug() << "socketDisconnected:" << pClient;
    if (pClient) {
        m_clients.removeAll(pClient);
        pClient->deleteLater();
    }
}
