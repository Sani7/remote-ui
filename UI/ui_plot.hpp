#pragma once
#include "ui_item.hpp"

/**
 * @brief Class representing a UI plot
 *
 */
class UI_plot : public UI_item
{
    Q_OBJECT
  public:
    /**
     * @brief Construct a UI plot object
     *
     * @param parent The parent QObject
     */
    explicit UI_plot(QObject *parent = nullptr);
    /**
     * @brief Construct a UI plot object
     *
     * @param text The text to display
     * @param x_label The label for the x-axis
     * @param y_label The label for the y-axis
     * @param parent The parent QObject
     */
    UI_plot(QString text, QString x_label, QString y_label, QObject *parent = nullptr);

    /**
     * @brief Set the label for the x-axis
     *
     * @param name The name to set
     */
    void set_x_label(QString name);
    /**
     * @brief Get the label for the x-axis
     *
     * @return QString The x-axis label
     */
    QString x_label() const &;

    /**
     * @brief Set the label for the y-axis
     *
     * @param name The name to set
     */
    void set_y_label(QString name);
    /**
     * @brief Get the label for the y-axis
     *
     * @return QString The y-axis label
     */
    QString y_label() const &;
    /**
     * @brief set_fg_color Set the pen color
     * @param color The color
     */
    void set_fg_color(Color color);
    /**
     * @brief fg_color Get the pen color
     * @return The color
     */
    Color fg_color() const&;
    /**
     * @brief set_bg_color Set the background color
     * @param color The color
     */
    void set_bg_color(Color color);
    /**
     * @brief bg_color Get the background color
     * @return The color
     */
    Color bg_color() const;

    /**
     * @brief Add a value to the plot
     *
     * @param y The y value to add
     */
    void add_value(double y);
    /**
     * @brief Add a value pair to the plot
     *
     * @param x The x value to add
     * @param y The y value to add
     */
    void add_value(double x, double y);
    /**
     * @brief Get the x and y values of the plot
     *        This is only for testing purposes
     *
     * @return a list of x values and a list of y values
     */
    const QList<double> &x_vals() const &;
    /**
     * @brief Get the y values of the plot
     *        This is only for testing purposes
     *
     * @return a list of y values
     */
    const QList<double> &y_vals() const &;
    /**
     * @brief Clear all values from the plot
     *
     */
    void clean_vals();
};
