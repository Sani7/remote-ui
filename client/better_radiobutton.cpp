#include "better_radiobutton.h"

Better_Radiobutton::Better_Radiobutton(QWidget* parent) :
QWidget{parent},
m_radiobuttons{}
{
}

Better_Radiobutton::Better_Radiobutton(std::vector<QString> names, QWidget* parent) :
    QWidget{parent},
    m_v_layout{new QVBoxLayout(this)},
    m_radiobuttons{},
    m_checked{""}
{
    for (const auto& name : names) {
        add_radiobutton(name);
    }
}

void Better_Radiobutton::add_radiobutton(QString name)
{
    auto radiobutton = new QRadioButton(name, this);
    m_radiobuttons.push_back(radiobutton);
    m_v_layout->addWidget(radiobutton);

    connect(radiobutton, &QRadioButton::toggled, this, [this, name, radiobutton] {
        emit toggled(name);
        if (radiobutton->isChecked()) {
            m_checked = name;
            emit checked(name);
        }
    });
}

QString Better_Radiobutton::isChecked() const
{
    return m_checked;
}
