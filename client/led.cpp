#include "led.hpp"

Led::Led(QWidget *parrent) : QPushButton(parrent)
{
    set_read_only(this, true);
}

Led::Led(const QString &text, QWidget *parent)
{
    set_read_only(this, true);
}

Led::Led(const QIcon &icon, const QString &text, QWidget *parent)
{
    set_read_only(this, true);
}

Led::~Led()
{
}

bool Led::is_read_only(QWidget *widget)
{
    return widget->testAttribute(Qt::WA_TransparentForMouseEvents) || (widget->focusPolicy() == Qt::NoFocus);
}

void Led::set_read_only(QWidget *widget, bool read_only)
{
    widget->setAttribute(Qt::WA_TransparentForMouseEvents, read_only);
    widget->setFocusPolicy(read_only ? Qt::NoFocus : Qt::StrongFocus);
}
