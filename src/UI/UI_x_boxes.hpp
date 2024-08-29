#pragma once

#include "UI_item.hpp"

class UI_x_boxes : public UI_item
{
  public:
    UI_x_boxes(std::string type);
    UI_x_boxes(std::string id, std::string type, std::string text, Color fg_color, Color bg_color,
               std::vector<std::string> options);

    void from_json(const json &j) override;
    json to_json() const override;

  protected:
    std::vector<std::string> m_options;
};