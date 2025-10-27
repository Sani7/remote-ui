#include <QtCore/QCommandLineOption>
#include <QtCore/QCommandLineParser>
#include <QtCore/QCoreApplication>

#include "spdlog/async.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"

#include "git_version.h"
#include "simulators.hpp"

void init_logger()
{
    spdlog::init_thread_pool(8192, 1);
    auto stdout_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    // auto rotating_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>("log-sim_us.txt", 1024 * 1024 * 10,
    // 3);
    std::vector<spdlog::sink_ptr> sinks{stdout_sink /*, rotating_sink*/};
    auto logger = std::make_shared<spdlog::async_logger>("unisim_cpp", sinks.begin(), sinks.end(),
                                                         spdlog::thread_pool(), spdlog::async_overflow_policy::block);
    spdlog::set_default_logger(logger);
    spdlog::set_pattern("[%H:%M:%S %z] [%^---%L---%$] %s:%# %!: %v");
    spdlog::flush_every(std::chrono::seconds(3));
    spdlog::set_level(spdlog::level::trace);
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QCommandLineParser parser;
    parser.setApplicationDescription("unisim_cpp server\n\tversion: " + QString::fromStdString(Git_version::branch) +
                                     "-" + QString::fromStdString(Git_version::shortSha1) +
                                     QString::fromStdString(Git_version::dirty ? "-dirty" : ""));
    parser.addHelpOption();

    QCommandLineOption port_option(QStringList() << "p"
                                                 << "port",
                                   QCoreApplication::translate("main", "Port for the unisim server [default: 9002]."),
                                   QCoreApplication::translate("main", "port"), QLatin1String("9002"));
    QCommandLineOption can_port0_option(QStringList() << "c0", QCoreApplication::translate("main", "CAN device 0."),
                                        QCoreApplication::translate("main", "can device 0."));
    QCommandLineOption can_port1_option(QStringList() << "c1", QCoreApplication::translate("main", "CAN device 1."),
                                        QCoreApplication::translate("main", "can device 1."));
    QCommandLineOption can_port2_option(QStringList() << "c2", QCoreApplication::translate("main", "CAN device 2."),
                                        QCoreApplication::translate("main", "can device 2."));
    QCommandLineOption uart_port0_option(QStringList() << "u0", QCoreApplication::translate("main", "uart device 0."),
                                         QCoreApplication::translate("main", "uart device 0."));
    QCommandLineOption uart_port1_option(QStringList() << "u1", QCoreApplication::translate("main", "uart device 1."),
                                         QCoreApplication::translate("main", "uart device 1."));
    QCommandLineOption uart_port2_option(QStringList() << "u2", QCoreApplication::translate("main", "uart device 2."),
                                         QCoreApplication::translate("main", "uart device 2."));
    QCommandLineOption uart_port3_option(QStringList() << "u3", QCoreApplication::translate("main", "uart device 3."),
                                         QCoreApplication::translate("main", "uart device 3."));
    parser.addOption(port_option);
    parser.addOption(can_port0_option);
    parser.addOption(can_port1_option);
    parser.addOption(can_port2_option);
    parser.addOption(uart_port0_option);
    parser.addOption(uart_port1_option);
    parser.addOption(uart_port2_option);
    parser.addOption(uart_port3_option);
    parser.process(a);
    uint16_t port = parser.value(port_option).toUShort();
    QStringList can_ports;
    QStringList uart_ports;
    can_ports << parser.value(can_port0_option) << parser.value(can_port1_option) << parser.value(can_port2_option);
    uart_ports << parser.value(uart_port0_option) << parser.value(uart_port1_option) << parser.value(uart_port2_option)
               << parser.value(uart_port3_option);

    // Initialize the logger
    init_logger();

    // The server starts in this thread
    SPDLOG_INFO("Starting server");
    Simulators simulators(port, can_ports, uart_ports);

    return a.exec();
}
