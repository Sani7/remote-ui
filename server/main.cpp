#include <QtCore/QCommandLineOption>
#include <QtCore/QCommandLineParser>
#include <QtCore/QCoreApplication>

#include "spdlog/async.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"

#include "simulators.hpp"
#include "websocket.hpp"

void init_logger()
{
    spdlog::init_thread_pool(8192, 1);
    auto stdout_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    //auto rotating_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>("log-sim_us.txt", 1024 * 1024 * 10, 3);
    std::vector<spdlog::sink_ptr> sinks{stdout_sink/*, rotating_sink*/};
    auto logger = std::make_shared<spdlog::async_logger>("sim_us", sinks.begin(), sinks.end(), spdlog::thread_pool(),
                                                         spdlog::async_overflow_policy::block);
    spdlog::set_default_logger(logger);
    spdlog::set_pattern("[%H:%M:%S %z] [%^---%L---%$] %@: %v");
    spdlog::flush_every(std::chrono::seconds(3));
    spdlog::set_level(spdlog::level::trace);
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QCommandLineParser parser;
    parser.setApplicationDescription("QtWebSockets example: echoserver");
    parser.addHelpOption();

    QCommandLineOption port_option(QStringList() << "p"
                                                << "port",
                                  QCoreApplication::translate("main", "Port for echoserver [default: 9002]."),
                                  QCoreApplication::translate("main", "port"), QLatin1String("9002"));
    QCommandLineOption can_port_option(QStringList() << "c"
                                                << "can",
                                  QCoreApplication::translate("main", "CAN device [default: can0]."),
                                  QCoreApplication::translate("main", "can device"), QLatin1String("can0"));
    parser.addOption(port_option);
    parser.addOption(can_port_option);
    parser.process(a);
    uint16_t port = parser.value(port_option).toUShort();
    QString can_port = parser.value(can_port_option);

    // Initialize the logger
    init_logger();

    // The server starts in this thread
    SPDLOG_INFO("Starting server");
    Simulators simulators(port, can_port);

    return a.exec();
}
