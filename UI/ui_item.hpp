#pragma once
#include "color.hpp"
#include <cstdint>
#include <nlohmann/json.hpp>
#include <string>

#include <QCanBusFrame>
#include <QObject>
using json = nlohmann::json;

#define UNUSED(x) (void)(x)

class UI_item : public QObject
{
    Q_OBJECT
  public:
    explicit UI_item(std::string type, QObject *parent = nullptr);
    explicit UI_item(std::string type, std::string text, Color fg_color, Color bg_color, QObject *parent = nullptr);
    explicit UI_item(const json &j, QObject *parent = nullptr);
    ~UI_item();

    std::string type() const;
    bool is_type(json j);

    virtual void set_text(std::string text);
    void append_text(std::string text);
    std::string text() const;

    void set_fg_color(Color fg_color);
    Color fg_color() const;

    void set_bg_color(Color bg_color);
    Color bg_color() const;

    void set_enabled(bool enabled);
    bool is_enabled() const;

    void set_visible(bool visible);
    bool is_visible() const;

    virtual void from_json(const json &j);

    virtual json to_json(size_t id) const;

    virtual void click();
    virtual void set_selected(size_t selected);
    virtual void set_value(double value);
    virtual void can_send(uint32_t id, uint8_t dlc, std::array<uint8_t, 8> payload);
    virtual void clear();

  signals:
    void ui_changed();

  protected:
    void setup_item(bool text, bool fg_color, bool bg_color);

    size_t m_id;
    std::string m_type;
    std::string m_text;
    bool m_text_enabled = true;
    Color m_fg_color;
    bool m_fg_color_enabled = true;
    Color m_bg_color;
    bool m_bg_color_enabled = true;
    bool m_enabled;
    bool m_visible;
};
