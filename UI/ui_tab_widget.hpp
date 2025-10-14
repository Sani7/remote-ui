#pragma once
#include "ui_item.hpp"

#define UI_TAB_WIDGET_TYPE std::string("ui_tab_widget")

class UI_tab_widget : public UI_item
{
    Q_OBJECT
  public:
    explicit UI_tab_widget(QObject *parent = nullptr);
    UI_tab_widget(std::vector<std::string> tab_names, size_t selected = 0, QObject *parent = nullptr);

    void set_selected(size_t selected) override;
    size_t selected() const;

    void set_visible(size_t index, bool visible);
    bool visible(size_t index) const;

    void set_tab_name(size_t index, std::string name);
    std::string tab_name(size_t index) const;

    void advance_tab();

    void from_json(const json &j) override;
    json to_json(size_t id) const override;
  signals:
    void selected(size_t index);

  private:
    size_t m_selected_tab;
    std::vector<std::string> m_tab_names;
    std::vector<bool> m_tab_visible;
};
