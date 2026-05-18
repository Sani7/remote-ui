#include "ui_plot.hpp"

UI_plot::UI_plot(QObject *parent) : UI_item{UI_typeGadget::UI_type::ui_plot, parent}
{
    m_ui_item.setPlot(UI_plot_m());
}

UI_plot::UI_plot(QString text, QString x_label, QString y_label, QObject *parent)
    : UI_item(UI_typeGadget::UI_type::ui_plot, parent)
{
    m_ui_item.setPlot(UI_plot_m());
    m_ui_item.plot().setText(text);
    m_ui_item.plot().setXLabel(x_label);
    m_ui_item.plot().setYLabel(y_label);
}

void UI_plot::set_x_label(QString name)
{
    m_ui_item.plot().setXLabel(name);

    emit ui_changed();
}

QString UI_plot::x_label() const &
{
    return m_ui_item.plot().xLabel();
}

void UI_plot::set_y_label(QString name)
{
    m_ui_item.plot().setYLabel(name);

    emit ui_changed();
}

QString UI_plot::y_label() const &
{
    return m_ui_item.plot().yLabel();
}

void UI_plot::set_fg_color(Color color)
{
    m_ui_item.plot().mutColor().setFgColor(color.color());

    emit ui_changed();
}

Color UI_plot::fg_color() const&
{
    return Color(m_ui_item.plot().color().fgColor());
}

void UI_plot::set_bg_color(Color color)
{
    m_ui_item.plot().mutColor().setBgColor(color.color());

    emit ui_changed();
}

Color UI_plot::bg_color() const
{
    return Color(m_ui_item.plot().color().bgColor());
}

void UI_plot::add_value(double y)
{
    auto x_vals = m_ui_item.plot().xVals();
    auto y_vals = m_ui_item.plot().yVals();
    x_vals.push_back((double)x_vals.size());
    y_vals.push_back(y);
    m_ui_item.plot().setXVals(x_vals);
    m_ui_item.plot().setYVals(y_vals);

    emit ui_changed();
}

void UI_plot::add_value(double x, double y)
{
    auto x_vals = m_ui_item.plot().xVals();
    auto y_vals = m_ui_item.plot().yVals();
    x_vals.push_back(x);
    y_vals.push_back(y);
    m_ui_item.plot().setXVals(x_vals);
    m_ui_item.plot().setYVals(y_vals);

    emit ui_changed();
}

const QList<double> &UI_plot::x_vals() const &
{
    return m_ui_item.plot().xVals();
}

const QList<double> &UI_plot::y_vals() const &
{
    return m_ui_item.plot().yVals();
}

void UI_plot::clean_vals()
{
    m_ui_item.plot().setXVals(QList<double>());
    m_ui_item.plot().setYVals(QList<double>());

    emit ui_changed();
}
