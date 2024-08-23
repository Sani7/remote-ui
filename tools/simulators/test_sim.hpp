#pragma once
#include "simulator_base.hpp"

class Test_Sim : public Simulator_base {
    public:
        Test_Sim();
        void timer() override;
        void run_at_startup() override;
    private:
        UI_button p_button;
        UI_combobox p_combobox;
        UI_label p_label;
        UI_checkbox p_checkbox;
        UI_slider p_slider;
};
