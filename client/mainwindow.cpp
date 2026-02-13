#include "mainwindow.hpp"
#include "ui_base.hpp"
#include "ui_mainwindow.h"
#include "web_socket_wrapper.hpp"
#include <QDirIterator>
#include <QLibrary>
#include <QMessageBox>
#include <QThread>
#include <magic_enum/magic_enum.hpp>
#include <spdlog/spdlog.h>

typedef UI_base *(*Get_UI)(Web_socket_wrapper *, QWidget *);

MainWindow::MainWindow(QUrl ws_url, QString sim, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), m_error(new QMessageBox()),
      m_web_socket(new Web_socket_wrapper(ws_url))
{
    ui->setupUi(this);
    QLibrary lib;
    QDirIterator it(QCoreApplication::applicationDirPath(), QStringList() << "*.so", QDir::Files,
                    QDirIterator::Subdirectories);
    while (it.hasNext())
    {
        lib.setFileName(it.next());
        lib.load();
        auto loaded_ui = Get_UI(lib.resolve("get_ui"));
        if (loaded_ui == nullptr)
        {
            SPDLOG_INFO("{} NOT FOUND", lib.fileName().toStdString());
            lib.unload();
            continue;
        }
        auto widget = loaded_ui(m_web_socket.get(), this);
        m_sims.insert(std::make_pair(widget->name(), widget));
        connect(widget, &UI_base::log_signal, this,
                [](const char *filename_in, int line_in, const char *funcname_in, int level, QString msg) {
                    spdlog::default_logger_raw()->log(spdlog::source_loc{filename_in, line_in, funcname_in},
                                                      (spdlog::level::level_enum)level, msg.toStdString());
                });
        lib.unload();
    }

    // Insert debug sims here
    // INSERT_SIMULATOR(SIM_NAME);

    setWindowTitle("Unisim cpp client");
    m_error->setIcon(QMessageBox::Critical);
    m_error->setWindowTitle("Critical");
    m_error->setText("Connection timed out\nCheck if the server is running");
    QPushButton *exit_app = m_error->addButton("Exit Application", QMessageBox::AcceptRole);
    m_error->addButton("Close", QMessageBox::RejectRole);
    connect(exit_app, &QPushButton::clicked, this, &QCoreApplication::quit, Qt::QueuedConnection);
    ui->connection->setText("Connected to " + ws_url.toString());
    // Default sim
    if (!sim.isEmpty())
        default_sim(sim);

    connect(ui->pushButton, &QPushButton::clicked, this, [=, this] { open_sim(ui->comboBox->currentText()); });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    connect(m_web_socket.get(), &Web_socket_wrapper::on_command_cb, this, [=, this](json &j) { on_cmd_cb(j); });
    connect(m_web_socket.get(), &Web_socket_wrapper::on_connected, this,
            [=, this] { m_web_socket->send_command(Web_socket_wrapper::Command::get_simulators); });
    connect(m_web_socket.get(), &Web_socket_wrapper::on_closed, this, [=, this] {
        m_error->open();
        ui->connection->setText("Could not connect to the server");
    });
}

void MainWindow::hideEvent(QHideEvent *event)
{
    QWidget::hideEvent(event);
    disconnect(m_web_socket.get(), nullptr, nullptr, nullptr);
}

void MainWindow::default_sim(QString name)
{
    this->m_selected_sim_name = name;
    this->m_default_sim = true;
}

void MainWindow::pase_sim_names(json &sims)
{
    for (json &item : sims)
    {
        ui->comboBox->addItem(QString::fromStdString(item));
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

    m_web_socket->send_command(Web_socket_wrapper::Command::get_active_simulator_name);
}

void MainWindow::open_sim_window(void)
{
    this->hide();
    m_selected_sim->show();
}

void MainWindow::check_active_sim(QString name)
{
    if (name == "")
    {
        SPDLOG_DEBUG("Name not set");
        m_web_socket->send_command(Web_socket_wrapper::Command::switch_simulator, m_selected_sim_name);
        return;
    }

    SPDLOG_DEBUG("Current sim:", name);

    if (name == m_selected_sim_name)
    {
        SPDLOG_DEBUG("Name set and the same");
        open_sim_window();
        return;
    }

    SPDLOG_DEBUG("Name set and wrong");
    m_web_socket->send_command(Web_socket_wrapper::Command::switch_simulator, m_selected_sim_name);
}

void MainWindow::on_cmd_cb(json &j)
{
    auto response = magic_enum::enum_cast<Web_socket_wrapper::Command>(std::string(j.at("type")))
                        .value_or(Web_socket_wrapper::Command::end);
    switch (response)
    {
    case Web_socket_wrapper::Command::get_simulators:
        pase_sim_names(j.at("simulators"));
        break;
    case Web_socket_wrapper::Command::get_active_simulator_name:
        check_active_sim(QString::fromStdString(j.at("name")));
        break;
    case Web_socket_wrapper::Command::switch_simulator:
        open_sim_window();
        break;
    default:
        break;
    }
}
