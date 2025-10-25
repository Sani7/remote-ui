#pragma once
#include <QObject>

/**
 * @brief Class representing a VISA interface also known as SCIPI over TCP/IP
 *
 */
class Visa : public QObject
{
    Q_OBJECT
  public:
    /**
     * @brief Construct a new Visa object
     *
     * @param parent The parent QObject
     */
    explicit Visa(QObject *parent = nullptr);
    /**
     * @brief Destroy the Visa object
     *
     */
    ~Visa();

    /**
     * @brief Connect to a VISA device
     *
     * @param ip The IP address of the VISA device
     * @param port The port number of the VISA device
     */
    void connect(QString ip, quint16 port);
    /**
     * @brief Disconnect from the VISA device
     *
     */
    void disconnect();

    /**
     * @brief Check if the VISA device is connected
     *
     * @return true If the device is connected
     * @return false If the device is not connected
     */
    bool connected() const;

    /**
     * @brief Send data to the VISA device
     *
     * @param data The data to send
     */
    void send(QString data);
    /**
     * @brief Query the VISA device
     *
     * @param data The query data
     * @return QString The response from the device
     */
    QString query(QString data);

  signals:
    /**
     * @brief Signal emitted when the device is disconnected
     *
     */
    void s_disconnected();
    /**
     * @brief Signal emitted when the device is connected
     *
     */
    void s_connected();
    /**
     * @brief Signal emitted when data is ready to be read
     *
     * @param data The data that is ready to be read
     */
    void s_data_ready(QString data);

  private:
    Tcp_client m_client;
    QString m_ip;
    quint16 m_port;
    bool m_connected;
    bool m_reconnect;
    bool m_data_ready;
    QString m_data;
};
