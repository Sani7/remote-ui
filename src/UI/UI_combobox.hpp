#pragma once

#include "UI_item.hpp"
#include "UI_x_boxes.hpp"

#define UI_COMBOBOX_TYPE std::string("UIcombobox")
typedef void (*UI_combobox_on_change) (std::string id, int selected);

class UI_combobox : public UI_x_boxes
{
    public:
        UI_combobox();
        UI_combobox(std::string id, std::string text, Color fg_color, Color bg_color, std::vector<std::string> options, int selected, UI_combobox_on_change on_change = nullptr);
        UI_combobox(std::string id, std::string text, Color fg_color, Color bg_color, std::vector<std::string> options, UI_combobox_on_change on_change = nullptr);
        UI_combobox(const json& j);

        void set_selected(size_t selected);
        void set_selected(std::string selected);
        size_t selected() const;
        std::string selected_text() const;
    
        void from_json(const json& j) override;
        json to_json() const override;
    private:
        size_t p_selected;

        UI_combobox_on_change p_on_change;
};