#include "ui_tab_widget.hpp"

UI_tab_widget::UI_tab_widget(QObject *parent) : UI_item(UI_typeGadget::UI_type::ui_tab_widget, parent)
{
    m_ui_item.setStackedWidget(UI_stacked_widget_m());
}

UI_tab_widget::UI_tab_widget(QList<QString> tab_names, qsizetype selected, QObject *parent)
    : UI_item(UI_typeGadget::UI_type::ui_tab_widget, parent)
{
    m_ui_item.setTabWidget(UI_tab_widget_m());
    m_ui_item.tabWidget().setTabNames(tab_names);
    m_ui_item.tabWidget().setSelecteTab(selected);
}

void UI_tab_widget::set_selected(qsizetype selected)
{
    if (selected >= m_ui_item.tabWidget().tabNames().size())
        return;
    m_ui_item.tabWidget().setSelecteTab(selected);
    emit ui_changed();
    emit changed(selected);
}
qsizetype UI_tab_widget::selected() const
{
    return m_ui_item.tabWidget().selecteTab();
}

void UI_tab_widget::set_visible(qsizetype index, bool visible)
{
    if (index >=  m_ui_item.tabWidget().tabNames().size())
    {
        // Only allow known tab amount from constructor
        return;
    }
    auto list = m_ui_item.tabWidget().tabVisible();
    list.replace(index, visible);
    m_ui_item.tabWidget().setTabVisible(list);
    emit ui_changed();
}
bool UI_tab_widget::visible(qsizetype index) const
{
    if (index >= m_ui_item.tabWidget().tabNames().size())
    {
        return false;
    }
    return m_ui_item.tabWidget().tabVisible().at(index);
}

void UI_tab_widget::set_tab_name(qsizetype index, QString name)
{
    if (index >= m_ui_item.tabWidget().tabNames().size() || index >= m_ui_item.tabWidget().tabVisible().size())
    {
        // Only allow known tab amount from constructor
        return;
    }
    auto list = m_ui_item.tabWidget().tabNames();
    list.replace(index, name);
    m_ui_item.tabWidget().setTabNames(list);
    emit ui_changed();
}
QString UI_tab_widget::tab_name(qsizetype index) const
{
    if (index >= m_ui_item.tabWidget().tabNames().size())
    {
        return "";
    }
    return m_ui_item.tabWidget().tabNames().at(index);
}

void UI_tab_widget::advance_tab()
{
    if (m_ui_item.tabWidget().tabNames().empty())
        return;
    m_ui_item.tabWidget().setSelecteTab((m_ui_item.tabWidget().selecteTab() + 1) % m_ui_item.tabWidget().tabNames().size());
    emit ui_changed();
    emit changed(m_ui_item.tabWidget().selecteTab());
}
