#include "UI_checkbox.hpp"

UI_checkbox::UI_checkbox()
    : UI_x_boxes(UI_CHECKBOX_TYPE), p_selected(), p_on_change(nullptr)
{
}

UI_checkbox::UI_checkbox(std::string id, std::string text, Color fg_color, Color bg_color, std::vector<std::string> options, std::function<void(std::string, std::vector<std::string>)> on_change)
    : UI_x_boxes(id, UI_CHECKBOX_TYPE, text, fg_color, bg_color, options), p_selected(), p_on_change(on_change)
{
}

UI_checkbox::UI_checkbox(const json& j) : 
UI_x_boxes(UI_CHECKBOX_TYPE), p_selected(), p_on_change(nullptr)
{
    from_json(j);
}

void UI_checkbox::toggle_selected(size_t selected)
{
    if (selected >= p_options.size())
        return;
    
    if (p_selected[selected] != p_selected[selected])
    {
        p_selected[selected] = !p_selected[selected];
        if (p_on_change != nullptr)
        {
            p_on_change(p_id, selected_text());
        }
    }
}

void UI_checkbox::toggle_selected(std::string selected)
{
    for (size_t i = 0; i < p_options.size(); i++)
    {
        if (p_options[i] == selected)
        {
            toggle_selected(i);
            return;
        }
    }
}

std::vector<bool> UI_checkbox::selected() const
{
    return p_selected;
}

std::vector<std::string> UI_checkbox::selected_text() const
{
    std::vector<std::string> selected_text;
    for (size_t i = 0; i < p_options.size(); i++)
    {
        if (p_selected[i])
        {
            selected_text.push_back(p_options[i]);
        }
    }
    return selected_text;
}

void UI_checkbox::from_json(const json& j)
{
    UI_x_boxes::from_json(j);
    
    if (j.contains("selected"))
    {
        p_selected.clear();
        for (auto selected : j["selected"])
        {
            p_selected.push_back(selected);
        }
    }
}

json UI_checkbox::to_json() const
{
    json j = UI_x_boxes::to_json();
    j["selected"] = p_selected;
    return j;
}
