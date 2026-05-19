#pragma once
#include <QtCore/QList>
#include <QtCore/QObject>

QT_FORWARD_DECLARE_CLASS(QWebSocketServer)
QT_FORWARD_DECLARE_CLASS(QWebSocket)

/**
 * @brief Class managing a WebSocket server
 *
 */
class Websocket : public QObject
{
    Q_OBJECT

  public:
    /**
     * @brief Construct a new Websocket object
     *
     * @param port The port to listen on
     * @param parent The parent QObject
     */
    Websocket(uint16_t port, QObject *parent = nullptr);
    /**
     * @brief Construct a new Websocket object with SSL
     *
     * @param port The port to listen on
     * @param key_file The path to the SSL key file
     * @param cert_file The path to the SSL certificate file
     * @param parent The parent QObject
     */
    Websocket(uint16_t port, QString key_file, QString cert_file, QObject *parent = nullptr);
    /**
     * @brief Destroy the Websocket object
     *
     */
    ~Websocket();

    /**
     * @brief Broadcast a message to all connected clients
     *
     * @param message The message to send
     */
    void broadcast_binary(QByteArray message);
    void broadcast_text(QString message);
    /**
     * @brief Send a message to a specific client
     *
     * @param conn The client connection
     * @param message The message to send
     */
    void send_binary(QWebSocket *conn, QByteArray message);
    void send_text(QWebSocket *conn, QString message);
  signals:
    /**
     * @brief Signal emitted when a message is received from a client
     *
     * @param conn The client connection
     * @param message The received message
     */
    void on_binary_message(QWebSocket *conn, QByteArray message);
    void on_text_message(QWebSocket *conn, QString message);
    /**
     * @brief Signal emitted when the server is closed
     *
     */
    void closed();

  private slots:
    /**
     * @brief Private Slot called when a new client connection is established
     *
     */
    void onNewConnection();
    /**
     * @brief Private Slot called when a text message is received from a client
     *
     * @param message The received message
     */
    void processBinaryMessage(QByteArray message);
    void processTextMessage(QString message);
    /**
     * @brief Private Slot called when a client disconnects
     *
     */
    void socketDisconnected();

  private:
    QWebSocketServer *m_pWebSocketServer;
    QList<QWebSocket *> m_clients;
};
