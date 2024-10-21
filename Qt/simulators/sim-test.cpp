#include "sim-test.h"
#include "ui_sim-test.h"

Test_Sim::Test_Sim(Web_socket_wrapper* web_socket, QWidget* parent) :
      SimulatorBase(web_socket, parent),
      ui(new Ui::Test_Sim)
{
    ui->setupUi(this);
    this->setWindowTitle("Test Sim");
    this->setWindowState(Qt::WindowMaximized);

    push_ui_item(ui->button);
    push_ui_item(ui->combobox);
    push_ui_item(ui->label);
    push_ui_item(ui->slider);
    push_ui_item(ui->dial);
    push_ui_item(ui->thermo);
    push_ui_item(ui->led);

    setup_buttons();
    setup_comboboxes();
    setup_sliders();
    setup_dials();
}

Test_Sim::~Test_Sim() {
    delete ui;
}

void Test_Sim::setup_buttons(void)
{
    for (size_t i = 0; i < ui_lookup.size(); i++)
    {
        QPushButton* button = qobject_cast<QPushButton*>(ui_lookup.at(i));
        if (button == nullptr)
            continue;

        connect(button, &QPushButton::clicked, this, [=, this] {
            button_update(i);
        });
    }
}

void Test_Sim::setup_sliders(void)
{
    for (size_t i = 0; i < ui_lookup.size(); i++)
    {
        QwtSlider* slider = qobject_cast<QwtSlider*>(ui_lookup.at(i));
        if (slider == nullptr)
            continue;

        connect(slider, &QwtSlider::sliderMoved, this, [=, this]{
            //value_lookup_e lookup_v = thermo_to_value_lookup(lookup);
            //value_lookup[lookup_v]->setText(format_value(lookup_v, thermo_lookup[lookup]->value()));
            slider_update(i);
        });
    }
}

void Test_Sim::setup_comboboxes(void)
{
    for (size_t i = 0; i < ui_lookup.size(); i++)
    {
        QComboBox* combobox = qobject_cast<QComboBox*>(ui_lookup.at(i));
        if (combobox == nullptr)
            continue;

        connect(combobox, &QComboBox::currentIndexChanged, this, [=, this]{combobox_update(i);});
    }
}

void Test_Sim::setup_dials(void)
{
    for (size_t i = 0; i < ui_lookup.size(); i++)
    {
        QwtDial* dial = qobject_cast<QwtDial*>(ui_lookup.at(i));
        if (dial == nullptr)
            continue;

        create_dial_needle(dial);
    }
}

QLabel* Test_Sim::id_to_label(size_t id)
{
    QWidget* widget = ui_lookup.at(id);
    if (widget == ui->slider)
        return ui->slider_label;
    if (widget == ui->thermo)
        return ui->thermo_label;
    if (widget == ui->dial)
        return ui->dial_label;

    return nullptr;
}
