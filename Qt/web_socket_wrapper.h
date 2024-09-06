#ifndef WEB_SOCKET_WRAPPER_H
#define WEB_SOCKET_WRAPPER_H

#include <QtWebSockets/QWebSocket>
#include <QTimer>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

class Web_socket_wrapper : public QObject
{
    Q_OBJECT
  public:
    enum class Command
    {
        get_UI_element,
        get_UI_elements,
        switch_simulator,
        get_active_simulator_name,
        get_simulators,
        end,
        begin = 0
    };

    enum class Event
    {
        clicked,
        value_changed,
        text_changed,
        selected,
        UI_changed,
        end,
        begin = 0
    };
  public:
    explicit Web_socket_wrapper(const QUrl &url, QObject *parent = nullptr);
    ~Web_socket_wrapper();
    void send_command(Command command, QString id = QString());
    void send_event(Event event, QString id);
    void send_event(Event event, QString id, double val);
    void send_event(Event event, QString id, QString val);
    void close();

  Q_SIGNALS:
    void on_connected();
    void on_closed();
    void on_command_cb(json& data);
    void on_event_cb(json& data);

  private Q_SLOTS:
    void m_on_connected();
    void m_on_received(QString message);
    void m_on_pong(quint64 elapsedTime);
    void m_on_pong_timeout();

  private:
    bool m_connected = false;
    QWebSocket* m_web_socket;
    QTimer* m_ping_timer;
    QTimer* m_pong_timer;
};

#endif // WEB_SOCKET_WRAPPER_H
