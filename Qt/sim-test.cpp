#include "sim-test.h"
#include "ui_sim-test.h"

Test_Sim::Test_Sim(Web_socket_wrapper* web_socket, QWidget* parent) :
      SimulatorBase(web_socket, parent),
      ui(new Ui::Test_Sim)
{
    ui->setupUi(this);
    button_lookup = {
        {button_lookup_e::button, ui->button}
    };
    slider_lookup = {
        {slider_lookup_e::slider, ui->Slider}
    };
    led_lookup = {
        {led_lookup_e::led, ui->led}
    };

    combobox_lookup = {
        {combobox_lookup_e::combobox, ui->combobox}
    };
    label_lookup = {
        {label_lookup_e::label, ui->label}
    };

    setup_buttons();
    setup_comboboxes();
    setup_sliders();
    setup_sliders();
}

Test_Sim::~Test_Sim() {
    delete ui;
}

void Test_Sim::setup_buttons(void)
{
    for (Iterable<button_lookup_e> lookup; lookup < button_lookup_e::end; lookup++)
    {
        switch (lookup)
        {
            default:
                connect(button_lookup[lookup], &QPushButton::clicked, this, [=]{button_update(lookup);});
                break;
        }
    }
}

void Test_Sim::setup_sliders(void)
{
    for (Iterable<slider_lookup_e> lookup; lookup < slider_lookup_e::end; lookup++)
    {
        connect(slider_lookup[lookup], &QwtSlider::valueChanged, this, [=]{
            // Update the text while moving the slider
            //value_lookup_e lookup_v = thermo_to_value_lookup(lookup);
            //value_lookup[lookup_v]->setText(format_value(lookup_v, thermo_lookup[lookup]->value()));
        });
        connect(slider_lookup[lookup], &QwtSlider::sliderPressed, this, [=]{
            // When changing the sliders lock that slider
            slider_lock[lookup] = true;
        });
        connect(slider_lookup[lookup], &QwtSlider::sliderReleased, this, [=]{
            // Only trigger the update event when the slider is released and unlock the slider
            slider_update(lookup);
            slider_lock[lookup] = false;
        });
    }
}

void Test_Sim::setup_comboboxes(void)
{
    for (Iterable<combobox_lookup_e> lookup; lookup < combobox_lookup_e::end; lookup++)
    {
        connect(combobox_lookup[lookup], &QComboBox::currentIndexChanged, this, [=]{combobox_update(lookup);});
    }
}

void Test_Sim::button_update(button_lookup_e lookup)
{
    m_web_socket->send_event(Web_socket_wrapper::Event::clicked, QString(magic_enum::enum_name(lookup).data()));
}

void Test_Sim::slider_update(slider_lookup_e lookup)
{
    m_web_socket->send_event(Web_socket_wrapper::Event::value_changed, QString(magic_enum::enum_name(lookup).data()),
                             slider_lookup[lookup]->value());
}

void Test_Sim::combobox_update(combobox_lookup_e lookup)
{
    m_web_socket->send_event(Web_socket_wrapper::Event::selected, QString(magic_enum::enum_name(lookup).data()),
                             combobox_lookup[lookup]->currentText());
}

QLabel* Test_Sim::id_to_label(QString name)
{
    label_lookup_e lookup = magic_enum::enum_cast<label_lookup_e>(name.toStdString()).value_or(label_lookup_e::end);
    if (lookup == label_lookup_e::end)
    {
        QD << "label lookup == end, instead of " << name;
        emit quit();
        return nullptr;
    }

    return label_lookup[lookup];
}

QPushButton* Test_Sim::id_to_led(QString name)
{
    led_lookup_e lookup = magic_enum::enum_cast<led_lookup_e>(name.toStdString()).value_or(led_lookup_e::end);
    if (lookup == led_lookup_e::end)
    {
        QD << "led lookup == end, instead of " << name;
        emit quit();
        return nullptr;
    }

    return led_lookup[lookup];
}

QPushButton* Test_Sim::id_to_button(QString name)
{
    button_lookup_e lookup = magic_enum::enum_cast<button_lookup_e>(name.toStdString()).value_or(button_lookup_e::end);
    if (lookup == button_lookup_e::end)
    {
        QD << "button lookup == end, instead of" << name;
        emit quit();
        return nullptr;
    }

    return button_lookup[lookup];
}

QwtSlider* Test_Sim::id_to_slider(QString name)
{
    slider_lookup_e lookup = magic_enum::enum_cast<slider_lookup_e>(name.toStdString()).value_or(slider_lookup_e::end);
    if (lookup == slider_lookup_e::end)
    {
        QD << "thermo lookup == end, instead of " << name;
        emit quit();
        return nullptr;
    }

    // Check if the slider is being changed
    if (slider_lock[lookup])
    {
        QD << "Slider in lock retrurning null";
        return nullptr;
    }

    return slider_lookup[lookup];
}

QLabel* Test_Sim::id_to_slider_label(QString name)
{
    return nullptr;
}

QString Test_Sim::format_slider_value(QString name, double val)
{
    return "";
}

QComboBox* Test_Sim::id_to_combobox(QString name)
{
    combobox_lookup_e lookup = magic_enum::enum_cast<combobox_lookup_e>(name.toStdString()).value_or(combobox_lookup_e::end);
    if (lookup == combobox_lookup_e::end)
    {
        QD << "combobox lookup == end, instead of " << name;
        emit quit();
        return nullptr;
    }

    return combobox_lookup[lookup];
}
