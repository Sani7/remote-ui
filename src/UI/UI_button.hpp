#pragma once

#include "UI_item.hpp"

#define UI_BUTTON_TYPE std::string("UIButton")

class UI_button : public UI_item
{
    public:
        UI_button(std::string id, std::string text, Color fg_color, Color bg_color, std::function<void(std::string)> on_click = nullptr);
        UI_button(const json& j);
    
        void from_json(const json& j) override;
        json to_json() const override;

        void click() const override;
    private:
        std::function<void(std::string)> p_on_click = nullptr;
};