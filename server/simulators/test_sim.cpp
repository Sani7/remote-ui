#include "test_sim.hpp"

Test_Sim::Test_Sim(CAN_Wrapper *can_wrapper, QObject *parent)
    : Simulator_base("Test Sim", can_wrapper, std::chrono::milliseconds(1000), parent),
      m_button("Off", Color::White, Color::Red),
      m_combobox(Color::White, Color::Black, {"Option 1", "Option 2", "Option 3"}, 0),
      m_label("Label", Color::White, Color::Black), m_checkbox("Option 1", Color::Default, Color::Default),
      m_slider("Slider", Color::White, Color::Black, 0, 100, 0), m_dial("Dial", Color::White, Color::Black, 0, 100, 0),
      m_thermo("Thermo", "%", Color::White, Color::Black, 0, 100, 0), m_led("Led", Color::White, Color::Red)
{
    this->m_thermo.set_start_color(Color::Red);
    this->m_thermo.set_end_color(Color::Red);
    this->m_thermo.add_color_stop(0.10, Color::Yellow);
    this->m_thermo.add_color_stop(0.50, Color::Green);
    this->m_thermo.add_color_stop(0.60, Color::Yellow);
    this->m_thermo.add_color_stop(0.90, Color::Red);

    this->push_ui_item(&this->m_button);
    this->push_ui_item(&this->m_combobox);
    this->push_ui_item(&this->m_label);
    this->push_ui_item(&this->m_slider);
    this->push_ui_item(&this->m_dial);
    this->push_ui_item(&this->m_thermo);
    this->push_ui_item(&this->m_led);
    this->push_ui_item(&this->m_checkbox);

    QObject::connect(&m_button, &UI_button::clicked, this, [=, this] { button_clicked(&m_button); });
    QObject::connect(&m_slider, &UI_slider::on_change, this,
                     [=, this](double value) { slider_changed(&m_slider, value); });
}

void Test_Sim::timer()
{
    this->m_button.click();
    if (this->m_slider.value() == this->m_slider.max())
    {
        this->m_slider.set_value(0);
    }
    this->m_slider.set_value(this->m_slider.value() + 1);
}

void Test_Sim::run_at_startup()
{
    // Run at startup
    SPDLOG_INFO("Test_Sim started");
}

void Test_Sim::button_clicked(UI_button *id)
{
    if (id == &this->m_button)
    {
        bool state = this->m_button.bg_color() == Color::Red ? true : false;
        this->m_button.set_bg_color(state ? Color::Green : Color::Red);
        this->m_button.set_text(state ? "On" : "Off");

        this->m_led.set_bg_color(state ? Color::Green : Color::Red);
        this->m_checkbox.set_state(state);
        return;
    }
}

void Test_Sim::slider_changed(UI_slider *id, double value)
{
    if (id == &this->m_slider)
    {
        SPDLOG_INFO("Test slider changed to {}", value);
        this->m_label.set_text(std::to_string(value));
        this->m_dial.set_value(value);
        this->m_thermo.set_value(value);
        return;
    }
}
