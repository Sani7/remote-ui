#include "test_sim.hpp"
#include <iostream>

Test_Sim::Test_Sim() :
Simulator_base("Test_Sim"),
p_button("button", "Button", Color(Color::White), Color(Color::Black), [](std::string id) { std::cout << "Button " << id << " clicked" << std::endl; }),
p_combobox("combobox", "Combobox", Color(Color::White), Color(Color::Black), {"Option 1", "Option 2", "Option 3"}, 0, [](std::string id, std::string selected) { std::cout << "Combobox " << id << " selected " << selected << std::endl; }),
p_label("label", "Label", Color(Color::White), Color(Color::Black)),
p_checkbox("checkbox", "Checkbox", Color(Color::White), Color(Color::Black), {"Option 1", "Option 2", "Option 3"}, [](std::string id, std::vector<std::string> selected) { std::cout << "Checkbox " << id << " selected " << (selected.size() > 0 ? selected[0] : "") << std::endl; }),
p_slider("slider", "Slider", Color(Color::White), Color(Color::Black), 0, 100, 50, [](std::string id, double value) { std::cout << "Slider " << id << " changed to " << value << std::endl; })
{
    this->add_UI_item(&this->p_button);
    this->add_UI_item(&this->p_combobox);
    this->add_UI_item(&this->p_label);
    this->add_UI_item(&this->p_checkbox);
    this->add_UI_item(&this->p_slider);
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
