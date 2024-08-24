#include "test_sim.hpp"
#include <iostream>

Test_Sim::Test_Sim() :
Simulator_base("Test_Sim"),
m_button("button", "Off", Color(Color::White), Color(Color::Red), [this](std::string id) { button_clicked(id); }),
m_combobox("combobox", "Combobox", Color(Color::White), Color(Color::Black), {"Option 1", "Option 2", "Option 3"}, 0, [](std::string id, std::string selected) { spdlog::info("Combobox {} selected {}", id, selected); }),
m_label("label", "Label", Color(Color::White), Color(Color::Black)),
m_checkbox("checkbox", "Checkbox", Color(Color::White), Color(Color::Black), {"Option 1", "Option 2", "Option 3"}, [](std::string id, std::vector<std::string> selected) { spdlog::info("Checkbox {} selected {}", id, (selected.size() > 0 ? selected[0] : "")); }),
m_slider("slider", "Slider", Color(Color::White), Color(Color::Black), 0, 100, 50, [](std::string id, double value) { spdlog::info("Slider {} changed to {}", id, value); })
{
    this->add_UI_item(&this->m_button);
    this->add_UI_item(&this->m_combobox);
    this->add_UI_item(&this->m_label);
    this->add_UI_item(&this->m_checkbox);
    this->add_UI_item(&this->m_slider);
}

void Test_Sim::timer()
{
    // Timer function
}

void Test_Sim::run_at_startup()
{
    // Run at startup
    spdlog::info("Test_Sim started");
}

void Test_Sim::button_clicked(std::string id)
{
    if (id == "button")
    {
        bool state = this->m_button.bg_color() == Color(Color::Red) ? true : false;
        this->m_button.set_bg_color(state ? Color(Color::Green) : Color(Color::Red));
        this->m_button.set_text(state ? "On" : "Off");
        return;
    }
}
