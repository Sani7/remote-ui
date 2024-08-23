#pragma once

#include <functional>
#include "UI_item.hpp"
#include "UI_x_boxes.hpp"

#define UI_COMBOBOX_TYPE std::string("UIcombobox")

class UI_combobox : public UI_x_boxes
{
    public:
        UI_combobox();
        UI_combobox(std::string id, std::string text, Color fg_color, Color bg_color, std::vector<std::string> options, size_t selected = 0, std::function<void(std::string, std::string)> on_change = nullptr);
        UI_combobox(const json& j);

        void set_selected(size_t selected);
        void set_selected(std::string selected) override;
        size_t selected() const;
        std::string selected_text() const;
    
        void from_json(const json& j) override;
        json to_json() const override;
    private:
        size_t m_selected;

        std::function<void(std::string, std::string)> m_on_change;
};