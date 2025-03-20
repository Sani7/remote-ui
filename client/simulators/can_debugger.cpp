#include "can_debugger.hpp"
#include "ui_can_debugger.h"

Can_Debugger::Can_Debugger(Web_socket_wrapper *web_socket, QWidget *parent)
    : Simulator_base("Can Debugger", web_socket, parent), ui(new Ui::Can_Debugger)
{
    ui->setupUi(this);
    this->setWindowTitle("CAN Debugger");

    this->push_ui_item(ui->can_transceive);
}

Can_Debugger::~Can_Debugger()
{
    delete ui;
}
