#ifndef SIM_TEST_H
#define SIM_TEST_H

#include <QMainWindow>
#include "simulatorbase.h"
#include "web_socket_wrapper.h"

namespace Ui {
class Test_Sim;
}

class Test_Sim : public SimulatorBase {
    Q_OBJECT

  public:
    ITERABLE_ENUM_BEGIN(button_lookup_e)
        button
    ITERABLE_ENUM_END;
    ITERABLE_ENUM_BEGIN(led_lookup_e)
        led
    ITERABLE_ENUM_END;
    ITERABLE_ENUM_BEGIN(label_lookup_e)
        label
    ITERABLE_ENUM_END;
    ITERABLE_ENUM_BEGIN(slider_lookup_e)
        slider
    ITERABLE_ENUM_END;
    ITERABLE_ENUM_BEGIN(combobox_lookup_e)
        combobox
    ITERABLE_ENUM_END;

    explicit Test_Sim(Web_socket_wrapper* web_socket, QWidget* parent = nullptr);
    ~Test_Sim();

    void setup_buttons(void);
    void setup_sliders(void);
    void setup_comboboxes(void);

    void button_update(button_lookup_e lookup);
    void slider_update(slider_lookup_e lookup);
    void combobox_update(combobox_lookup_e lookup);

    QLabel* id_to_label(QString name);
    QPushButton* id_to_led(QString name);
    QPushButton* id_to_button(QString name);
    QwtSlider* id_to_slider(QString name);
    QLabel* id_to_slider_label(QString name);
    QString format_slider_value(QString name, double val);
    QComboBox* id_to_combobox(QString name);

  private:
    Ui::Test_Sim* ui;
    std::unordered_map<button_lookup_e, QPushButton*> button_lookup;
    std::unordered_map<slider_lookup_e, bool> slider_lock;
    std::unordered_map<slider_lookup_e, QwtSlider*> slider_lookup;
    std::unordered_map<led_lookup_e, QPushButton*> led_lookup;
    std::unordered_map<combobox_lookup_e, QComboBox*> combobox_lookup;
    std::unordered_map<label_lookup_e, QLabel*> label_lookup;
};

#endif // SIM_TEST_H
