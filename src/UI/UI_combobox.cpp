#include "UI_combobox.hpp"

UI_combobox::UI_combobox(QObject *parrent) : UI_x_boxes(UI_COMBOBOX_TYPE, parrent)
{
}

UI_combobox::UI_combobox(Color fg_color, Color bg_color,
                         std::vector<std::string> options, size_t selected, QObject *parent)
    : UI_x_boxes(UI_COMBOBOX_TYPE, "", fg_color, bg_color, options, parent), m_selected(selected)
{
}

UI_combobox::UI_combobox(const json &j, QObject *parrent) : UI_x_boxes(UI_COMBOBOX_TYPE, parrent)
{
    from_json(j);
}

void UI_combobox::set_selected(size_t selected)
{
    if (selected == m_selected)
        return;
    m_selected = selected;
    emit value_changed();
    emit on_change(selected_text());
}

void UI_combobox::set_selected(std::string selected)
{
    for (size_t i = 0; i < m_options.size(); i++)
    {
        if (m_options[i] == selected)
        {
            set_selected(i);
            return;
        }
    }
}

size_t UI_combobox::selected() const
{
    return m_selected;
}

std::string UI_combobox::selected_text() const
{
    return m_options[m_selected];
}

void UI_combobox::from_json(const json &j)
{
    UI_x_boxes::from_json(j);
    std::string selected_str = j.at("selected");
    for (size_t i = 0; i < m_options.size(); i++)
    {
        if (m_options[i] == selected_str)
        {
            m_selected = i;
            break;
        }
    }
}

json UI_combobox::to_json(size_t id) const
{
    json j = UI_x_boxes::to_json(id);
    j["selected"] = m_options[m_selected];
    return j;
}
