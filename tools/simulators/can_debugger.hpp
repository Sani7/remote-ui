#pragma once
#include "simulator_base.hpp"

class Can_Debugger : public Simulator_base {
    Q_OBJECT
    public:
        Can_Debugger();
        void timer() override;
        void run_at_startup() override;
        void run_at_stop() override;
    private:
        CAN_Wrapper* m_wrapper;
        UI_can m_can_ui;
};
