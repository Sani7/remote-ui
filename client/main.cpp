#include "mainwindow.h"
#include "spdlog/async.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"
#include <QApplication>

void init_logger()
{
    spdlog::init_thread_pool(8192, 1);
    auto stdout_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    // auto rotating_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>("log-sim_us.txt", 1024 * 1024 * 10,
    // 3);
    std::vector<spdlog::sink_ptr> sinks{stdout_sink /*, rotating_sink*/};
    auto logger = std::make_shared<spdlog::async_logger>("unisim_cpp", sinks.begin(), sinks.end(), spdlog::thread_pool(),
                                                         spdlog::async_overflow_policy::block);
    spdlog::set_default_logger(logger);
    spdlog::set_pattern("[%H:%M:%S %z] [%^---%L---%$] %s:%# %!: %v");
    spdlog::flush_every(std::chrono::seconds(3));
    spdlog::set_level(spdlog::level::trace);
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCommandLineParser parser;
    parser.setApplicationDescription("unisim_cpp client");
    parser.addHelpOption();

    QCommandLineOption hostOption(QStringList() << "s"
                                                << "server",
                                  QCoreApplication::translate("main", "sim-us server host [default: localhost]."),
                                  QCoreApplication::translate("main", "host"), QLatin1String("localhost"));
    parser.addOption(hostOption);

    QCommandLineOption portOption(QStringList() << "p"
                                                << "port",
                                  QCoreApplication::translate("main", "Port for echoserver [default: 9002]."),
                                  QCoreApplication::translate("main", "port"), QLatin1String("9002"));
    parser.addOption(portOption);

    parser.process(a);
    // Initialize the logger
    init_logger();

    QUrl url;
    url.setScheme("ws");
    url.setHost(parser.value(hostOption));
    url.setPort(parser.value(portOption).toInt());
    MainWindow w(url);
    w.show();
    return a.exec();
}
