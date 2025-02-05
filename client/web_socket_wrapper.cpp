#include "web_socket_wrapper.h"
#include "spdlog/spdlog.h"
#include "magic_enum.hpp"

Web_socket_wrapper::Web_socket_wrapper(const QUrl &url, QObject *parent) :
      QObject(parent),
      m_web_socket(new QWebSocket("", QWebSocketProtocol::VersionLatest, this)),
      m_ping_timer(new QTimer(this)),
      m_pong_timer(new QTimer(this))
{
    connect(m_web_socket, &QWebSocket::connected, this, &Web_socket_wrapper::m_on_connected);
    connect(m_web_socket, &QWebSocket::disconnected, this, &Web_socket_wrapper::on_closed);
    connect(m_web_socket, &QWebSocket::pong, this, &Web_socket_wrapper::m_on_pong);
    connect(m_ping_timer, &QTimer::timeout, this, [=, this]{
        m_web_socket->ping();
        m_pong_timer->start(500);
    });
    connect(m_pong_timer, &QTimer::timeout, this, &Web_socket_wrapper::m_on_pong_timeout);
    m_ping_timer->setSingleShot(true);
    m_pong_timer->setSingleShot(true);
    m_web_socket->open(url);
}

Web_socket_wrapper::~Web_socket_wrapper()
{
    close();
}

void Web_socket_wrapper::send_command(Command command, size_t id)
{
    json j;
    switch (command)
    {
        case Command::get_UI_element:
            j["command"]["type"] = magic_enum::enum_name(command).data();
            j["command"]["id"] = id;
            m_web_socket->sendTextMessage(QString(j.dump().data()));
            break;
        case Command::get_UI_elements:
            j["command"]["type"] = magic_enum::enum_name(command).data();
            m_web_socket->sendTextMessage(QString(j.dump().data()));
            break;
        case Command::get_active_simulator_name:
            j["command"]["type"] = magic_enum::enum_name(command).data();
            m_web_socket->sendTextMessage(QString(j.dump().data()));
            break;
        case Command::get_simulators:
            j["command"]["type"] = magic_enum::enum_name(command).data();
            m_web_socket->sendTextMessage(QString(j.dump().data()));
            break;
        default:
            break;
    }
}

void Web_socket_wrapper::send_command(Command command, QString id)
{
    json j;
    switch (command)
    {
        case Command::switch_simulator:
            j["command"]["type"] = magic_enum::enum_name(command).data();
            j["command"]["name"] = id.toStdString();
            m_web_socket->sendTextMessage(QString(j.dump().data()));
            break;
        default:
            break;
    }
}

void Web_socket_wrapper::send_event(Event event, size_t id)
{
    json j;
    switch (event)
    {
        case Event::clicked:
        case Event::can_clear:
            j["event"]["type"] = magic_enum::enum_name(event).data();
            j["event"]["id"] = id;
            m_web_socket->sendTextMessage(QString(j.dump().data()));
            break;            
        default:
            break;
    }
}

void Web_socket_wrapper::send_event(Event event, size_t id, double val)
{
    json j;
    switch (event)
    {
        case Event::value_changed:
            j["event"]["type"] = magic_enum::enum_name(event).data();
            j["event"]["id"] = id;
            j["event"]["value"] = val;
            m_web_socket->sendTextMessage(QString(j.dump().data()));
            break;
        default:
            break;
    }
}

void Web_socket_wrapper::send_event(Event event, size_t id, QString val)
{
    json j;
    switch (event)
    {
        case Event::text_changed:
            j["event"]["type"] = magic_enum::enum_name(event).data();
            j["event"]["id"] = id;
            j["event"]["text"] = val.toStdString();
            m_web_socket->sendTextMessage(QString(j.dump().data()));
            break;
        case Event::selected:
            j["event"]["type"] = magic_enum::enum_name(event).data();
            j["event"]["id"] = id;
            j["event"]["selected"] = val.toStdString();
            m_web_socket->sendTextMessage(QString(j.dump().data()));
            break;
        default:
            break;
    }
}

void Web_socket_wrapper::send_event(Event event, size_t id, uint32_t sid, uint8_t dlc, std::array<uint8_t, 8> payload)
{
    json j;
    switch (event)
    {
        case Event::can_send:
            j["event"]["type"] = magic_enum::enum_name(event).data();
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
    connect(m_web_socket, &QWebSocket::textMessageReceived,
            this, &Web_socket_wrapper::m_on_received);

    m_ping_timer->start(1000);

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
    if (elapsedTime > 500)
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
