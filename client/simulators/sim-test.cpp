#include "sim-test.h"
#include "ui_sim-test.h"

Test_Sim::Test_Sim(Web_socket_wrapper *web_socket, QWidget *parent)
    : Simulator_base(web_socket, parent), ui(new Ui::Test_Sim)
{
    ui->setupUi(this);
    this->setWindowTitle("Test Sim");
    this->setWindowState(Qt::WindowMaximized);

    push_ui_item(ui->button);
    push_ui_item(ui->combobox);
    push_ui_item(ui->label);
    push_ui_item(ui->slider);
    push_ui_item(ui->dial);
    push_ui_item(ui->thermo);
    push_ui_item(ui->led);
}

Test_Sim::~Test_Sim()
{
    delete ui;
}

QLabel *Test_Sim::id_to_label(size_t id)
{
    QWidget *widget = id_to_ui(id);
    if (widget == ui->slider)
        return ui->slider_label;
    if (widget == ui->thermo)
        return ui->thermo_label;
    if (widget == ui->dial)
        return ui->dial_label;

    return nullptr;
}
