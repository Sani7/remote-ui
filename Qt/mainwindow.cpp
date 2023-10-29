#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "sim-scope_mux_tester.h"
#include "sim-can_debugger.h"
#include "sim-cvs_i10.h"

MainWindow::MainWindow(QWidget *parent) :
      QMainWindow(parent),
      ui(new Ui::MainWindow),
      api(new OpenAPI::OAICANApiApi()),
      error_dialog(new NetworkError(this)),
      sims{{"CANDebugger", new Can_Debugger(api, this)},
           {"Scope_Mux_Tester", new Scope_Mux_Tester(api, this)},
           {"CVS_I10", new CVS_I10(api, this)}}
{
    ui->setupUi(this);
    this->setWindowTitle("Simulator Selector");
    connect(this, &MainWindow::quit, this, &QCoreApplication::quit, Qt::QueuedConnection);

    api->setNewServerForAllOperations(
        //QUrl("http://localhost:1880"),
        QUrl("https://cvs-i10.local/api"),
        "No description provided",
        QMap<QString, OpenAPI::OAIServerVariable>());
    api->apiCanapiSimsGet();

    //Default sim
    defaultSim("CVS_I10");

    connect(ui->pushButton, &QPushButton::clicked, this, [=]{open_sim(ui->comboBox->currentText());});
}

MainWindow::~MainWindow()
{
    delete ui;
    delete api;
    delete error_dialog;
}

void MainWindow::setup_cb(void)
{
    connect(api, &OpenAPI::OAICANApiApi::apiCanapiSimsGetSignal, this, [=](QList<QString> summary){Parse_Sims(summary);});
    connect(api, &OpenAPI::OAICANApiApi::apiCanapiSimsGetSignalError, this, [=](QList<QString> summary, QNetworkReply::NetworkError error_type, const QString &error_str){Parse_Sims(summary, error_type, error_str);});
    connect(api, &OpenAPI::OAICANApiApi::apiCanapiCurrentsimPutSignal, this, [this](bool output){apiCanapiUielements_bool_cb(output);});
    connect(api, &OpenAPI::OAICANApiApi::apiCanapiCurrentsimPutSignalError, this, [this](bool output, QNetworkReply::NetworkError error_type, QString error_str){
        apiCanapiUielements_bool_cb(false, error_type, error_str);});
    connect(api, &OpenAPI::OAICANApiApi::apiCanapiCurrentsimGetSignal, this, [this](OpenAPI::OAISimulatorInfo summary){apiCanapiCurrentsimGet_cb(summary);});
    connect(api, &OpenAPI::OAICANApiApi::apiCanapiCurrentsimGetSignalError, this, [this](OpenAPI::OAISimulatorInfo summary, QNetworkReply::NetworkError error_type, QString error_str){apiCanapiCurrentsimGet_cb(OpenAPI::OAISimulatorInfo(), error_type, error_str);});
}

void MainWindow::showEvent( QShowEvent* event )
{
    QWidget::showEvent( event );
    setup_cb();
}

void MainWindow::hideEvent(QHideEvent* event)
{
    QWidget::hideEvent(event);
    disconnect(api, nullptr, nullptr, nullptr);
}

void MainWindow::defaultSim(QString name)
{
    this->selected_sim_name = name;
    this->default_sim = true;
}

void MainWindow::Parse_Sims(QList<QString> sims, QNetworkReply::NetworkError error_type, const QString error_str)
{
    if (error_type) {
        QD << "( " << error_type << "," << error_str << ")";
        error_dialog->set_error(error_str + "\n" + network_reply_to_fix(error_type));
        error_dialog->open();
        return;
    } if (sims.empty()) {
        QD << " got called with invalid data and no error!";
        error_dialog->set_error(QString(__FUNCTION__) + " got called with invalid data and no error!");
        error_dialog->open();
        return;
    }

    for (size_t i = 0; i < sims.length(); i++)
    {
        ui->comboBox->addItem(sims[i]);
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

    api->apiCanapiCurrentsimGet();
}

void MainWindow::open_sim_window(void)
{
    this->hide();
    selected_sim->show();
}

void MainWindow::apiCanapiUielements_bool_cb(bool output, QNetworkReply::NetworkError error_type, QString error_str)
{
    if (error_type) {
        QD << "( " << Qt::hex << output << "," << error_type << "," << error_str << ")";
        error_dialog->set_error(error_str + "\n" + network_reply_to_fix(error_type));
        error_dialog->open();
        return;
    } if (!output) {
        QD << " got called with invalid data and no error!";
        error_dialog->set_error(QString(__FUNCTION__) + " got called with invalid data and no error!");
        error_dialog->open();
        return;
    }

    open_sim_window();
}

void MainWindow::apiCanapiCurrentsimGet_cb(OpenAPI::OAISimulatorInfo summary, QNetworkReply::NetworkError error_type, QString error_str)
{
    if (error_type) {
        QD << "( " << summary.asJson() << "," << error_type << "," << error_str << ")";
        error_dialog->set_error(error_str + "\n" + network_reply_to_fix(error_type));
        error_dialog->open();
        return;
    } if (!summary.isValid()) {
        QD << " got called with invalid data and no error!";
        error_dialog->set_error(QString(__FUNCTION__) + " got called with invalid data and no error!");
        error_dialog->open();
        return;
    }

    if (!summary.is_name_Set())
    {
        QD << "Name not set";
        api->apiCanapiCurrentsimPut(selected_sim_name);
        return;
    }

    QD << "Current sim:" << summary.getName();

    if (summary.getName() == selected_sim_name)
    {
        QD << "Name set and right";
        open_sim_window();
        return;
    }

    QD << "Name set and wrong";
    api->apiCanapiCurrentsimPut(selected_sim_name);
}
