#include "can_debugger.hpp"
#include "factory.hpp"
#include "ui_can_debugger.h"

REGISTER_UI(Can_Debugger);

Can_Debugger::Can_Debugger(Web_socket_wrapper *web_socket, QWidget *parent)
    : UI_base(web_socket, parent), ui(new Ui::Can_Debugger)
{
    ui->setupUi(this);

    this->PUSH_UI_ITEM(can_transceive);
}

Can_Debugger::~Can_Debugger()
{
    delete ui;
}
