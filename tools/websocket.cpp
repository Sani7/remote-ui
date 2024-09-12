#include "websocket.hpp"
#include "QtWebSockets/qwebsocket.h"
#include "QtWebSockets/qwebsocketserver.h"
#include <spdlog/spdlog.h>

Websocket::Websocket(uint16_t port, QObject *parent)
    : QObject(parent),
      m_pWebSocketServer(new QWebSocketServer(QStringLiteral("sim_us Server"), QWebSocketServer::NonSecureMode, this))
{
    if (m_pWebSocketServer->listen(QHostAddress::Any, port))
    {
        spdlog::debug("Echoserver listening on port {}", port);
        connect(m_pWebSocketServer, &QWebSocketServer::newConnection, this, &Websocket::onNewConnection);
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
    foreach (QWebSocket *conn, m_clients)
    {
        conn->sendTextMessage(message);
    }
}

void Websocket::send(QWebSocket *conn, QString message)
{
    conn->sendTextMessage(message);
    spdlog::trace("sending message: {}", message.toStdString());
}

void Websocket::onNewConnection()
{
    QWebSocket *pSocket = m_pWebSocketServer->nextPendingConnection();

    connect(pSocket, &QWebSocket::textMessageReceived, this, &Websocket::processTextMessage);
    connect(pSocket, &QWebSocket::binaryMessageReceived, this, &Websocket::processBinaryMessage);
    connect(pSocket, &QWebSocket::disconnected, this, &Websocket::socketDisconnected);

    m_clients << pSocket;
    spdlog::debug("socket connected");
}

void Websocket::processTextMessage(QString message)
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    spdlog::trace("Message received: {}", message.toStdString());
    emit on_message(pClient, message);
}

void Websocket::processBinaryMessage(QByteArray message)
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    spdlog::trace("Binary Message received: {}", message.data());
    Q_UNUSED(message);
    Q_UNUSED(pClient);
}

void Websocket::socketDisconnected()
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    spdlog::debug("socket Disconnected");
    if (pClient)
    {
        m_clients.removeAll(pClient);
        pClient->deleteLater();
    }
}
