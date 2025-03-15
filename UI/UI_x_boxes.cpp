#include "UI_x_boxes.hpp"

UI_x_boxes::UI_x_boxes(std::string type, QObject *parent) : UI_item(type, parent), m_options()
{
}

UI_x_boxes::UI_x_boxes(std::string type, std::string text, Color fg_color, Color bg_color,
                       std::vector<std::string> options, QObject *parent)
    : UI_item(type, text, 12, fg_color, bg_color, parent), m_options(options)
{
}

void UI_x_boxes::from_json(const json &j)
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
}

json UI_x_boxes::to_json(size_t id) const
{
    json j = UI_item::to_json(id);
    j["options"] = this->m_options;
    return j;
}
