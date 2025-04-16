#pragma once
#include "ui_item.hpp"
#include <string>

#define UI_LABEL_TYPE std::string("ui_label")

class UI_label : public UI_item
{
    Q_OBJECT
  public:
    UI_label(std::string text, Color fg_color, Color bg_color, QObject *parent = nullptr);
    UI_label(const json &j, QObject *parent = nullptr);

    void from_json(const json &j) override;
    json to_json(size_t id) const override;
};
