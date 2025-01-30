#pragma once
#include <map>
#include <memory>
#include <mutex>

#include "simulator_base.hpp"
#include "websocket.hpp"

#define INSERT_SIMULATOR(type) m_simulators.insert(std::make_pair(type().name(), std::make_unique<type>()));

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
        ui_changed,
        end,
        begin = 0
    };

    Simulators(uint16_t port, QString can_dev, QObject *parent = nullptr);
    void setup_connections();
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
    Websocket *m_server;
    std::map<std::string, std::unique_ptr<Simulator_base>> m_simulators;
    std::string m_current_simulator = "";
    std::mutex m_mutex;
    json m_before;
};
