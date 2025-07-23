#include "ui_combobox.hpp"

UI_combobox::UI_combobox(QObject *parent) : UI_item(UI_COMBOBOX_TYPE, parent), m_options()
{
}

UI_combobox::UI_combobox(Color fg_color, Color bg_color, std::vector<std::string> options, size_t selected,
                         QObject *parent)
    : UI_item(UI_COMBOBOX_TYPE, "", 12, fg_color, bg_color, parent), m_options(options), m_selected(selected)
{
}

UI_combobox::UI_combobox(const json &j, QObject *parent) : UI_item(UI_COMBOBOX_TYPE, parent)
{
    from_json(j);
}

void UI_combobox::set_selected(size_t selected)
{
    if (selected == m_selected)
        return;
    m_selected = selected;
    emit ui_changed();
    emit changed(selected_text());
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
    UI_item::from_json(j);
    if (j.contains("options"))
    {
        this->m_options.clear();
        for (auto option : j["options"])
        {
            this->m_options.push_back(option);
        }
    }
    m_selected = j.at("selected");
}

json UI_combobox::to_json(size_t id) const
{
    json j = UI_item::to_json(id);
    j["options"] = this->m_options;
    j["selected"] = m_selected;
    return j;
}
