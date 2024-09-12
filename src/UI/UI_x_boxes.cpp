#include "UI_x_boxes.hpp"

UI_x_boxes::UI_x_boxes(std::string type, QObject *parrent) : UI_item(type, parrent), m_options()
{
}

UI_x_boxes::UI_x_boxes(std::string id, std::string type, std::string text, Color fg_color, Color bg_color,
                       std::vector<std::string> options, QObject *parrent)
    : UI_item(id, type, text, 12, fg_color, bg_color, parrent), m_options(options)
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

json UI_x_boxes::to_json() const
{
    json j = UI_item::to_json();
    j["options"] = this->m_options;
    return j;
}
