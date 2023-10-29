#ifndef SIM_CAN_DEBUGGER_H
#define SIM_CAN_DEBUGGER_H

#include <QMainWindow>
#include "simulatorbase.h"

namespace Ui {
class Can_Debugger;
}

class Can_Debugger : public SimulatorBase {
    Q_OBJECT

  public:
    explicit Can_Debugger(OpenAPI::OAICANApiApi* api, QWidget *parent = nullptr);
    ~Can_Debugger();

  private:
    Ui::Can_Debugger* ui;
};

#endif // SIM_CAN_DEBUGGER_H
