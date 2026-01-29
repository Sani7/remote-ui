#pragma once
#include "simulator_base.hpp"

class Can_Debugger : public Simulator_base
{
    Q_OBJECT
  public:
    explicit Can_Debugger(Communication *serial = nullptr, QObject *parent = nullptr);
    void timer() override;
    void run_at_startup() override;
    void run_at_stop() override;

  private:
    CAN_Interface *m_if;
    UI_can m_can_ui;
};
