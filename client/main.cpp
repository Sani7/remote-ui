#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCommandLineParser parser;
    parser.setApplicationDescription("sim-us client");
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

    QString host = parser.value(hostOption);
    uint16_t port = parser.value(portOption).toUShort();
    MainWindow w(host, port);
    w.show();
    return a.exec();
}
