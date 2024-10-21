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
    ITERABLE_ENUM_BEGIN(ui_lookup_e)
        button,
        led,
        label,
        slider,
        dial,
        thermo,
        combobox
    ITERABLE_ENUM_END;

    explicit Test_Sim(Web_socket_wrapper* web_socket, QWidget* parent = nullptr);
    ~Test_Sim();

    void setup_buttons(void);
    void setup_sliders(void);
    void setup_comboboxes(void);
    void setup_dials(void);

    QLabel* id_to_label(size_t name) override;

  private:
    Ui::Test_Sim* ui;
};

#endif // SIM_TEST_H
