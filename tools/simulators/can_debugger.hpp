#pragma once
#include "simulator_base.hpp"

class Can_Debugger : public Simulator_base {
    public:
        Can_Debugger();
        void timer() override;
        void run_at_startup() override;
    private:

};
