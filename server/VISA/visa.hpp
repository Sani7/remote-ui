#pragma once
#include <QObject>

class Visa : public QObject
{
    Q_OBJECT
  public:
    explicit Visa(QObject *parent = nullptr);
    ~Visa();
    void connect(QString ip, quint16 port);
    void disconnect();

    bool connected() const;

    void send(QString data);
    QString querry(QString data);

  signals:
    void s_disconnected();
    void s_connected();
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
