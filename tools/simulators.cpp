#include "simulators.hpp"
#include "can_debugger.hpp"
#include "test_sim.hpp"
#include <QDebug>
#include <unordered_set>

Simulators::Simulators(uint16_t port, QObject *parrent) : QObject(parrent), m_server(new Websocket(port, this))
{
    INSERT_SIMULATOR(Test_Sim);
    INSERT_SIMULATOR(Can_Debugger);
    setup_connections();
}

void Simulators::setup_connections()
{
    connect(m_server, &Websocket::on_message, this, [this](QWebSocket *conn, QString message) {
        QString response = message_parser(message);
        if (response.isEmpty() || response == "{}")
            return;
        m_server->send(conn, response);
    });
}

std::string Simulators::active_simulator_name() const
{
    return this->m_current_simulator;
}

void Simulators::run()
{
    if (this->m_simulators.empty())
    {
        spdlog::warn("no simulators found");
        return;
    }
    if (this->m_current_simulator.empty())
    {
        spdlog::warn("no active simulator");
        return;
    }

    m_before = this->m_simulators.at(m_current_simulator)->get_UI_items();
    this->m_simulators.at(m_current_simulator)->run();
}

void Simulators::stop()
{
    if (this->m_simulators.empty())
    {
        spdlog::warn("no simulators found");
        return;
    }
    if (this->m_current_simulator.empty())
    {
        spdlog::warn("no active simulator");
        return;
    }

    this->m_simulators.at(m_current_simulator)->stop();
}

void Simulators::switch_simulator(std::string name)
{
    if (this->m_simulators.empty())
    {
        spdlog::warn("no simulators found");
        return;
    }
    if (!this->m_current_simulator.empty())
    {
        spdlog::info("stopping current simulator {} and switching to {}", m_current_simulator, name);
        this->m_simulators.at(m_current_simulator)->stop();
    }

    if (this->m_simulators.find(name) == this->m_simulators.end())
    {
        spdlog::warn("simulator {} not found", name);
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
    changed["event"]["type"] = "UI_changed";

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
        spdlog::error(e.what());
    }

    m_before = after;
    return changed;
}
