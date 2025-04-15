#include "tcp_client.hpp"

Tcp_client::Tcp_client() : QObject(0)
{
    m_tcp_socket = new QTcpSocket(this);

    connect(m_tcp_socket, &QTcpSocket::readyRead, this, [=, this] { ready_to_read(); });
    connect(m_tcp_socket, &QTcpSocket::connected, this, [=, this] { emit connected(); });
    connect(m_tcp_socket, &QTcpSocket::disconnected, this, [=, this] { emit disconnected(); });
}

void Tcp_client::tcp_connect(QString ip, quint16 port)
{
    m_tcp_socket->connectToHost(ip, port);
}

void Tcp_client::tcp_disconnect()
{
    m_tcp_socket->disconnectFromHost();
}

void Tcp_client::tcp_send(QString data)
{
    m_tcp_socket->write((const char *)data.data());
}

void Tcp_client::ready_to_read()
{
    emit data_ready(m_tcp_socket->read(1024));
}
