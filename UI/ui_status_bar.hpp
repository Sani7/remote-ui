#ifndef UI_STATUS_BAR_HPP
#define UI_STATUS_BAR_HPP

#include "ui_item.hpp"
#define UI_STATUS_BAR_TYPE std::string("ui_status_bar")

class UI_status_bar : public UI_item
{
    Q_OBJECT
  public:
    explicit UI_status_bar(QObject *parent = nullptr);
    UI_status_bar(std::string message, QObject *parent = nullptr);

    void show_message(std::string message, size_t timeout = 0);
    std::string message() const;

    void from_json(const json &j) override;
    json to_json(size_t id) const override;

  private:
    std::string m_current_message;
    size_t m_timeout;
};

#endif // UI_STATUS_BAR_HPP
