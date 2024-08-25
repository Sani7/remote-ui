#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "sim-scope_mux_tester.h"
#include "sim-can_debugger.h"
#include "sim-cvs_i10.h"

MainWindow::MainWindow(QWidget *parent) :
      QMainWindow(parent),
      ui(new Ui::MainWindow),
      m_web_socket(new Web_socket_wrapper(QUrl("ws://localhost:9002"), this)),
      error_dialog(new NetworkError(this)),
      sims{{"Can_Debugger", new Can_Debugger(m_web_socket, this)},
           {"Scope_Mux_Tester", new Scope_Mux_Tester(m_web_socket, this)},
           {"CVS_I10", new CVS_I10(m_web_socket, this)}}
{
    ui->setupUi(this);
    this->setWindowTitle("Simulator Selector");

    //Default sim
    //defaultSim("CVS_I10");

    connect(ui->pushButton, &QPushButton::clicked, this, [=]{open_sim(ui->comboBox->currentText());});
    connect(this, &MainWindow::quit, this, &QCoreApplication::quit, Qt::QueuedConnection);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_web_socket;
    delete error_dialog;
}

void MainWindow::setup_cb(void)
{
    connect(m_web_socket, &Web_socket_wrapper::on_command_cb, this, [=](json& j){on_cmd_cb(j);});
    connect(m_web_socket, &Web_socket_wrapper::on_connected, this, [=]{m_web_socket->send_command(Web_socket_wrapper::Command::get_simulators);});
}

void MainWindow::showEvent( QShowEvent* event )
{
    QWidget::showEvent( event );
    setup_cb();
}

void MainWindow::hideEvent(QHideEvent* event)
{
    QWidget::hideEvent(event);
    disconnect(m_web_socket, nullptr, nullptr, nullptr);
}

void MainWindow::defaultSim(QString name)
{
    this->selected_sim_name = name;
    this->default_sim = true;
}

void MainWindow::pase_sim_names(json& sims)
{
    for (json& item : sims)
    {
        ui->comboBox->addItem(QString::fromStdString(item));
    }

    QThread::msleep(30);

    if (this->default_sim)
    {
        ui->comboBox->setCurrentText(this->selected_sim_name);
        open_sim(this->selected_sim_name);
    }
}

void MainWindow::open_sim(QString sim_name)
{
    try
    {
        selected_sim = sims.at(sim_name);
        this->selected_sim_name = sim_name;
    }
    catch (const std::out_of_range& ex)
    {
        QD << "out_of_range::what(): " << ex.what();
        return;
    }

    m_web_socket->send_command(Web_socket_wrapper::Command::get_active_simulator_name);
}

void MainWindow::open_sim_window(void)
{
    this->hide();
    selected_sim->show();
}

void MainWindow::check_active_sim(QString name)
{
        if (name == "")
        {
            QD << "Name not set";
            m_web_socket->send_command(Web_socket_wrapper::Command::switch_simulator, selected_sim_name);
            return;
        }

         QD << "Current sim:" << name;

        if (name == selected_sim_name)
        {
            QD << "Name set and right";
            open_sim_window();
            return;
        }

        QD << "Name set and wrong";
        m_web_socket->send_command(Web_socket_wrapper::Command::switch_simulator, selected_sim_name);
}

void MainWindow::on_cmd_cb(json& j)
{
    auto response = magic_enum::enum_cast<Web_socket_wrapper::Command>(std::string(j.at("type"))).value_or(Web_socket_wrapper::Command::end);
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
