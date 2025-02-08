#include "test_sim.hpp"

Test_Sim::Test_Sim(QObject* parent) :
Simulator_base("Test_Sim", std::chrono::milliseconds(1000), parent),
m_button("Off", Color(Color::White), Color(Color::Red)),
m_combobox(Color(Color::White), Color(Color::Black), {"Option 1", "Option 2", "Option 3"}, 0),
m_label("Label", Color(Color::White), Color(Color::Black)),
m_checkbox("Option 1", Color(Color::White), Color(Color::Black)),
m_slider("Slider", Color(Color::White), Color(Color::Black), 0, 100, 0),
m_dial("Dial", Color(Color::White), Color(Color::Black), 0, 100, 0),
m_thermo("Thermo", "%", Color(Color::White), Color(Color::Black), 0, 100, 0),
m_led("Led", Color(Color::White), Color(Color::Black), Color(Color::Red))
{
    this->m_thermo.set_start_color(Color(Color::Red));
    this->m_thermo.set_end_color(Color(Color::Red));
    this->m_thermo.add_color_stop(0.10, Color(Color::Yellow));
    this->m_thermo.add_color_stop(0.50, Color(Color::Green));
    this->m_thermo.add_color_stop(0.60, Color(Color::Yellow));
    this->m_thermo.add_color_stop(0.90, Color(Color::Red));

    this->add_UI_item(&this->m_button);
    this->add_UI_item(&this->m_combobox);
    this->add_UI_item(&this->m_label);
    this->add_UI_item(&this->m_slider);
    this->add_UI_item(&this->m_dial);
    this->add_UI_item(&this->m_thermo);
    this->add_UI_item(&this->m_led);
    this->add_UI_item(&this->m_checkbox);

    QObject::connect(&m_button, &UI_button::on_click, this, [=, this] { button_clicked(&m_button); });
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
        bool state = this->m_button.bg_color() == Color(Color::Red) ? true : false;
        this->m_button.set_bg_color(state ? Color(Color::Green) : Color(Color::Red));
        this->m_button.set_text(state ? "On" : "Off");

        this->m_led.set_led_color(state ? Color(Color::Green) : Color(Color::Red));
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
