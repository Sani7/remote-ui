#pragma once
#include "UI_item.hpp"
#include <string>

#define UI_LABEL_TYPE std::string("UI_label")

class UI_label : public UI_item
{
  public:
    UI_label(std::string id, std::string text, Color fg_color, Color bg_color);
    UI_label(const json &j);

    void from_json(const json &j) override;
    json to_json() const override;
};