#ifndef SIM_CAN_DEBUGGER_H
#define SIM_CAN_DEBUGGER_H

#include "simulator_base.h"
#include <QMainWindow>

namespace Ui
{
class Can_Debugger;
}

class Can_Debugger : public SimulatorBase
{
    Q_OBJECT

  public:
    explicit Can_Debugger(Web_socket_wrapper *web_socket, QWidget *parent = nullptr);
    ~Can_Debugger();

  private:
    Ui::Can_Debugger *ui;
};

#endif // SIM_CAN_DEBUGGER_H
