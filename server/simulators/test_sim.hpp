#pragma once
#include "simulator_base.hpp"

class Test_Sim : public Simulator_base
{
    Q_OBJECT
  public:
    explicit Test_Sim(Communication* comms = nullptr, QObject *parent = nullptr);
    void timer() override;
    void run_at_startup() override;

  private:
    void button_clicked(UI_button *id);
    void slider_changed(UI_slider *id, double value);

  private:
    UI_button m_button;
    UI_combobox m_combobox;
    UI_label m_label;
    UI_checkbox m_checkbox;
    UI_slider m_slider;
    UI_dial m_dial;
    UI_thermo m_thermo;
    UI_led m_led;
    UI_plot m_plot;
    UI_table m_table;
    UI_spinbox m_spinbox;
    UI_double_spinbox m_double_spinbox;
    UI_hex_spinbox m_hex_spinbox;
};
