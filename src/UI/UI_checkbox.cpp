#include "UI_checkbox.hpp"

UI_checkbox::UI_checkbox() : UI_x_boxes(UI_CHECKBOX_TYPE), m_selected(), m_on_change(nullptr)
{
}

UI_checkbox::UI_checkbox(std::string id, std::string text, Color fg_color, Color bg_color,
                         std::vector<std::string> options,
                         std::function<void(std::string, std::vector<std::string>)> on_change)
    : UI_x_boxes(id, UI_CHECKBOX_TYPE, text, fg_color, bg_color, options), m_selected(), m_on_change(on_change)
{
    for (size_t i = 0; i < m_options.size(); i++)
    {
        m_selected.push_back(false);
    }
}

UI_checkbox::UI_checkbox(const json &j) : UI_x_boxes(UI_CHECKBOX_TYPE), m_selected(), m_on_change(nullptr)
{
    from_json(j);
}

void UI_checkbox::toggle_selected(size_t selected)
{
    if (selected >= m_options.size())
        return;
    m_selected[selected] = !m_selected[selected];
    if (m_on_change != nullptr)
    {
        std::thread([this] { m_on_change(m_id, selected_text()); }).detach();
    }
}

void UI_checkbox::toggle_selected(std::string selected)
{
    for (size_t i = 0; i < m_options.size(); i++)
    {
        if (m_options[i] == selected)
        {
            toggle_selected(i);
            return;
        }
    }
}

std::vector<bool> UI_checkbox::selected() const
{
    return m_selected;
}

std::vector<std::string> UI_checkbox::selected_text() const
{
    std::vector<std::string> selected_text;
    for (size_t i = 0; i < m_options.size(); i++)
    {
        if (m_selected[i])
        {
            selected_text.push_back(m_options[i]);
        }
    }
    return selected_text;
}

void UI_checkbox::from_json(const json &j)
{
    UI_x_boxes::from_json(j);

    if (j.contains("selected"))
    {
        m_selected.clear();
        for (auto selected : j["selected"])
        {
            m_selected.push_back(selected);
        }
    }
}

json UI_checkbox::to_json() const
{
    json j = UI_x_boxes::to_json();
    j["selected"] = m_selected;
    return j;
}
