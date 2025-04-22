#include "test_sim.hpp"

EXPORT_SIM(Test_Sim);

Test_Sim::Test_Sim(Communication *com, QObject *parent)
    : Simulator_base("Test Sim", com, std::chrono::milliseconds(1000), parent),
      m_button("Off", Color::White, Color::Red),
      m_combobox(Color::White, Color::Black, {"Option 1", "Option 2", "Option 3"}, 0),
      m_label("Label", Color::White, Color::Black), m_checkbox("Option 1", Color::Default, Color::Default),
      m_slider("Slider", Color::White, Color::Black, 0, 100, 0), m_dial("Dial", Color::White, Color::Black, 0, 100, 0),
      m_thermo("Thermo", "%", Color::White, Color::Black, 0, 100, 0), m_led("Led", Color::White, Color::Red),
      m_table(4, 4, {"Row 1", "Row 2", "Row 3", "Row 4"}, {"Column 1", "Column 2", "Column 3", "Column 4"}),
      m_spinbox(0, 255, 0), m_double_spinbox(0, 255, 0), m_hex_spinbox(0, 255, 0)
{
    m_thermo.set_start_color(Color::Red);
    m_thermo.set_end_color(Color::Red);
    m_thermo.add_color_stop(0.10, Color::Yellow);
    m_thermo.add_color_stop(0.50, Color::Green);
    m_thermo.add_color_stop(0.60, Color::Yellow);
    m_thermo.add_color_stop(0.90, Color::Red);

    PUSH_UI_ITEM(m_button);
    PUSH_UI_ITEM(m_combobox);
    PUSH_UI_ITEM(m_label);
    PUSH_UI_ITEM(m_slider);
    PUSH_UI_ITEM(m_dial);
    PUSH_UI_ITEM(m_thermo);
    PUSH_UI_ITEM(m_led);
    PUSH_UI_ITEM(m_checkbox);
    PUSH_UI_ITEM(m_plot);
    PUSH_UI_ITEM(m_table);
    PUSH_UI_ITEM(m_spinbox);
    PUSH_UI_ITEM(m_double_spinbox);
    PUSH_UI_ITEM(m_hex_spinbox);

    for (size_t i = 0; i < m_table.capacity(); i++)
    {
        m_table.insert_item(i / 4, i % 4, QString("%0 %1").arg(i / 4).arg(i % 4).toStdString());
    }

    QObject::connect(&m_button, &UI_button::clicked, this, [=, this] { button_clicked(&m_button); });
    QObject::connect(&m_slider, &UI_slider::changed, this,
                     [=, this](double value) { slider_changed(&m_slider, value); });
}

void Test_Sim::timer()
{
    m_button.click();
    if (m_slider.value() == m_slider.max())
    {
        m_slider.set_value(0);
    }
    m_slider.set_value(m_slider.value() + 1);
}

void Test_Sim::run_at_startup()
{
    // Run at startup
    SPDLOG_INFO("Test_Sim started");
}

void Test_Sim::button_clicked(UI_button *id)
{
    if (id == &m_button)
    {
        bool state = m_button.bg_color() == Color::Red ? true : false;
        m_button.set_bg_color(state ? Color::Green : Color::Red);
        m_button.set_text(state ? "On" : "Off");

        m_led.set_bg_color(state ? Color::Green : Color::Red);
        m_checkbox.set_state(state);
        return;
    }
}

void Test_Sim::slider_changed(UI_slider *id, double value)
{
    if (id == &m_slider)
    {
        SPDLOG_INFO("Test slider changed to {}", value);
        m_label.set_text(std::to_string(value));
        m_dial.set_value(value);
        m_thermo.set_value(value);
        m_plot.add_value(value);
        m_spinbox.set_value(value);
        m_double_spinbox.set_value(value);
        m_hex_spinbox.set_value(value);
        return;
    }
}
