#include "websocket.hpp"
#include "QtWebSockets/qwebsocket.h"
#include "QtWebSockets/qwebsocketserver.h"
#include <QtCore/QFile>
#include <QtNetwork/QSslCertificate>
#include <QtNetwork/QSslKey>
#include <spdlog/spdlog.h>
#include <QCoreApplication>

Websocket::Websocket(uint16_t port, QObject *parent)
    : QObject(parent),
      m_pWebSocketServer(new QWebSocketServer(QStringLiteral("unisim Server"), QWebSocketServer::NonSecureMode, parent))
{
    if (!m_pWebSocketServer->listen(QHostAddress::Any, port))
    {
        SPDLOG_CRITICAL("Unable to bind websocket to port {}", port);
        exit(-1);
    }
    connect(m_pWebSocketServer, &QWebSocketServer::newConnection, this, &Websocket::onNewConnection);
    connect(m_pWebSocketServer, &QWebSocketServer::closed, this, &Websocket::closed);
}

Websocket::Websocket(uint16_t port, QString key_file, QString cert_file, QObject *parent)
    : QObject(parent),
      m_pWebSocketServer(new QWebSocketServer(QStringLiteral("unisim Server"), QWebSocketServer::SecureMode, parent))
{
    QSslConfiguration sslConfiguration;
    QFile certFile(cert_file);
    QFile keyFile(key_file);
    bool certFileOpened = certFile.open(QIODevice::ReadOnly);
    if (!certFileOpened)
    {
        SPDLOG_CRITICAL("WebSocket: Unable to open certificate file: {}", cert_file.toStdString());
    }
    bool keyFileOpened = keyFile.open(QIODevice::ReadOnly);
    if (!keyFileOpened)
    {
        SPDLOG_CRITICAL("WebSocket: Unable to open key file: {}", key_file.toStdString());
    }
    QSslCertificate certificate(&certFile, QSsl::Pem);
    QSslKey sslKey(&keyFile, QSsl::Rsa, QSsl::Pem);
    certFile.close();
    keyFile.close();
    sslConfiguration.setPeerVerifyMode(QSslSocket::VerifyNone);
    sslConfiguration.setLocalCertificate(certificate);
    sslConfiguration.setPrivateKey(sslKey);
    m_pWebSocketServer->setSslConfiguration(sslConfiguration);
    if (!m_pWebSocketServer->listen(QHostAddress::Any, port))
    {
        SPDLOG_CRITICAL("Unable to bind websocket to port {}", port);
        exit(-1);
    }
    connect(m_pWebSocketServer, &QWebSocketServer::newConnection, this, &Websocket::onNewConnection);
    connect(m_pWebSocketServer, &QWebSocketServer::closed, this, &Websocket::closed);
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
}

void Websocket::onNewConnection()
{
    QWebSocket *pSocket = m_pWebSocketServer->nextPendingConnection();

    connect(pSocket, &QWebSocket::textMessageReceived, this, &Websocket::processTextMessage);
    connect(pSocket, &QWebSocket::disconnected, this, &Websocket::socketDisconnected);

    m_clients << pSocket;
}

void Websocket::processTextMessage(QString message)
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    emit on_message(pClient, message);
}

void Websocket::socketDisconnected()
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (pClient)
    {
        m_clients.removeAll(pClient);
        pClient->deleteLater();
    }
}
