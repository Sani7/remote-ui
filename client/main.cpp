#include "git_version.h"
#include "magic_enum/magic_enum.hpp"
#include "mainwindow.hpp"
#include "spdlog/async.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"
#include <QApplication>
#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QProcess>
#include <QUrl>

#ifdef EMSCRIPTEN
#include <emscripten.h>
#include <emscripten/val.h>
#endif

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
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCommandLineParser parser;
    parser.setApplicationDescription(QString("unisim_cpp client\n\tVersion: %1-%2%3")
                                         .arg(Git_version::branch)
                                         .arg(Git_version::shortSha1)
                                         .arg(Git_version::dirty ? "-dirty" : ""));
    parser.addHelpOption();

    QCommandLineOption debug_option(QStringList() << "debug",
                                   QCoreApplication::translate("main", "debug unisim_cpp server host [default: localhost:9002]."),
                                   QCoreApplication::translate("main", "host"), QLatin1String("localhost:9002"));
    parser.addOption(debug_option);

    QCommandLineOption host_option(QStringList() << "s"
                                                 << "server",
                                   QCoreApplication::translate("main", "unisim_cpp server host [default: localhost]."),
                                   QCoreApplication::translate("main", "host"), QLatin1String("localhost"));
    parser.addOption(host_option);

    QCommandLineOption default_sim_option(QStringList() << "d"
                                                        << "default",
                                          QCoreApplication::translate("main", "Default sim that loads on startup."),
                                          QCoreApplication::translate("main", "default"), QLatin1String(""));
    parser.addOption(default_sim_option);

    QCommandLineOption log_option(
        QStringList() << "l"
                      << "log",
        QCoreApplication::translate("main", "Log level for the unisim server [default: trace]."),
        QCoreApplication::translate("main", "log"), QLatin1String("trace"));
    parser.addOption(log_option);

    parser.process(a);
    // Initialize the logger
    init_logger();
    spdlog::set_level(magic_enum::enum_cast<spdlog::level::level_enum>(parser.value(log_option).toStdString())
                          .value_or(spdlog::level::off));

    QUrl url;
    if (parser.isSet(debug_option))
    {
        url = QUrl::fromUserInput(parser.value(debug_option));
        url.setScheme("ws");
    } else
    {
        url.setPath("/ws");
        url.setPort(443);
        url.setScheme("wss");
        url.setHost(parser.value(host_option));
    }
#ifdef EMSCRIPTEN
    emscripten::val location = emscripten::val::global("location");
    url.setHost(QString::fromStdString(location["hostname"].as<std::string>()));
#endif
    QString default_sim = parser.value(default_sim_option);
    SPDLOG_INFO("Connecting client on {}", url.toString().toStdString());
    MainWindow w(url, default_sim);
    w.show();
    return a.exec();
}
