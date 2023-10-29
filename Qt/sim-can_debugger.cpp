#include "sim-can_debugger.h"
#include "ui_sim-can_debugger.h"

Can_Debugger::Can_Debugger(OpenAPI::OAICANApiApi* api, QWidget *parent) :
      SimulatorBase(api, parent),
      ui(new Ui::Can_Debugger) {
    ui->setupUi(this);
    this->setWindowTitle("CAN Debugger");
}

Can_Debugger::~Can_Debugger() {
    delete ui;
}
