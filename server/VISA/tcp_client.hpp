#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QTcpSocket>

#define QD qDebug() << "File:" << __FILE__ << "Function:" << __PRETTY_FUNCTION__ << "Line:" << __LINE__ << "=>"

class Tcp_client : public QObject
{
    Q_OBJECT
  public:
    Tcp_client();
    void tcp_connect(QString ip, quint16 port);
    void tcp_disconnect(void);
    void tcp_send(QString data);

  signals:
    void data_ready(QString data);
    void connected();
    void disconnected();

  private:
    void ready_to_read();
    QTcpSocket *m_tcp_socket;
};

#endif // TCPCLIENT_H
