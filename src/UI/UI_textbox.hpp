#pragma once
#include <string>
#include "UI_item.hpp"

#define UI_TEXTBOX_TYPE std::string("UItextbox")

class UI_textbox : public UI_item
{
public:
    UI_textbox(std::string id, std::string hint, Color fg_color, Color bg_color);
    UI_textbox(const json& j);

    void set_hint(std::string hint);
    std::string hint() const;

    void from_json(const json& j) override;
    json to_json() const override;
private:
    std::string m_hint;
};