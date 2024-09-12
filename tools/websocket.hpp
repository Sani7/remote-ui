#pragma once

#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QByteArray>

QT_FORWARD_DECLARE_CLASS(QWebSocketServer)
QT_FORWARD_DECLARE_CLASS(QWebSocket)

class Websocket : public QObject
{
    Q_OBJECT

  public:
    Websocket(uint16_t port, QObject *parent = nullptr);
    ~Websocket();

    void broadcast(QString message);
    void send(QWebSocket* conn, QString message);
  signals:
    void on_message(QWebSocket* conn, QString message);
    void closed();

  private slots:
    void onNewConnection();
    void processTextMessage(QString message);
    void processBinaryMessage(QByteArray message);
    void socketDisconnected();

  private:
    QWebSocketServer *m_pWebSocketServer;
    QList<QWebSocket *> m_clients;
};
