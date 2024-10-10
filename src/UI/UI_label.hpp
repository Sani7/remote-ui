#pragma once
#include "UI_item.hpp"
#include <string>

#define UI_LABEL_TYPE std::string("UI_label")

class UI_label : public UI_item
{
    Q_OBJECT
  public:
    UI_label(std::string text, Color fg_color, Color bg_color, QObject *parrent = nullptr);
    UI_label(const json &j, QObject *parrent = nullptr);

    void from_json(const json &j) override;
    json to_json(size_t id) const override;
};
