#pragma once
#include "ui_item.hpp"

/**
 * @brief Class representing a UI table
 *
 */
class UI_table : public UI_item
{
    Q_OBJECT
  public:
    /**
     * @brief Construct a UI table object
     *
     * @param parent The parent QObject
     */
    explicit UI_table(QObject *parent = nullptr);
    /**
     * @brief Construct a UI table object
     *
     * @param row_count The number of rows
     * @param column_count The number of columns
     * @param row_labels The labels for the rows
     * @param column_labels The labels for the columns
     * @param parent The parent QObject
     */
    explicit UI_table(qsizetype row_count, qsizetype column_count, QList<QString> row_labels,
                      QList<QString> column_labels, QObject *parent = nullptr);

    /**
     * @brief Set the number of rows in the table
     *
     * @param count The number of rows
     */
    void set_row_count(qsizetype count);
    /**
     * @brief Get the number of rows in the table
     *
     * @return size_t The number of rows
     */
    qsizetype row_count() const;

    /**
     * @brief Set the number of columns in the table
     *
     * @param count The number of columns
     */
    void set_column_count(qsizetype count);
    /**
     * @brief Get the number of columns in the table
     *
     * @return size_t The number of columns
     */
    qsizetype column_count() const;

    /**
     * @brief Get the capacity of the table (rows * columns)
     *
     * @return size_t The capacity of the table
     */
    qsizetype capacity() const;

    /**
     * @brief Set the label for a specific row
     *
     * @param index The index of the row
     * @param label The label to set
     */
    void set_row_label(qsizetype index, QString label);
    /**
     * @brief Get the label of a specific row
     *
     * @param index The index of the row
     * @return QString The label of the row
     */
    QString row_label(qsizetype index) const;

    /**
     * @brief Set the label for a specific column
     *
     * @param index The index of the column
     * @param label The label to set
     */
    void set_column_label(qsizetype index, QString label);
    /**
     * @brief Get the label of a specific column
     *
     * @param index The index of the column
     * @return QString The label of the column
     */
    QString column_label(qsizetype index) const &;

    /**
     * @brief Insert an item into the table at the specified row and column
     *
     * @param row The row of the item
     * @param column The column of the item
     * @param text The text of the item
     */
    void insert_item(qsizetype row, qsizetype column, QString text);
    /**
     * @brief Get the item at the specified row and column
     *
     * @param row The row of the item
     * @param column The column of the item
     * @return QString The text of the item
     */
    QString item(qsizetype row, qsizetype column) const &;

    /**
     * @brief empty_item This function clears the item
     * @param row The row of the item
     * @param column The column of the item
     */
    void empty_item(qsizetype row, qsizetype column);
};
