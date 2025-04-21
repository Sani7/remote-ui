#pragma once
#include <QThread>
#include <map>
#include <memory>
#include <mutex>

#include "CAN_wrapper.hpp"
#include "simulator_base.hpp"
#include "websocket.hpp"

#define INSERT_SIMULATOR(type) m_simulators.insert(std::make_pair(type().name(), std::make_unique<type>(&m_com, this)));

class Simulators : public QObject
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
        can_send,
        can_clear,
        ui_changed,
        end,
        begin = 0
    };

    Simulators(uint16_t port, QString can_dev, QString uart_dev, QObject *parent = nullptr);
    ~Simulators();
    std::string active_simulator_name() const;
    void run();
    void stop();
    void switch_simulator(std::string name);
    std::vector<std::string> list_simulators() const;
    Simulator_base *invoke_active_simulator();
    json changed_UI_items();

    // parsing of websocket messages
    QString message_parser(QString message);
    json command_parser(json command);
    void event_handler(json event);

  private:
    QThread *m_server_thread;
    Websocket *m_server;
    CAN_Wrapper *m_can_wrapper;
    QSerialPort *m_serial;
    Communication m_com;
    std::map<std::string, std::unique_ptr<Simulator_base>> m_simulators;
    std::string m_current_simulator = "";
    std::mutex m_mutex;
    json m_before;
};
