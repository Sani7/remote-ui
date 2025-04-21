#include "simulators.hpp"
#include "can_debugger.hpp"
#include "cvs_i10.hpp"
#include "test_sim.hpp"
#include <unordered_set>

Simulators::Simulators(uint16_t port, QString can_dev, QString uart_dev, QObject *parent)
    : QObject(parent), m_server_thread(new QThread), m_server(new Websocket(port, nullptr)),
      m_can_wrapper(new CAN_Wrapper(new CAN_Interface(this), this)), m_serial(new QSerialPort(this))
{
    m_server->moveToThread(m_server_thread);
    m_server_thread->start();
    if (!can_dev.isEmpty())
    {
        m_can_wrapper->connect_to_dev(can_dev);
    }
    if (!uart_dev.isEmpty())
    {
        m_serial->setPortName(uart_dev);
        m_serial->setBaudRate(QSerialPort::Baud115200);
        m_serial->setDataBits(QSerialPort::Data8);
        m_serial->setParity(QSerialPort::NoParity);
        m_serial->setStopBits(QSerialPort::OneStop);
        m_serial->setFlowControl(QSerialPort::NoFlowControl);

        if (!m_serial->open(QIODevice::ReadWrite))
        {
            SPDLOG_CRITICAL("Error {}", m_serial->errorString().toStdString());
        }
    }
    m_com.c_if1 = m_can_wrapper;
    m_com.s_if1 = m_serial;

    INSERT_SIMULATOR(Test_Sim);
    INSERT_SIMULATOR(Can_Debugger);

    connect(m_server, &Websocket::on_message, this, [this](QWebSocket *conn, QString message) {
        QString response = message_parser(message);
        if (response.isEmpty() || response == "{}")
            return;
        m_server->send(conn, response);
    });
}

Simulators::~Simulators()
{
    if (m_serial->isOpen())
        m_serial->close();
}

std::string Simulators::active_simulator_name() const
{
    return this->m_current_simulator;
}

void Simulators::run()
{
    if (this->m_simulators.empty())
    {
        SPDLOG_WARN("no simulators found");
        return;
    }
    if (this->m_current_simulator.empty())
    {
        SPDLOG_WARN("no active simulator");
        return;
    }

    m_before = this->m_simulators.at(m_current_simulator)->get_UI_items();
    this->m_simulators.at(m_current_simulator)->run();
}

void Simulators::stop()
{
    if (this->m_simulators.empty())
    {
        SPDLOG_WARN("no simulators found");
        return;
    }
    if (this->m_current_simulator.empty())
    {
        SPDLOG_WARN("no active simulator");
        return;
    }

    this->m_simulators.at(m_current_simulator)->stop();
}

void Simulators::switch_simulator(std::string name)
{
    if (this->m_simulators.empty())
    {
        SPDLOG_WARN("no simulators found");
        return;
    }
    if (!this->m_current_simulator.empty())
    {
        SPDLOG_INFO("stopping current simulator {} and switching to {}", m_current_simulator, name);
        this->m_simulators.at(m_current_simulator)->stop();
    }

    if (this->m_simulators.find(name) == this->m_simulators.end())
    {
        SPDLOG_WARN("simulator {} not found", name);
        return;
    }

    this->stop();
    if (!this->m_current_simulator.empty())
    {
        disconnect(m_simulators.at(m_current_simulator).get());
    }
    this->m_current_simulator = name;
    connect(m_simulators.at(m_current_simulator).get(), &Simulator_base::sim_changed, this,
            [this] { m_server->broadcast(QString::fromStdString(changed_UI_items().dump())); });
    this->run();
}

std::vector<std::string> Simulators::list_simulators() const
{
    std::vector<std::string> names;
    for (const auto &simulator : this->m_simulators)
    {
        names.push_back(simulator.first);
    }
    return names;
}

Simulator_base *Simulators::invoke_active_simulator()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (this->m_simulators.empty())
    {
        return nullptr;
    }
    if (this->m_current_simulator.empty())
    {
        return nullptr;
    }

    m_before = this->m_simulators.at(m_current_simulator)->get_UI_items();

    return this->m_simulators.at(m_current_simulator).get();
}

json Simulators::changed_UI_items()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (this->m_simulators.empty())
    {
        return "{}"_json;
    }
    if (this->m_current_simulator.empty())
    {
        return "{}"_json;
    }

    json after = this->m_simulators.at(m_current_simulator)->get_UI_items();
    json changed;
    changed["event"]["type"] = "ui_changed";

    json diff = json::diff(m_before, after);

    if (diff.empty())
    {
        return "{}"_json;
    }

    // diff to ui items list
    // get index out of each item in diff /UI_items/0/**
    // but the index can be the same for multiple items
    // Then push the ui_item at that index to the changed json
    std::unordered_set<size_t> addedIndices;

    try
    {
        for (auto &item : diff)
        {
            std::string path = item["path"];
            size_t index = std::stoul(path.substr(10, path.find("/", 10) - 10));
            if (addedIndices.find(index) != addedIndices.end())
                continue;

            changed["event"]["UI_items"].push_back(after["UI_items"][index]);
            addedIndices.insert(index);
        }
    }
    catch (const std::exception &e)
    {
        SPDLOG_ERROR(e.what());
    }

    m_before = after;
    return changed;
}
