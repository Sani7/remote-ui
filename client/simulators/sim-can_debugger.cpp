#include "sim-can_debugger.h"
#include "ui_sim-can_debugger.h"

Can_Debugger::Can_Debugger(Web_socket_wrapper* web_socket, QWidget *parent) :
      SimulatorBase(web_socket, parent),
      ui(new Ui::Can_Debugger) {
    ui->setupUi(this);
    this->setWindowTitle("CAN Debugger");

    this->push_ui_item(ui->can_transceive);
}

Can_Debugger::~Can_Debugger() {
    delete ui;
}
