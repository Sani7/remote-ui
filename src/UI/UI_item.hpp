#pragma once
#include <functional>
#include <thread>
#include <cstdint>
#include <nlohmann/json.hpp>
#include <string>
#include "color.hpp"
using json = nlohmann::json;

#define UNUSED(x) (void)(x)

class UI_item {
  public:
    UI_item(std::string type);
    UI_item(std::string id, std::string type, std::string text, uint8_t text_size, Color fg_color, Color bg_color);
    UI_item(const json& j);

    std::string id() const;
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

    virtual void from_json(const json& j);

    virtual json to_json() const;

    virtual void click() const {};
    virtual void set_selected(std::string selected) {UNUSED(selected);};
    virtual void set_value(double value) {UNUSED(value);};

  protected:
    std::string m_id;
    std::string m_type;
    std::string m_text;
    uint8_t m_text_size;
    Color m_fg_color;
    Color m_bg_color;
    bool enabled;
    bool visible;
};