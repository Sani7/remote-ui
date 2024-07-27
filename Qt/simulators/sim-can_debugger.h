#ifndef SIM_CAN_DEBUGGER_H
#define SIM_CAN_DEBUGGER_H

#include <QMainWindow>
#include "simulator_base.h"
#include "hex_spinbox.h"

namespace Ui {
class Can_Debugger;
}

class Can_Debugger : public SimulatorBase {
    Q_OBJECT

  public:
    explicit Can_Debugger(Web_socket_wrapper* web_socket, QWidget *parent = nullptr);
    ~Can_Debugger();
    void enable_can_send_data();

  private:
    Ui::Can_Debugger* ui;
    std::vector<HexSpinBox*> can_send_hexspinboxes;
};

#endif // SIM_CAN_DEBUGGER_H
