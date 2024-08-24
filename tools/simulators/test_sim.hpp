#pragma once
#include "simulator_base.hpp"

class Test_Sim : public Simulator_base {
    public:
        Test_Sim();
        void timer() override;
        void run_at_startup() override;
    private:
        void button_clicked(std::string id);
    private:
        UI_button m_button;
        UI_combobox m_combobox;
        UI_label m_label;
        UI_checkbox m_checkbox;
        UI_slider m_slider;
        UI_led m_led;
};
