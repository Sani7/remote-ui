#include "test.hpp"
#include "ui_test.h"

EXPORT_UI(Test_Sim);

Test_Sim::Test_Sim(Web_socket_wrapper *web_socket, QWidget *parent)
    : Simulator_base("Test Sim", web_socket, parent), ui(new Ui::Test_Sim)
{
    ui->setupUi(this);

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
    PUSH_UI_ITEM(spinBox);
    PUSH_UI_ITEM(doubleSpinBox);
    PUSH_UI_ITEM(hexSpinBox);
    PUSH_UI_ITEM(tabWidget);
    PUSH_UI_ITEM(stackedWidget);
    PUSH_UI_ITEM(statusbar);
}

Test_Sim::~Test_Sim()
{
    delete ui;
}

QLabel *Test_Sim::id_to_label(QWidget *widget)
{
    if (widget == ui->slider)
        return ui->slider_label;
    if (widget == ui->thermo)
        return ui->thermo_label;
    if (widget == ui->dial)
        return ui->dial_label;

    return nullptr;
}
