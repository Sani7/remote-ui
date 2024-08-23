#include "test_sim.hpp"
#include <iostream>

Test_Sim::Test_Sim() :
Simulator_base("Test_Sim"),
m_button("button", "Button", Color(Color::White), Color(Color::Black), [](std::string id) { std::cout << "Button " << id << " clicked" << std::endl; }),
m_combobox("combobox", "Combobox", Color(Color::White), Color(Color::Black), {"Option 1", "Option 2", "Option 3"}, 0, [](std::string id, std::string selected) { std::cout << "Combobox " << id << " selected " << selected << std::endl; }),
m_label("label", "Label", Color(Color::White), Color(Color::Black)),
m_checkbox("checkbox", "Checkbox", Color(Color::White), Color(Color::Black), {"Option 1", "Option 2", "Option 3"}, [](std::string id, std::vector<std::string> selected) { std::cout << "Checkbox " << id << " selected " << (selected.size() > 0 ? selected[0] : "") << std::endl; }),
m_slider("slider", "Slider", Color(Color::White), Color(Color::Black), 0, 100, 50, [](std::string id, double value) { std::cout << "Slider " << id << " changed to " << value << std::endl; })
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
    std::cout << "Test_Sim started" << std::endl;
}
