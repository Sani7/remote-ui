#include "ui_table.hpp"

UI_table::UI_table(QObject* parent)
    : UI_item(UI_TABLE_TYPE, parent), m_row_count(0), m_column_count(0)
{
}

UI_table::UI_table(size_t row_count, size_t column_count, std::vector<std::string> row_labels, std::vector<std::string> column_labels, QObject* parent)
    : UI_item(UI_TABLE_TYPE, parent), m_row_count(row_count), m_column_count(column_count), m_row_labels(row_labels), m_column_labels(column_labels)
{
}

void UI_table::set_row_count(size_t count)
{
    m_row_count = count;
}

size_t UI_table::row_count() const
{
    return m_row_count;
}

void UI_table::set_column_count(size_t count)
{
    m_column_count = count;
}

size_t UI_table::column_count() const
{
    return m_column_count;
}

void UI_table::set_row_label(size_t index, std::string label)
{
    if (m_row_count <= index)
    {
        return;
    }

    m_row_labels[index] = label;
}

std::string UI_table::row_label(size_t index) const
{
    if (m_row_count <= index)
    {
        return "";
    }

    return m_row_labels[index];
}

void UI_table::set_column_label(size_t index, std::string label)
{
    if (m_column_count <= index)
    {
        return;
    }

    m_column_labels[index] = label;
}

std::string UI_table::column_label(size_t index) const
{
    if (m_column_count <= index)
    {
        return "";
    }

    return m_column_labels[index];
}

void UI_table::insert_item(size_t row, size_t column, std::string text)
{
    if (m_column_count <= column || m_row_count <= row)
    {
        return;
    }
    size_t index = row * m_column_count + column;

    m_table[index] = text;
    m_valid[index] = true;
}

void UI_table::empty_item(size_t row, size_t column)
{
    size_t index = row * m_column_count + column;

    m_table[index] = "";
    m_valid[index] = false;
}

void UI_table::from_json(const json &j)
{
    UI_item::from_json(j);

    this->m_row_count = j.at("row_count");
    this->m_row_labels = j.at("row_labels");
    this->m_column_count = j.at("column_count");
    this->m_column_labels = j.at("column_labels");
    this->m_valid = (std::vector<bool>)j.at("valid");
    this->m_table = j.at("table");
}

json UI_table::to_json(size_t id) const
{
    json j = UI_item::to_json(id);
    j["row_count"] = m_row_count;
    j["row_labels"] = m_row_labels;
    j["column_count"] = m_column_count;
    j["column_labels"] = m_column_labels;
    j["valid"] = m_valid;
    j["table"] = m_table;

    return j;
}
