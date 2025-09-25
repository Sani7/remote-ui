#ifndef UI_STACKED_WIDGET_HPP
#define UI_STACKED_WIDGET_HPP

#include "ui_item.hpp"
#define UI_STACKED_WIDGET_TYPE std::string("ui_stacked_widget")

class UI_stacked_widget : public UI_item
{
    Q_OBJECT
  public:
    explicit UI_stacked_widget(QObject *parent = nullptr);
    UI_stacked_widget(size_t tab_count, size_t current_tab, QObject *parent = nullptr);

    void set_selected(size_t selected) override;
    size_t selected() const;
    void advance_tab();

    void from_json(const json &j) override;
    json to_json(size_t id) const override;

  private:
    size_t m_tab_count;
    size_t m_current_tab;
};

#endif // UI_STACKED_WIDGET_HPP
