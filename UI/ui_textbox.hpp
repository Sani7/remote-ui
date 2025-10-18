#pragma once
#include "ui_item.hpp"
#include <string>

#define UI_TEXTBOX_TYPE std::string("ui_textbox")

class UI_textbox : public UI_item
{
    Q_OBJECT
  public:
    UI_textbox(std::string hint, Color fg_color, Color bg_color, QObject *parent = nullptr);
    UI_textbox(QObject *parent = nullptr);

    void set_text(std::string text) override;

    void set_hint(std::string hint);
    std::string hint() const;

    void from_json(const json &j) override;
    json to_json(size_t id) const override;
  signals:
    void changed(std::string text);

  private:
    std::string m_hint;
};
