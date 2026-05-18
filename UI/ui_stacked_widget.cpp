#include "ui_stacked_widget.hpp"

UI_stacked_widget::UI_stacked_widget(QObject *parent) : UI_item(UI_typeGadget::UI_type::ui_stacked_widget, parent)
{
    m_ui_item.setStackedWidget(UI_stacked_widget_m());
}

UI_stacked_widget::UI_stacked_widget(qsizetype tab_count, qsizetype current_tab, QObject *parent)
    : UI_item(UI_typeGadget::UI_type::ui_stacked_widget, parent)
{
    m_ui_item.setStackedWidget(UI_stacked_widget_m());
    m_ui_item.stackedWidget().setTabCount(tab_count);
    m_ui_item.stackedWidget().setCurrentTab(current_tab);
}

void UI_stacked_widget::set_selected(qsizetype selected)
{
    if (selected >= m_ui_item.stackedWidget().tabCount())
        return;
    m_ui_item.stackedWidget().setCurrentTab(selected);
    emit ui_changed();
}
qsizetype UI_stacked_widget::selected() const
{
    return m_ui_item.stackedWidget().currentTab();
}

void UI_stacked_widget::advance_tab()
{
    if (m_ui_item.stackedWidget().tabCount() == 0)
        return;
    m_ui_item.stackedWidget().setCurrentTab((m_ui_item.stackedWidget().currentTab() + 1) % m_ui_item.stackedWidget().tabCount());
    emit ui_changed();
}
