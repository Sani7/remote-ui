#pragma once

#include "UI_item.hpp"

#define UI_BUTTON_TYPE std::string("UI_button")

class UI_button : public UI_item
{
    Q_OBJECT
  public:
    UI_button(std::string id, std::string text, Color fg_color, Color bg_color, QObject* parrent = nullptr);
    UI_button(const json &j, QObject* parrent = nullptr);

    void from_json(const json &j) override;
    json to_json() const override;

    void click() override;

  signals:
    void on_click(std::string id);
};
