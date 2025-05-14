#include "mainwindow.hpp"
#include "spdlog/async.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"
#include <QApplication>
#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QProcess>

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
    spdlog::set_level(spdlog::level::trace);
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCommandLineParser parser;
    parser.setApplicationDescription("unisim_cpp client");
    parser.addHelpOption();

    QCommandLineOption host_option(QStringList() << "s"
                                                 << "server",
                                   QCoreApplication::translate("main", "unisim_cpp server host [default: localhost]."),
                                   QCoreApplication::translate("main", "host"), QLatin1String("localhost"));
    parser.addOption(host_option);

    QCommandLineOption port_option(QStringList() << "p"
                                                 << "port",
                                   QCoreApplication::translate("main", "Port for unisim_cpp [default: 9002]."),
                                   QCoreApplication::translate("main", "port"), QLatin1String("9002"));
    parser.addOption(port_option);

    QCommandLineOption default_sim_option(QStringList() << "d"
                                                        << "default",
                                          QCoreApplication::translate("main", "Default sim that loads on startup."),
                                          QCoreApplication::translate("main", "default"), QLatin1String(""));
    parser.addOption(default_sim_option);

    parser.process(a);
    // Initialize the logger
    init_logger();

    QUrl url;
    url.setScheme("ws");
#ifdef EMSCRIPTEN
    emscripten::val location = emscripten::val::global("location");
    url.setHost(QString::fromStdString(location["href"].as<std::string>()));
#else
    url.setHost(parser.value(host_option));
    if (parser.value(host_option).endsWith(".local"))
    {
        QProcess p;
        p.start("avahi-resolve", {"-4", "--name", parser.value(host_option)});
        p.waitForFinished(-1);
        QString stdout = p.readAllStandardOutput();
        QString stderr = p.readAllStandardError();
        if (!stderr.isEmpty())
        {
            SPDLOG_CRITICAL("Unable to resolve {}", parser.value(host_option).toStdString());
            return -1;
        }

        auto ret = stdout.split(u'\t', Qt::SkipEmptyParts);
        if (ret.count() < 2)
        {
            SPDLOG_CRITICAL("Unable to resolve {}: not enough paramteters", parser.value(host_option).toStdString());
        }

        ret[1].removeLast();
        url.setHost(ret[1]);
    }
#endif
    url.setPort(parser.value(port_option).toInt());
    QString default_sim = parser.value(default_sim_option);
    MainWindow w(url, default_sim);
    w.show();
    return a.exec();
}
