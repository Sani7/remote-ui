#pragma once
#include <QObject>
#include <map>
#include <memory>
#include <mutex>
#include <nlohmann/json.hpp>

#define INSERT_SIMULATOR(type)                                                                                         \
    m_simulators.insert(std::make_pair(type(&m_com).name(), std::make_unique<type>(&m_com, this)));

Q_FORWARD_DECLARE_OBJC_CLASS(QThread);
Q_FORWARD_DECLARE_OBJC_CLASS(Websocket);
Q_FORWARD_DECLARE_OBJC_CLASS(CAN_Wrapper);
Q_FORWARD_DECLARE_OBJC_CLASS(QSerialPort);
Q_FORWARD_DECLARE_OBJC_CLASS(Simulator_base);
Q_FORWARD_DECLARE_OBJC_CLASS(Communication);

using json = nlohmann::json;

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
     * @param can_dev The CAN device path
     * @param uart_dev The UART device path
     * @param parent The parent QObject
     */
    Simulators(uint16_t port, QString can_dev, QString uart_dev, QObject *parent = nullptr);
    /**
     * @brief Destroy the Simulators object
     *
     */
    ~Simulators();

    /**
     * @brief Get the name of the active simulator
     *
     * @return std::string The name of the active simulator
     */
    std::string active_simulator_name() const;
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
    void switch_simulator(std::string name);
    /**
     * @brief List all available simulators
     *
     * @return std::vector<std::string> A list of simulator names
     */
    std::vector<std::string> list_simulators() const;
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
    json changed_UI_items();

    // Functions in message_parser.cpp
    /**
     * @brief Parse a websocket message
     *
     * @param message The message to parse
     * @return QString The parsed message
     */
    QString message_parser(QString message);
    /**
     * @brief Parse a command in json format
     *
     * @param command The command to parse
     * @return json The parsed command
     */
    json command_parser(json command);
    /**
     * @brief Handle an event in json format
     *
     * @param event The event to handle
     */
    void event_handler(json event);

  private:
    QThread *m_server_thread;
    Websocket *m_server;
    CAN_Wrapper *m_can_wrapper;
    QSerialPort *m_serial;
    Communication *m_com;
    std::map<std::string, std::unique_ptr<Simulator_base>> m_simulators;
    std::string m_current_simulator = "";
    std::mutex m_mutex;
    json m_before;
};
