#pragma once
#include "simulator_base.hpp"
#include "web_socket_wrapper.hpp"
#include <QMainWindow>

namespace Ui
{
class Test_Sim;
}

class Test_Sim : public Simulator_base
{
    Q_OBJECT

  public:
    explicit Test_Sim(Web_socket_wrapper *web_socket = nullptr, QWidget *parent = nullptr);
    ~Test_Sim();

    QLabel *id_to_label(size_t name) override;

  private:
    Ui::Test_Sim *ui;
};
