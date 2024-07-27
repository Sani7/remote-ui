#include "sim-can_debugger.h"
#include "ui_sim-can_debugger.h"

Can_Debugger::Can_Debugger(Web_socket_wrapper* web_socket, QWidget *parent) :
      SimulatorBase(web_socket, parent),
      ui(new Ui::Can_Debugger) {
    ui->setupUi(this);
    this->setWindowTitle("CAN Debugger");
    can_send_hexspinboxes = {
        ui->s_can_send_data0,
        ui->s_can_send_data1,
        ui->s_can_send_data2,
        ui->s_can_send_data3,
        ui->s_can_send_data4,
        ui->s_can_send_data5,
        ui->s_can_send_data6,
        ui->s_can_send_data7,
    };

    enable_can_send_data();
    connect(ui->c_can_send_dlc, &QComboBox::currentIndexChanged, this, [=]{enable_can_send_data();});
}

void Can_Debugger::enable_can_send_data()
{
    uint8_t selected = atoi(ui->c_can_send_dlc->currentText().toLocal8Bit());
    for (size_t i = 0; i < can_send_hexspinboxes.size(); i++)
    {
        can_send_hexspinboxes[i]->setEnabled(i < selected);
        if (i >= selected)
        {
            can_send_hexspinboxes[i]->setValue(0);
        }
    }
}

Can_Debugger::~Can_Debugger() {
    delete ui;
}
