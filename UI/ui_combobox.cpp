#include "ui_combobox.hpp"

UI_combobox::UI_combobox(QObject *parent) : UI_item(UI_typeGadget::UI_type::ui_combobox, parent)
{
    m_ui_item.setCombobox(UI_combobox_m());
}

UI_combobox::UI_combobox(QList<QString> options, qsizetype selected,
                         QObject *parent)
    : UI_item(UI_typeGadget::UI_type::ui_combobox, parent)
{
    m_ui_item.setCombobox(UI_combobox_m());
    m_ui_item.combobox().setOptions(options);
    m_ui_item.combobox().setSelected(selected);
}

void UI_combobox::set_selected(qsizetype selected)
{
    if (selected == m_ui_item.combobox().selected())
        return;
    m_ui_item.combobox().setSelected(selected);
    emit ui_changed();
    emit changed(selected_text());
}

void UI_combobox::set_selected(QString selected)
{
    for (qsizetype i = 0; i < m_ui_item.combobox().options().size(); i++)
    {
        if (m_ui_item.combobox().options().at(i) == selected)
        {
            set_selected(i);
            return;
        }
    }
}

qsizetype UI_combobox::selected() const
{
    return m_ui_item.combobox().selected();
}

QString UI_combobox::selected_text() const &
{
    return m_ui_item.combobox().options().at(m_ui_item.combobox().selected());
}
