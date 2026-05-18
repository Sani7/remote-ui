#include "ui_status_bar.hpp"

UI_status_bar::UI_status_bar(QObject *parent) : UI_item(UI_typeGadget::UI_type::ui_status_bar, parent)
{
    m_ui_item.setStatusBar(UI_status_bar_m());
    m_ui_item.statusBar().setTimeout(0);
}

UI_status_bar::UI_status_bar(QString message, QObject *parent)
    : UI_item(UI_typeGadget::UI_type::ui_status_bar, parent)
{
    m_ui_item.setStatusBar(UI_status_bar_m());
    m_ui_item.statusBar().setMessage(message);
    m_ui_item.statusBar().setTimeout(0);
}

void UI_status_bar::show_message(QString message, size_t timeout)
{
    // This should work because the message gets only send once when it changes
    m_ui_item.statusBar().setMessage(message);
    m_ui_item.statusBar().setTimeout(timeout);
    emit ui_changed();
}

QString UI_status_bar::message() const &
{
    return m_ui_item.statusBar().message();
}

size_t UI_status_bar::timeout() const
{
    return m_ui_item.statusBar().timeout();
}
