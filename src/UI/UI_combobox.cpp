#include "UI_combobox.hpp"

UI_combobox::UI_combobox()
    : UI_x_boxes(UI_COMBOBOX_TYPE)
{
}

UI_combobox::UI_combobox(std::string id, std::string text, Color fg_color, Color bg_color, std::vector<std::string> options, size_t selected, UI_combobox_on_change on_change)
    : UI_x_boxes(id, UI_COMBOBOX_TYPE, text, fg_color, bg_color, options), p_selected(selected), p_on_change(on_change)
{
}

UI_combobox::UI_combobox(std::string id, std::string text, Color fg_color, Color bg_color, std::vector<std::string> options, UI_combobox_on_change on_change)
    : UI_x_boxes(id, UI_COMBOBOX_TYPE, text, fg_color, bg_color, options), p_selected(0), p_on_change(on_change)
{
}

UI_combobox::UI_combobox(const json& j)
: UI_x_boxes(UI_COMBOBOX_TYPE), p_on_change(nullptr)
{
    from_json(j);
}

void UI_combobox::set_selected(size_t selected)
{
    if (selected != p_selected)
    {
        p_selected = selected;
        if (p_on_change != nullptr)
        {
            p_on_change(p_id, selected);
        }
    }
}

void UI_combobox::set_selected(std::string selected)
{
    for (size_t i = 0; i < p_options.size(); i++)
    {
        if (p_options[i] == selected)
        {
            set_selected(i);
            return;
        }
    }
}

size_t UI_combobox::selected() const
{
    return p_selected;
}

std::string UI_combobox::selected_text() const
{
    return p_options[p_selected];
}

void UI_combobox::from_json(const json& j)
{
    UI_x_boxes::from_json(j);
}

json UI_combobox::to_json() const
{
    json j = UI_x_boxes::to_json();
    return j;
}