#include "UI_plot.hpp"

UI_plot::UI_plot(QObject *parent) : UI_item{UI_PLOT_TYPE, parent}
{
}

UI_plot::UI_plot(std::string text, std::string x_label, std::string y_label, QObject *parent)
    : UI_item(UI_PLOT_TYPE, parent), m_x_label(x_label), m_y_label(y_label)
{
    set_text(text);
}

UI_plot::UI_plot(const json &j, QObject *parent) : UI_item{UI_PLOT_TYPE, parent}
{
    from_json(j);
}

void UI_plot::set_x_label(std::string name)
{
    m_x_label = name;
}

std::string UI_plot::x_label() const
{
    return m_x_label;
}

void UI_plot::set_y_label(std::string name)
{
    m_y_label = name;
}

std::string UI_plot::y_label() const
{
    return m_y_label;
}

void UI_plot::add_value(double x, double y)
{
    m_x_vals.push_back(x);
    m_y_vals.push_back(y);
}

void UI_plot::clean_vals()
{
    m_x_vals.clear();
    m_y_vals.clear();
}

void UI_plot::from_json(const json &j)
{
    UI_item::from_json(j);
    this->m_x_label = j.at("x_label");
    this->m_y_label = j.at("y_label");
    this->m_x_vals = (std::vector<double>)j.at("x_vals");
    this->m_y_vals = (std::vector<double>)j.at("y_vals");
}

json UI_plot::to_json(size_t id) const
{
    json j = UI_item::to_json(id);
    j["x_label"] = m_x_label;
    j["y_label"] = m_y_label;
    j["x_vals"] = m_x_vals;
    j["y_vals"] = m_y_vals;
    return j;
}
