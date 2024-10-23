#pragma once
#include "color.hpp"
#include <cstdint>
#include <functional>
#include <nlohmann/json.hpp>
#include <string>
#include <thread>

#include <QObject>
#include <QCanBusFrame>
using json = nlohmann::json;

#define UNUSED(x) (void)(x)

class UI_item : public QObject
{
    Q_OBJECT
  public:
    explicit UI_item(std::string type, QObject *parent = nullptr);
    explicit UI_item(std::string type, std::string text, uint8_t text_size, Color fg_color,
                     Color bg_color, QObject *parent = nullptr);
    explicit UI_item(const json &j, QObject *parent = nullptr);
    ~UI_item();

    std::string type() const;
    bool is_type(json j);

    virtual void set_text(std::string text);
    std::string text() const;

    void set_text_size(uint8_t text_size);
    uint8_t text_size() const;

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
    virtual void set_selected(std::string selected);
    virtual void set_value(double value);
    virtual void can_send(uint32_t id, uint8_t dlc, std::array<uint8_t, 8> payload);

  signals:
    void value_changed();

  protected:
    size_t m_id;
    std::string m_type;
    std::string m_text;
    uint8_t m_text_size;
    Color m_fg_color;
    Color m_bg_color;
    bool enabled;
    bool visible;
};
