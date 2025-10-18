#pragma once
#include "ui_item.hpp"

#define UI_CHECKBOX_TYPE std::string("ui_checkbox")

class UI_checkbox : public UI_item
{
    Q_OBJECT
  public:
    UI_checkbox(QObject *parent = nullptr);
    UI_checkbox(std::string text, Color fg_color, Color bg_color, QObject *parent = nullptr);
    UI_checkbox(const json &j, QObject *parent = nullptr);

    void click() override;
    void set_state(bool state);
    bool state() const;

    void from_json(const json &j) override;
    json to_json(size_t id) const override;

  signals:
    void changed(bool);

  private:
    bool m_checked;
};
