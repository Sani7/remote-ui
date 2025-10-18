#include "web_socket_wrapper.hpp"
#include "spdlog/spdlog.h"
#include <QTimer>
#include <QtWebSockets/QWebSocket>
#include <magic_enum/magic_enum.hpp>

Web_socket_wrapper::Web_socket_wrapper(const QUrl &url, QObject *parent)
    : QObject(parent), m_web_socket(new QWebSocket("", QWebSocketProtocol::VersionLatest, this)),
      m_ping_timer(new QTimer(this)), m_pong_timer(new QTimer(this))
{
    connect(m_web_socket, &QWebSocket::connected, this, &Web_socket_wrapper::m_on_connected);
    connect(m_web_socket, &QWebSocket::disconnected, this, &Web_socket_wrapper::on_closed);
    connect(m_web_socket, &QWebSocket::pong, this, &Web_socket_wrapper::m_on_pong);
    connect(m_web_socket, &QWebSocket::errorOccurred, this,
            [=](QAbstractSocket::SocketError error) { SPDLOG_ERROR(magic_enum::enum_name(error)); });
    connect(m_ping_timer, &QTimer::timeout, this, [=, this] {
        m_web_socket->ping();
        m_pong_timer->start(1000);
    });
    connect(m_pong_timer, &QTimer::timeout, this, &Web_socket_wrapper::m_on_pong_timeout);
    m_ping_timer->setSingleShot(true);
    m_pong_timer->setSingleShot(true);
    m_web_socket->setProxy(QNetworkProxy::NoProxy);
    m_web_socket->open(url);
}

Web_socket_wrapper::~Web_socket_wrapper()
{
    close();
}

void Web_socket_wrapper::send_command(Command c, size_t id)
{
    json j;
    j["command"]["type"] = magic_enum::enum_name(c).data();
    switch (c)
    {
    case Command::get_UI_element:
        j["command"]["id"] = id;
        m_web_socket->sendTextMessage(QString(j.dump().data()));
        break;
    case Command::get_UI_elements:
        m_web_socket->sendTextMessage(QString(j.dump().data()));
        break;
    case Command::get_active_simulator_name:
        m_web_socket->sendTextMessage(QString(j.dump().data()));
        break;
    case Command::get_simulators:
        m_web_socket->sendTextMessage(QString(j.dump().data()));
        break;
    default:
        break;
    }
}

void Web_socket_wrapper::send_command(Command command, QString val)
{
    json j;
    j["command"]["type"] = magic_enum::enum_name(command).data();
    switch (command)
    {
    case Command::switch_simulator:
        j["command"]["name"] = val.toStdString();
        m_web_socket->sendTextMessage(QString(j.dump().data()));
        break;
    default:
        break;
    }
}

void Web_socket_wrapper::send_event(Event e, size_t id)
{
    if (m_inhibit_events)
        return;
    json j;
    j["event"]["type"] = magic_enum::enum_name(e).data();
    switch (e)
    {
    case Event::clicked:
    case Event::clear:
        j["event"]["id"] = id;
        m_web_socket->sendTextMessage(QString(j.dump().data()));
        break;
    default:
        break;
    }
}

void Web_socket_wrapper::send_event(Event e, size_t id, size_t val)
{
    if (m_inhibit_events)
        return;
    json j;
    j["event"]["type"] = magic_enum::enum_name(e).data();
    switch (e)
    {
    case Event::selected:
        j["event"]["id"] = id;
        j["event"]["selected"] = val;
        m_web_socket->sendTextMessage(QString(j.dump().data()));
        break;
    default:
        break;
    }
}

void Web_socket_wrapper::send_event(Event e, size_t id, double val)
{
    if (m_inhibit_events)
        return;
    json j;
    j["event"]["type"] = magic_enum::enum_name(e).data();
    switch (e)
    {
    case Event::value_changed:
        j["event"]["id"] = id;
        j["event"]["value"] = val;
        m_web_socket->sendTextMessage(QString(j.dump().data()));
        break;
    default:
        break;
    }
}

void Web_socket_wrapper::send_event(Event e, size_t id, QString val)
{
    if (m_inhibit_events)
        return;
    json j;
    j["event"]["type"] = magic_enum::enum_name(e).data();
    switch (e)
    {
    case Event::text_changed:
        j["event"]["id"] = id;
        j["event"]["text"] = val.toStdString();
        m_web_socket->sendTextMessage(QString(j.dump().data()));
        break;
    default:
        break;
    }
}

void Web_socket_wrapper::send_event(Event e, size_t id, uint32_t sid, uint8_t dlc, std::array<uint8_t, 8> payload)
{
    if (m_inhibit_events)
        return;
    json j;
    j["event"]["type"] = magic_enum::enum_name(e).data();
    switch (e)
    {
    case Event::can_send:
        j["event"]["id"] = id;
        j["event"]["sid"] = sid;
        j["event"]["dlc"] = dlc;
        j["event"]["payload"] = payload;
        m_web_socket->sendTextMessage(QString(j.dump().data()));
        break;
    default:
        break;
    }
}

void Web_socket_wrapper::close()
{
    if (m_connected)
        m_web_socket->close();
}

void Web_socket_wrapper::m_on_connected()
{
    m_connected = true;
    connect(m_web_socket, &QWebSocket::textMessageReceived, this, &Web_socket_wrapper::m_on_received);

    m_ping_timer->start(5000);

    emit on_connected();
}

void Web_socket_wrapper::m_on_received(QString message)
{
    json j = json::parse(message.toStdString());
    if (j.contains("response"))
    {
        emit on_command_cb(j.at("response"));
        return;
    }
    if (j.contains("event"))
    {
        emit on_event_cb(j.at("event"));
        return;
    }
}

void Web_socket_wrapper::m_on_pong(quint64 elapsedTime)
{
    if (elapsedTime > 1000)
    {
        m_web_socket->close();
        emit on_closed();
    }

    m_pong_timer->stop();
    m_ping_timer->start(1000);
}

void Web_socket_wrapper::m_on_pong_timeout()
{
    SPDLOG_CRITICAL("Pong timeout");
    m_web_socket->close();
    emit on_closed();
}

void Web_socket_wrapper::inhibit_events(bool inhibit)
{
    m_inhibit_events = inhibit;
}
