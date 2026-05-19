#pragma once
#include <QObject>
#include <QMap>
#include <memory>
#include <mutex>
#include <QProtobufSerializer>
#include <messages.qpb.h>

#define INSERT_SIMULATOR(type)                                                                                         \
    m_simulators.insert(std::make_pair(type(&m_com).name(), std::make_unique<type>(&m_com, this)));

Q_FORWARD_DECLARE_OBJC_CLASS(QThread);
Q_FORWARD_DECLARE_OBJC_CLASS(Websocket);
Q_FORWARD_DECLARE_OBJC_CLASS(CAN_Wrapper);
Q_FORWARD_DECLARE_OBJC_CLASS(QSerialPort);
Q_FORWARD_DECLARE_OBJC_CLASS(Simulator_base);
Q_FORWARD_DECLARE_OBJC_CLASS(Communication);
Q_FORWARD_DECLARE_OBJC_CLASS(UI_item_m);

/**
 * @brief Class managing all simulators
 *
 */
class Simulators : public QObject
{
    Q_OBJECT
  public:
    /**
     * @brief Enumeration of commands
     *
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

    /**
     * @brief Construct a new Simulators object
     *
     * @param port The port for the websocket server
     * @param can_devs The list of CAN device paths
     * @param uart_devs The list of UART device paths
     * @param parent The parent QObject
     */
    Simulators(uint16_t port, QStringList can_devs, QStringList uart_devs, QObject *parent = nullptr);
    /**
     * @brief Destroy the Simulators object
     *
     */
    ~Simulators();

    /**
     * @brief Get the name of the active simulator
     *
     * @return QString The name of the active simulator
     */
    QString active_simulator_name() const;
    /**
     * @brief Start the active simulator
     *
     */
    void start();
    /**
     * @brief Stop the active simulator
     *
     */
    void stop();
    /**
     * @brief Switch to a different simulator
     *
     * @param name The name of the simulator to switch to
     */
    void switch_simulator(QString name);
    /**
     * @brief List all available simulators
     *
     * @return std::vector<QString> A list of simulator names
     */
    QList<QString> list_simulators() const;
    /**
     * @brief Check if no simulator is active
     *
     * @return true If no simulator is active
     * @return false If a simulator is active
     */
    bool is_not_active() const;
    /**
     * @brief Get a pointer to the active simulator
     *        Throws a lock on the simulators map
     *
     * @return Simulator_base* A pointer to the active simulator
     */
    Simulator_base *invoke_active_simulator();
    /**
     * @brief Get the changed UI items in json format
     *        Throws a lock on the simulators map
     *
     * @return json The changed UI items
     */
    QList<UI_item_m> changed_UI_items();

    // Functions in message_parser.cpp
    /**
     * @brief Parse a websocket message
     *
     * @param message The message to parse
     * @return QString The parsed message
     */
    QByteArray message_parser(QByteArray message);
    /**
     * @brief Parse a command in json format
     *
     * @param command The command to parse
     * @return json The parsed command
     */
    //json command_parser(json command);
    /**
     * @brief Handle an event in json format
     *
     * @param event The event to handle
     */
    void event_handler(Event_m event);

  private:
    QThread *m_server_thread;
    Websocket *m_server;
    Communication *m_com;
    QMap<QString, Simulator_base*> m_simulators;
    QString m_current_simulator = "";
    std::mutex m_mutex;
    QList<UI_item_m> m_before;
    QProtobufSerializer m_serializer;
};
