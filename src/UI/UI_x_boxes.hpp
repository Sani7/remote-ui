#pragma once

#include "UI_item.hpp"

class UI_x_boxes : public UI_item
{
    Q_OBJECT
  public:
    UI_x_boxes(std::string type, QObject *parrent = nullptr);
    UI_x_boxes(std::string id, std::string type, std::string text, Color fg_color, Color bg_color,
               std::vector<std::string> options, QObject *parrent = nullptr);

    void from_json(const json &j) override;
    json to_json() const override;

  protected:
    std::vector<std::string> m_options;
};
