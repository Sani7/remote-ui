#include "test.hpp"
#include "ui_test.h"

Test_Sim::Test_Sim(Web_socket_wrapper *web_socket, QWidget *parent)
    : Simulator_base("Test Sim", web_socket, parent), ui(new Ui::Test_Sim)
{
    ui->setupUi(this);
    this->setWindowTitle("Test Sim");
    this->setWindowState(Qt::WindowMaximized);

    PUSH_UI_ITEM(button);
    PUSH_UI_ITEM(combobox);
    PUSH_UI_ITEM(label);
    PUSH_UI_ITEM(slider);
    PUSH_UI_ITEM(dial);
    PUSH_UI_ITEM(thermo);
    PUSH_UI_ITEM(led);
    PUSH_UI_ITEM(checkBox);
    PUSH_UI_ITEM(plot);
    PUSH_UI_ITEM(table);
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
