#pragma once

#include "ui_item.hpp"
#include "ui_x_boxes.hpp"

#define UI_COMBOBOX_TYPE std::string("ui_combobox")

class UI_combobox : public UI_x_boxes
{
    Q_OBJECT
  public:
    UI_combobox(QObject *parent = nullptr);
    UI_combobox(Color fg_color, Color bg_color, std::vector<std::string> options, size_t selected = 0,
                QObject *parent = nullptr);
    UI_combobox(const json &j, QObject *parent = nullptr);

    void set_selected(size_t selected);
    void set_selected(std::string selected) override;
    size_t selected() const;
    std::string selected_text() const;

    void from_json(const json &j) override;
    json to_json(size_t id) const override;
  signals:
    void changed(std::string selected);

  private:
    size_t m_selected;
};
