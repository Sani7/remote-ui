#include "simulators.hpp"
#include "CAN_wrapper.hpp"
#include "simulator_base.hpp"
#include "websocket.hpp"
#include <QDirIterator>
#include <QLibrary>
#include <QThread>
#include <unordered_set>

typedef Simulator_base *(*Get_Sim)(Communication *, QObject *);

Simulators::Simulators(uint16_t port, QStringList can_devs, QStringList uart_devs, QObject *parent)
    : QObject(parent), m_server_thread(new QThread), m_server(new Websocket(port, nullptr)), m_com(new Communication)
{
    m_server->moveToThread(m_server_thread);
    m_server_thread->start();

    size_t count = 0;
    for (const auto &can_dev : can_devs)
    {
        count++;
        if (can_dev.isEmpty())
            continue;
        m_com->can_if[count - 1] = std::make_unique<CAN_Interface>(this);
        m_com->can_if[count - 1]->connect_to_dev(can_dev);
    }

    count = 0;
    for (const auto &uart_dev : uart_devs)
    {
        count++;
        if (uart_dev.isEmpty())
            continue;
        m_com->uart_if[count - 1] = std::make_unique<QSerialPort>(this);

        m_com->uart_if[count - 1]->setPortName("/dev/" + uart_dev);
        m_com->uart_if[count - 1]->setBaudRate(QSerialPort::Baud115200);
        m_com->uart_if[count - 1]->setDataBits(QSerialPort::Data8);
        m_com->uart_if[count - 1]->setParity(QSerialPort::NoParity);
        m_com->uart_if[count - 1]->setStopBits(QSerialPort::OneStop);
        m_com->uart_if[count - 1]->setFlowControl(QSerialPort::NoFlowControl);

        if (!m_com->uart_if[count - 1]->open(QIODevice::ReadWrite))
        {
            SPDLOG_CRITICAL("UART: /dev/{} {}", m_com->uart_if[count - 1]->portName().toStdString(),
                            m_com->uart_if[count - 1]->errorString().toStdString());
            m_com->uart_if[count - 1]->deleteLater();
            continue;
        }
    }

    QLibrary lib;
    QDirIterator it(QCoreApplication::applicationDirPath(), QStringList() << "*.so", QDir::Files,
                    QDirIterator::Subdirectories);
    while (it.hasNext())
    {
        lib.setFileName(it.next());
        lib.load();
        auto loaded_sim = Get_Sim(lib.resolve("get_sim"));
        if (loaded_sim == nullptr)
        {
            SPDLOG_INFO("{} NOT FOUND", lib.fileName().toStdString());
            lib.unload();
            continue;
        }
        auto widget = loaded_sim(m_com, this);
        m_simulators.insert(std::make_pair(widget->name(), widget));
        lib.unload();
    }

    // Insert Debug sims here
    // INSERT_SIMULATOR(TEST_SIM);

    connect(m_server, &Websocket::on_message, this, [this](QWebSocket *conn, QString message) {
        QString response = message_parser(message);
        if (response == "{}")
            return;
        m_server->send(conn, response);
    });
}

Simulators::~Simulators()
{
}

std::string Simulators::active_simulator_name() const
{
    return this->m_current_simulator;
}

void Simulators::start()
{
    if (this->m_simulators.empty())
    {
        SPDLOG_WARN("No simulators found");
        return;
    }
    if (this->m_current_simulator.empty())
    {
        SPDLOG_WARN("No active simulator");
        return;
    }

    m_before = this->m_simulators.at(m_current_simulator)->get_UI_items();
    this->m_simulators.at(m_current_simulator)->start();
}

void Simulators::stop()
{
    if (this->m_simulators.empty())
    {
        SPDLOG_WARN("No simulators found");
        return;
    }
    if (this->m_current_simulator.empty())
    {
        SPDLOG_WARN("No active simulator");
        return;
    }

    this->m_simulators.at(m_current_simulator)->stop();
}

void Simulators::switch_simulator(std::string name)
{
    if (this->m_simulators.empty())
    {
        SPDLOG_WARN("No simulators found");
        return;
    }
    if (this->m_simulators.find(name) == this->m_simulators.end())
    {
        SPDLOG_WARN("Simulator {} not found", name);
        return;
    }

    if (!this->m_current_simulator.empty())
    {
        SPDLOG_INFO("Stopping current simulator {} and switching to {}", m_current_simulator, name);
        this->m_simulators.at(m_current_simulator)->stop();
        disconnect(m_simulators.at(m_current_simulator).get());
    }

    this->m_current_simulator = name;
    connect(
        m_simulators.at(m_current_simulator).get(), &Simulator_base::sim_changed, this,
        [this] {
            auto json = changed_UI_items();
            if (!json.empty())
            {
                m_server->broadcast(QString::fromStdString(json.dump()));
            }
        },
        Qt::QueuedConnection);
    this->start();
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

bool Simulators::is_not_active() const
{
    return this->m_current_simulator.empty();
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
