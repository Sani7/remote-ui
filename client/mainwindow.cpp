#include "mainwindow.hpp"
#include "factory.hpp"
#include "git_version.h"
#include "ui_base.hpp"
#include "ui_mainwindow.h"
#include "messages_client.grpc.qpb.h"
#include <QDirIterator>
#include <QLibrary>
#include <QMessageBox>
#include <QThread>
#include <QTimer>
#include <magic_enum/magic_enum.hpp>
#include <spdlog/spdlog.h>
#include <QGrpcHttp2Channel>
#include <QGrpcChannelOptions>
#include <QSslConfiguration>
#include <QSslSocket>

typedef UI_base *(*Get_UI)(RemoteUiService::Client *, QWidget *);

MainWindow::MainWindow(QUrl server_url, QString sim, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), m_error(new QMessageBox()),
      m_remote_ui_client(new RemoteUiService::Client()), m_ping(new QTimer())
{
    ui->setupUi(this);
    ui->statusbar->showMessage(QString("Version: %1-%2%3")
                                   .arg(Git_version::branch)
                                   .arg(Git_version::shortSha1)
                                   .arg(Git_version::dirty ? "-dirty" : ""));
    m_ping->setInterval(std::chrono::milliseconds(1000));

    for (const QString &name : UIFactory::instance().keys())
    {
        if (auto *w = UIFactory::instance().create(name, m_remote_ui_client.get(), this))
        {
            w->set_name(name);
            m_sims.insert(std::make_pair(name, w));
            connect(w, &UI_base::log_signal, this,
                    [](const char *filename_in, int line_in, const char *funcname_in, int level, QString msg) {
                        if (spdlog::default_logger_raw()->should_log((spdlog::level::level_enum)level))
                            spdlog::default_logger_raw()->log(spdlog::source_loc{filename_in, line_in, funcname_in},
                                                              (spdlog::level::level_enum)level, msg.toStdString());
                    });
        }
    }

    setWindowTitle("Unisim cpp client");
    m_error->setIcon(QMessageBox::Critical);
    m_error->setWindowTitle("Critical");
    m_error->setText("Connection timed out\nCheck if the server is running");
    QPushButton *exit_app = m_error->addButton("Exit Application", QMessageBox::AcceptRole);
    m_error->addButton("Close", QMessageBox::RejectRole);
    connect(exit_app, &QPushButton::clicked, this, &QCoreApplication::quit, Qt::QueuedConnection);
    ui->connection->setText("Connected to " + server_url.toString());
    // Default sim
    if (!sim.isEmpty())
        default_sim(sim);

    QSslConfiguration ssl_config = QSslConfiguration::defaultConfiguration();

    // Required for HTTP/2
    ssl_config.setAllowedNextProtocols({ "h2" });

    // Disable certificate verification
    ssl_config.setPeerVerifyMode(QSslSocket::VerifyNone);

    QGrpcChannelOptions options;
    options.setSslConfiguration(ssl_config);

    server_url.setScheme("https");
    auto channel = std::make_shared<QGrpcHttp2Channel>(server_url, options);
    m_remote_ui_client->attachChannel(channel);

    connect(ui->pushButton, &QPushButton::clicked, this, [=, this] { open_sim(ui->comboBox->currentText()); });
    connect(m_ping, &QTimer::timeout, this, [this]{
// TODO: find a way to check if the channel is open
    });
    connect(this, &MainWindow::on_show_event, this, [this]{
        auto reply = m_remote_ui_client->Command_get_simulators(Empty());
        connect(reply.get(), &QGrpcCallReply::finished, this, [this, reply = reply.get()](const QGrpcStatus &status) {
            auto response = reply->read<Simulators_m>();
            if (!response.has_value() || !status.isOk())
            {
                SPDLOG_CRITICAL("Command get simulators has got no response");
                return;
            }
            pase_sim_names(response.value());
        }, Qt::SingleShotConnection);
    }, Qt::SingleShotConnection);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    emit on_show_event();
    m_ping->start();
}

void MainWindow::hideEvent(QHideEvent *event)
{
    QWidget::hideEvent(event);
    disconnect(m_remote_ui_client.get(), nullptr, nullptr, nullptr);
}

void MainWindow::default_sim(QString name)
{
    this->m_selected_sim_name = name;
    this->m_default_sim = true;
}

void MainWindow::pase_sim_names(Simulators_m &sims)
{
    for (auto &item : sims.simulator())
    {
        ui->comboBox->addItem(item);
    }

    QThread::msleep(30);

    if (this->m_default_sim)
    {
        ui->comboBox->setCurrentText(this->m_selected_sim_name);
        open_sim(this->m_selected_sim_name);
    }
}

void MainWindow::open_sim(QString sim_name)
{
    try
    {
        m_selected_sim = m_sims.at(sim_name).get();
        this->m_selected_sim_name = sim_name;
    }
    catch (const std::out_of_range &ex)
    {
        SPDLOG_CRITICAL("out_of_range: {}", ex.what());
        return;
    }

    auto cmd = Simulator_m();
    cmd.setSimuator(sim_name);
    auto reply = m_remote_ui_client->Command_switch_simulator(cmd);
    connect(reply.get(), &QGrpcCallReply::finished, this, [this, reply = reply.get()] {
        auto response = reply->read<Empty>();
        if (!response.has_value())
        {
            qDebug("Switch simulator deserialization failed");
            return;
        }
        open_sim_window();
    }, Qt::SingleShotConnection);
}

void MainWindow::open_sim_window(void)
{
    this->hide();
    m_selected_sim->show();
}

void MainWindow::check_active_sim(QString name)
{
    if (name == m_selected_sim_name)
    {
        SPDLOG_DEBUG("Name set and the same");
        open_sim_window();
        return;
    }
    
    auto cmd = Simulator_m();
    cmd.setSimuator(m_selected_sim_name);
    auto reply = m_remote_ui_client->Command_switch_simulator(cmd);
    connect(reply.get(), &QGrpcCallReply::finished, this, [this, reply = reply.get()] {
        auto response = reply->read<Empty>();
        if (!response.has_value())
        {
            qDebug("Switch simulator deserialization failed");
            return;
        }
        open_sim_window();
    }, Qt::SingleShotConnection);
}
