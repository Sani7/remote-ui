#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "can_debugger.hpp"
#include "cvs_i10.hpp"
#include "scope_mux_tester.hpp"
#include "test.hpp"

MainWindow::MainWindow(QUrl ws_url, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), m_web_socket(new Web_socket_wrapper(ws_url)),
      m_error_dialog(new NetworkError(this))
{
    ui->setupUi(this);
    INSERT_SIMULATOR(Cable_Tester);
    INSERT_SIMULATOR(Can_Debugger);
    INSERT_SIMULATOR(Scope_Mux_Tester);
    INSERT_SIMULATOR(CVS_I10);
    INSERT_SIMULATOR(Test_Sim);

    m_error_dialog->set_error("Connection timed out\nCheck if the server is running");
    ui->connection->setText("Connected to " + ws_url.toString());
    // Default sim
    // defaultSim("CVS_I10");

    connect(ui->pushButton, &QPushButton::clicked, this, [=, this] { open_sim(ui->comboBox->currentText()); });
    connect(this, &MainWindow::quit, this, &QCoreApplication::quit, Qt::QueuedConnection);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setup_cb(void)
{
    connect(m_web_socket.get(), &Web_socket_wrapper::on_command_cb, this, [=, this](json &j) { on_cmd_cb(j); });
    connect(m_web_socket.get(), &Web_socket_wrapper::on_connected, this,
            [=, this] { m_web_socket->send_command(Web_socket_wrapper::Command::get_simulators); });
    connect(m_web_socket.get(), &Web_socket_wrapper::on_closed, this, [=, this] {
        m_error_dialog->open();
        ui->connection->setText("Could not connect to the server");
    });
}

void MainWindow::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    setup_cb();
}

void MainWindow::hideEvent(QHideEvent *event)
{
    QWidget::hideEvent(event);
    disconnect(m_web_socket.get(), nullptr, nullptr, nullptr);
}

void MainWindow::defaultSim(QString name)
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
