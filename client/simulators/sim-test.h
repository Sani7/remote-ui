#ifndef SIM_TEST_H
#define SIM_TEST_H

#include "simulator_base.h"
#include "web_socket_wrapper.h"
#include <QMainWindow>

namespace Ui
{
class Test_Sim;
}

class Test_Sim : public SimulatorBase
{
    Q_OBJECT

  public:
    explicit Test_Sim(Web_socket_wrapper *web_socket, QWidget *parent = nullptr);
    ~Test_Sim();

    QLabel *id_to_label(size_t name) override;

  private:
    Ui::Test_Sim *ui;
};

#endif // SIM_TEST_H
