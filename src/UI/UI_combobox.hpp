#pragma once

#include "UI_item.hpp"
#include "UI_x_boxes.hpp"
#include <functional>

#define UI_COMBOBOX_TYPE std::string("UI_combobox")

class UI_combobox : public UI_x_boxes
{
    Q_OBJECT
  public:
    UI_combobox(QObject *parrent = nullptr);
    UI_combobox(std::string id, std::string text, Color fg_color, Color bg_color, std::vector<std::string> options,
                size_t selected = 0, QObject *parrent = nullptr);
    UI_combobox(const json &j, QObject *parrent = nullptr);

    void set_selected(size_t selected);
    void set_selected(std::string selected) override;
    size_t selected() const;
    std::string selected_text() const;

    void from_json(const json &j) override;
    json to_json() const override;
  signals:
    void on_change(std::string id, std::string selected);

  private:
    size_t m_selected;
};
