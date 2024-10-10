#include "UI_checkbox.hpp"

UI_checkbox::UI_checkbox(QObject *parrent) : UI_x_boxes(UI_CHECKBOX_TYPE, parrent), m_selected()
{
}

UI_checkbox::UI_checkbox(std::string text, Color fg_color, Color bg_color,
                         std::vector<std::string> options, QObject *parrent)
    : UI_x_boxes(UI_CHECKBOX_TYPE, text, fg_color, bg_color, options, parrent), m_selected()
{
    for (size_t i = 0; i < m_options.size(); i++)
    {
        m_selected.push_back(false);
    }
}

UI_checkbox::UI_checkbox(const json &j, QObject *parrent) : UI_x_boxes(UI_CHECKBOX_TYPE, parrent), m_selected()
{
    from_json(j);
}

void UI_checkbox::toggle_selected(size_t selected)
{
    if (selected >= m_options.size())
        return;
    m_selected[selected] = !m_selected[selected];
    emit value_changed();
    emit on_change(selected_text()[selected]);
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

json UI_checkbox::to_json(size_t id) const
{
    json j = UI_x_boxes::to_json(id);
    j["selected"] = m_selected;
    return j;
}
