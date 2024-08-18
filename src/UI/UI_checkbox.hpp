#pragma once
#include <cstdint>
#include "UI_item.hpp"
#include "UI_x_boxes.hpp"

#define UI_CHECKBOX_TYPE std::string("UIcheckbox")
typedef void (*UI_checkbox_on_change) (std::string id, std::vector<std::string> selected);

class UI_checkbox : public UI_x_boxes
{
    public:
        UI_checkbox();
        UI_checkbox(std::string id, std::string text, Color fg_color, Color bg_color, std::vector<std::string> options, UI_checkbox_on_change on_change = nullptr);
        UI_checkbox(const json& j);

        void toggle_selected(size_t selected);
        void toggle_selected(std::string selected);
        std::vector<bool> selected() const;
        std::vector<std::string> selected_text() const;
    
        void from_json(const json& j) override;
        json to_json() const override;
    private:
        std::vector<bool> p_selected;

        UI_checkbox_on_change p_on_change;
};