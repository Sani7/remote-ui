#pragma once

#include "UI_item.hpp"

#define UI_BUTTON_TYPE std::string("UIButton")
typedef void(*UI_button_on_click)(std::string id);

class UI_button : public UI_item
{
    public:
        UI_button(std::string id, std::string text, Color fg_color, Color bg_color, UI_button_on_click on_click);
        UI_button(const json& j);
    
        void from_json(const json& j) override;
        json to_json() const override;

        void click() const;
    private:
        UI_button_on_click p_on_click = nullptr;
};