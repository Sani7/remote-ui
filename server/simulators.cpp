#include "simulators.hpp"
#define INTERNAL
#include "simulator_base.hpp"
#include "spdlog/spdlog.h"
#include "websocket.hpp"
#include <QCoreApplication>
#include <QDirIterator>
#include <QLibrary>
#include <QThread>

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

    QDirIterator it(QCoreApplication::applicationDirPath(), QStringList() << "*libsim_*.so", QDir::Files,
                    QDirIterator::Subdirectories);
    while (it.hasNext())
    {
        QLibrary lib;
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
        if (widget == nullptr)
        {
            lib.unload();
            continue;
        }
        m_simulators.insert(widget->name(), widget);
    }

    // Insert Debug sims here
    // INSERT_SIMULATOR(TEST_SIM);

    connect(m_server, &Websocket::on_binary_message, this, [this](QWebSocket *conn, QByteArray message) {
        QByteArray response = message_parser(message);
        if (response.isEmpty())
            return;
        m_server->send_binary(conn, response);
    });
}

Simulators::~Simulators()
{
}

QString Simulators::active_simulator_name() const
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
    if (this->m_current_simulator.isEmpty())
    {
        SPDLOG_WARN("No active simulator");
        return;
    }

    m_before = this->m_simulators.value(m_current_simulator)->get_UI_items();
    this->m_simulators.value(m_current_simulator)->start();
}

void Simulators::stop()
{
    if (this->m_simulators.empty())
    {
        SPDLOG_WARN("No simulators found");
        return;
    }
    if (this->m_current_simulator.isEmpty())
    {
        SPDLOG_WARN("No active simulator");
        return;
    }

    this->m_simulators.value(m_current_simulator)->stop();
}

void Simulators::switch_simulator(QString name)
{
    if (this->m_simulators.empty())
    {
        SPDLOG_WARN("No simulators found");
        return;
    }
    if (this->m_simulators.find(name) == this->m_simulators.end())
    {
        SPDLOG_WARN("Simulator {} not found", name.toStdString());
        return;
    }

    if (!this->m_current_simulator.isEmpty())
    {
        SPDLOG_INFO("Stopping current simulator {} and switching to {}", m_current_simulator.toStdString(), name.toStdString());
        this->m_simulators.value(m_current_simulator)->stop();
        disconnect(m_simulators.value(m_current_simulator));
    }

    this->m_current_simulator = name;
    connect(
        m_simulators.value(m_current_simulator), &Simulator_base::sim_changed, this,
        [this] {
            auto changed = changed_UI_items();
            UI_items_m items;
            items.setItem(changed);
            if (!changed.isEmpty())
            {
                m_server->broadcast_binary(m_serializer.serialize(&items));
            }
        },
        Qt::QueuedConnection);
    connect(m_simulators.value(m_current_simulator), &Simulator_base::log_signal, this,
            [](const char *filename_in, int line_in, const char *funcname_in, int level, QString msg) {
                if (spdlog::default_logger_raw()->should_log((spdlog::level::level_enum)level))
                    spdlog::default_logger_raw()->log(spdlog::source_loc{filename_in, line_in, funcname_in},
                                                      (spdlog::level::level_enum)level, msg.toStdString());
            });
    this->start();
}

QList<QString> Simulators::list_simulators() const
{
    return this->m_simulators.keys();
}

bool Simulators::is_not_active() const
{
    return this->m_current_simulator.isEmpty();
}

Simulator_base *Simulators::invoke_active_simulator()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (this->m_simulators.empty())
    {
        return nullptr;
    }
    if (this->m_current_simulator.isEmpty())
    {
        return nullptr;
    }

    m_before = this->m_simulators.value(m_current_simulator)->get_UI_items();

    return this->m_simulators.value(m_current_simulator);
}

QList<UI_item_m> Simulators::changed_UI_items()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    QList<UI_item_m> ret;

    if (this->m_simulators.empty() || this->m_current_simulator.isEmpty())
        return ret;

    auto after = this->m_simulators.value(m_current_simulator)->get_UI_items();

    // Fast path: nothing at all changed
    if (after == m_before)
        return ret;

    const qsizetype count = after.size();
    for (qsizetype i = 0; i < count; ++i)
    {
        // If item is new or any field changed -> send full current item
        if (i >= m_before.size() || after[i] != m_before[i])
        {
            ret.push_back(after[i]);
        }
    }

    m_before = std::move(after);
    return ret;
}