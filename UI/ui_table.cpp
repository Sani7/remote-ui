#include "ui_table.hpp"

UI_table::UI_table(QObject *parent) : UI_item(UI_typeGadget::UI_type::ui_table, parent)
{
    m_ui_item.setTable(UI_table_m());
}

UI_table::UI_table(qsizetype row_count, qsizetype column_count, QList<QString> row_labels,
                   QList<QString> column_labels, QObject *parent)
    : UI_item(UI_typeGadget::UI_type::ui_table, parent)
{
    m_ui_item.setTable(UI_table_m());
    m_ui_item.table().setRowCount(row_count);
    m_ui_item.table().setColumnCount(column_count);
    m_ui_item.table().setColumnLabels(column_labels);
    m_ui_item.table().setRowLabels(row_labels);
}

void UI_table::set_row_count(qsizetype count)
{
    m_ui_item.table().setRowCount(count);
    auto column_count = m_ui_item.table().columnCount();
    auto row_count = m_ui_item.table().rowCount();
    auto row_labels = m_ui_item.table().rowLabels();

    if (row_count != 0 || column_count != 0)
    {
        auto table = m_ui_item.table().table();
        table.resize(column_count * count);
        m_ui_item.table().setTable(table);
    }

    row_labels.resize(count);
    m_ui_item.table().setRowLabels(row_labels);

    emit ui_changed();
}

qsizetype UI_table::row_count() const
{
    return m_ui_item.table().rowCount();
}

void UI_table::set_column_count(qsizetype count)
{
    m_ui_item.table().setColumnCount(count);
    auto column_count = m_ui_item.table().columnCount();
    auto row_count = m_ui_item.table().rowCount();
    auto column_labels = m_ui_item.table().columnLabels();

    if (row_count != 0 || column_count != 0)
    {
        auto table = m_ui_item.table().table();
        table.resize(column_count * count);
        m_ui_item.table().setTable(table);
    }

    column_labels.resize(count);
    m_ui_item.table().setColumnLabels(column_labels);

    emit ui_changed();
}

qsizetype UI_table::column_count() const
{
    return m_ui_item.table().columnCount();
}

qsizetype UI_table::capacity() const
{
    return m_ui_item.table().rowCount() * m_ui_item.table().columnCount();
}

void UI_table::set_row_label(qsizetype index, QString label)
{
    if (m_ui_item.table().rowCount() <= index)
    {
        return;
    }

    auto labels = m_ui_item.table().rowLabels();
    labels.replace(index, label);
    m_ui_item.table().setRowLabels(labels);

    emit ui_changed();
}

QString UI_table::row_label(qsizetype index) const
{
    if (m_ui_item.table().rowCount() <= index)
    {
        return "";
    }

    return m_ui_item.table().rowLabels().at(index);
}

void UI_table::set_column_label(qsizetype index, QString label)
{
    if (m_ui_item.table().columnCount() <= index)
    {
        return;
    }

    auto labels = m_ui_item.table().columnLabels();
    labels.replace(index, label);
    m_ui_item.table().setColumnLabels(labels);

    emit ui_changed();
}

QString UI_table::column_label(qsizetype index) const &
{
    if (m_ui_item.table().columnCount() <= index)
    {
        return "";
    }

    return m_ui_item.table().columnLabels().at(index);
}

void UI_table::insert_item(qsizetype row, qsizetype column, QString text)
{
    if (m_ui_item.table().columnCount() <= column || m_ui_item.table().rowCount() <= row)
    {
        return;
    }
    if (capacity() > m_ui_item.table().table().size())
    {
        auto table = m_ui_item.table().table();
        table.resize(capacity());
        m_ui_item.table().setTable(table);
    }

    qsizetype index = row * m_ui_item.table().columnCount() + column;

    auto table = m_ui_item.table().table();
    table.replace(index, text);
    m_ui_item.table().setTable(table);

    emit ui_changed();
}

QString UI_table::item(qsizetype row, qsizetype column) const &
{
    if (m_ui_item.table().columnCount() <= column || m_ui_item.table().rowCount() <= row)
    {
        return "";
    }
    qsizetype index = row * m_ui_item.table().columnCount() + column;

    return m_ui_item.table().table().at(index);
}

void UI_table::empty_item(qsizetype row, qsizetype column)
{
    if (m_ui_item.table().columnCount() <= column || m_ui_item.table().rowCount() <= row)
    {
        return;
    }
    qsizetype index = row * m_ui_item.table().columnCount() + column;

    auto table = m_ui_item.table().table();
    table.replace(index, "");
    m_ui_item.table().setTable(table);

    emit ui_changed();
}
