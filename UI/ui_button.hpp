#pragma once
#include "ui_item.hpp"

#define UI_BUTTON_TYPE std::string("ui_button")

class UI_button : public UI_item
{
    Q_OBJECT
  public:
    UI_button(std::string text, Color fg_color, Color bg_color, QObject *parent = nullptr);
    UI_button(const json &j, QObject *parent = nullptr);

    void from_json(const json &j) override;
    json to_json(size_t id) const override;

    void click() override;

  signals:
    void clicked();
};
