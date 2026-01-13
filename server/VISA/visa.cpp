#include "visa.hpp"
#include <QCoreApplication>

Visa::Visa(QObject *parent)
    : QObject(parent), m_tcp_socket(new QTcpSocket(this)), m_connected(false), m_reconnect(false), m_data_ready(false)
{
    QObject::connect(&m_client, &Tcp_client::connected, this, [=, this] {
        m_connected = true;
        emit s_connected();
    });
    QObject::connect(&m_client, &Tcp_client::disconnected, this, [=, this] {
        m_connected = false;
        emit s_disconnected();
        if (m_reconnect)
        {
            connect(m_ip, m_port);
            m_reconnect = false;
        }
    });
    QObject::connect(m_tcp_socket, &QTcpSocket::readyRead, this, [=, this] {
        m_data = QString::fromUtf8(m_tcp_socket->readAll());
        m_data_ready = true;
        emit s_data_ready(m_data);
    });
}
Visa::~Visa()
{
    if (m_connected)
        m_client.disconnect();
}

void Visa::connect(QString ip, quint16 port)
{
    if (m_connected)
    {
        m_client.disconnect();
        m_ip = ip;
        m_port = port;
        m_reconnect = true;
        return;
    }
    m_client.tcp_connect(ip, port);
}

void Visa::disconnect()
{
    m_client.tcp_disconnect();
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
