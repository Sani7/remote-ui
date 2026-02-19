#pragma once
#include "ui_base.hpp"

namespace Ui
{
class Test_Sim;
}

class Test_Sim : public UI_base
{
    Q_OBJECT

  public:
    explicit Test_Sim(Web_socket_wrapper *web_socket = nullptr, QWidget *parent = nullptr);
    ~Test_Sim();

    static QString name()
    {
        return "Test Sim";
    };

    QLabel *id_to_label(QWidget *widget) override;

  private:
    Ui::Test_Sim *ui;
};
