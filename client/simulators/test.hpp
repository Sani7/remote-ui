#pragma once
#include "simulator_base.hpp"

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

    QLabel *id_to_label(QWidget *widget) override;

  private:
    Ui::Test_Sim *ui;
};
