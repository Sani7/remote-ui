#pragma once
#include <QObject>
#include <nlohmann/json.hpp>
Q_FORWARD_DECLARE_OBJC_CLASS(QWebSocket);
Q_FORWARD_DECLARE_OBJC_CLASS(QTimer);
using json = nlohmann::json;

/**
 * @brief Class wrapping a WebSocket connection
 *
 */
class Web_socket_wrapper : public QObject
{
    Q_OBJECT
  public:
    /**
     * @brief Enumeration of commands
     */ 
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

    /**
     * @brief Enumeration of events
     *
     */
    enum class Event
    {
        clicked,
        value_changed,
        text_changed,
        selected,
        can_send,
        clear,
        ui_changed,
        end,
        begin = 0
    };

  public:
    /**
     * @brief Construct a new Web_socket_wrapper object
     *
     * @param url The WebSocket URL to connect to
     * @param parent The parent QObject
     */
    explicit Web_socket_wrapper(const QUrl &url, QObject *parent = nullptr);

    /**
     * @brief Send a command to the server
     *
     * @param c The command to send
     * @param id The ID associated with the command (if applicable)
     */
    void send_command(Command c, size_t id = 0);
    /**
     * @brief Send a command to the server with a string value
     *
     * @param c The command to send
     * @param val The string value associated with the command
     */
    void send_command(Command c, QString val);

    /**
     * @brief Send an event to the server
     *
     * @param e The event to send
     * @param id The ID associated with the event
     */
    void send_event(Event e, size_t id);
    /**
     * @brief Send an event to the server with a size_t value
     *
     * @param e The event to send
     * @param id The ID associated with the event
     * @param val The size_t value associated with the event
     */
    void send_event(Event e, size_t id, size_t val);
    /**
     * @brief Send an event to the server with a double value
     *
     * @param e The event to send
     * @param id The ID associated with the event
     * @param val The double value associated with the event
     */
    void send_event(Event e, size_t id, double val);
    /**
     * @brief Send an event to the server with a string value
     *
     * @param e The event to send
     * @param id The ID associated with the event
     * @param val The string value associated with the event
     */
    void send_event(Event e, size_t id, QString val);
    /**
     * @brief Send a CAN event to the server
     *
     * @param e The event to send
     * @param id The ID associated with the event
     * @param sid The CAN standard identifier
     * @param dlc The CAN data length code
     * @param payload The CAN data payload
     */
    void send_event(Event e, size_t id, uint32_t sid, uint8_t dlc, std::array<uint8_t, 8> payload);
    /**
     * @brief Close the WebSocket connection
     *
     */
    void close();
    /**
     * @brief Inhibit or allow sending events to the server
     *
     * @param inhibit True to inhibit events, false to allow
     */
    void inhibit_events(bool inhibit);

  Q_SIGNALS:
    /**
     * @brief Signal emitted when the WebSocket is connected
     *
     */
    void on_connected();
    /**
     * @brief Signal emitted when the WebSocket is closed
     *
     */
    void on_closed();
    /**
     * @brief Signal emitted when a command is received from the server
     *
     * @param data The JSON command data
     */
    void on_command_cb(json &data);
    /**
     * @brief Signal emitted when an event is received from the server
     *
     * @param data The JSON event data
     */
    void on_event_cb(json &data);

  private Q_SLOTS:
    /**
     * @brief Internal slot for handling WebSocket connection established
     *
     */
    void m_on_connected();
    /**
     * @brief Internal slot for handling WebSocket disconnection
     *
     */
    void m_on_received(QString message);
    /**
     * @brief Internal slot for handling pong responses
     *
     * @param elapsedTime The elapsed time since the ping was sent
     */
    void m_on_pong(quint64 elapsedTime);
    /**
     * @brief Internal slot for handling pong timeouts
     *
     */
    void m_on_pong_timeout();

  private:
    bool m_connected = false;
    bool m_inhibit_events = false;
    QWebSocket *m_web_socket;
    QTimer *m_ping_timer;
    QTimer *m_pong_timer;
};
