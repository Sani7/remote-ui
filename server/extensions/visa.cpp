#include "visa.hpp"
#include <QCoreApplication>
#include <QTcpSocket>

Visa::Visa(QObject *parent)
    : QObject(parent), m_tcp_socket(new QTcpSocket(this)), m_connected(false), m_reconnect(false)
{
    QObject::connect(m_tcp_socket, &QTcpSocket::connected, this, [=, this] {
        m_connected = true;
        emit s_connected();
    });
    QObject::connect(m_tcp_socket, &QTcpSocket::disconnected, this, [=, this] {
        m_connected = false;
        emit s_disconnected();
        if (m_reconnect)
        {
            connect(m_ip, m_port);
            m_reconnect = false;
        }
    });
    QObject::connect(m_tcp_socket, &QTcpSocket::readyRead, this, [=, this] {
        emit s_data_ready(QString::fromUtf8(m_tcp_socket->readAll()));
    });
}
Visa::~Visa()
{
    if (m_connected)
        m_tcp_socket->disconnect();
}

void Visa::connect(QString ip, quint16 port)
{
    if (m_connected)
    {
        m_tcp_socket->disconnect();
        m_ip = ip;
        m_port = port;
        m_reconnect = true;
        return;
    }
    m_tcp_socket->connectToHost(ip, port);
}

void Visa::disconnect()
{
    m_tcp_socket->disconnect();
}

bool Visa::connected() const
{
    return m_connected;
}

void Visa::send(QString data)
{
    data.append('\n');
    m_tcp_socket->write(data.toUtf8());
    m_tcp_socket->waitForBytesWritten();
}

QString Visa::query(QString data)
{
    uint32_t counter = 0;
    m_data.clear();
    send(data);
    while (!m_data_ready && counter < 1024)
    {
        QCoreApplication::processEvents();
        counter++;
    }

    return m_data;
}
